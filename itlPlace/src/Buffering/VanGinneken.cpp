#include "HDesign.h"
#include "VanGinneken.h"
#include "Utils.h"
#include "Reporting.h"
#include <stdio.h>
#include "STA.h"
#include "Auxiliary.h"
#include <math.h>
#include "Legalization.h"
#include <float.h>

int VanGinneken::FindBufferNumberByIndex(int index)
{
  for (int i = 0; i < m_BufferIndexes.size(); ++i)
    if (m_BufferIndexes[i] == index)
      return i;

  return -1;
}

VanGinneken::VanGinneken(HDesign& design): 
m_hd(design), m_vgNetSplitted(nullSP, nullSP, nullSP, 0, 0, 0, 0, 0, m_hd, 0, 0), DPGrid(design) 
{

  LoadAvailableBuffers();
  m_WirePhisics = m_hd.RoutingLayers.Physics;
  m_WirePhisics.SetLinearC(m_WirePhisics.LinearC);  //convert to the femtoFarad/10^-6m
  m_WirePhisics.SetRPerDist(m_WirePhisics.RPerDist); //convert to the ohms/10^-6m

  m_nCandidatesForBuffering = 0;
  m_nReverts = 0;
  m_nCriticalPaths = 0;

  m_vgNetSplitted.NULLPointer();

  m_freeSpace = m_hd.Circuit.PlacementWidth() * m_hd.Circuit.PlacementHeight();
  for (HCells::CellsEnumeratorW cellW = m_hd.Cells.GetEnumeratorW(); cellW.MoveNext(); )
    if (cellW.PlacementStatus() == PlacementStatus_Movable
      || cellW.PlacementStatus() == PlacementStatus_Fixed)
      m_freeSpace = m_freeSpace - cellW.Height() * cellW.Width(); 
  m_isFreeSpaceEnded = false;

  m_doReportBuffering = m_hd.cfg.ValueOf(".doReportBuffering", false);
  m_finalLocationVan = new Comp();
  bestTNS = INFINITY;
  bestWNS = INFINITY;
};

VanGinneken::RLnode *VanGinneken::create_list(VGNode *t)
{
  RLnode *last = new RLnode();
  last->cap = t->Cap();
  last->time = t->Time();
  last->next = 0;
  last->com = 0;
  return last;
}

void VanGinneken::list_delete(RLnode *l)
{
  while (l != 0)
  {
    RLnode *tmp = l->next;
#if QM
    if (l->com != 0)
      l->com->counter--;
#endif
    comp_delete(l->com);
    delete l;
    l = tmp;
  }
}

void VanGinneken::comp_delete(Comp *com)
{
  if (com == 0) return;
  if (com->counter == 0)
  {
    if (com->left != 0)
    {
      com->left->counter--;
      comp_delete(com->left);
    }
    if (com->right != 0)
    {
      com->right->counter--;
      comp_delete(com->right);
    }
    delete com;
  }
}

VanGinneken::RLnode *VanGinneken::redundent(RLnode *list)
{
  RLnode dummy;
  double x;

  dummy.cap = 0;
  dummy.time = -INFINITY;

  dummy.com = 0;
  dummy.next = 0;
  dummy.next = list;

  RLnode* pred = &dummy;
  RLnode* r = list;

  while (r != 0)
  {
#if PREDICT
    x = (r->time - m_AvailableBuffers[m_AvailableBuffers.size()-1].Resistance() * r->cap) - 
      (pred->time - m_AvailableBuffers[m_AvailableBuffers.size()-1].Resistance() * pred->cap);
#else
    x = r->time - pred->time;
#endif
    if (x < (EPS))
    {
      pred->next = r->next;
#if QM
      if (r->com != 0)
        r->com->counter--;
#endif
      comp_delete(r->com);
      delete r;
    } 
    else
      pred = r;
    r = pred->next;
  }

  return dummy.next; 
}

