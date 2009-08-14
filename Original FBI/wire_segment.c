/* Released Software: Fast Buffer Insertion for Interconnect Optimization */
/*                      Version 2.3: 2003 - 2007 */

/* Author:  Zhuo Li                       -- zhybear@gmail.com  */
/* Author:  Weiping Shi                   -- wshi@ee.tamu.edu */

/* Permission to use for non-commercial purposes is granted provided */
/* that proper acknowledgments are given. */
/* For information about commercial use, please contact authors */

#include "def.h"
/* equal wire segmenting algorithm */
void wire_segment( N_SEG )
     int N_SEG; 
{
  int i,j, index_s, index_t, M, shift_index;
  int x1, y1, z, dummy, new_node, new_edge;
  double l, l1;

  new_node = 0; new_edge = 0;
  shift_index = 50000;
  for ( z=0;z<n_edges;z++ ) {
    index_s = -1; index_t = -1;
    for ( j=0;j<n_nodes;j++ ) {
      if ( edges[z].s_index == nodes[j].index ) {
	index_s = j;
	for ( i=0;i<n_sinks+n_candidates;i++ )
	  if ( edges[z].t_index==nodes[i].index ) {
	    index_t = i;
	    break;
	  }
	break;
      }		
    }

    l = distance( &nodes[index_s], &nodes[index_t] );
    if ( l < eps_distance )
      continue;
    else {
      dummy = index_s;			
      M = N_SEG;			
      for ( i=0;i<M;i++ ) {
	l1 = (i+1.0)/(M+1.0)*l;
	x1 = i + new_node + n_nodes;
	nodes[x1].index = shift_index + n_candidates + new_node + i;
	if ( fabs( nodes[index_s].y-nodes[index_t].y ) >= l1 ) {
	  nodes[x1].x = nodes[index_s].x;
	  if ( nodes[index_t].y >= nodes[index_s].y )								
	    nodes[x1].y = nodes[index_s].y + l1;
	  else
	    nodes[x1].y = nodes[index_s].y - l1;
	} else {
	  nodes[x1].y = nodes[index_t].y;
	  if ( nodes[index_t].x >= nodes[index_s].x )
	    nodes[x1].x = nodes[index_s].x + l1 - fabs( nodes[index_s].y-nodes[index_t].y );
	  else
	    nodes[x1].x = nodes[index_s].x - ( l1 - fabs( nodes[index_s].y-nodes[index_t].y ) );
	}
	nodes[x1].type = IN;
	y1 = n_edges + new_edge + i;
	if ( dummy == index_s )	{
	  if ( nodes[dummy].left->index == nodes[index_t].index )
	    nodes[dummy].left = &nodes[x1];
	  else
	    nodes[dummy].right = &nodes[x1];
	} else {
	  nodes[dummy].left = &nodes[x1];
	  nodes[dummy].right = (Node *)NULL;
	}
	edges[y1].s_index = nodes[dummy].index;
	edges[y1].t_index = nodes[x1].index;
	dummy = x1;
      }
      if ( M!=0 ) {
	nodes[dummy].left = &nodes[index_t];			
	nodes[dummy].right = (Node *)NULL;
      }
      edges[z].s_index = nodes[dummy].index;
      new_edge +=M;
      new_node +=M;
    }		
  }
  n_edges += new_edge;
  n_candidates += new_node;
  n_nodes += new_node;
  return;
}
