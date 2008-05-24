/* Released Software: Fast Buffer Insertion for Interconnect Optimization */
/*                      Version 2.3: 2003 - 2007 */

/* Author:  Zhuo Li                       -- zhybear@gmail.com  */
/* Author:  Weiping Shi                   -- wshi@ee.tamu.edu */

/* Permission to use for non-commercial purposes is granted provided */
/* that proper acknowledgments are given. */
/* For information about commercial use, please contact authors */

#include "def.h"
int main(argc, argv)
     int argc;
     char **argv;
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
  char* ft; 
  double sys, *k;
  double dtime = 0.0;

  logo();
  filename=(char *)malloc( STRSIZE*sizeof(char) );

  nodes = (FBI_Node *)malloc( MAXNODE*sizeof(FBI_Node) );	
  edges = (Edge *)malloc( MAXEDGE*sizeof(Edge) );

  fp = stdin;
  debug = 0; 
  reduce_solution = total_solution = m_used = m_reduced = max_space = 0; 
	
  if( argc < 4 ) {
    fprintf(stderr, "Usage: %s -f netfile -b libraryfile [-d] [-t] [-o]\n", argv[0]);
    exit(0);
  } 
  while ((c = getopt(argc, argv, "f:b:tod")) != -1)
    switch (c) {
    case 'f':
      if ((fp = fopen(optarg, "r")) == NULL) {
	fprintf(stderr, "Cannot open file: %s\n", optarg);
	exit (0);
      } else
	strcpy(filename,optarg);
      break;
    case 'b':
      if( ( flib = fopen(optarg, "r") ) == NULL ) {
	fprintf(stderr, "Cannot open file: %s\n", optarg);
	exit( 0 );
      }
      break;
    case 'o':        // output
      output = 1;
      break;
    case 't':        // time
      t = 1;
      break;
    case 'd':        // debug
      debug = 1;
      break;
    default:
      fprintf(stderr, "Usage: %s -f netfile -b libraryfile [-d] [-t] [-o]\n", argv[0]);
      exit(0);
    }
  if ( fp == stdin || flib == NULL ) {
    fprintf(stderr, "Usage: %s -f netfile -b libraryfile [-d] [-t] [-o]\n", argv[0]);
    exit(0);
  }
  printf("Van Ginneken (1) or FBI ( Fast Buffer Insertion) (2)? ");
  scanf("%d", &c);
  InputLib(flib);
  fclose(flib);
  ntree=Input();
  fclose (fp); 

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

  Printoutput( ntree );
  ft=strrchr(filename, (int)'.');
  if (ft!=NULL) {
    i=ft-filename+1;
    strcpy(&filename[i],"buf");
  } else
    strcat(filename,".buf");
  fp = fopen(filename, "w");

  if( output ) {
    printf("Nonredundant qc list before the source node:\n");
    if( c == 1 ) {
      print( rl );
    } else 
      travel( rt[n_buffertype-1] );
  }
  printf("The number of nodes %d\n", n_candidates);
  if (c == 1) {
    printf("The slack after buffer insertion : %10.10g\n", van_answer);
    printbuffer( final_location_van, &number_of_buffers );
    list_delete( rl );
  } else {
    printf("The slack after buffer insertion : %10.10g\n", shili_answer);
    printbuffer_shili( final_location_shili, &number_of_buffers );
    for ( i=n_buffertype-1;i>-1;i-- ) {
      tree_delete( rt[i] );
      tree_delete_swap( swap_list[i] );
    }
  }
  if( output ) {
    printf("the memory of solutions opened are %fM bytes \n", (float)total_solution/(1024*1024));/* unit is Mbyte */
    printf("the memory of solutions freezed are %fM bytes \n", (float)reduce_solution/(1024*1024));
    printf("the memory of composition opened are %fM bytes \n", (float)m_used/(1024*1024));
    printf("the memory of composition freezed are %fM bytes \n", (float)m_reduced/(1024*1024)); 
  }
  printf("The number of inserted buffer is : %d\n", number_of_buffers);

  fclose(fp);
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
  return 0;
}