VanGinneken::RLnode *VanGinneken::add_buffer(double distance, RLnode *list, int px, int py)
{
  RLnode *z[MAXBUFFERTYPE];
  ERROR_ASSERT(m_AvailableBuffers.size() < MAXBUFFERTYPE);

  /*NOTE: if the wire has length 0, then quit */
#if TREATZERO
  if (distance == 0.0)
    return list;
#endif

  /*NOTE: get the solution after the edge from the current node to its child */
  for (RLnode* nd = list; nd != 0; nd = nd->next)
  {
    //NOTE:WARNING: delays calculation
    nd->time = nd->time
      - 0.5 * m_WirePhisics.RPerDist * m_WirePhisics.LinearC * distance * distance //NOTE:ÒÓ·ÒÚ‚ÂÌÌ‡ˇ Á‡‰ÂÊÍ‡ Ì‡ ÔÓ‚Ó‰Â
      - m_WirePhisics.RPerDist * distance * nd->cap; //NOTE:Á‡‰ÂÊÍ‡ Ì‡ Ó·ÓÁÂ‚‡ÂÏÓÈ ∏ÏÍÓÒÚË
    nd->cap = nd->cap
      + distance * m_WirePhisics.LinearC; //NOTE:wire capacitance
#if !QM
    Comp* p = new Comp();
    p->left = nd->com;
    p->right = 0;
    if (p->left != 0)
      p->left->counter++;
    p->buffertype = 0;
    p->counter = 0;
    p->x = px;
    p->y = py;
    nd->com = p;
#endif
  }

  RLnode dummy;
  dummy.next = list;

  /*NOTE:  consider the buffer solution */
  for (int i = int(m_AvailableBuffers.size()) - 1; i >= 0; --i)
  {
    RLnode* y = dummy.next;
    RLnode* x = 0;
    int btype = -1;
    double qmax = -INFINITY;

    while (y != 0)
    {
      //WARNING: delays calculation
      double q = y->time 
        - m_AvailableBuffers[i].TIntrinsic() //NOTE:own delay
        - m_AvailableBuffers[i].Resistance() * y->cap; //NOTE:driver delay on observed capacitance
      if (q > qmax)
      {
        qmax = q;
        btype = i + 1;
        x = y;
      }
      y = y->next;
    }

    z[i] = new RLnode();
    //NOTE:total_solution += sizeof(RLnode);
    z[i]->cap = m_AvailableBuffers[i].Capacitance();
    z[i]->time = qmax;

    Comp* p = new Comp();
    //NOTE:m_used += sizeof(Comp);
#if QM
    p->left = x->com;
    if (p->left != 0)
      p->left->counter++;
    p->right = 0;
    p->counter = 1;
#else
    p->left = x->com->left;
    p->right = x->com->right;
    if (p->left != 0)
      p->left->counter++;
    if (p->right != 0)
      p->right->counter++;
    p->counter = 0;
#endif
    p->buffertype = btype;
    p->x = px;
    p->y = py;
    z[i]->com = p;
  }

  /*NOTE: add buffer solution answer to the list */
  for (int j = int(m_AvailableBuffers.size()) - 1; j >=0; --j)
  {
    RLnode* y = dummy.next;
    RLnode* pred = &dummy;
    while (y != 0)
    {
      if (fabs(z[j]->cap - y->cap) < (1e-10))//!!!
      {
        if (z[j]->time > y->time)
        {
          z[j]->next = y->next;
          pred->next = z[j];
#if QM
          if (y->com != 0)
            y->com->counter--;
#endif
          comp_delete(y->com);
          delete y;
          //NOTE:reduce_solution += sizeof(RLnode);
        }
        else
        {
#if QM
          if (z[j]->com != 0)
            z[j]->com->counter--;
#endif
          comp_delete(z[j]->com);
          delete z[j];
          //NOTE:reduce_solution += sizeof(RLnode);
        }
        break;
      }
      if (z[j]->cap < y->cap)
      {
        z[j]->next = y;
        pred->next = z[j];
        break;
      }
      else
      {
        if (z[j]->cap > y->cap)
        {
          y = y->next;
          pred = pred->next;
        }
      }
    }
    if (y == 0)
    {
      z[j]->next = 0;
      pred->next = z[j];
    }
  }

  //NOTE:if (total_solution-reduce_solution+m_used-m_reduced>max_space)
  //NOTE:  max_space=total_solution-reduce_solution+m_used-m_reduced;

  return dummy.next;
}

double VanGinneken::distance(VGNode *t, VGNode *t1)
{
  return fabs((t->X(0) - t1->X(1))) + fabs(t->Y(0) - t1->Y(1));
}

VanGinneken::RLnode* VanGinneken::MergeLists(RLnode* sleft, RLnode* sright, VGNode *t)
{
  RLnode dummy;
  RLnode* s = &dummy;
  dummy.cap = 0.0;
  dummy.time = 0.0;
  dummy.com = 0;
  dummy.next = 0;
  RLnode* slefthead = sleft;
  RLnode* srighthead = sright;

  while (sleft != 0 && sright != 0)
  {
    double c = sleft->cap + sright->cap;
    double q = sright->time;
    s->next = new RLnode();
    s = s->next;

    if (sleft->time < sright->time)
    {
      q = sleft->time;
      Comp* p = new Comp();
      p->left = sleft->com;
      p->right = sright->com;
      if (p->left != 0)
        p->left->counter++;
      if (p->right != 0)
        p->right->counter++;
      p->buffertype = -1;
#if QM
      p->counter = 1;
#else
      p->counter = 0;
#endif
      s->com = p;
      sleft = sleft->next;
      p->x = t->Index();
      p->y = t->Index();
    }
    else
    {
      Comp* p = new Comp();
      p->left = sleft->com;
      p->right = sright->com;
      if (p->left != 0)
        p->left->counter++;
      if (p->right != 0)
        p->right->counter++;
      p->buffertype = -1;    //NOTE: steiner merge point, indeed no buffer here
#if QM
      p->counter = 1;
#else
      p->counter = 0;
#endif
      s->com = p;
      sright = sright->next;
      p->x = t->Index();
      p->y = t->Index();
    }

    s->cap = c;
    s->time = q;

  }
  s->next = 0;

  list_delete(slefthead);
  list_delete(srighthead);
  return dummy.next;
}

