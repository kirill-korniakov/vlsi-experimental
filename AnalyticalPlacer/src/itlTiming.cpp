#include <stack>
#include <queue>
#include "../include/itlTiming.h"
#include "../include/RouteTrees.h"
#include <assert.h>
#include "../include/global.h"
#include <math.h>

//#define EXTRACT_RC(circuit,src,dest,R,C) ExtractRCexact(circuit,src,dest,R,C);
#define EXTRACT_RC(circuit,src,dest,R,C) ExtractRClumped(circuit,src,dest,R,C);


inline void ExtractRCexact(Circuit& c, StNodeEx* src, StNodeEx* dest, double& R, double &C)
{
  if(src->x == dest->x)
  {
    double L = src->y - dest->y;
    if(L < 0) L = -L;
    R = L * c.vLayer->RPerDist;
    C = L * c.vLayer->CPerDist + 2.0 * (L + c.vLayer->Width) * c.vLayer->EdgeCap;
  }
  else
  {
    double L = src->x - dest->x;
    if(L < 0) L = -L;
    R = L * c.hLayer->RPerDist;
    C = L * c.hLayer->CPerDist + 2.0 * (L + c.hLayer->Width) * c.hLayer->EdgeCap;
  }
}

inline void ExtractRClumped(Circuit& c, StNodeEx* src, StNodeEx* dest, double& R, double &C)
{
  double L = (src->x == dest->x) ? (src->y - dest->y) : (src->x - dest->x);
  if(L < 0) L = -L;
  R = L * c.hLayer->RPerDist;
  C = L * (c.hLayer->CPerDist + 2.0 * c.hLayer->EdgeCap);
}

void HPWLTiming(Circuit& circuit, Net& net)
{
  double R,C;
  StNodeEx first, second;
  first.x = second.x = 0.0;
  second.y = 0.0;
  first.y = net.tree->wl;
  EXTRACT_RC(circuit,&first,&second,R,C);
  int sourceInd = HASREALSOURCE(net) ? net.sourceIdx : 0;
  double totalC = 0.0;
  double Tbase = net.tree->srcRes * net.arrPins[sourceInd].routeInfo->cap
    + (net.tree->srcRes + R / 2.0) * C;
  for(int i = 0; i < net.numOfPins; i++)
    if(i != sourceInd)
    {
      net.arrPins[i].routeInfo->ExtractedC = C;
      net.arrPins[i].routeInfo->ExtractedR = R;
      totalC += net.arrPins[i].routeInfo->ObservedC = net.arrPins[i].routeInfo->cap;
      net.arrPins[i].routeInfo->NetPathDelay = Tbase
        + (net.tree->srcRes + R) * net.arrPins[i].routeInfo->ObservedC;
    }
    totalC += (net.numOfPins > 2) ? 2.0 * C : C;
    net.arrPins[sourceInd].routeInfo->ObservedC = totalC;
}

void ATreeTiming(Circuit& circuit, Net& Net)
{
  std::stack<StNodeEx*> NodeStack;
  StNodeEx* cur = Net.tree->nodes;
  cur->ObservedC = cur->cap;
  StNodeEx* next = cur;

  while(true)
  {
    while(next == cur && cur->left)
    {
      next = (StNodeEx*)cur->left;
      next->ObservedC = next->cap;
      EXTRACT_RC(circuit, cur, next, next->ExtractedR, next->ExtractedC);
      NodeStack.push(cur);
      cur = next;
    }
    if(next == cur && cur->right)
    {
      next = (StNodeEx*)cur->right;
      next->ObservedC = next->cap;
      EXTRACT_RC(circuit, cur, next, next->ExtractedR, next->ExtractedC);
      NodeStack.push(cur);
      cur = next;
      continue;
    }
    else
    {
      if(NodeStack.empty()) break;
      next = cur;
      cur = NodeStack.top();
      cur->ObservedC += next->ObservedC + next->ExtractedC;
      if(next == cur->left && cur->right)
      {
        next = (StNodeEx*)cur->right;
        next->ObservedC = next->cap;
        EXTRACT_RC(circuit, cur, next, next->ExtractedR, next->ExtractedC);
        cur = next;
        continue;
      }
      else
        NodeStack.pop();
    }
  }

  cur = Net.tree->nodes;
  cur->NetPathDelay = Net.tree->srcRes * cur->ObservedC;
  while(true)
  {
    while(cur->left)
    {
      next = (StNodeEx*)cur->left;
      if(cur->right)
        NodeStack.push(cur);
      next->NetPathDelay = cur->NetPathDelay + next->ExtractedR * (next->ExtractedC * 0.5 + next->ObservedC);
      cur = next;
    }
    if(cur->right)
    {
      next = (StNodeEx*)cur->right;
      next->NetPathDelay = cur->NetPathDelay + next->ExtractedR * (next->ExtractedC * 0.5 + next->ObservedC);
      cur = next;
      continue;
    }
    else
    {
      if(NodeStack.empty())
        break;
      cur = NodeStack.top();
      NodeStack.pop();
      next = (StNodeEx*)cur->right;
      next->NetPathDelay = cur->NetPathDelay + next->ExtractedR * (next->ExtractedC * 0.5 + next->ObservedC);
      cur = next;
    }
  }
}

