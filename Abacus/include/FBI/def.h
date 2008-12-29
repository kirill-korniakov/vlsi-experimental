#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <time.h>
#include "getopt.h"

#include <string.h>
#include <math.h>

#define	INFINITY	20000000000.0
#define BUFSIZE 256
#define STRSIZE 256
#define MAXNODE 1600000
#define MAXEDGE 1605500
#define MAXBUFFERTYPE 64    // number of buffertype( one with no buffer )
#define SINK 2
#define FBI_IN 1
#define SOURCE 0
#define eps	0.000000000000000000000001
#define eps_distance 0.00000000001


/* Tree FBI_Node Structure */
typedef struct NODE 
{
  int indexCirc; //ITLab 

  int index;      /* index of the node */
  double x;       /* x coordinate */
  double y;       /* y coordinate */
  int type;       /* 0 source node, 1 internal node, 2 sink node */
  double cap;     /* node capacitance */
  double time;    /* required arrival time at the node */  
  struct NODE *left;
  struct NODE *right;  
}FBI_Node;

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
  double cap;   /* downstream capacitance */
  double time;  /* required arrival time at the node */
  Comp *com;    /* buffer location */
} RLnode;

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
extern void InputLib(FILE *flib);           /* Read the Library file */
extern FBI_Node *Input();             /* Read the tree input */
extern void Printoutput(FBI_Node *p);        /* print tree to the output file */
extern void ntree_print(FBI_Node *p, int ind);        /* print tree to the screen */
extern FBI_Node *produce_tree();      /* produce routing tree according to node and edge */
extern void edge_print(FILE *stream, FBI_Node *p);         /* print edge according to dFS */

/* in build_tree.c */
extern void Build_rsa_tree(); 
extern void rsa_tree();
extern void connect();

/* in wire_segment.c*/
extern void wire_segment(int N_SEG); // wire segment algorithm

/* in van.c*/
extern RLnode *van(FBI_Node *t);
extern RLnode *add_buffer(double k,RLnode *list, int px, int py);
extern RLnode *redundent(RLnode *list);
extern RLnode *create_list(FBI_Node *t);
extern void list_delete(RLnode *l);
extern void comp_delete(Comp *com);  
extern void print(RLnode *x);
extern void printbuffer(Comp *x, int *i);
extern void buffer_blgb(); /* function for generating b solutions in blogb time*/

/* in tree.c */
extern RTnode * insert(RTnode **root,  double q,  double c, Comp *com, RTnode **list);
extern void node_delete(RTnode **root, RTnode *z);
extern void tree_delete(RTnode *x);
extern void update(RTnode *x);
extern void travel(RTnode *x);
extern RTnode *search(RTnode *root, double q);
extern RTnode *pred(RTnode *root, double q);
extern RTnode *succ(RTnode *root, double q);
extern RTnode *node_succ(RTnode *x);
extern RTnode *node_pred(RTnode *x);
extern RTnode *minimum(RTnode *x);
extern RTnode *maximum(RTnode *x);
extern void fixup(RTnode **root, RTnode *z);
extern void balance(RTnode **root, RTnode *x);
extern void travel_update(RTnode *root, RTnode *x);
extern void printbuffer_shili(Comp *x, int *i);

/* in swap_tree.c */    
extern RTnode * insert_swap(RTnode **root, double q, double c);
extern void node_delete_swap(RTnode **root, RTnode  *z);
extern void tree_delete_swap(RTnode *x);

/* in shili.c */
extern void interval(RTnode *lb, RTnode *rb, RTnode *x);
extern void shili(double *k, FBI_Node *t, RTnode **rt, RTnode **list);
extern void create(double *k, RTnode **rt, FBI_Node *t, RTnode **list);
extern double distance(FBI_Node *t, FBI_Node *t1);
extern void buffer_update(double *k, double k1, RTnode **Rnext, RTnode **list, int px, int py);

/* nodes and edges of the routing tree */
extern FBI_Node *nodes;
extern Edge *edges;
/* wire_unit_res, wire_unit_cap, driver_resistance */
extern double r0, c0, rd; 
/* buffer_res, cap and delay */
extern double rb[MAXBUFFERTYPE], cb[MAXBUFFERTYPE], tb[MAXBUFFERTYPE]; 
/* driver location */
extern double dx, dy; 
/* number of sinks, internal nodes, total nodes, total edges, buffer types */
extern int n_sinks, n_candidates, n_nodes, n_edges, n_buffertype;

/* give out the final location of buffer of van algorithm */
extern Comp *final_location_van; extern double van_answer;  
/* give out the final location of buffer of shili algorithm */
extern Comp *final_location_shili; extern double shili_answer;

/* record number of number of memory opend and freezed */ 
extern double reduce_solution, total_solution, m_used, m_reduced, max_space; 


extern int debug;
extern FILE *fp;
extern RTnode *NIL;
extern char *optarg;
extern char* filename;
       
/* define macros for time and resident memory usage checks */
        
extern double dtime;
        
//#include "windows.h"

//LARGE_INTEGER freq;
//QueryPerformanceFrequency(&freq);
//LARGE_INTEGER sQP, fQP;
//
//#define starttimer QueryPerformanceCounter(&sQP);
//#define stoptimer QueryPerformanceCounter(&fQP); dtime=(fQP.QuadPart-sQP.QuadPart)/(double)freq.QuadPart;

extern clock_t cl;

#define starttimer  cl = clock(); 
#define stoptimer  dtime = (double)(clock() - cl) / CLOCKS_PER_SEC;

               
                        

