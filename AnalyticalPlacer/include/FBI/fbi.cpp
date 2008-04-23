#include "fbi.h"

#include <sys/timeb.h>


#define WIRE_SEGMENTS 8

double tnslack=0.0;
int bufcount=0;
int lowSlack=0;
int highSlack=0;

//------------FBI global data------------
/* nodes and edges of the routing tree */
//FBI_Node *nodes;
//Edge *edges;
/* wire_unit_res, wire_unit_cap, driver_resistance */
double r0, c0, rd; 
/* buffer_res, cap and delay */
double rb[MAXBUFFERTYPE], cb[MAXBUFFERTYPE], tb[MAXBUFFERTYPE]; 
/* driver location */
double dx, dy; 
/* number of sinks, internal nodes, total nodes, total edges, buffer types */
int n_sinks =0, n_candidates =0, n_nodes=0, n_edges=0, n_buffertype=0;

/* give out the final location of buffer of van algorithm */
Comp *final_location_van; double van_answer;  
/* give out the final location of buffer of shili algorithm */
Comp *final_location_shili; double shili_answer;

/* record number of number of memory opend and freezed */ 
double reduce_solution, total_solution, m_used, m_reduced, max_space; 
//------------end FBI global data------------

char*  filename = NULL;
FBI_Node*  nodes = NULL;	
Edge*  edges = NULL;

int debug;
FILE *fp;
RTnode *NIL;
//char* filename;

double dtime;
clock_t cl;

Circuit* myCircuit;
FBI_Node *myTree;
RoutingTree *myCTree;

//---BufInfo

typedef struct
{
	PinInfo* in;
	PinInfo* out;

	MacroInfo *info;
} FBIBufInfo;

FBIBufInfo bufInfo[MAXBUFFERTYPE];

//---end BufInfo

void TraceTree(int parent, StNode *stNode)
{
  if(stNode->type==2)
  {
    nodes[n_nodes].index=n_sinks+1;
    nodes[n_nodes].indexCirc = ((StNodeEx*)stNode)->origin->cellIdx;
    nodes[n_nodes].x=stNode->x;
    nodes[n_nodes].y=stNode->y;
    nodes[n_nodes].type = SINK;
    nodes[n_nodes].cap = stNode->cap;				   
	  nodes[n_nodes].left = (FBI_Node *)NULL;
	  nodes[n_nodes].right = (FBI_Node *)NULL;				   
    nodes[n_nodes].time=stNode->time;
	  n_sinks ++;				
	  n_nodes ++;
  }
  else
    if(stNode->type==1)
    {
      nodes[n_nodes].index=n_nodes + 20000; //Могуть быть траблы.
      nodes[n_nodes].indexCirc = -1;
      nodes[n_nodes].x=stNode->x;
      nodes[n_nodes].y=stNode->y;
      nodes[n_nodes].type = FBI_IN;
	    nodes[n_nodes].cap = 0.0;				   
	    nodes[n_nodes].left = (FBI_Node *)NULL;
	    nodes[n_nodes].right = (FBI_Node *)NULL;				   		
	    n_nodes ++;
    }
    else
      printf("Error! Wrong stainer tree.");

  edges[n_edges].s_index=parent;
  edges[n_edges].t_index=nodes[n_nodes-1].index;

  n_edges ++;

  {
    int ind=edges[n_edges-1].t_index;
    if(stNode->left)
      TraceTree(ind, stNode->left);

    if(stNode->right)
      TraceTree(ind, stNode->right);
  }
}

FBI_Node* MakeTree(StNode *stNode)
{
  n_sinks=0; n_nodes=0; n_edges=0;   

  nodes[n_nodes].x=stNode->x;
  nodes[n_nodes].y=stNode->y;

  nodes[n_nodes].index = 0;
  nodes[n_nodes].indexCirc = ((StNodeEx*)stNode)->origin->cellIdx;
  dx = nodes[n_nodes].x;
  dy = nodes[n_nodes].y;				   
  nodes[n_nodes].cap = 0.0;	 //stNode->cap
  nodes[n_nodes].type = SOURCE;				  
  nodes[n_nodes].left = (FBI_Node *)NULL;
  nodes[n_nodes].right = (FBI_Node *)NULL;
  n_sinks ++;
  n_nodes ++;

  if(stNode->left)
    TraceTree(0, stNode->left);

  if(stNode->right)
    TraceTree(0, stNode->right);

  n_candidates=n_nodes-n_sinks;

  FBI_Node *t=produce_tree();
  wire_segment(WIRE_SEGMENTS);

  return t;
}