VanGinneken::RLnode *VanGinneken::van(VGNode *t, double& van_answer, double rd)
{
  if (t->IsSink())
  {
    return create_list(t);
  }

  RLnode* sleft = 0;
  RLnode* sright = 0;
  double k1 = 0;
  double k2 = 0;

  sleft = van(t->LeftStep(), van_answer, rd);
  t->BackStep();
  k1 = distance(t, t->LeftStep());
  t->BackStep();  

  if (t->IsRightNodeExist())
  {
    sright = van(t->RightStep(), van_answer, rd);
    t->BackStep();
    k2 = distance(t, t->RightStep());
    t->BackStep();
  }

  if (t->IsSource())
  {
    van_answer = -INFINITY;
    for (RLnode* x = sleft; x != 0; x = x->next)
    {
      //WARNING: delays calculation
      double q = x->time
        - 0.5 * m_WirePhisics.RPerDist * m_WirePhisics.LinearC * k1 * k1
        - (m_WirePhisics.RPerDist * k1 + rd) * x->cap
        - rd * k1 * m_WirePhisics.LinearC;
      if (q > van_answer)
      {
        van_answer = q;
        m_finalLocationVan = x->com; //NOTE: ‚ÓÚ ÚÛÚ Ë ·ÂÂÏ ÂÁÛÎ¸Ú‡Ú!!!
      }
    }
    return sleft;
  }
  else
  {
    int x = t->Index();
    int y = t->LeftStep()->Index();
    t->BackStep();
    sleft = add_buffer(k1, sleft, x, y);

    sleft = redundent(sleft);
    //NOTE: if the node has only one child (default left), only 
    //return update solution of left child 
    if (!t->IsRightNodeExist())
    {
      return sleft;
    }
    else
    {
      int x = t->Index();
      int y = t->RightStep()->Index();
      t->BackStep();
      sright = add_buffer(k2, sright, x, y);

      sright = redundent(sright);
    }
  }	

  return MergeLists(sleft, sright, t);
}


void VanGinneken::LoadAvailableBuffers()
{
  /*BufferInfo buf;
  const char* name = m_hd.cfg.ValueOf("GlobalPlacement.bufferName", (const char*)"INVX1");
  buf.BufferMacroType = Utils::FindMacroTypeByName(m_hd, name);
  Utils::CalcElementTRC(m_hd, buf.BufferMacroType, &buf.TIntrinsic(), &buf.Resistance(), &buf.Capacitance());
  buf.Resistance()  *= FBI_CELL_RESISTANCE_SCALING;
  buf.Capacitance() *= FBI_CELL_CAPACITANCE_SCALING;
  m_AvailableBuffers.push_back(buf);*/
  BufferInfo buf = BufferInfo::Create(m_hd);
  m_AvailableBuffers.push_back(buf);
}

void VanGinneken::InitializeBuffersIdxs()
{
  if (m_finalLocationVan != 0)
    ParseFinalVanLocation(m_finalLocationVan);
  std::sort(m_BufferIndexes.begin(), m_BufferIndexes.end());
}

int VanGinneken::MathBuffering(HNet& net)
{
  netInfo = NetInfo::Create(m_hd, net, m_AvailableBuffers[0]);

  if (!(netInfo.X2opt() < 0.0 && netInfo.Xmax() > 0.0))
    return 0;

  m_vgNetSplitted.Destroy();
  int steps = m_hd.cfg.ValueOf(".steps", 1);
  double sinkCapacitance = m_AvailableBuffers[0].Capacitance();
  int nUnits = m_vgNetSplitted.InitializeTree(m_hd.SteinerPoints[m_hd[net].Source()], 
    sinkCapacitance, 0, steps, 0, 2, 0);

  m_BufferIndexes.clear();
  m_BufferIndexes.push_back(0);//‚ Í‡ÍÓÂ Á‚ÂÌÓ ‚ÒÚ‡‚ÎˇÂÏ ·ÛÙÂ

  InitializeBuffersIdxs();

  m_hd.Nets.Set<HNet::Kind>(net, NetKind_Buffered);
  CreateNetsAndCells(net);

  return 1;
}

