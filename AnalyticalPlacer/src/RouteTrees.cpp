#include "..\include\RouteTrees.h"
#include "..\include\FastAccess.h"
#include <stack>

namespace __FLUTE
{
    #pragma warning (disable : 4244)
    #pragma warning (disable : 4005)
    #pragma warning (disable : 4101)
    #pragma warning (disable : 4305)
    #define DTYPE double
    #include "flute.cpp"
    #pragma warning (default : 4244)
    #pragma warning (default : 4005)
    #pragma warning (default : 4101)
    #pragma warning (default : 4305)
}

#define FLUTE_ACCURACY 3

#define NODES_POOL_CHUNK_SIZE 1024
static StNodeEx* __ex_nodes_pool = 0;

StNodeEx* CreateNodeEx()
{
    if(!__ex_nodes_pool)
    {
        __ex_nodes_pool = new StNodeEx[NODES_POOL_CHUNK_SIZE];
        for(StNodeEx *i = __ex_nodes_pool; i < __ex_nodes_pool + NODES_POOL_CHUNK_SIZE - 1; i++)
            i->left = i + 1;
        __ex_nodes_pool[NODES_POOL_CHUNK_SIZE-1].left = 0;
    }
    StNodeEx *ret = __ex_nodes_pool;
    __ex_nodes_pool = (StNodeEx*)__ex_nodes_pool->left;
    ret->cap = 0;
    ret->left = 0;
    ret->right = 0;
    ret->time = 0;
    ret->type = 0;
    ret->x = 0;
    ret->y = 0;
    //additional fields
    ret->origin =0;
    ret->ExtractedC = 0.0;
    ret->ExtractedR = 0.0;
    ret->ObservedC = 0.0;
    ret->ArrivalTime = 0.0;
    return ret;
}

void ReleaseNodeEx(StNodeEx* node)
{
    node->left = __ex_nodes_pool;
    __ex_nodes_pool = node;
}

void ReleaseNodesTree(StNodeEx* node)
{
    std::stack<StNodeEx*> st;
    StNodeEx* n = node;
    while(n)
    {
        if(n->left)
        {
            if(n->right)
                st.push((StNodeEx*)n->right);
            StNodeEx* tmp = n;
            n = (StNodeEx*)n->left;
            ReleaseNodeEx(tmp);
        }
        else
            if(n->right)
            {
                StNodeEx* tmp = n;
                n = (StNodeEx*)n->right;
                ReleaseNodeEx(tmp);
            }
            else
                if(st.empty())
                    return;
                else
                {
                    StNodeEx* tmp = n;
                    n = st.top();
                    st.pop();
                    ReleaseNodeEx(tmp);
                }
    }
}

static double *__xx = 0;
static double *__yy = 0;
static int*    __idxs = 0;
static int __size = 0;

struct __edge_info
{
    int count;
    int inds[4];
};

static __edge_info* __edges = 0;


inline int __load_lut_inline() {__FLUTE::readLUT();return 0;}
static int __load_lut = __load_lut_inline();

struct sp_node_info
{
    StNodeEx* curr;
    int src;
    int dest;
};

void AddNode(StNodeEx* src, StNodeEx* dest)
{
    if(src->x == dest->x)
    {
        if(src->left)
            src->right = dest;
        else
            src->left = dest;
    }
    else
    {
        if(src->y == dest->y)
        {
            if(src->left)
                src->right = dest;
            else
                src->left = dest;
        }
        else
        {
            StNodeEx* diag = CreateNodeEx();
            diag->x = src->x;
            diag->y = dest->y;
            diag->type = NODE_TYPE_INTERNAL;
            diag->left = dest;
            if(src->left)
                src->right = diag;
            else
                src->left = diag;
        }
    }
}

