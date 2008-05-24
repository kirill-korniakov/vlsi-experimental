/* Released Software: Fast Buffer Insertion for Interconnect Optimization */
/*                      Version 2.3: 2003 - 2007 */

/* Author:  Zhuo Li                       -- zhybear@gmail.com  */
/* Author:  Weiping Shi                   -- wshi@ee.tamu.edu */

/* Permission to use for non-commercial purposes is granted provided */
/* that proper acknowledgments are given. */
/* For information about commercial use, please contact authors */

#include "def.h"

char logo1[] = "Fast Buffer Insertion Software Package 1.2 Copyright 2005 by Zhuo Li and Weiping Shi.\n";
char logo2[] = "Permission to use for non-commercial purposes is granted\n";
char logo3[] = "provided that proper acknowledgements are given.\n";
char logo4[] = "For information about commercial use, please contact authors.\n";
char logo5[] = "Please send bug reports to zhybear@gmail.com.\n";

void logo()
{
  printf("%s", logo1);
  printf("%s", logo2);
  printf("%s", logo3);
  printf("%s", logo4);
  printf("%s", logo5);
}

void InputLib(FILE *flib)
{
  char buf[BUFSIZE];
  char buf2[]="    ";
  char buf3[BUFSIZE];
  char c;
  int ix, begin;

  ix=0;
  begin=0;

  n_buffertype=0;
  while((c = fgetc(flib)) != EOF) {
    if( c == '\n') {
      buf[ix]='\0';
      ix=0;
      if( !begin ) {
	sscanf(buf, "%4c", buf2);
	if( !strcmp(buf2, "buff" ) ) {
	  sscanf(buf, "%s %lf %lf %lf", buf3, &cb[n_buffertype], 
		 &tb[n_buffertype],&rb[n_buffertype]);
	  cb[n_buffertype]=cb[n_buffertype]/1000;
	  n_buffertype++;
	}
	begin=1;
      }
    } else {
      buf[ix++] = c;
      begin=0;
    }
  }
}

FBI_Node *Input()
{
  char buf[BUFSIZE];
  char buf1[]="       ";
  char buf2[]="    "; 
  char buf3[BUFSIZE];
  int c, N_SEG;
  int ix, begin, j=0;
  FBI_Node *t; 

  ix=0;   
  begin=0;
  n_sinks=0; n_nodes=0; n_edges=0;   

  /* Parser */
  printf("Parsing the input tree file....\n");
  while( (c = fgetc(fp)) != EOF) {
    if ( c == '\n' ) {
      buf[ix] = '\0';
      ix = 0;
      if ( !begin ) { 
	sscanf(buf, "%7c", buf1);
	if ( !strcmp(buf1, "wire_re") )			   	   
	  sscanf(buf, "%s %lf", buf3, &r0);
	if ( !strcmp(buf1, "wire_ca") ) {
	  sscanf(buf, "%s %lf", buf3, &c0);
	  c0 = c0/1000;  // turn fF to pF
	}
	sscanf(buf, "%4c", buf2); 			  
	if ( !strcmp(buf2, "driv") )  {	 
	  sscanf(buf, "%s %lf %lf %lf", buf3, &nodes[n_nodes].x, &nodes[n_nodes].y, &rd);				   
	  nodes[n_nodes].index = 0;
	  dx = nodes[n_nodes].x;
	  dy = nodes[n_nodes].y;				   
	  nodes[n_nodes].cap = 0.0;	
	  nodes[n_nodes].type = SOURCE;				  
	  nodes[n_nodes].left = (FBI_Node *)NULL;
	  nodes[n_nodes].right = (FBI_Node *)NULL;
	  n_sinks ++;
	  n_nodes ++;
	}
	if ( !strcmp(buf2, "sink") ) {
	  sscanf(buf, "%s %d %lf %lf %lf %lf", buf3, &nodes[n_nodes].index, &nodes[n_nodes].x, &nodes[n_nodes].y, &nodes[n_nodes].cap, &nodes[n_nodes].time);
	  if ( nodes[n_nodes].index == 0 )  {
	    printf("Error! The index of sink can not be zero.\n");
	    exit;
	  }				   
	  nodes[n_nodes].type = SINK;
	  nodes[n_nodes].cap = nodes[n_nodes].cap/1000;				   
	  nodes[n_nodes].left = (FBI_Node *)NULL;
	  nodes[n_nodes].right = (FBI_Node *)NULL;				   
	  n_sinks ++;				
	  n_nodes ++;
	}
	if ( !strcmp(buf2, "cand") )  {
	  sscanf(buf, "%s %d %lf %lf", buf3, &nodes[n_nodes].index, &nodes[n_nodes].x, &nodes[n_nodes].y);
	  if ( nodes[n_nodes].index == 0 ) {
	    printf("Error! The index of candidates can not be zero.\n");
	    exit;
	  }				  
	  nodes[n_nodes].type = FBI_IN;				  
	  nodes[n_nodes].cap = 0.0;				  
	  nodes[n_nodes].left = (FBI_Node *)NULL;
	  nodes[n_nodes].right = (FBI_Node *)NULL;				   
	  n_nodes ++;				
	}
	if ( !strcmp(buf2, "edge") ) {
	  sscanf(buf, "%s %d %d", buf3, &edges[n_edges].s_index, &edges[n_edges].t_index);				   				   
	  n_edges ++;				
	}
	begin = 1;
      }
    } else {
      buf[ix++] = c;
      begin = 0;
    }
  }  

  n_candidates=n_nodes-n_sinks;   
  t=produce_tree();

  /* wire segmenting algorithm */
  N_SEG = 10; //Wire Segmenting coef (0 means no segmenting:)
  if (N_SEG!=0)
    wire_segment(N_SEG);
  return t;
}