int VanGinneken::InsertBuffers(HNet& net)
{
  bool isNewNet = (m_hd.Nets.GetString<HNet::Name>(net).find("BufferedPart") != -1);
  bool isNetBuffered = (m_hd.Nets.Get<HNet::Kind, NetKind>(net) == NetKind_Buffered);
  bool isNetBufferable = !isNewNet && !m_isFreeSpaceEnded && !isNetBuffered;

  if (!isNetBufferable)
    return 0;

  netInfo = NetInfo::Create(m_hd, net, m_AvailableBuffers[0]);

  if (m_hd.cfg.ValueOf(".plotBuffering", false))
  {
    m_hd.Plotter.ShowPlacement();
    m_hd.Plotter.PlotNetSteinerTree(net, Color_Black);
    m_hd.Plotter.PlotText(m_hd.Nets.GetString<HNet::Name>(net));
    m_hd.Plotter.Refresh((HPlotter::WaitTime)m_hd.cfg.ValueOf(".plotWait", 1));
  }

  int nUnits = RunVG(net);

  if (nUnits > 0)
  {
    m_BufferIndexes.clear();
    m_BufferIndexes.reserve(nUnits);
  }
  else
  {
    ALERTFORMAT(("RunVG failed on net %s", m_hd[net].Name().c_str()));
    return 0;
  }

  m_nCandidatesForBuffering++;
  InitializeBuffersIdxs();
  int nBuffersInserted = m_BufferIndexes.size();

  if (m_doReportBuffering)
  {
    ALERTFORMAT(("Buffered net: %s", m_hd[net].Name().c_str()));
    double lengthTree = m_vgNetSplitted.LengthTree(true);
    ALERTFORMAT(("Length Tree = %f", lengthTree));
    ALERTFORMAT(("  Pin count: %d\tBuffers count: %d", m_hd[net].PinsCount(), nBuffersInserted));
    int x;
    printbuffer(m_finalLocationVan, &x);
  }

  //ƒ¿À‹ÿ≈ Õ¿◊»Õ¿ﬁ“—ﬂ › —œ≈–≈Ã≈Õ“€
  netInfo = NetInfo::Create(m_hd, net, m_AvailableBuffers[0]);
  //ÔÓ‚ÂˇÂÏ ÛÒÎÓ‚ËÂ
  //if ((netInfo.X2opt() < 0) && (netInfo.Xmax() > 0))
  //{
  //  //—¡Œ– ƒ¿ÕÕ€’ ƒŒ ¡”‘≈–»«¿÷»»:
  //  ALERTFORMAT(("net name %s", m_hd.Nets.GetString<HNet::Name>(net).c_str()));
  //  ALERT("STA before buffering:");
  //  STA(m_hd);
  //  double tns = Utils::TNS(m_hd);
  //  double wns = Utils::WNS(m_hd);
  //  ALERTFORMAT(("maxSlack = %f", netInfo.MaxRealDelay()));
  //  ALERTFORMAT(("Pins count = %d", m_hd.Nets.GetInt<HNet::PinsCount>(net)));
  //  ALERTFORMAT(("HPWL = %f", netInfo.HPWL()));
  //  ALERTFORMAT(("WL = %f", netInfo.WL()));
  //  ALERTFORMAT(("Lext = %f", netInfo.Lext()));
  //  ALERTFORMAT(("Buffer count = %d", nBuffersInserted));
  //}
  //else
  //  return 0;


  //¡ÀŒ  –≈¿À‹ÕŒ… ¡”‘≈–»«¿÷»»
  //{
  double vgRSlack = 0;
  if (nBuffersInserted > 0) 
  {
    m_hd.Nets.Set<HNet::Kind>(net, NetKind_Buffered);
    CreateNetsAndCells(net);
    //ALERTFORMAT(("STA after buffering (real):"));
    //STA(m_hd);
    //double tns2 = Utils::TNS(m_hd);
    //double wns2 = Utils::WNS(m_hd);
    //vgRSlack =  TimingHelper(m_hd).GetBufferedNetMaxDelay(net, netInfo, m_AvailableBuffers[0]);
    //ALERTFORMAT(("vgrSlack = %f", vgRSlack ));
    //Utils::RestoreBufferedNet(m_hd, net);
    //ERROR_ASSERT(Utils::VerifyTimingCalculationOrder(m_hd));
    // (1) –¿— ŒÃ≈Õ“»–Œ¬¿“‹ ≈—À» ¬—“¿¬ ¿ ¡”‘≈–¿ ¬ ƒ–¿…¬≈– Õ≈ Õ”∆Õ¿
    return nBuffersInserted;
  }
  else
  {
    //ALERTFORMAT(("STA after buffering (real):"));
    //STA(m_hd);
    //double tns2 = Utils::TNS(m_hd);
    //double wns2 = Utils::WNS(m_hd);
    //vgRSlack =  TimingHelper(m_hd).GetBufferedNetMaxDelay(net, netInfo, m_AvailableBuffers[0]);
    //ALERTFORMAT(("vgrSlack = %f", vgRSlack ));
    // (2) –¿— ŒÃ≈Õ“»–Œ¬¿“‹ ≈—À» ¬—“¿¬ ¿ ¡”‘≈–¿ ¬ ƒ–¿…¬≈– Õ≈ Õ”∆Õ¿
    return 0;
  }
  //}

  //¡ÀŒ  ¬—“¿¬ » ¡”‘≈–¿ ¬ ƒ–¿…¬≈–(ÂÒÎË ÓÌ ÌÂ ÌÛÊÂÌ ‡ÒÍÓÏÂÌÚËÓ‚‡Ú¸ (1) Ë (2) )
  //{
  m_BufferIndexes.clear();
  m_BufferIndexes.push_back(0);//‚ Í‡ÍÓÂ Á‚ÂÌÓ ‚ÒÚ‡‚ÎˇÂÏ ·ÛÙÂ
  nBuffersInserted = 1;
  m_hd.Nets.Set<HNet::Kind>(net, NetKind_Buffered);
  CreateNetsAndCells(net);
  ALERT("STA after buffering in source:");
  STA(m_hd);
  double tns2 = Utils::TNS(m_hd);
  double wns2 = Utils::WNS(m_hd);
  double vgSlack = TimingHelper(m_hd).GetBufferedNetMaxDelay(net, netInfo, m_AvailableBuffers[0]);
  ALERTFORMAT(("vgSlack = %f", vgSlack ));
  Utils::RestoreBufferedNet(m_hd, net);
  ERROR_ASSERT(Utils::VerifyTimingCalculationOrder(m_hd));
  STA(m_hd);

  return 1;
  //}

}

