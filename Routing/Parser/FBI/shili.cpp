/* Released Software: Fast Buffer Insertion for Interconnect Optimization */
/*                      Version 2.3: 2003 - 2007 */

/* Author:  Zhuo Li                       -- zhybear@gmail.com  */
/* Author:  Weiping Shi                   -- wshi@ee.tamu.edu */

/* Permission to use for non-commercial purposes is granted provided */
/* that proper acknowledgments are given. */
/* For information about commercial use, please contact authors */

#define TREATZERO 1
#include "def.h"

/* Shi and li's (FBI) fast buffer insertion time algorithm */

/* NOTE:  This implementation runs in O(nlog^2) time. */

void shili(double *k, FBI_Node *t, RTnode **rt, RTnode **list)
{
  RTnode *Rleft[MAXBUFFERTYPE], *Rright[MAXBUFFERTYPE], *dummytree[MAXBUFFERTYPE], *x, *y, *z, *lbound, *rbound, *a;
  RTnode *swap_listl[MAXBUFFERTYPE], *swap_listr[MAXBUFFERTYPE], *dummylist[MAXBUFFERTYPE];
  RLnode Rlist, *l;
  Comp *p;
  double dis, q;
  double kleft[MAXBUFFERTYPE], kright[MAXBUFFERTYPE], k1, k2, dummyk;

  int i; 
	
  if( t->type==SINK ) {
    create( k, rt, t, list );
    return;
  }	
  for ( i=0;i<n_buffertype;i++ ) {
    swap_listl[i] = NIL;
    Rleft[i] = NIL;
    kleft[i] = 0.0;
  }
  shili( kleft, t->left, Rleft, swap_listl );
  k1 = distance( t, t->left );
  for ( i=0;i<n_buffertype;i++ )
    kleft[i] += k1;
  if ( t->right != (FBI_Node *)NULL )	{
    for ( i=0;i<n_buffertype;i++ ) {
      swap_listr[i] = NIL;
      Rright[i] = NIL;
      kright[i] = 0.0;
    }
    shili( kright, t->right, Rright, swap_listr );
    k2 = distance( t, t->right );
    for ( i=0;i<n_buffertype;i++ )
      kright[i] += k2;
  }
  /* if t is source, calculate the maximum q. Note that source can only have degree 1
     if t is internal node, update the left child and right child, and merge them together */
  if( t->type==SOURCE ) {
    shili_answer = 0-INFINITY;
    x = minimum( Rleft[n_buffertype-1] );		
    while( x != NIL ) {
      q = x->time-0.5*r0*c0*k1*k1-(r0*k1+rd)*x->cap-rd*k1*c0;
      if ( q > shili_answer ) {
	shili_answer = q;
	final_location_shili = x->com;
      }
      x = node_succ( x );
    }		
    for (i=0;i<n_buffertype;i++) {
      rt[i] = Rleft[i];
      list[i] = swap_listl[i];
    }
    return;
  } else {
    buffer_update( kleft, k1, Rleft, swap_listl, t->index, t->left->index );		
    /* if the node has only one child (default left), 
       only return update solution of left child */
    if ( t->right == (FBI_Node *)NULL ) {			
      q = k[n_buffertype-1];
      for ( i=0;i<n_buffertype;i++ ) {
	list[i] = swap_listl[i];
	k[i] = kleft[i];
      }
      if (debug) {
	printf( "Debug Information: qc list at %d\n", t->index );
	travel( Rleft[n_buffertype-1] );
	printf("\n");
      }
      for ( i=0;i<n_buffertype;i++ )
	rt[i]=Rleft[i];			
      return;
    } else 
      buffer_update( kright, k2, Rright, swap_listr, t->index, t->right->index );		
  }	

  if (debug) {
    printf("Debug Information: qc list at left %d\n", t->index);
    travel( Rleft[n_buffertype-1] );
    printf("Debug Information: qc list at right %d\n", t->index);
    travel( Rright[n_buffertype-1] );
    printf("\n");
  }
	
  for ( i=0;i<n_buffertype;i++ ) {
    if ( Rleft[i]->size > Rright[i]->size ) {
      dummytree[i] = Rleft[i];
      Rleft[i] = Rright[i];
      Rright[i] = dummytree[i];
      dummylist[i] = swap_listl[i];
      swap_listl[i] = swap_listr[i];
      swap_listr[i] = dummylist[i];			
      dummyk = kleft[i];
      kleft[i] = kright[i];
      kright[i] = dummyk;
    }
  }
		
  /* now Rleft small than Rright */

	
  /* NOTE:  This part can be modified so that if Rleft
     is about the same as Rright, do a simple merge */


  for (i=0;i<n_buffertype;i++) {

    /* Step 1.  arrival time determined by Rleft or both.
       For each realization x in Rleft, find an option y
       in Rright, and store the results in Rlist->next */

    l = &Rlist;
    l->next = (RLnode *) NULL;
    x = minimum(Rleft[i]);
    while (x != NIL) {
      if ( (y = succ(Rright[i], x->time)) != NIL ) {
	l->next = (RLnode *) malloc(sizeof(RLnode));
	total_solution += sizeof(RLnode);

	l = l->next;
	l->next = (RLnode *) NULL;
	l->time = x->time;
	l->cap = x->cap + y->cap;

	p = (Comp *)malloc(sizeof(Comp));
	m_used += sizeof(Comp);
	p->left = x->com;
	p->right = y->com;
	if (p->left != (Comp *)NULL)
	  p->left->counter ++;
	if (p->right != (Comp *)NULL)
	  p->right->counter ++;
	p->counter = 1;
	p->buffertype = -1;
	p->x = t->index;
	p->y = t->index;
				
	l->com = p;
	x = node_succ( x );
      }
      else
	break;
    }

    if (total_solution-reduce_solution+m_used-m_reduced>max_space)
      max_space=total_solution-reduce_solution+m_used-m_reduced;
    /* Step 2. Arrival determined by Rright.  For each
       realization x in Rleft, find an interval [lb, rb]
       in Rright that is lesser than x but greater than prev(x),
       change cadd fields of the interval, and update swap list*/

    list[i] = swap_listr[i];
    k[i] = kright[i];
    x = maximum(Rleft[i]);
    rbound = pred(Rright[i], x->time - eps);
    if ( rbound == NIL ) {			
      tree_delete(Rright[i]);
      tree_delete_swap( swap_listr[i] );
      Rright[i] = NIL;
      list[i] = NIL;
      k[i] = kleft[i];			
    } else {
      while ( (y = maximum(Rright[i])) != rbound ) {			
	if ( (a=y->swap_option)!=NIL ) {
	  node_delete_swap( &list[i], a );
	  y->swap_option = NIL;
	}
	node_delete( &Rright[i], y );		
      }		
    }
    while (rbound != NIL) {		
      if ((y = node_pred(x)) == NIL) {	
	/* first x in Rleft */
	interval( minimum(Rright[i]),rbound,x );
	break;
      } 
      else {
	lbound = succ( Rright[i], y->time + eps );
	if ( lbound != NIL && lbound->time <= rbound->time ) {
	  interval( lbound, rbound, x );
	  rbound = node_pred(lbound);
	}
	x = y;
      }
    }

    k1=k[i];
		
    /* Step 3. Insert Rlist into Rright. */
    while (Rlist.next != (RLnode *) NULL) {
      l = Rlist.next;			
      x = insert( &Rright[i], l->time, l->cap, l->com, &list[i] );
//      if ( (x = search(Rright[i],l->time))!=NIL ) {
      if (x != NIL) {
//	while ( x->swap_option != NIL )
//          x = node_succ(x);       // precise requirement
	if ( (z=node_succ(x))!=NIL ) {
	  dis = ((z->time-x->time)/(z->cap-x->cap)-rb[i])/r0;
	  dis = dis+k1;
	  if ( (a=z->swap_option)!=NIL ) 
	    node_delete_swap(&list[i], a);
	  a = insert_swap( &list[i], dis, z->time );
//	  if ( (a=search(list[i],dis))!=NIL )
	  if (a != NIL)
          {
//	      while ( a->qc_option!=NIL )
//		a = node_succ(a);
	      z->swap_option = a;        
	      a->qc_option = z;
	    } else
	    z->swap_option = NIL;
	}
	if ( (y=node_pred(x))!=NIL ) {
	  dis = ((x->time-y->time)/(x->cap-y->cap)-rb[i])/r0;
	  dis = dis+k1;
	  a = insert_swap( &list[i], dis, x->time );
//	  if ( (a=search( list[i],dis))!=NIL )
          if (a != NIL)
	    {
//	      while (a->qc_option!=NIL)
//		a = node_succ(a);
	      x->swap_option = a;
	      a->qc_option = x;
	    } else
	    x->swap_option = NIL;
	}		
      }
      Rlist.next = l->next;
      if (l->com != (Comp *)NULL)
	l->com->counter--;
      comp_delete( l->com );
      free(l);
      reduce_solution += sizeof(RLnode);
    }
  }			

  for ( i=0;i<n_buffertype;i++ )
    {
      tree_delete( Rleft[i] );
      tree_delete_swap( swap_listl[i] );
    }
  for (i=0;i<n_buffertype;i++)
    rt[i] = Rright[i];
  return;
}