FBI_Node* FindNode(int index, FBI_Node *ntree)
{
  if(ntree->index == index)
    return ntree;

  FBI_Node *tem;

  if(ntree->left)
  {
    tem = FindNode(index, ntree->left);
    if(tem)
      return tem;
  }

  if(ntree->right)
  {
    tem = FindNode(index, ntree->right);
    if(tem)
      return tem;
  }

  return NULL;
}

void GetCells(FBI_Node *node, std::vector<int> *vec)
{
  if (node->index < 20000)
    vec->push_back(node->indexCirc);

  if(node->left)
    GetCells(node->left, vec);

  if(node->right)
    GetCells(node->right, vec);
}


FBIBufInfo GetBufferInfo(std::string bufName, Circuit *circuit)
{
	FBIBufInfo bufInfo;

	if (circuit)
	{
		bufInfo.info = circuit->tech->CellTypes->find(bufName)->second;

		bufInfo.in = NULL;
		bufInfo.out = NULL;

		for(std::map<std::string,PinInfo*>::iterator i = bufInfo.info->Pins.begin();
			i != bufInfo.info->Pins.end(); ++i)
			if(!i->second->isSpecial)
			{
				if(i->second->dir == PinInfo::INPUT)
					bufInfo.in = i->second;
				else
					if(i->second->dir == PinInfo::OUTPUT)
						bufInfo.out = i->second;
			}
	}
	
	return bufInfo;		
}