int VanGinneken::NetBufferNotDegradation(HNet &net)
{

  bool isNewNet = (m_hd.Nets.GetString<HNet::Name>(net).find("BufferedPart") != -1);
  bool isNetBuffered = (m_hd.Nets.Get<HNet::Kind, NetKind>(net) == NetKind_Buffered);
  bool isNetBufferable = !isNewNet && !m_isFreeSpaceEnded && !isNetBuffered;

  if (!isNetBufferable)
    return 0;

  if (m_hd.cfg.ValueOf(".plotBuffering", false))
  {
    m_hd.Plotter.ShowPlacement();
    m_hd.Plotter.PlotNetSteinerTree(net, Color_Black);
    m_hd.Plotter.PlotText(m_hd.Nets.GetString<HNet::Name>(net));
    m_hd.Plotter.Refresh((HPlotter::WaitTime)m_hd.cfg.ValueOf(".plotWait", 1));
  }

  int nUnits = RunVG(net);

  if (nUnits > 0)
  {
    m_BufferIndexes.clear();
    m_BufferIndexes.reserve(nUnits);
  }
  else
  {
    ALERTFORMAT(("RunVG failed on net %s", m_hd[net].Name().c_str()));
    return 0;
  }

  m_nCandidatesForBuffering++;
  InitializeBuffersIdxs();
  int nBuffersInserted = m_BufferIndexes.size();

  if (m_doReportBuffering)
  {
    ALERTFORMAT(("Buffered net: %s", m_hd[net].Name().c_str()));
    double lengthTree = m_vgNetSplitted.LengthTree(true);
    ALERTFORMAT(("Length Tree = %f", lengthTree));
    ALERTFORMAT(("  Pin count: %d\tBuffers count: %d", m_hd[net].PinsCount(), nBuffersInserted));
    int x;
    printbuffer(m_finalLocationVan, &x);
  }

  if (nBuffersInserted > 0) 
  { 
    double tns = Utils::TNS(m_hd);
    double wns = Utils::WNS(m_hd);
    netInfo = NetInfo::Create(m_hd, net, m_AvailableBuffers[0]);

    //m_Placement.SavePlacement(m_hd);
    m_hd.Nets.Set<HNet::Kind>(net, NetKind_Buffered);

    CreateNetsAndCells(net);

    double vgSlack = TimingHelper(m_hd).GetBufferedNetMaxDelay(net, netInfo, m_AvailableBuffers[0]);
    double maxSlack = netInfo.MaxRealDelay();
    STA(m_hd, false);
    double btns = Utils::TNS(m_hd);
    double bwns = Utils::WNS(m_hd);
    //if (vgSlack < maxSlack)
    if  (((tns >= btns) && (wns >= bwns)) && (vgSlack < maxSlack))
    {
      if (m_doReportBuffering)
        ALERTFORMAT(("buffer insite = %d in net %s", nBuffersInserted, m_hd.Nets.GetString<HNet::Name>(net).c_str()));
      return nBuffersInserted;
    }

    m_nReverts++;
    
    Utils::RestoreBufferedNet(m_hd, net);

    STA(m_hd, false);
    ERROR_ASSERT(Utils::VerifyTimingCalculationOrder(m_hd));

    return 0;
  }
  else
  {
    return 0;
  }
}

void VanGinneken::AddSinks2Net(HCell* insertedBuffers, HNet& subNet, VGNode& nodeStart, int startNodeIdx,  
                               HNetWrapper::PinsEnumeratorW& subNetPinEnum, bool doIndexesClear)
{
  bool f = doIndexesClear;
  if (doIndexesClear)
    nodeStart.IndexesClear();

  int bufferNumber = FindBufferNumberByIndex(nodeStart.Index());

  //insert sink pin (buffer input) to the subnet
  if (bufferNumber != -1)
  {
    if ((nodeStart.Index() != startNodeIdx) || ((nodeStart.Index() == 0) && f))
    {      
      HPin bufferInput = Utils::FindCellPinByName(m_hd, insertedBuffers[bufferNumber], m_hd.cfg.ValueOf("Buffering.DefaultBuffer.InputPin", "A"));

      subNetPinEnum.MoveNext();

      m_hd.Nets.AssignPin(subNet, subNetPinEnum, bufferInput);

      {//update topological order
        HTimingPoint source = m_hd.TimingPoints[m_hd.Get<HNet::Source, HPin>(subNet)];
        //TODO: fix next line
        HPin bufferOutput = Utils::FindCellPinByName(m_hd, insertedBuffers[bufferNumber], m_hd.cfg.ValueOf("Buffering.DefaultBuffer.OutputPin", "Y"));
        Utils::InsertNextPoint(m_hd, m_hd.TimingPoints[bufferInput], source);
        Utils::InsertNextPoint(m_hd, m_hd.TimingPoints[bufferOutput], m_hd.TimingPoints[bufferInput]);
        //ERROR_ASSERT(Utils::VerifyTimingCalculationOrder(m_hd));
      }
      return;
    }
  }

  //insert pins from left subtree
  if (nodeStart.IsLeftNodeExist())
  {
    AddSinks2Net(insertedBuffers, subNet, *nodeStart.LeftStep(), startNodeIdx, subNetPinEnum);
    nodeStart.BackStep();
  }

  //insert pins from right subtree
  if (nodeStart.IsRightNodeExist())
  {
    AddSinks2Net(insertedBuffers, subNet, *nodeStart.RightStep(), startNodeIdx, subNetPinEnum);
    nodeStart.BackStep();
  }

  //insert sink pin to the final subnets
  if (!nodeStart.IsInternal() && nodeStart.Index() != startNodeIdx)
  {
    VGItem result;
    nodeStart.GetSteinerPoint(nodeStart.Index(), nodeStart, result);
    subNetPinEnum.MoveNext();
    m_hd.Nets.AssignPin(subNet, subNetPinEnum, m_hd.SteinerPoints.Get<HSteinerPoint::Pin, HPin>(result.steinerPoint));
    return;
  }
}