void CalculateDalays(Circuit& circuit, Net& Net)
{
  if((Net.tree->RoutingType & A_TREE))
  {
    if(HASREALSOURCE(Net))
      ATreeTiming(circuit,Net);
    else
      HPWLTiming(circuit,Net);
  }
  else
    if((Net.tree->RoutingType & HPWL_STUB) == HPWL_STUB)
      HPWLTiming(circuit,Net);
}

void PrintPrev(Circuit& c, int nodeID)
{
  cout << "---------" << endl;
  cout << c.tableOfNames[nodeID].name << "   " << nodeID << endl;
  cout << "---------" << endl;
  for(size_t i = 0; i < c.tableOfConnections[nodeID].size(); i++)
  {
    int netID = c.tableOfConnections[nodeID][i];
    cout << "Net " << netID << " - " << c.nets[netID].name << endl;
    for(int j = 0; j < c.nets[netID].numOfPins; j++)
      if(c.nets[netID].arrPins[j].chtype == 'O')
        cout << "Source: " << c.nets[netID].arrPins[j].cellIdx 
        << " - "  << c.tableOfNames[c.nets[netID].arrPins[j].cellIdx].name << endl
        << "---------" << endl;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void MakeTimingLists(Circuit& c)
{
  int* netDegrees = new int[c.nNets];//number of input pins per net
  int* netDegrees_2 = new int[c.nNets];
  int* nodeDegrees = new int[c.Shift_ + c.nTerminals];//number of output pins per node
  for(int q = 0; q < c.Shift_ + c.nTerminals; q++)
    nodeDegrees[q] = 0;
  for(int i = 0; i < c.nNets; i++)
  {
    netDegrees[i] = 0;
    for(int l = 0; l < c.nets[i].numOfPins; l++)
      if(c.nets[i].arrPins[l].chtype != 'O')
        netDegrees[i]++;
    if(HASREALSOURCE(c.nets[i]))
      nodeDegrees[c.nets[i].arrPins[c.nets[i].sourceIdx].cellIdx]++;
  }
  for(int p = 0; p < c.nNets; p++)
    netDegrees_2[p] = netDegrees[p];

  Net __top;
  Net* current = &__top;

  //propagate by required order
  //inverced list - is arrival order
  std::queue<int> readyNodes;
  std::queue<int> readyNets;

  for(int k = 0; k < c.nPrimaryOutputs; k++)
    readyNodes.push(c.primaryOutputs[k]);

  for(int f = 0; f < c.nNodes; f++)
    if(c.nodes[f].type->Type == MacroInfo::FlipFlop || c.nodes[f].type->Type == MacroInfo::Latch)
      readyNodes.push(f);

  while(!readyNodes.empty() || !readyNets.empty())
  {
    while(!readyNodes.empty())
    {
      int nodeIndex = readyNodes.front();
      readyNodes.pop();
      //cout << c.tableOfNames[nodeIndex].name << " - " << nodeIndex << endl;
      for(size_t j = 0; j < c.tableOfConnections[nodeIndex].size(); j++)
      {
        int netID = c.tableOfConnections[nodeIndex][j];
        if(HASREALSOURCE(c.nets[netID]))
          if(c.nets[netID].arrPins[c.nets[netID].sourceIdx].cellIdx != nodeIndex)
            if(!(netDegrees[netID]-=1))
              readyNets.push(netID);
      }
    }
    while(!readyNets.empty())
    {
      int netIndex = readyNets.front();
      readyNets.pop();
      if(HASREALSOURCE(c.nets[netIndex]))
      {
        if(!(nodeDegrees[c.nets[netIndex].arrPins[c.nets[netIndex].sourceIdx].cellIdx]-=1))
          readyNodes.push(c.nets[netIndex].arrPins[c.nets[netIndex].sourceIdx].cellIdx);
      }
      current->requiredOrder = c.nets + netIndex;
      current->requiredOrder->arrivalOrder = current;
      current = current->requiredOrder;
    }
    if(readyNodes.empty())
    {
      for(int j = 0; j < c.nNets; j++)
        if(netDegrees[j] > 0 && netDegrees[j] < netDegrees_2[j])
        {
          netDegrees[j] = 0;
          readyNets.push(j);
          cout << "WARNING: Timing - net " << c.nets[j].name << " was broken." << endl;
          break;
        }
    }
  }

  c.firstRequired = __top.requiredOrder;
  c.firstArrival = current;
  c.firstArrival->requiredOrder = 0;
  c.firstRequired->arrivalOrder = 0;

  delete[] netDegrees;
  delete[] nodeDegrees;
  delete[] netDegrees_2;
}

inline double GetArrivalTime(Circuit& c, int node)
{
  if(c.nodes[node].type->Type == MacroInfo::FlipFlop || c.nodes[node].type->Type == MacroInfo::Latch)
    return 0.0;//arrival time for flip-flops == setup time
  else
    return c.placement[node].arrivalTime;
}

void PropagateArrivalTime(Circuit& circuit, bool reanalize, bool reroute)
{
  const double minimalArrival = 0.0;
  const double pi_arrival_time = 0.0;
  //int critPathCellIdx;

  for(int i = 0; i < circuit.nNodes; i++)
    circuit.placement[i].arrivalTime = minimalArrival;

  for(int j = 0; j < circuit.nPrimaryOutputs; j++)//time on primary inputs predefined
    circuit.placement[circuit.primaryOutputs[j]].arrivalTime = pi_arrival_time;

  for(Net* current = circuit.firstArrival; current; current = current->arrivalOrder)
  {
    if(reroute) AdaptiveRoute(circuit,*current);
    if(reanalize) CalculateDalays(circuit,*current);

    double base = pi_arrival_time;
    if(HASREALSOURCE((*current)))
      base = GetArrivalTime(circuit, current->arrPins[current->sourceIdx].cellIdx);
    int hMax = 0;
    for(int h = 0; h < current->numOfPins; h++)
    {
      if(h == current->sourceIdx) continue;
      double time = base + current->arrPins[h].routeInfo->NetPathDelay;
      
      if(circuit.placement[current->arrPins[h].cellIdx].arrivalTime < time)
      {
        circuit.placement[current->arrPins[h].cellIdx].arrivalTime = time;
        //circuit.placement[current->arrPins[h].cellIdx].critNetIdx = current;
        //hMax = h;
        //critPathCellIdx = current->arrPins[h].cellIdx;
      }
    }
    //circuit.placement[current->arrPins[hMax].cellIdx].critNetIdx = current;
  }

  /*for (int i = 0; i < circuit.nNodes; ++i)
  {
    if (circuit.placement[i].critNetIdx)
    {
      circuit.placement[i].critNetIdx->nCritPaths++;
    }
  }*/
}

inline double GetRequiredTime(Circuit& c, int node)
{
  if(c.nodes[node].type->Type == MacroInfo::FlipFlop || c.nodes[node].type->Type == MacroInfo::Latch)
    return 0.0;//required time for flip-flops
  else
    return c.placement[node].requiredTime;
}

void PropagateRequiredTime(Circuit& circuit, bool reanalize, bool reroute)
{
  const double maximumRequired = 0.0;
  const double primary_required = 0.0;

  for(int i = 0; i < circuit.nNodes; i++)
    circuit.placement[i].requiredTime = maximumRequired;

  for(int j = 0; j < circuit.nPrimaryInputs; j++)//time on primary outputs predefined
    circuit.placement[circuit.primaryInputs[j]].requiredTime = primary_required;

  for(Net* current = circuit.firstRequired; current; current = current->requiredOrder)
  {
    if(reroute) AdaptiveRoute(circuit,*current);
    if(reanalize) CalculateDalays(circuit,*current);
    //PrintTimingReport(circuit,current->name);
    //PrintTree(current->tree,stdout);
    //assert(HASREALSOURCE((*current)));
    if(!HASREALSOURCE((*current)))
      continue;

    double minTime = maximumRequired;
    for(int h = 0; h < current->numOfPins; h++)
    {
      if(h == current->sourceIdx) continue;
      double req = GetRequiredTime(circuit,current->arrPins[h].cellIdx)
        - current->arrPins[h].routeInfo->NetPathDelay;
      if(req < minTime) minTime = req;
    }
    if(circuit.placement[current->arrPins[current->sourceIdx].cellIdx].requiredTime > minTime)
      circuit.placement[current->arrPins[current->sourceIdx].cellIdx].requiredTime = minTime;
  }
}

void PrintCircuitArrivals(Circuit& circuit)
{
  cout << "Arrival times on primary outputs:" << endl;
  for(int i = 0; i < circuit.nPrimaryOutputs; i++)
    cout << circuit.tableOfNames[circuit.primaryOutputs[i]].name << "\t"
    << circuit.placement[circuit.primaryOutputs[i]].arrivalTime << endl;
}

void PrintFFArrivals(Circuit& circuit)
{
  cout << "Arrival times on flip-flops && latches:" << endl;
  for(int i = 0; i < circuit.nNodes; i++)
    if(circuit.nodes[i].type->Type == MacroInfo::FlipFlop || circuit.nodes[i].type->Type == MacroInfo::Latch)
      cout << circuit.tableOfNames[i].name << "\t"
      << circuit.placement[i].arrivalTime << endl;
}

void PrintCircuitRequireds(Circuit& circuit)
{
  cout << "Required times on primary inputs:" << endl;
  for(int i = 0; i < circuit.nPrimaryInputs; i++)
    cout << circuit.tableOfNames[circuit.primaryInputs[i]].name << "\t"
    << circuit.placement[circuit.primaryInputs[i]].requiredTime << endl;
}

void PrintFFRequireds(Circuit& circuit)
{
  cout << "Required times on flip-flops && latches:" << endl;
  for(int i = 0; i < circuit.nNodes; i++)
    if(circuit.nodes[i].type->Type == MacroInfo::FlipFlop || circuit.nodes[i].type->Type == MacroInfo::Latch)
      cout << circuit.tableOfNames[i].name << "\t"
      << circuit.placement[i].requiredTime << endl;
}

void PrintCircuitSlackResults(Circuit& c)
{
  cout << "Primary outputs:" << endl;
  for(int i = 0; i < c.nPrimaryOutputs; i++)
    cout << c.tableOfNames[c.primaryOutputs[i]].name << "\t"
    << c.placement[c.primaryOutputs[i]].arrivalTime << endl;
  cout << "Flip-flops & latches:" << endl;
  for(int i = 0; i < c.nNodes; i++)
    if(c.nodes[i].type->Type == MacroInfo::FlipFlop || c.nodes[i].type->Type == MacroInfo::Latch)
      cout << c.tableOfNames[i].name << "\t"
      << c.placement[i].arrivalTime - c.placement[i].requiredTime << endl;
}

void PrintTimingReport(Circuit& circuit, char* filename)
{
  FILE* out = fopen(filename,"w");
  for(int i = 0; i < circuit.Shift_ + circuit.nTerminals; i++)
  {
    if(i == circuit.nNodes) i = circuit.Shift_;
    fprintf(out,"%s - %d\n",circuit.tableOfNames[i].name,i);
    fprintf(out,"\tarrival:  %8.3f\n",circuit.placement[i].arrivalTime);
    fprintf(out,"\trequired: %8.3f\n",circuit.placement[i].requiredTime);
  }
  fclose(out);
}

void PrintNets(Circuit& circuit, char* filename)
{
  FILE* out = fopen(filename,"w");
  for(int i = 0; i < circuit.nNets; i++)
  {
    fprintf(out,"%s - %d\n",circuit.nets[i].name,i);
    for(int j = 0; j < circuit.nets[i].numOfPins; j++)
    {
      fprintf(out,"\t%2s - %c - %d\n",circuit.nets[i].arrPins[j].type->Name.c_str(),circuit.nets[i].arrPins[j].chtype,circuit.nets[i].arrPins[j].cellIdx);
    }
  }
  fclose(out);
}

void PrintRequiredOrder(Circuit& circuit)
{
  Net* cur = circuit.firstRequired;
  while(cur)
  {
    cout << cur->name << " - " << (int)(cur - circuit.nets) << endl;
    cur = cur->requiredOrder;
  }
}

void PrintArrivalOrder(Circuit& circuit)
{
  Net* cur = circuit.firstArrival;
  while(cur)
  {
    cout << cur->name << " - " << (int)(cur - circuit.nets) << endl;
    cur = cur->arrivalOrder;
  }
}

void ComputeNetWeights(Circuit& circuit)
{
  if (!gOptions.isLEFDEFinput)
  {
    cout << "Skipping computing net-weights step\n";
    return;
  }
  
  FindCriticalPaths(circuit);

  std::vector<int>* tableOfCritPaths;  // what critical path is the current net included in
  // the idea is the same as in tableOfConnections
  tableOfCritPaths = new std::vector<int>[circuit.nNets];
  for (int i = 0; i < circuit.nPrimaryOutputs; ++i)
  {
    for (int j = 0; j < circuit.criticalPaths[i].size(); ++j)
    {
      tableOfCritPaths[circuit.criticalPaths[i][j]].push_back(i);
    }
  }

  /*for (int i = 0; i < circuit.nNets; ++i)
  {
    if (tableOfCritPaths[i].size() == 0)
    {
      continue;
    }
    cout << "netIdx = " << i << ", ";
    for (int j = 0; j < tableOfCritPaths[i].size(); ++j)
    {
      cout << tableOfCritPaths[i][j] << "\t";
    }
    cout << endl;
  }*/

  double maxPathDelay = 0.0;

  for (int i = 0; i < circuit.nPrimaryOutputs; ++i)
  {
  	if (maxPathDelay < circuit.placement[circuit.primaryOutputs[i]].arrivalTime)
  	{
      maxPathDelay = circuit.placement[circuit.primaryOutputs[i]].arrivalTime;
  	}
  }
  
  double u = 0.3;
  double beta = 9;
  double T = (1 - u) * maxPathDelay;
  double sum = 0.0;
  /*for (int i = 0; i < circuit.nNets; ++i)
  {
    sum = 0.0;
    for (int j = 0; j < tableOfCritPaths[i].size(); ++j)
    {
      int poIdx = tableOfCritPaths[i][j];
    	sum += D(T - circuit.placement[circuit.primaryOutputs[poIdx]].arrivalTime, T, beta) - 1;
    }
    circuit.netWeights[i] = 1.0 + sum;
  }*/
  for (int i = 0; i < circuit.nNets; ++i)
  {
    circuit.netWeights[i] = 1.0 + circuit.nets[i].nCritPaths;
  }

  //double* nodesSlack = new double[circuit.Shift_ + circuit.nTerminals];
  //double sumNodesSlack; // сумма слэков нодов в одном нете
  //double maxNodeSlack = -1e10;
  //int nodeIdx;

  //for (int i = 0; i < circuit.Shift_ + circuit.nTerminals; ++i)
  //{
  //  nodesSlack[i] = circuit.placement[i].arrivalTime - circuit.placement[i].requiredTime;
  //  if (maxNodeSlack < nodesSlack[i])
  //  {
  //    maxNodeSlack = nodesSlack[i];
  //  }
  //}

  //for (int i = 0; i < circuit.nNets; ++i)
  //{
  //  circuit.netWeights[i] = 1.0;
  //  sumNodesSlack = 0.0;

  //  for (int j = 0; j < circuit.nets[i].numOfPins; ++j)
  //  {
  //    nodeIdx = circuit.nets[i].arrPins[j].cellIdx;
  //    //if (nodeIdx >= circuit.Shift_)
  //    sumNodesSlack += nodesSlack[nodeIdx];
  //  }

  //  circuit.netWeights[i] += sumNodesSlack / maxNodeSlack;
  //}

  //delete[] nodesSlack;
}

double D(double s, double T, double beta)
{
  if (s < 0)
  {
    return pow(1 - s / T, beta);
  } 
  else
  {
    return 1.0;
  }
}

void ExportToGraphViz(Circuit& c, char* filename)
{
  FILE* out = fopen(filename,"w");
  fprintf(out,"digraph %s {\n",c.designName.name);
  fprintf(out,"node [shape = doublecircle]; _");
  for(int pi = 0; pi < c.nPrimaryInputs; pi++)
    fprintf(out,",%s",c.tableOfNames[c.primaryInputs[pi]].name);
  fprintf(out,";\n");
  fprintf(out,"node [shape = circle]; ");
  for(int po = 0; po < c.nPrimaryOutputs; po++)
  {
    if(po > 0) fprintf(out,",");
    fprintf(out,"%s",c.tableOfNames[c.primaryOutputs[po]].name);
  }
  fprintf(out,";\n");
  fprintf(out,"node [shape = hexagon]; ");
  int nff = 0;
  for(int i = 0; i < c.nNodes; i++)
    if(c.nodes[i].type->Type == MacroInfo::FlipFlop || c.nodes[i].type->Type == MacroInfo::Latch)
    {
      if(nff > 0) fprintf(out,",");
      fprintf(out,"%s",c.tableOfNames[i].name);
      nff++;
    }

    fprintf(out,";\nnode [shape = square];\nrankdir=TD;\n");

    for(int j =0; j < c.nNets; j++)
    {
      char* src_name = "_";
      if(HASREALSOURCE(c.nets[j]))
        src_name = c.tableOfNames[c.nets[j].arrPins[c.nets[j].sourceIdx].cellIdx].name;
      for(int k = 0; k < c.nets[j].numOfPins; k++)
        if(c.nets[j].arrPins[k].chtype == 'I')
          fprintf(out,"%s->%s [label=""%s""]\n",
          src_name,
          c.tableOfNames[c.nets[j].arrPins[k].cellIdx].name,
          c.nets[j].name);
    }

    fprintf(out,"}");
    fclose(out);
}

void FindCriticalPaths(Circuit& circuit)
{
  int currNetIdx;
  int currNodeIdx;
  double base  = 0.0;
  double delay = 0.0;
  bool isBeginningReached = false;

  MakeTimingLists(circuit);
  PropagateArrivalTime(circuit, true, true);
  PrintCircuitArrivals(circuit);

  circuit.criticalPaths = new std::vector<int>[circuit.nPrimaryOutputs];

  for (int poIdx = 0; poIdx < circuit.nPrimaryOutputs; poIdx++)  // poIdx - primary output index
  {
    //cout << poIdx << "\t";
    currNodeIdx = circuit.primaryOutputs[poIdx];
    isBeginningReached = false;
    while (!isBeginningReached)
    {
      int nNets = circuit.tableOfConnections[currNodeIdx].size();
      
      if (nNets == 0) // strange case but it does happen
      {
        break;
      }
      
      for (int i = 0; i < nNets; ++i)
      {
        currNetIdx = circuit.tableOfConnections[currNodeIdx][i];

        int nodeIdxInNet = -1;

        for (int j = 0; j < circuit.nets[currNetIdx].numOfPins; ++j)
        {
          if (circuit.nets[currNetIdx].arrPins[j].cellIdx == currNodeIdx)
          {
            nodeIdxInNet = j;
            break;
          }
        }

        if (circuit.nets[currNetIdx].arrPins[nodeIdxInNet].chtype == 'O')
        {
          if (i == nNets - 1) // we've reached the beginning of the critical path
          {
            isBeginningReached = true;
            break;
          }
          continue;
        }

        AdaptiveRoute(circuit, circuit.nets[currNetIdx]);
        CalculateDalays(circuit, circuit.nets[currNetIdx]);
      
        base  = GetArrivalTime(circuit, circuit.nets[currNetIdx].arrPins[circuit.nets[currNetIdx].sourceIdx].cellIdx);
        delay = circuit.nets[currNetIdx].arrPins[nodeIdxInNet].routeInfo->NetPathDelay;

        if (base + delay == circuit.placement[currNodeIdx].arrivalTime)
        {
          currNodeIdx = circuit.nets[currNetIdx].arrPins[circuit.nets[currNetIdx].sourceIdx].cellIdx;
          circuit.criticalPaths[poIdx].push_back(currNetIdx);
          circuit.nets[currNetIdx].nCritPaths++;
          break;
        } 
        else
          continue;
      }
    }
  }

  /*cout << "These are the critical paths:\n" << endl;
  for (int i = 0; i < circuit.nPrimaryOutputs; ++i)
  {
    for (int j = 0; j < circuit.criticalPaths[i].size(); ++j)
    {
    	cout << circuit.criticalPaths[i][j] << "\t"; 
    }
  	cout << endl;
  }*/
}




