void UpdateCircuit(RLnode *rl, FBI_Node *ntree, int netIndex)
{
  Node* node = myCircuit->nodes;
  Net* net = myCircuit->nets;
  Place * pl = myCircuit->placement;
  str*  names = myCircuit->tableOfNames;
  std::vector<int>* conn = myCircuit->tableOfConnections;

//  int nNodes = myCircuit->nNodes;
//  int nNets = myCircuit->nNets;
  int nTerminals = myCircuit->nTerminals;

  if(rl)
    do
    {
      if(rl->com)
        if(rl->com->buffertype>=0)
        {
    		  int bufType = rl->com->buffertype;
          FBI_Node *fbiNode = FindNode(rl->com->y, ntree);

          std::vector<int> vecNodeR;
          std::vector<int> vecNodeAll;
          std::vector<int> vecNodeL;

          //Get all nodes in net
          GetCells(fbiNode, &vecNodeR);

          //Get "right" nodes in net
          GetCells(ntree, &vecNodeAll);

          //Construct "left" nodes in net (all nodes - "right" nodes)
          for(int i=0; i<vecNodeAll.size(); i++)
          {
            bool exist = false;
            for (int j=0; j<vecNodeR.size(); j++)
              if(vecNodeR[j] == vecNodeAll[i])
              {
                exist = true;
                break;
              }

            if(!exist)
              vecNodeL.push_back(vecNodeAll[i]);
          }

          int pinSrcIndex = net[netIndex].sourceIdx;
          int cellSrcIndex = -1;

          if (pinSrcIndex >= 0)
          {
            cellSrcIndex = net[netIndex].arrPins[pinSrcIndex].cellIdx;
          }
          else
            cellSrcIndex = pinSrcIndex;

		      for(int z = 0; z < vecNodeR.size(); z++)
			      if(cellSrcIndex == vecNodeR[z])
			      {
				       std::vector<int> tmp_ = vecNodeR;
				       vecNodeR = vecNodeL;
				       vecNodeL = tmp_;
				       break;
			      }

		      node[myCircuit->nNodes].width = bufInfo[bufType].info->SizeX;
          node[myCircuit->nNodes].height = bufInfo[bufType].info->SizeY;
          node[myCircuit->nNodes].type =  bufInfo[bufType].info;

          pl[myCircuit->nNodes].xCoord = fbiNode->x;
          pl[myCircuit->nNodes].yCoord = fbiNode->y;
          memcpy(pl[myCircuit->nNodes].orient, "N", 2);

          char s[100];
          struct _timeb timebuffer;
          _ftime64_s( &timebuffer );

          sprintf(s, "myBuffer%i_%I64u%i", myCircuit->nBuffers, timebuffer.time, timebuffer.millitm);
          //names[myCircuit->nNodes] = *(new str(s));
		      new ((void*)(&names[myCircuit->nNodes])) str(s);

          std::vector<int> bufConnections;
          bufConnections.push_back(netIndex);
          bufConnections.push_back(myCircuit->nNets);

          conn[myCircuit->nNodes] = bufConnections;

          for(int i=0; i<vecNodeR.size(); i++)
          {
            //int qqq=vecR[i];
            vector<int> &v = conn[vecNodeR[i]];
            for (int j=0; j<v.size(); j++)
            {
                if( v[j] == netIndex)
                {
                  v[j] = myCircuit->nNets;
                  break;
                }
            }
          }

          Pin *pinVecAll = new Pin[net[netIndex].numOfPins];
          Pin *pinVecL = new Pin[net[netIndex].numOfPins];
          Pin *pinVecR = new Pin[net[netIndex].numOfPins];

          int numL = 0, numR = 0;

          for(int i=0; i<net[netIndex].numOfPins; i++)
            pinVecAll[i] = net[netIndex].arrPins[i];

          for(int i=0; i<net[netIndex].numOfPins; i++)
            for(int j=0; j<vecNodeL.size(); j++)
            {
              if(pinVecAll[i].cellIdx == vecNodeL[j])
              {
                pinVecL[numL] = pinVecAll[i];
                numL++;
                break;
              }
            }

			pinVecL[numL].type = bufInfo[bufType].in;
			pinVecL[numL].cellIdx = myCircuit->nNodes;
			pinVecL[numL].chtype = 'I';
			pinVecL[numL].routeInfo = NULL;
			pinVecL[numL].xOffset = pl[myCircuit->nNodes].xCoord;
			pinVecL[numL].yOffset = pl[myCircuit->nNodes].yCoord;

          numL++;

          for(int i=0; i<net[netIndex].numOfPins; i++)
            for(int j=0; j<vecNodeR.size(); j++)
            {
              if(pinVecAll[i].cellIdx == vecNodeR[j])
              {
                pinVecR[numR] = pinVecAll[i];
                numR++;
                break;
              }
            }

			pinVecR[numR].type = bufInfo[bufType].out;
			pinVecR[numR].cellIdx = myCircuit->nNodes;
			pinVecR[numR].chtype = 'O';
			pinVecR[numR].routeInfo = NULL;
			pinVecR[numR].xOffset = pl[myCircuit->nNodes].xCoord;
			pinVecR[numR].yOffset = pl[myCircuit->nNodes].yCoord;
          numR++;

		  delete[] pinVecAll;

		  Net &netIn = myCircuit->nets[netIndex], 
			  &netOut = myCircuit->nets[myCircuit->nNets];

          netOut.arrPins = pinVecR;
          netOut.numOfPins=numR;
		  netOut.currWL = myCircuit->nets[netIndex].currWL / 2; //Bug
		  netOut.sourceIdx = numR-1; 

		  char *qqq=new char[100];
		  sprintf(qqq, "%s_%i_r", net[netIndex].name, myCircuit->nBuffers);
		  netOut.name = qqq;

		  netOut.arrivalOrder = NULL; //ReCalculate
		  netOut.requiredOrder = NULL; //ReCalculate

		  netOut.tree = NULL;

		  //delete[] (char*)(netIn.arrPins);
		  netIn.arrPins = pinVecL;
          netIn.numOfPins=numL;
		  netIn.currWL = myCircuit->nets[netIndex].currWL / 2; //Bug

      bool exist = false;
      int pinLIndex = -1;

      for (int i=0; i<numL; i++)
        if(pinVecL[i].cellIdx == cellSrcIndex)
        {
          pinLIndex = i;
          exist = true;
          break;
        }

      if(pinSrcIndex == NO_SOURCE)
        pinLIndex = NO_SOURCE;
      else
        if(!exist)
          pinLIndex = UKNOWN_INDEX;

		  netIn.sourceIdx = pinLIndex; 

		  qqq=new char[100];
		  sprintf(qqq, "%s_%i_l", net[netIndex].name, myCircuit->nBuffers);
		  delete[] netIn.name;
          netIn.name = qqq;

		  netIn.arrivalOrder = NULL; //ReCalculate
		  netIn.requiredOrder = NULL; //ReCalculate

          myCircuit->nNodes++;
		  myCircuit->nNets++;
		  myCircuit->nBuffers++;

		  AdaptiveRoute(*myCircuit,myCircuit->nets[netIndex]); //In the end
		  AdaptiveRoute(*myCircuit,myCircuit->nets[myCircuit->nNets - 1]); //In the end

      bufcount++;
		}
      rl = rl->next;

      break; //Test on one buffer
    }
    while(rl->next);

}

void InitFBI(Circuit *circuit)
{
  filename=(char *)malloc( STRSIZE*sizeof(char) );
  nodes = (FBI_Node *)malloc( MAXNODE*sizeof(FBI_Node) );	
  edges = (Edge *)malloc( MAXEDGE*sizeof(Edge) );

	for(int i=0; i<MAXBUFFERTYPE; i++)
		bufInfo[i] = GetBufferInfo("BUFX1", circuit);
}