/* Generate Routing Tree topology */
FBI_Node *produce_tree()
{
  int i, j , k, a;
  int target[2];
  for ( i=0;i<n_sinks+n_candidates;i++ ) {
    a = 0;
    target[0] = -1; target[1] = -1;
    for ( j=0;j<n_edges;j++ ) {
      if ( a > 1 )
	break;
      if ( nodes[i].index == edges[j].s_index ) {
	for ( k=0;k<n_sinks+n_candidates;k++ ) {
	  if ( nodes[k].index == edges[j].t_index ) {
	    target[a] = k;
	    a ++;			
	  }
	}						
      }
    }
    if ( target[0] > -1 )
      nodes[i].left = &nodes[target[0]];
    if ( target[1] > -1 )
      nodes[i].right = &nodes[target[1]];
    else
      nodes[i].right = (FBI_Node *)NULL;		
  }
  return &nodes[0];
}

/* Print tree on screen */
void ntree_print( FBI_Node *p, int ind )
{
  printf("%d %f %f %d \n", p->index, p->x, p->y, p->type);
  switch( p->type ) {
  case FBI_IN:
    if ( p->right != (FBI_Node *)NULL ) {
      ntree_print( p->left, ind+1 );
      ntree_print( p->right, ind+1 );
    } else
      ntree_print( p->left, ind );
    return;
  case SINK:
    return;
  default:
    ntree_print( p->left, ind );
    return;
  }
}

void Printoutput(FBI_Node *p)
{
  int i;
  FILE* stream;
  char *ft;
  ft=strrchr(filename, (int)'.');
  if (ft!=NULL) {
    i=ft-filename+1;
    strcpy(&filename[i],"finalnet"); 
  } else
    strcat(filename,".finalnet");
  stream=fopen(filename, "w");
  
  fprintf(stream, "%s %f\n", "wire_res_per_unit", r0);
  fprintf(stream, "%s %f\n", "wire_cap_per_unit", c0*1000);
  fprintf(stream, "\n%s %f %f %f\n", "driver", dx, dy, rd);
  fprintf(stream, "%s %d\n", "number_of_sinks", n_sinks-1);
  for ( i=1;i<n_sinks;i++ )
    fprintf(stream, "%s %d %f %f %f %f\n", "sink", nodes[i].index, nodes[i].x, nodes[i].y, nodes[i].cap*1000, nodes[i].time);  
  fprintf(stream, "\n%s %d\n", "number_of_candidate_nodes", n_candidates);
  for ( i=n_sinks;i<n_sinks+n_candidates;i++ )
    fprintf(stream, "%s %d %f %f\n", "candidate", nodes[i].index, nodes[i].x, nodes[i].y);
 
  fprintf(stream, "\n");
  edge_print(stream, p);
  fclose(stream);  
}

/* print edges of tree p into file stream*/
void edge_print( FILE *stream, FBI_Node *p )
{
  switch( p->type ) {
  case FBI_IN:
    if ( p->right != (FBI_Node *)NULL ) {
      fprintf(stream, "edge %d %d \n", p->index, p->left->index);
      fprintf(stream, "edge %d %d \n", p->index, p->right->index);
      edge_print( stream, p->left );
      edge_print( stream, p->right );
    } else {
      fprintf(stream, "edge %d %d \n", p->index, p->left->index);
      edge_print( stream, p->left );
    }
    return;
  case SINK:
    return;
  default:
    fprintf(stream, "edge %d %d \n", p->index, p->left->index);
    edge_print(stream, p->left);
    return;
  }
}













