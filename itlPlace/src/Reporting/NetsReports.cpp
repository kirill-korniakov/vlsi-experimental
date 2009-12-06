#include "Reporting.h"
#include "Utils.h"
#include "AdaptiveRoute.h"

void ReportNetPins(HDesign& design, HNet net)
{
  WRITELINE("Net %s details:", design.GetString<HNet::Name>(net).c_str());
  for (HNet::PinsEnumeratorW pin = design.Get<HNet::Pins, HNet::PinsEnumeratorW>(net); pin.MoveNext(); )
  {
    WRITELINE("  %s(%s) %s %s",
      design.GetString<HCell::Name>(pin.Cell()).c_str(),
      design.GetString<HMacroType::Name>(design.Get<HCell::MacroType, HMacroType>(pin.Cell())).c_str(),
      pin.Name().c_str(),
      Utils::GetPinDirectionStr(pin.Direction())
      );
  }
}

void ReportNetPinsCoordinates(HDesign& design, HNet net)
{
  WRITELINE("Net %s pins:", design.GetString<HNet::Name>(net).c_str());
  for (HNet::PinsEnumeratorW pin = design.Get<HNet::Pins, HNet::PinsEnumeratorW>(net); pin.MoveNext(); )
  {
    WRITELINE("  %s\tx = %f\ty = %f",
      pin.Name().c_str(),
      design.GetDouble<HPin::X>(pin),
      design.GetDouble<HPin::Y>(pin)
      );
  }
}

void ReportNetsInfo(HDesign& design) 
{
  int maxPinCount = 0;
  for (HNets::NetsEnumeratorW nIter = design.Nets.GetFullEnumeratorW(); nIter.MoveNext(); )
  {
    if (nIter.PinsCount() > maxPinCount)
      maxPinCount = nIter.PinsCount();
  }

  int *nNetsWithPins = new int [maxPinCount + 1];

  for (int j = 0; j < maxPinCount + 1; j++)
  {

    nNetsWithPins[j] = 0;
  }

  int index = 0;

  for (HNets::NetsEnumeratorW nIter = design.Nets.GetFullEnumeratorW(); nIter.MoveNext(); )
  {
    nNetsWithPins[nIter.PinsCount()]++;
  }

  WRITELINE("");
  WRITELINE("Reporting: Number pins in nets\n");
  for (int i = 0; i < maxPinCount + 1; i++)
    if (nNetsWithPins[i] != 0)
      WRITELINE("  Number of nets with\t%d\tpin =\t%d", i, nNetsWithPins[i]);

   delete [] nNetsWithPins;
}

void ReportCountNetsWithTypes(HDesign& design)
{
  WRITELINE("");
  WRITELINE("Reporting: count nets with type : all, skipped, buffered, removed, Active\n");
  WRITELINE("  Nets count all = %d", design.Nets.Count());
  WRITELINE("  Nets with active type   = %d", design.Nets.Count(NetKind_Active));
  WRITELINE("  Nets with skipped type  = %d", design.Nets.Count(NetKind_Skipped));
  WRITELINE("  Nets with buffered type = %d", design.Nets.Count(NetKind_Buffered));
  WRITELINE("  Nets with removed type  = %d", design.Nets.Count(NetKind_Removed));
}

void ReportNetTiming(HDesign& design, HNet net)
{
  HNetWrapper netW = design[net]; 
  double RAT, AAT, NS;
  string pinName;
  ALERTFORMAT(("Reporting net %s timing", netW.Name().c_str()));
  
  for (HNetWrapper::PinsEnumeratorW currPin = netW.GetPinsEnumeratorW(); currPin.MoveNext();)
  {
    HTimingPointWrapper tp = design[design.TimingPoints[currPin]];
    RAT = tp.RequiredTime();
    AAT = tp.ArrivalTime();
    NS = tp.NegativeSlack();

    if (currPin.IsPrimary())
      pinName = currPin.Name();
    else
      pinName = design.Cells.GetString<HCell::Name>(currPin.Cell()) + "." + currPin.Name();

    ALERTFORMAT(("RAT at %s\t= %.10f", pinName.c_str(), RAT));
    ALERTFORMAT(("AAT at %s\t= %.10f", pinName.c_str(), AAT));
    ALERTFORMAT(("NS at %s\t= %.10f", pinName.c_str(), NS));
  }
}