void VanGinneken::PinsCountCalculation(VGNode& startNode, int startNodeIdx, int& nPins, bool doIndexesClear)
{
  bool f = doIndexesClear;
  if (doIndexesClear)
    startNode.IndexesClear();

  if ((FindBufferNumberByIndex(startNode.Index()) != -1))
  {
    if (((startNode.Index() == 0) && f) && startNode.Index() == startNodeIdx)
    {    
      nPins += 2;
      return;
    }
    if (startNode.Index() != startNodeIdx)
    {
      nPins++;
      return;
    }
  }
  if (startNode.LeftStep() != NULL)
  {
    startNode.BackStep();
    PinsCountCalculation(*startNode.LeftStep(), startNodeIdx, nPins);
    startNode.BackStep();
  }
  if (startNode.IsRightNodeExist())
  {
    PinsCountCalculation(*startNode.RightStep(), startNodeIdx, nPins);
    startNode.BackStep();
  }
  if (!startNode.IsInternal())
  {
    if (!((f == false) && (startNode.Index() ==  0) ))
      nPins++;
    return;
  }

  return;
}

int VanGinneken::RunVG(HNet& net)
{
  WARNING_ASSERT(net.Kind != NetKind_Buffered);

  m_vgNetSplitted.Destroy();
  int steps = m_hd.cfg.ValueOf(".steps", 1);
  double sinkCapacitance = m_AvailableBuffers[0].Capacitance();
  int nUnits = m_vgNetSplitted.InitializeTree(m_hd.SteinerPoints[m_hd[net].Source()], 
    sinkCapacitance, 0, steps, 0, 2, 0);

  if (m_hd.SteinerPoints.Get<HSteinerPoint::Right, HSteinerPoint>((m_hd.SteinerPoints[m_hd[net].Source()])) != nullSP)
  {
    ALERTFORMAT(("In source 2 edges"));
  }
  if (nUnits > 0)
  {
    double newSlackAtSource;
    double driverResistance = netInfo.Rd();//m_AvailableBuffers[0].Resistance();
    m_VGOutput = van(&m_vgNetSplitted, newSlackAtSource, driverResistance);

    if (m_doReportBuffering)
    {
      ALERTFORMAT(("(vananswer) RAT at source AB = %.10f", newSlackAtSource));
      //ALERTFORMAT(("Buffer count optimal = %.10f", CalculationOptimumNumberBuffers(net)));
    }
    return nUnits;
  }
  else 
  {
    return 0;
  }
}

void VanGinneken::CreateCells(string bufferName, HCell* insertedBuffers)
{
  VGItem vgBuffer;
  char bufferIdx[10];
  string bufferFullName;

  double bufferWidth = m_hd[m_AvailableBuffers[0].Type()].SizeX();
  double bufferHeight =  m_hd[m_AvailableBuffers[0].Type()].SizeY();
  double bufferSquare = bufferWidth * bufferHeight;
  double bufferX, bufferY;

  for (int i = 0; i < m_BufferIndexes.size(); i++)
  {
    m_freeSpace = m_freeSpace - bufferSquare;
    if (m_freeSpace <= 0)
    {
      m_BufferIndexes.resize(i);
      m_isFreeSpaceEnded = true;
      WARNING_ASSERT(m_isFreeSpaceEnded);
      break;
    }
    else
    {
      insertedBuffers[i] = m_hd.Cells.AllocateCell();

      //set name
      sprintf(bufferIdx, "%d", i);
      bufferFullName = bufferName + string("_") + string(bufferIdx);
      m_hd.Cells.Set<HCell::Name>(insertedBuffers[i], bufferFullName);

      //set sizes
      m_hd.Cells.Set<HCell::Width>(insertedBuffers[i], bufferWidth);
      m_hd.Cells.Set<HCell::Height>(insertedBuffers[i], bufferHeight);

      //set coordinates
      m_vgNetSplitted.GetSteinerPoint(m_BufferIndexes[i], m_vgNetSplitted, vgBuffer, true);
      bufferX = vgBuffer.x - bufferWidth/2.0;
      bufferY = vgBuffer.y - bufferHeight/2.0;
      m_hd.Cells.Set<HCell::X>(insertedBuffers[i], bufferX);
      m_hd.Cells.Set<HCell::Y>(insertedBuffers[i], bufferY);

      //set macrotype and initialize pins
      m_hd.Cells.Set<HCell::MacroType>(insertedBuffers[i], m_AvailableBuffers[0].Type());
      m_hd.Pins.AllocatePins(insertedBuffers[i]);

      if (m_doReportBuffering)
        ALERTFORMAT(("  %s: x = %.10f\ty = %.10f\tindex = %d", bufferFullName.c_str(), bufferX, bufferY, m_BufferIndexes[i]));

      if (m_hd.cfg.ValueOf(".plotBuffering", false))
      {
        m_hd.Plotter.PlotCell(insertedBuffers[i], Color_Orange);
        m_hd.Plotter.Refresh((HPlotter::WaitTime)m_hd.cfg.ValueOf(".plotWait", 1));
      }
    }
  }
}