void FinalizeFBI()
{
  free( nodes );
  free( edges );
  free( filename );
}

void InsertBuffers(char *lib, double wcap, double wre, RoutingTree *tree,int netIndex, Circuit &circuit)
{
  /* Van Ginneken list */
  RLnode *rl;
  /* FBI's candidate tree and expiration_list (swap_list) 
     for swap_list, l is defined in field "time" of structure RTnode, all others are useless  */
  RTnode *rt[MAXBUFFERTYPE];  
  RTnode *swap_list[MAXBUFFERTYPE]; 
  FILE* flib;
  FBI_Node *ntree;
  int  i, output=0, c = 2, t = 0,  number_of_buffers=0;
  double sys, *k;
  double dtime = 0.0;

  myCircuit = &circuit;
  myCTree = tree;

  n_candidates=0, n_buffertype=0;

  //filename=(char *)malloc( STRSIZE*sizeof(char) );
  //nodes = (FBI_Node *)malloc( MAXNODE*sizeof(FBI_Node) );	
  //edges = (Edge *)malloc( MAXEDGE*sizeof(Edge) );
  memset(filename, 0, STRSIZE*sizeof(char));
  memset(nodes, 0, n_nodes*sizeof(FBI_Node));
  memset(edges, 0, n_edges*sizeof(Edge));

  fp = stdin;
  debug = 0; 
  reduce_solution = total_solution = m_used = m_reduced = max_space = 0; 
	
  output = 0;      // output
  t = 0;           // time
  debug = 0;       // debug
  c=1;              //Van Ginneken (1) or FBI ( Fast Buffer Insertion) (2)

  flib = fopen(lib, "r");
  InputLib(flib); //Get data for buffers (res, cap and delay)
  fclose(flib);



  r0=wre;
  c0=wcap;
  rd=tree->srcRes;

  //StNode *stNode=tree->nodes;
  ntree=MakeTree(tree->nodes);

  myTree=ntree;

  if ( c == 1 ) {
    final_location_van = (Comp *)malloc(sizeof(Comp));
  } else {
    /* Define NIL for Red-Black Tree */
    NIL = (RTnode *) malloc(sizeof(RTnode));
    NIL->color = BLACK;
    NIL->ra = 0;
    NIL->ca = 0;
    NIL->da=0;
    NIL->dirty=0;
    NIL->size=0;
    NIL->com=(Comp *) NULL;
    NIL->coma=(Comp *) NULL;
		
    final_location_shili=(Comp *)malloc(sizeof(Comp));
    /* k is defined as the distance from the current node to the most critical sink, in 
       this version, the la is not used in expiration_list. Instead, we use global variable 
       k to record the distance. Note that this is only valid for there is only one unit
       resistance and capacitance. la will be implemented in later version to consider more 
       general case, like via, multiple layer */
    k=(double *)malloc(n_buffertype*sizeof(double));
    for ( i=0;i<n_buffertype;i++ ) {
      k[i] = 0.0;
      swap_list[i] = NULL;
      rt[i] = NULL;
    }
  }	

  if ( debug )
    ntree_print(ntree, 0); 
  starttimer;     
  if (c == 1)
    rl = van( ntree );
  else                      
    shili( &k[0], ntree, rt, swap_list ); 
  stoptimer;
  sys = dtime;   

  //printf("The number of nodes %d\n", n_candidates);
  if (c == 1) {
    //printf("%10.10g\n", van_answer);
    printbuffer( final_location_van, &number_of_buffers );
    tnslack += van_answer;

    if(van_answer > -100.0) lowSlack++;
    else 
    {
      highSlack++;
      printf("%i: %f\n", highSlack, van_answer);
    }

    UpdateCircuit(rl, ntree, netIndex);

    list_delete( rl );
  } else {
    printf("The slack after buffer insertion : %10.10g\n", shili_answer);
    printbuffer_shili( final_location_shili, &number_of_buffers );
    for ( i=n_buffertype-1;i>-1;i-- ) {
      tree_delete( rt[i] );
      tree_delete_swap( swap_list[i] );
    }
  }

  //bufcount+=number_of_buffers;

  if( t ) {
    printf("Maximum memory usage is %fM bytes \n", (float)max_space/(1024*1024));
    printf("Algorithm time is : %f seconds\n", sys);
  }

  //free( nodes );
  //free( edges );
  //free( filename );

  if( c == 2 ) {
    free ( NIL );
    free ( k );
  }
}