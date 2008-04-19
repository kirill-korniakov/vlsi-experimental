#include <stack>
#include <queue>
#include "itlTiming.h"
#include "RouteTrees.h"
#include <assert.h>
#include "global.h"

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
            net.arrPins[i].routeInfo->ArrivalTime = Tbase
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
    cur->ArrivalTime = Net.tree->srcRes * cur->ObservedC;
    while(true)
    {
        while(cur->left)
        {
            next = (StNodeEx*)cur->left;
            if(cur->right)
                NodeStack.push(cur);
            next->ArrivalTime = cur->ArrivalTime + next->ExtractedR * (next->ExtractedC * 0.5 + next->ObservedC);
            cur = next;
        }
        if(cur->right)
        {
            next = (StNodeEx*)cur->right;
            next->ArrivalTime = cur->ArrivalTime + next->ExtractedR * (next->ExtractedC * 0.5 + next->ObservedC);
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
            next->ArrivalTime = cur->ArrivalTime + next->ExtractedR * (next->ExtractedC * 0.5 + next->ObservedC);
            cur = next;
        }
    }
}

void CalculateDalays(Circuit& circuit, Net& Net)
{
    if(Net.tree->RoutingType & A_TREE)
        ATreeTiming(circuit,Net);
    else
        if(Net.tree->RoutingType & HPWL_STUB)
            HPWLTiming(circuit,Net);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//true if node with "nodeIndex" index contains source of net with "netIndex" index 
//bool IsSource(Circuit& c, int nodeIndex, int netIndex)
//{
//    if(c.nets[netIndex].sourceIdx >= 0)
//        return c.nets[netIndex].arrPins[c.nets[netIndex].sourceIdx].cellIdx == nodeIndex;
//    for(int i = 0; i < c.nets[netIndex].numOfPins; i++)
//        if(c.nets[netIndex].arrPins[i].chtype == 'O')
//        {
//            c.nets[netIndex].sourceIdx = i;
//            if(c.nets[netIndex].arrPins[i].cellIdx == nodeIndex)
//                return true;
//            else
//                return false;//don't support multiple sources
//        }
//    if(c.nets[netIndex].arrPins[0].cellIdx == nodeIndex)
//    {
//        c.nets[netIndex].sourceIdx = 0;
//        return true;
//    }
//    return false;
//}
//
//void Initialize(Circuit& circuit)
//{
//    
//
//    //calculate number of input dependencies for each node
//    int totalElements = circuit.nNodes+circuit.nTerminals;
//    int* waitedNetsCount = new int[totalElements];
//    for(int q = 0; q < totalElements; q++) 
//    {
//        waitedNetsCount[q] = 0;
//        for(size_t t = 0; t < circuit.tableOfConnections[q].size(); t++)
//            if(!IsSource(circuit,q,circuit.tableOfConnections[q][t]))
//                waitedNetsCount[q]++;
//    }
//
//    Net __top;
//    Net* current = &__top;
//
//    //perform queues for determining net order
//    std::queue<int> readyNets;
//    std::queue<int> readyNodes;
//    for(int a1 = 0; a1 < circuit.nPrimaryInputs; a1++)
//        readyNodes.push(circuit.primaryInputs[a1]);
//
//    while(!readyNodes.empty())
//    {
//        while(!readyNodes.empty())
//        {
//            int nodeIndex = readyNodes.front();
//            readyNodes.pop();
//            for(size_t k = 0; k < circuit.tableOfConnections[nodeIndex].size(); k++)
//                if(IsSource(circuit,nodeIndex,circuit.tableOfConnections[nodeIndex][k]))
//                    readyNets.push(circuit.tableOfConnections[nodeIndex][k]);
//        }
//        while(!readyNets.empty())
//        {
//            int netIndex = readyNets.front();
//            readyNets.pop();
//            current = current->arrivalOrder = circuit.nets + netIndex;
//            bool hasRealSource = false;
//            for(int k = 1; k < current->numOfPins; k++)
//                if(current->arrPins[k].chtype == 'O')
//                    hasRealSource = true;
//                else
//                    if(!(waitedNetsCount[current->arrPins[k].cellIdx]-=1))
//                        readyNodes.push(current->arrPins[k].cellIdx);
//            if(hasRealSource)
//                if(!(waitedNetsCount[current->arrPins[0].cellIdx]-=1))
//                        readyNodes.push(current->arrPins[0].cellIdx);
//        }
//    }
//    circuit.firstArrival = __top.arrivalOrder;
//    delete[] waitedNetsCount;
//}

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
    int* netDegrees = new int[c.nNets];
    int* netDegrees_2 = new int[c.nNets];
    int* nodeDegrees = new int[c.nNodes + c.nTerminals];
    for(int q = 0; q < c.nNodes + c.nTerminals; q++)
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
            {//completely ignore nets without real source
                if(!(nodeDegrees[c.nets[netIndex].arrPins[c.nets[netIndex].sourceIdx].cellIdx]-=1))
                    readyNodes.push(c.nets[netIndex].arrPins[c.nets[netIndex].sourceIdx].cellIdx);

                current->requiredOrder = c.nets + netIndex;
                current->requiredOrder->arrivalOrder = current;
                current = current->requiredOrder;
            }
        }
        if(readyNodes.empty())
        {
            for(int j = 0; j < c.nNets; j++)
                if(netDegrees[j] > 0 && netDegrees[j] < netDegrees_2[j])
                {
                    netDegrees[j] = 0;
                    readyNets.push(j);
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

void PropagateArrivalTime(Circuit& circuit, bool reanalize, bool reroute)
{
    const double minimalArrival = 0.0;

    for(int i = 0; i < circuit.nNodes; i++)
        circuit.placement[i].arrivalTime = minimalArrival;
    for(int j = 0; j < circuit.nPrimaryOutputs; j++)//time on primary inputs predefined
        circuit.placement[circuit.primaryOutputs[j]].arrivalTime = minimalArrival;
    
    for(Net* current = circuit.firstArrival; current; current = current->arrivalOrder)
    {
        if(reroute) AdaptiveRoute(circuit,*current);
        if(reanalize) CalculateDalays(circuit,*current);

        assert(HASREALSOURCE((*current)));

        double base = circuit.placement[current->arrPins[current->sourceIdx].cellIdx].arrivalTime;
        for(int h = 0; h < current->numOfPins; h++)
        {
            if(h == current->sourceIdx) continue;
            double time = base + current->arrPins[h].routeInfo->ArrivalTime;
            if(circuit.placement[current->arrPins[h].cellIdx].arrivalTime < time)
                circuit.placement[current->arrPins[h].cellIdx].arrivalTime = time;
        }
    }
}

void PropagateRequiredTime(Circuit& circuit, bool reanalize, bool reroute)
{
    const double maximumRequired = 0.0;

    for(int i = 0; i < circuit.nNodes; i++)
        circuit.placement[i].requiredTime = maximumRequired;
    for(int j = 0; j < circuit.nPrimaryInputs; j++)//time on primary outputs predefined
        circuit.placement[circuit.primaryInputs[j]].requiredTime = maximumRequired;
    
    for(Net* current = circuit.firstRequired; current; current = current->requiredOrder)
    {
        if(reroute) AdaptiveRoute(circuit,*current);
        if(reanalize) CalculateDalays(circuit,*current);

        assert(HASREALSOURCE((*current)));

        double minTime = maximumRequired;
        for(int h = 0; h < current->numOfPins; h++)
        {
            if(h == current->sourceIdx) continue;
            double req = circuit.placement[current->arrPins[h].cellIdx].requiredTime 
                - current->arrPins[h].routeInfo->ArrivalTime;
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
        cout << circuit.tableOfNames[circuit.primaryOutputs[i]].name << "  "
        << circuit.placement[circuit.primaryOutputs[i]].arrivalTime << endl;
}

void PrintCircuitRequireds(Circuit& circuit)
{
    cout << "Required times on primary inputs:" << endl;
    for(int i = 0; i < circuit.nPrimaryInputs; i++)
        cout << circuit.tableOfNames[circuit.primaryInputs[i]].name << "  "
        << circuit.placement[circuit.primaryInputs[i]].requiredTime << endl;
}

void PrintTimingReport(Circuit& circuit, char* filename)
{
    FILE* out = fopen(filename,"w");
    for(int i = 0; i < circuit.nNodes + circuit.nTerminals; i++)
    {
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