void VanGinneken::CreateNets(HNet& net, HCell* insertedBuffers, HNet* newNet)
{
  WARNING_ASSERT(m_BufferIndexes.size() > 0);
  int nNewNetPin = 0;
  int nPins = 0;
  char cellIdx[10];
  VGItem vgItem;

  //allocate new net
  HNet subNet = m_hd.Nets.AllocateNet(false);
  newNet[0] = subNet;
  m_hd.Nets.Set<HNet::Kind, NetKind>(subNet, NetKind_Active);
  sprintf(cellIdx, "%d", 0);
  m_hd.Nets.Set<HNet::Name>(subNet, m_hd[net].Name() + "__BufferedPart_" + string(cellIdx));

  //allocate pins
  PinsCountCalculation(m_vgNetSplitted, 0, nPins, true);
  nNewNetPin += nPins;
  //nPins++;
  m_hd.Nets.AllocatePins(subNet, nPins);  

  //init source
  m_hd.Nets.Set<HNet::Source, HPin>(subNet, m_hd[net].Source());

  //add other pins
  AddSinks2Net(insertedBuffers, subNet, m_vgNetSplitted, 0, m_hd[subNet].GetSinksEnumeratorW(), true);

  if (m_hd.cfg.ValueOf(".plotBuffering", false))
  {
    m_hd.Plotter.PlotNetSteinerTree(subNet, Color_Magenta);
    m_hd.Plotter.Refresh((HPlotter::WaitTime)m_hd.cfg.ValueOf(".plotWait", 1));
  }

  for (int j = 0; j < m_BufferIndexes.size(); j++)
  {
    //allocate new net
    subNet = m_hd.Nets.AllocateNet(false);
    newNet[j] = subNet;
    m_hd.Nets.Set<HNet::Kind, NetKind>(subNet, NetKind_Active);
    sprintf(cellIdx, "%d", j);
    m_hd.Nets.Set<HNet::Name>(subNet, m_hd[net].Name() + "__BufferedPart_" + string(cellIdx));

    //allocate pins
    nPins = 0;
    VGNode& nodeStart = m_vgNetSplitted.GetSteinerPoint(m_BufferIndexes[j], m_vgNetSplitted, vgItem, true);
    PinsCountCalculation(nodeStart, m_BufferIndexes[j], nPins);
    nPins++;
    nNewNetPin += nPins;
    //nPins++;
    m_hd.Nets.AllocatePins(subNet, nPins);

    //init source
    m_hd.Nets.Set<HNet::Source, HPin>(subNet, Utils::FindCellPinByName(m_hd, insertedBuffers[j], 
      m_hd.cfg.ValueOf("Buffering.DefaultBuffer.OutputPin", "Y")));

    //add other pins
    VGNode& nodeStart2 = m_vgNetSplitted.GetSteinerPoint(m_BufferIndexes[j], m_vgNetSplitted, vgItem, true);
    AddSinks2Net(insertedBuffers, subNet, nodeStart2, m_BufferIndexes[j], m_hd[subNet].GetSinksEnumeratorW());


    if (m_hd.cfg.ValueOf(".plotBuffering", false))
    {
      m_hd.Plotter.PlotNetSteinerTree(subNet, Color_Magenta);
      m_hd.Plotter.Refresh((HPlotter::WaitTime)m_hd.cfg.ValueOf(".plotWait", 1));
    }
  }
  int pinCount = m_hd.Nets.GetInt<HNet::PinsCount>(net);
  if (nNewNetPin != (pinCount + m_BufferIndexes.size() * 2))
  {
    ERROR_ASSERT(nNewNetPin == pinCount);
    ALERTFORMAT(("new net pin count = %d", nNewNetPin));
  }
}