/* create realization tree of sink t */
void create(double *k, RTnode **rt, FBI_Node *t, RTnode **list)
{
  int i;
  for ( i=0;i<n_buffertype;i++ ) {
    rt[i] = NIL;
    insert_swap( &rt[i], t->time, t->cap );
    list[i] = NIL;
    k[i] = 0.0;
  }	
  return;
}

/* add wire and buffer from previous node
   (if merge, before merge point)*/
void buffer_update(double *k, double k1, RTnode **Rnext, RTnode **list, int px, int py)
{
  RTnode *x, *y, *z, *min, *a;
  Comp *plist[MAXBUFFERTYPE];
  double dis,q[MAXBUFFERTYPE],c[MAXBUFFERTYPE];
  int size, i, j;	
    
  /* if the wire has length 0, then quit */
#if TREATZERO
  if ( k1==0 )   
    return;
#endif        
  for ( i=n_buffertype-1;i>-1;i-- )	{     
    /* Step 1: check if there is redundancy after adding the wire
       if it happens,delete the corresponding qc pairs */
    size=Rnext[i]->size;
    while( size>=2 && (min=minimum(list[i]))!=NIL )	{
      if (min->time<=k[i]+eps_distance) {
	if ( (x = min->qc_option)!=NIL ) { 
	  /* x node is i node that satisfy l(i)=((Q(i)-Q(i-1))/(C(i)-C(i-1))-Rb)/R0<=0;*/
	  if ( (z = node_succ(x))!=NIL ) {
	    travel_update( Rnext[i],z );						
	    if ( (y=node_pred(x))!=NIL ) {
	      travel_update( Rnext[i],y );						
	      dis = ((z->time-y->time)/(z->cap-y->cap)-rb[i])/r0;
	      dis = (dis+k[i]-k1);
	      if ( (a=z->swap_option)!=NIL )
		node_delete_swap( &list[i], a );									
	      a = insert_swap( &list[i], dis, z->time );
//	      if ( (a=search(list[i], dis))!=NIL ) {
              if (a != NIL) {
//		while ( a->qc_option!=NIL )
//		  a = node_succ(a);
		z->swap_option = a;
		a->qc_option = z;
	      } else
		z->swap_option=NIL;							
	    } else if ( (a=z->swap_option)!=NIL ) {
	      node_delete_swap( &list[i], a );	
	      z->swap_option = NIL;
	    }
	  }
	  node_delete_swap( &list[i], min );
	  x->swap_option = NIL;
	  node_delete( &Rnext[i], x );
	  size = size-1;				
	}
      }
      else 
	break;
    }		
    min = maximum(Rnext[i]);
    q[i] = (min->time-0.5*r0*c0*k1*k1-r0*k1*min->cap-rb[i]*(min->cap+k1*c0)-tb[i]);	
    c[i] = cb[i];

    plist[i] = (Comp *)malloc(sizeof(Comp));
    m_used += sizeof(Comp);			
    if (total_solution-reduce_solution+m_used-m_reduced>max_space)
      max_space=total_solution-reduce_solution+m_used-m_reduced;
    plist[i]->left = min->com;
    plist[i]->right = (Comp *)NULL;
    if (plist[i]->left != (Comp *)NULL)
      plist[i]->left->counter++;
    plist[i]->counter = 1;
    plist[i]->buffertype = i+1;
    plist[i]->x = px;
    plist[i]->y = py;
  }
	
	
  for (i=0;i<n_buffertype;i++) {
    Rnext[i]->dirty = 1;
    Rnext[i]->ra += r0*k1;
    Rnext[i]->da += 0.5*r0*c0*k1*k1;
    Rnext[i]->ca += c0*k1;	 	  		
  }

	
  /* Step 2: add the buffer solution and update the swap_list */
  for ( i=0;i<n_buffertype;i++ ) {				
    for ( j=0;j<n_buffertype;j++ ) {		
      x = insert( &Rnext[j],q[i],c[i],plist[i], &list[j] );
//      if ( (x=search(Rnext[j],q[i]))!=NIL ) {
      if (x != NIL) {
//	while ( x->swap_option!=NIL )
//	  x = node_succ(x);         // precise requirement
	if ( (z=node_succ(x))!=NIL ) {		
	  dis = ((z->time-x->time)/(z->cap-x->cap)-rb[j])/r0;
	  dis = (dis+k[j]);
	  if ( (a=z->swap_option)!=NIL )
	    node_delete_swap( &list[j], a );
	  a = insert_swap( &list[j], dis, z->time );
//	  if ( (a=search(list[j], dis))!=NIL ) {
          if (a != NIL) {
//	    while ( a->qc_option!=NIL )
//	      a = node_succ(a);
	    z->swap_option = a;
	    a->qc_option = z;
	  } else
	    z->swap_option = NIL;									
	}
	if ( (y=node_pred(x))!=NIL ) {
	  dis = ((x->time-y->time)/(x->cap-y->cap)-rb[j])/r0;
	  dis = (dis+k[j]);
	  a = insert_swap( &list[j], dis, x->time );
//	  if ( (a=search(list[j], dis))!=NIL ) {
          if (a != NIL) {
//	    while ( a->qc_option!=NIL )
//	      a = node_succ(a);
	    x->swap_option = a;
	    a->qc_option = x;
	  } else
	    x->swap_option = NIL;
	}
      }
    }
    if (plist[i] != (Comp *)NULL)
      plist[i]->counter--;
    comp_delete(plist[i]);
  }
  return;
}