void MakeRouteTree(Circuit &c, Net& net)
{
    double source_x = -1e128;
    double source_y = -1e128;
    int __size2 = net.numOfPins * 2 -2;
    
    if(__size < net.numOfPins)
    {
        delete[] __xx;
        delete[] __yy;
        delete[] __edges;
        delete[] __idxs;
        __size = net.numOfPins;
        __xx = new double[__size];
        __yy = new double[__size];
        __idxs = new int[__size];
        __edges = new __edge_info[__size2];
    }

    for(int i = 0; i < net.numOfPins; i++)
    {
        __xx[i] = GetPlace(c,net,i).xCoord + GetNetPin(net,i).xOffset;
        __yy[i] = GetPlace(c,net,i).yCoord + GetNetPin(net,i).yOffset;
        if(GetNetPin(net,i).chtype == 'I')
            ;
        else
            if(GetNetPin(net,i).chtype == 'O')
            {
                source_x = __xx[i];
                source_y = __yy[i];
            }
            else//'B'
                if(source_x == -1e128)
                {
                    source_x = __xx[i];
                    source_y = __yy[i];
                }
    }
    if(source_x == -1e128)
    {
        source_x = __xx[0];
        source_y = __yy[0];
    }

    for(int l = 0; l < net.numOfPins; l++)
        __idxs[l] = l;

    {//sort indexes: y - first, x - second
        for(int q = 0; q < net.numOfPins - 1; q++)
        {
            int minidx = q;
            for(int j = q + 1; j < net.numOfPins; j++)
            {
                if (__yy[__idxs[minidx]] > __yy[__idxs[j]] 
                    || (__yy[__idxs[minidx]] == __yy[__idxs[j]] && __xx[__idxs[minidx]] > __xx[__idxs[j]]))
                {
                    minidx = j;
                }
            }
            int tmpp = __idxs[q];
            __idxs[q] = __idxs[minidx];
            __idxs[minidx] = tmpp;
        }
    }

    __FLUTE::Tree fltree = __FLUTE::flute(net.numOfPins,__xx,__yy,FLUTE_ACCURACY);

    if(net.tree)
        ReleaseNodesTree(net.tree->nodes);
    else
        net.tree = new RoutingTree();
    net.tree->wl = fltree.length;

    int src_ind = -1;
    memset(__edges,0,sizeof(__edge_info)*__size2);
    for(int k = 0; k < __size2; k++)
    {
        if(fltree.branch[k].x == source_x && fltree.branch[k].y == source_y && src_ind < 0)
            src_ind = k;
        if(k == fltree.branch[k].n) continue;
        __edges[k].inds[__edges[k].count] = fltree.branch[k].n;
        __edges[k].count++;
        __edges[fltree.branch[k].n].inds[__edges[fltree.branch[k].n].count] = k;
        __edges[fltree.branch[k].n].count++;
    }

    std::stack<sp_node_info> Stack;
    sp_node_info This;
    This.curr = net.tree->nodes = CreateNodeEx();
    This.curr->x = source_x;
    This.curr->y = source_y;
    net.tree->srcRes = net.arrPins[__idxs[src_ind]].type->Resistance;
    This.src = src_ind;
    This.dest = src_ind;
    Stack.push(This);

    while(!Stack.empty())
    {
        This = Stack.top();
        Stack.pop();
        if(This.dest < net.numOfPins)
        {
            if(This.dest == src_ind)
                This.curr->type = NODE_TYPE_SOURCE;
            else
                This.curr->type = NODE_TYPE_SINK;
            //This.curr->cap = net.arrPins[__idxs[This.dest]].type->Capacitance;
            This.curr->origin = net.arrPins + __idxs[This.dest];
            This.curr->origin->routeInfo = This.curr;
            This.curr->cap = This.curr->origin->type->Capacitance;
        }
        else
            This.curr->type = NODE_TYPE_INTERNAL;
        sp_node_info nds[4];
        int nnum = 0;
        for(int k = 0; k < __edges[This.dest].count; k++)
            if(__edges[This.dest].inds[k] != This.src)
            {
                nds[nnum].src = This.dest;
                nds[nnum].dest = __edges[This.dest].inds[k];
                nds[nnum].curr = CreateNodeEx();
                nds[nnum].curr->x = fltree.branch[nds[nnum].dest].x;
                nds[nnum].curr->y = fltree.branch[nds[nnum].dest].y;
                nnum++;
            }
        switch(nnum)
        {
        case 0://sink
            continue;
            break;
        case 1:
            {
                AddNode(This.curr, nds[0].curr);
                Stack.push(nds[0]);
            }
            break;
        case 2:
            {
                AddNode(This.curr, nds[0].curr);
                AddNode(This.curr, nds[1].curr);
                Stack.push(nds[0]);
                Stack.push(nds[1]);
            }
            break;
        case 3:
            {
                StNodeEx* imag = CreateNodeEx();
                imag->x = This.curr->x;
                imag->y = This.curr->y;
                imag->type = NODE_TYPE_INTERNAL;
                AddNode(This.curr, nds[0].curr);
                AddNode(This.curr, imag);
                AddNode(imag, nds[1].curr);
                AddNode(imag, nds[2].curr);
                Stack.push(nds[0]);
                Stack.push(nds[1]);
                Stack.push(nds[2]);
            }
            break;
        case 4://only for source
            if(This.curr->type != NODE_TYPE_SOURCE)
                throw std::exception("Node has 4 output edges!!!!");
            {
                StNodeEx* imag1 = CreateNodeEx();
                StNodeEx* imag2 = CreateNodeEx();
                imag1->x = imag2->x = This.curr->x;
                imag1->y = imag2->y = This.curr->y;
                imag1->type = imag2->type = NODE_TYPE_INTERNAL;
                This.curr->left = imag1;
                This.curr->right = imag2;
                AddNode(imag1, nds[0].curr);
                AddNode(imag1, nds[1].curr);
                AddNode(imag2, nds[2].curr);
                AddNode(imag2, nds[3].curr);
                Stack.push(nds[0]);
                Stack.push(nds[1]);
                Stack.push(nds[2]);
                Stack.push(nds[3]);
            }
            break;
        default:
            throw std::exception("Pin has more than 4 edges!!!!");
        }
    }
    free(fltree.branch);
}

void PrintTree(RoutingTree* tree, FILE* output)
{
    char tab = ' ';
    std::stack<StNodeEx*> Stack;
    int depth = 0;
    StNodeEx *cur = tree->nodes;

    while(true)
    {
        while(cur)
        {
            for(int w = 0; w < depth; w++) fwrite(&tab,1,1,output);
            if(cur->type == NODE_TYPE_SINK)
                fprintf(output,"(%8.2f; %8.2f) SINK %f\n",cur->x,cur->y,cur->ArrivalTime);
            else
                fprintf(output,"(%8.2f; %8.2f) %f\n",cur->x,cur->y,cur->ArrivalTime);
            Stack.push((StNodeEx*)cur->right);
            cur = (StNodeEx*)cur->left;
            depth++;
        }
        if(Stack.empty())
            break;
        else
        {
            cur = Stack.top();
            Stack.pop();
            if(!cur)
                depth--;
        }
    }
}