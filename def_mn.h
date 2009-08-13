#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <string.h>
#include <math.h>
//#include "getopt.h"

#define	INFINITY	20000000000.0
#define BUFSIZE 256
#define STRSIZE 256
#define MAXNODE 1600000
#define MAXEDGE 1605500
#define MAXBUFFERTYPE 64    // number of buffertype( one with no buffer )
#define SINK 2
#define IN 1
#define SOURCE 0
#define eps  1e-18
#define eps_distance 0.00000000001


/* Tree Node Structure */
typedef struct NODE 
{
  int index;      /* index of the node */
  double x;       /* x coordinate */
  double y;       /* y coordinate */
  int type;       /* 0 source node, 1 internal node, 2 sink node */
  double cap;     /* node capacitance */
  double time;    /* required arrival time at the node */  
  struct NODE *left;
  struct NODE *right;  
}Node;

/* Tree Edge Structure */
typedef struct EDGE
{
  int s_index;   /* start node  */
  int t_index;   /* target node */
}Edge;

/* composition node */
typedef struct comp{ 
  struct comp *left;  
  struct comp *right; 
  int buffertype; 
  /* (x, y) means the buffer is inserted at point with index y with the direction x->y  */
  int x;  
  int y;  
  int counter;  /* use for comp deletion and comp chare */
} Comp;

/* Ordered list used in Van Ginneken algorithm */
typedef struct rlnode {
  struct rlnode *next;
  struct rlnode *prev;
  double cap;   /* downstream capacitance */
  double time;  /* required arrival time at the node */
  Comp *com;    /* buffer location */
} RLnode;

/* New data structure for O(bmn) algorithm passed variables */
typedef struct bestpointer {
  double ra;
  double da;
  double ca;
  RLnode *beta[MAXBUFFERTYPE];
  RLnode *betaD[MAXBUFFERTYPE];
  RLnode *alpha[MAXBUFFERTYPE];
} Bp;

/* Candidate tree and expiration list structure used in Shi and Li's algorithm */
#define RED     	'R'
#define BLACK   	'B'
typedef struct rtnode {
  char color;	/* RED or BLACK */
  int dirty;	/* whether to update */
  int size;	/* number of nodes in tree */
  double cap;
  double time;
  double ra;  /* resistance caused by extra wire */
  double da;  /* constant delay part caused by extra wire */
  double ca;  /* capacitance caused by merge or extra wire */
  struct rtnode *left;
  struct rtnode *right;
  struct rtnode *parent;
  struct rtnode *swap_option; /* pointer for the candidate tree 
				 pointing to the corresponding node in expiration_list (swap_list) */
  struct rtnode *qc_option;   /* pointer for expiration_list (swap_list) 
				 pointing to the corresponding node in candidate tree */
  Comp *com;                  /* buffer location */
  Comp *coma;                 /* extra buffer location */
} RTnode;

/* io.c */
extern void InputLib();           /* Read the Library file */
extern Node *Input();             /* Read the tree input */
extern void Printoutput();        /* print tree to the output file */
extern void ntree_print();        /* print tree to the screen */
extern Node *produce_tree();      /* produce routing tree according to node and edge */
extern void edge_print();         /* print edge according to dFS */

/* in build_tree.c */
extern void Build_rsa_tree(); 
extern void rsa_tree();
extern void connect();

/* in wire_segment.c*/
extern void wire_segment(); // wire segment algorithm

/* in van.c*/
extern RLnode **van();
extern RLnode **add_buffer();
extern RLnode *redundent();
extern RLnode **create_list();
extern void list_delete();
extern void comp_delete();  
extern void print();
extern void printbuffer();
extern void cover();        /* function to find the concave cover of (q,c) list */
extern RLnode * cover_bn();
extern void update_nblist();
void error(char *);
extern double P();

/* in tree.c */
extern void insert();
extern void node_delete();
extern void tree_delete();
extern void update();
extern void travel();
extern RTnode *search();
extern RTnode *pred();
extern RTnode *succ();
extern RTnode *node_succ();
extern RTnode *node_pred();
extern RTnode *minimum();
extern RTnode *maximum();
extern void fixup();
extern void balance();
extern void travel_update();
extern void printbuffer_shili();

/* in swap_tree.c */    
extern void insert_swap();
extern void node_delete_swap();
extern void tree_delete_swap();

/* in shili.c */
extern void interval();
extern void shili();
extern void create();
extern double distance();
extern void buffer_update();

/* nodes and edges of the routing tree */
Node *nodes;
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


int debug;
FILE *fp;
RTnode *NIL;
extern char *optarg;
char* filename;

struct rusage timestuff;
        
/* define macros for time and resident memory usage checks */
        
double dtime;
long stime, utime;
        
#define starttimer getrusage(RUSAGE_SELF, &timestuff);	\
  stime = timestuff.ru_utime.tv_sec;			\
  utime = timestuff.ru_utime.tv_usec
#define stoptimer getrusage(RUSAGE_SELF, &timestuff);		\
  dtime = (double)(timestuff.ru_utime.tv_sec - stime)		\
    + 1.0e-6*(double)(timestuff.ru_utime.tv_usec - utime)
                
                        