/* change the ca, com and coma  fields of interval [lb, rb] */
void interval( RTnode *lb, RTnode *rb, RTnode *x )
{
  RTnode *y, *lca;
  Comp *p;

  if (lb == rb) {
    lb->cap += x->cap;

    p = (Comp *)malloc(sizeof(Comp));
    m_used += sizeof(Comp);
    if ( total_solution-reduce_solution+m_used-m_reduced>max_space )
      max_space=total_solution-reduce_solution+m_used-m_reduced;
    p->right = lb->com;
    p->left = x->com;
    if ( p->left!=(Comp *)NULL )
      p->left->counter ++;
    p->counter = 1;
    p->buffertype = -1;
    p->x = 0;
    p->y = 0;
    lb->com = p;
    return;
  }
  /* find least common ancester */
  for (y = lb; y != NIL; y = y->parent)
    if (y->time >= lb->time && y->time <= rb->time)
      lca = y;

  /* left boundary */
  for ( y = lb; y != lca; y = y->parent )
    if ( y->time >= lb->time ) {
      y->cap += x->cap;
      y->right->ca += x->cap;
      y->right->dirty = 1;
			
      p = (Comp *)malloc(sizeof(Comp));
      m_used += sizeof(Comp);
      p->right = y->com;
      p->left = x->com;
      if ( p->left!=(Comp *)NULL )
	p->left->counter ++;
      p->counter = 1;
      p->buffertype = -1;
      p->x = 0;
      p->y = 0;
      y->com = p;

      if (y->right != NIL) {
	if ( y->right->coma == (Comp *) NULL ) {
	  y->right->coma = x->com;
	  if ( y->right->coma != (Comp *)NULL )
	    y->right->coma->counter ++;
	} else {
	  p = (Comp *)malloc(sizeof(Comp));
	  m_used += sizeof(Comp);
	  p->right = y->right->coma;
	  p->left = x->com;
	  if ( p->left!=(Comp *)NULL )
	    p->left->counter ++;
	  p->counter = 1;
	  p->buffertype = -1;
	  p->x = 0;
	  p->y = 0;
	  y->right->coma = p;
	}
      }
    }

  /* right boundary */
  for (y = rb; y != lca; y = y->parent)
    if (y->time <= rb->time) {
      y->cap += x->cap;
      y->left->ca += x->cap;
      y->left->dirty = 1;

      p = (Comp *)malloc(sizeof(Comp));
      m_used += sizeof(Comp);
      p->right = y->com;
      p->left = x->com;
      if ( p->left != (Comp *)NULL )
	p->left->counter++;
      p->counter = 1;
      p->buffertype = -1;
      p->x = 0;
      p->y = 0;
      y->com = p;

      if ( y->left!=NIL ) {
	if ( y->left->coma == (Comp *) NULL ) {
	  y->left->coma = x->com;
	  if ( y->left->coma != (Comp *) NULL )
	    y->left->coma->counter ++;
	} else {
	  p = (Comp *)malloc(sizeof(Comp));
	  m_used += sizeof(Comp);
	  p->right = y->left->coma;
	  p->left = x->com;
	  if ( p->left!=(Comp *)NULL )
	    p->left->counter ++;
	  p->counter = 1;
	  p->buffertype = -1;
	  p->x = 0;
	  p->y = 0;
	  y->left->coma = p;
	}
      }
    }

  /* least common ancester */
  lca->cap += x->cap;

  p = (Comp *)malloc(sizeof(Comp));
  m_used += sizeof(Comp);
  if (total_solution-reduce_solution+m_used-m_reduced>max_space)
    max_space=total_solution-reduce_solution+m_used-m_reduced;
  p->right = lca->com;
  p->left = x->com;
  if ( p->left!=(Comp *)NULL )
    p->left->counter ++;
  p->counter = 1;
  p->buffertype = -1;
  p->x = 0;
  p->y = 0;
  lca->com = p;			
}

double distance(FBI_Node *t, FBI_Node *t1)
{
  return (fabs((t->x-t1->x))+fabs(t->y-t1->y));
}

/***************************************************
02/21/2007: Since the inserted node is returned now in insert() function, I disable the search part after inserting a node.
This should speed up the program a little bit and right now I didn't see the bug
***************************************************/