void VanGinneken::CreateNetsAndCells(HNet& net)
{
  HCell* insertedBuffers = new HCell[m_BufferIndexes.size()];
  HNet* newNet = new HNet[m_BufferIndexes.size() + 1];

  CreateCells(string("InsertedBuffer_") + m_hd[net].Name(), insertedBuffers);
  CreateNets(net, insertedBuffers, newNet);

  delete [] insertedBuffers;
  delete [] newNet;
}

int VanGinneken::BufferingOfMostCriticalPaths(int nPaths)
{
  struct PathBuffering
  {
    VanGinneken& vg;
    int bufs;
    PathBuffering(VanGinneken& VG): vg(VG), bufs(0){}

    void DoIt(HDesign& hd, HCriticalPath path, int n)
    {
      bufs += vg.CriticalPathBuffering(path);
    }
  };

  PathBuffering pb(*this);
  Utils::IterateMostCriticalPaths(m_hd, nPaths, Utils::CriticalPathHandler(&pb, &PathBuffering::DoIt));
  return pb.bufs;
}

int VanGinneken::CriticalPathBuffering(HCriticalPath aPath)
{
  int nBuf = 0; 
  HCriticalPathWrapper criticalPathW = m_hd[aPath];
  for(HCriticalPath::PointsEnumeratorW i = criticalPathW.GetEnumeratorW(); i.MoveNext() && !m_isFreeSpaceEnded; )
  {
    HPinWrapper pin = m_hd[m_hd.Get<HTimingPoint::Pin, HPin>(i.TimingPoint())]; 
    HNet net = pin.Net();
    m_nCriticalPaths++;
    if (m_hd.cfg.ValueOf(".isNetBufferNotDegradation", false))
      nBuf += NetBufferNotDegradation(net);
    else
      nBuf += InsertBuffers(net);
  }
  if (m_hd.cfg.ValueOf(".isLegalization", false))
    Legalization(DPGrid);
  return nBuf;
}

void VanGinneken::ParseFinalVanLocation(VanGinneken::Comp *com)
{
  if (com->left != 0) ParseFinalVanLocation(com->left);
  if (com->right != 0) ParseFinalVanLocation(com->right);

  if ((com->buffertype != 0) && (com->buffertype != -1))
    if (FindBufferNumberByIndex(com->y) == -1)
    {
      VGItem result;
      VGNode *node = &m_vgNetSplitted.GetSteinerPoint(com->y, m_vgNetSplitted, result, true);
      if (node->IsReal())
        if (node->IsSink())
         m_BufferIndexes.push_back(com->y - 1);
        else
          if (node->IsSource())
            m_BufferIndexes.push_back(com->y + 1);
          else
            m_BufferIndexes.push_back(com->y - 1);
      else
          m_BufferIndexes.push_back(com->y);
    }
}


int VanGinneken::BufferingTillDegradation()
{
  struct PathBuffering
  {
    VanGinneken& vg;
    double tns, wns;
    int bufs;
    int pathPack;
    PathBuffering(VanGinneken& VG, int pp): vg(VG), pathPack(pp), bufs(0), tns(DBL_MAX), wns(DBL_MAX){}

    bool DoIt(HDesign& hd, HCriticalPath path, int n)
    {
      bufs += vg.CriticalPathBuffering(path);
      if ((n % pathPack) == 0)
      {
        STA(hd, DO_NOT_REPORT);
        double tns2 = Utils::TNS(hd);
        double wns2 = Utils::WNS(hd);
        if (tns2 > tns && wns2 > wns)
          return false;
        tns = tns2;
        wns = wns2;
      }
      return true;
    }
  };

  WRITELINE("");
  ALERT("BUFFERING STARTED");

  int pathPack = m_hd.cfg.ValueOf(".pathPack", 1);
  int buffered = 0;

  if (pathPack > 0)
  {
    PathBuffering pb(*this, pathPack);
    Utils::IterateMostCriticalPaths(m_hd, Utils::ALL_PATHS, Utils::CriticalPathStopableHandler(&pb, &PathBuffering::DoIt));
    buffered = pb.bufs;
  }
  else
    buffered = BufferingOfMostCriticalPaths();

  ALERT("BUFFERING FINISHED");
  return buffered;
}

/* print list */
void VanGinneken::print(RLnode *x)
{
  int i;
  if (x == (RLnode *)NULL) {
    printf("empty tree\n");
    return;
  }
  printf("\tq=%10.10g\tc=%10.10g\n", x->time, x->cap*1000);
  //  if (debug)
  printbuffer(x->com, &i);	
  if (x->next != (RLnode *)NULL) 
    print(x->next);	 
}

/* print location */
void VanGinneken::printbuffer(Comp *x, int *i)
{
  if (x == (Comp *)NULL) {
    printf("empty tree\n");
    return;
  }
  if ( x->buffertype!=0 && x->buffertype!=-1 ) {
    printf( "start %d  end %d  buffertype %d\n", x->x, x->y, x->buffertype);
    *i=*i+1;
  }
  if (x->left != (Comp *)NULL)
    printbuffer(x->left, i);	
  if (x->right != (Comp *)NULL)
    printbuffer(x->right, i);
}

int VanGinneken::GetNCandidatesForBuffering()
{
  return m_nCandidatesForBuffering;
}

int VanGinneken::GetNReverts()
{
  return m_nReverts;
}