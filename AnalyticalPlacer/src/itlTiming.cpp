#include <stack>
#include "..\include\itlTiming.h"

#define EXTRACT_RC(circuit,src,dest,R,C) ExtractRCexact(circuit,src,dest,R,C);
//#define EXTRACT_RC(circuit,src,dest,R,C) ExtractRClumped(circuit,src,dest,R,C);


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

void AnalizeTiming(Circuit& circuit, Net& Net)
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