int SteinerTreeReport(HDesign& design, HNet net, bool isReport)
{
  design.Plotter.PlotNetSteinerTree(net, Color_Black);
  design.Plotter.Refresh(design.Plotter.WAIT_3_SECONDS);
  design.Plotter.Clear();
  design.Plotter.ShowPlacement();

  AdaptiveRoute(design, net);

  HNetWrapper netw = design[net];
  HPinWrapper src =  design[netw.Source()];
  HSteinerPointWrapper srcPoint = design[design.SteinerPoints[src]];
  HSteinerPointWrapper nextPoint = srcPoint;
  double xi1 = 0, yi1 = 0, xi2 = 0, yi2 = 0, xj1 = 0, yj1 = 0, xj2 = 0, yj2 = 0, Ai = 0, Aj = 0, Bi = 0, Bj = 0, x = 0, y =0;
  bool isBad = false;
  int crossing = 0, onlyRight = 0, noTree = 0;

  //m_hd.Plotter.DrawCircle(srcPoint.X(), srcPoint.Y(), 4, color);
  if (!(srcPoint.HasLeft() || srcPoint.HasRight()))
    noTree++;

  TemplateTypes<HSteinerPoint>::stack points;
  points.push(srcPoint);
  
  while (!points.empty())
  {
    isBad = false;
    srcPoint = points.top();
    xi1 = srcPoint.X();
    yi1 = srcPoint.Y();
    points.pop();

    if (srcPoint.HasLeft())
    {
      nextPoint = srcPoint.Left();
      xi2 = nextPoint.X();
      yi2 = nextPoint.Y();
      points.push(nextPoint);
      
      Ai = (yi2 - yi1)/(xi2 - xi1);
      Bi = yi1 - Ai * xi1;


      //начало1
      TemplateTypes<HSteinerPoint>::stack points2;
      HSteinerPointWrapper srcPoint2 = design[design.SteinerPoints[src]];
      HSteinerPointWrapper nextPoint2 = srcPoint2;
      points2.push(srcPoint2);
      while (!points2.empty())
      {
        srcPoint2 = points2.top();
        xj1 = srcPoint2.X();
        yj1 = srcPoint2.Y();
        points2.pop();

        if (srcPoint2.HasLeft())
        {
          nextPoint2 = srcPoint2.Left();
          xj2 = nextPoint2.X();
          yj2 = nextPoint2.Y();
          points2.push(nextPoint2);

          Aj = (yj2 - yj1)/(xj2 - xj1);
          Bj = yj1 - Aj * xj1;

          if (srcPoint2.HasRight())
          {
            nextPoint2 = srcPoint2.Right();
            xj2 = nextPoint2.X();
            yj2 = nextPoint2.Y();
            points2.push(nextPoint2);

            Aj = (yj2 - yj1)/(xj2 - xj1);
            Bj = yj1 - Aj * xj1;

          }
        }
      }
      //конец1
     


      if (srcPoint.HasRight())
      {
        nextPoint = srcPoint.Right();
        xi2 = nextPoint.X();
        yi2 = nextPoint.Y();
        points.push(nextPoint);

        Ai = (yi2 - yi1)/(xi2 - xi1);
        Bi = yi1 - Ai * xi1;

        //начало2
        TemplateTypes<HSteinerPoint>::stack points2;
        HSteinerPointWrapper srcPoint2 = design[design.SteinerPoints[src]];
        HSteinerPointWrapper nextPoint2 = srcPoint2;
        points2.push(srcPoint2);
        while (!points2.empty())
        {
          srcPoint2 = points2.top();
          xj1 = srcPoint2.X();
          yj1 = srcPoint2.Y();
          points2.pop();

          if (srcPoint2.HasLeft())
          {
            nextPoint2 = srcPoint2.Left();
            xj2 = nextPoint2.X();
            yj2 = nextPoint2.Y();
            points2.push(nextPoint2);

            Aj = (yj2 - yj1)/(xj2 - xj1);
            Bj = yj1 - Aj * xj1;

            if (srcPoint2.HasRight())
            {
              nextPoint2 = srcPoint2.Right();
              xj2 = nextPoint2.X();
              yj2 = nextPoint2.Y();
              points2.push(nextPoint2);

              Aj = (yj2 - yj1)/(xj2 - xj1);
              Bj = yj1 - Aj * xj1;

            }
          }
        }
        //конец2


      }
    }
    else
    {
      if (srcPoint.HasRight())
      {
        onlyRight++;
        isBad = true;
      }
      //design.Plotter.DrawCircle(srcPoint.X(), srcPoint.Y(), 1, color);
    }
    if (!isBad)
    {
      x = (Bj - Bi) / (Ai - Aj);
      y = Ai * x + Bi;
      if (((((x < xi1) && (x > xi2)) || ((x < xi2) && (x > xi1))) &&
          (((x < xj1) && (x > xj2)) || ((x < xj2) && (x > xj1)))) &&
         ((((y < yi1) && (y > yi2)) || ((y < yi2) && (y > yi1))) &&
          (((y < yj1) && (y > yj2)) || ((y < yj2) && (y > yj1)))))
      {
        crossing++;
      }

    }


  }
  if (isReport)
  {
    ALERTFORMAT(("net = %s", netw.Name().c_str()))
    ALERTFORMAT(("crossing\t=%d", crossing));
    ALERTFORMAT(("onlyRight\t=%d", onlyRight));
    ALERTFORMAT(("noTree\t=%d", noTree));
  }
  return crossing;
  //начало
 /* TemplateTypes<HSteinerPoint>::stack points2;
  HSteinerPointWrapper srcPoint2 = design[design.SteinerPoints[src]];
  HSteinerPointWrapper nextPoint2 = srcPoint;
  while (!points.empty())
  {
    srcPoint2 = points2.top();
    points2.pop();

    if (srcPoint2.HasLeft())
    {
      nextPoint2 = srcPoint2.Left();
      points.push(nextPoint2);

      if (srcPoint2.HasRight())
      {
        nextPoint2 = srcPoint2.Right();
        points2.push(nextPoint2);
      }
    }
  }*/
  //конец
  

}
void SteinerWoodReport(HDesign& design)
{
  ALERTFORMAT(("Steiner wood report:"))
  int crossing = 0; 
  for (HNets::ActiveNetsEnumeratorW net = design.Nets.GetActiveNetsEnumeratorW(); net.MoveNext(); )
    crossing += SteinerTreeReport(design, net);  
  ALERTFORMAT(("crossing = %d", crossing));
}