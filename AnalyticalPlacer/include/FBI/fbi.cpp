#include "def.h"
#include "../data_structures.h"

#define WIRE_SEGMENTS 8

double tnslack=0.0;
int bufcount=0;
int lowSlack=0;
int highSlack=0;

//------------FBI global data------------
/* nodes and edges of the routing tree */
FBI_Node *nodes;
Edge *edges;
/* wire_unit_res, wire_unit_cap, driver_resistance */
double r0, c0, rd; 
/* buffer_res, cap and delay */
double rb[MAXBUFFERTYPE], cb[MAXBUFFERTYPE], tb[MAXBUFFERTYPE]; 
/* driver location */
double dx, dy; 
/* number of sinks, internal nodes, total nodes, total edges, buffer types */
int n_sinks, n_candidates, n_nodes, n_edges, n_buffertype;

/* give out the final location of buffer of van algorithm */
Comp *final_location_van; double van_answer;  
/* give out the final location of buffer of shili algorithm */
Comp *final_location_shili; double shili_answer;

/* record number of number of memory opend and freezed */ 
double reduce_solution, total_solution, m_used, m_reduced, max_space; 
//------------end FBI global data------------

int debug;
FILE *fp;
RTnode *NIL;
char* filename;

double dtime;
clock_t cl;

void TraceTree(int parent, StNode *stNode)
{
  if(stNode->type==2)
  {
    nodes[n_nodes].index=n_sinks+1;
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
      nodes[n_nodes].index=n_nodes+20000; //Могуть быть траблы.
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

void InsertBuffers(char *lib, double wcap, double wre, RoutingTree *tree)
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

  filename=(char *)malloc( STRSIZE*sizeof(char) );
  nodes = (FBI_Node *)malloc( MAXNODE*sizeof(FBI_Node) );	
  edges = (Edge *)malloc( MAXEDGE*sizeof(Edge) );

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
    tnslack+=van_answer;

    if(van_answer>-1.0) lowSlack++;
    else highSlack++;

    list_delete( rl );
  } else {
    printf("The slack after buffer insertion : %10.10g\n", shili_answer);
    printbuffer_shili( final_location_shili, &number_of_buffers );
    for ( i=n_buffertype-1;i>-1;i-- ) {
      tree_delete( rt[i] );
      tree_delete_swap( swap_list[i] );
    }
  }

  bufcount+=number_of_buffers;

  if( t ) {
    printf("Maximum memory usage is %fM bytes \n", (float)max_space/(1024*1024));
    printf("Algorithm time is : %f seconds\n", sys);
  }

  free( nodes );
  free( edges );
  free( filename );

  if( c == 2 ) {
    free ( NIL );
    free ( k );
  }
}