/* Released Software: Fast Buffer Insertion for Interconnect Optimization */
/*                      Version 2.3: 2003 - 2007 */

/* Author:  Zhuo Li                       -- zhybear@gmail.com  */
/* Author:  Weiping Shi                   -- wshi@ee.tamu.edu */

/* Permission to use for non-commercial purposes is granted provided */
/* that proper acknowledgments are given. */
/* For information about commercial use, please contact authors */

#define TREATZERO 1
#include "def.h"
/* Van Ginekin's O(n^2) time algorithm */
/* the data in tcad version and thesis is got from version
   under distributed FBI version not this one , the 
   difference is in that one when wire is propagated, y
   is created also in addtion to location memory */
/* main subroutine */
#define QM 1
#define PREDICT 1 /* predictive pruning */
RLnode *van(FBI_Node *t)
{
  RLnode dummy, *sleft, *sright, *s, *x, *slefthead, *srighthead;
  double k1, k2, c, q;
  Comp *p;

  if ( t->type==SINK )
    return create_list(t);
    
  sleft = van(t->left); 	
  k1 = distance(t,t->left);
  if ( t->right != (FBI_Node *)NULL ) {
    sright = van(t->right);
    k2 = distance(t,t->right);		
  }	

  if ( t->type == SOURCE ) {
    van_answer=0-INFINITY;
    x=sleft;
    while( x!=(RLnode *)NULL ) {
      q = x->time-0.5*r0*c0*k1*k1-(r0*k1+rd)*x->cap-rd*k1*c0;			
      if ( q > van_answer ) {
	van_answer = q;
	final_location_van = x->com;
      }
      x = x->next;
    }
    return sleft;
  } else {
    sleft = add_buffer(k1,sleft, t->index, t->left->index);
    sleft = redundent(sleft);
    /* if the node has only one child (default left), only 
       return update solution of left child */
    if ( t->right == (FBI_Node *)NULL ) {			
      if ( debug ) {
	printf("Debug Information: qc list at %d \n", t->index);
	print( sleft );
	printf("\n");
      }
      return sleft;
    } else { 
      sright = add_buffer(k2, sright, t->index, t->right->index);
      sright = redundent(sright);
    }
  }	
  if ( debug ) 
    {
      printf("Debug Information: qc list at left %d \n", t->index);
      print( sleft );
      printf("Debug Information: qc list at right %d \n", t->index);
      print( sright );
      printf("\n");
    }


  /* merge two list */
  s=&dummy;
  dummy.cap=0;
  dummy.time=0;
  dummy.com=(Comp *)NULL;
  dummy.next=(RLnode *)NULL;
  slefthead=(RLnode *)NULL; 
  srighthead=(RLnode *)NULL;
  slefthead=sleft;srighthead=sright;
  while ( sleft!=(RLnode *)NULL && sright!=(RLnode *)NULL ) {
    c=sleft->cap+sright->cap;
    q=sright->time;
    s->next=(RLnode *)malloc(sizeof(RLnode));
    total_solution+=sizeof(RLnode);
    if (total_solution-reduce_solution+m_used-m_reduced>max_space)
      max_space=total_solution-reduce_solution+m_used-m_reduced;
    s = s->next;
		
    if (sleft->time<sright->time)
      {
	q=sleft->time;
			
	p=(Comp *)malloc(sizeof(Comp));
	m_used+=sizeof(Comp);
	p->left=sleft->com;
	p->right=sright->com;
	if (p->left!=(Comp *)NULL)
	  p->left->counter++;
	if (p->right!=(Comp *)NULL)
	  p->right->counter++;
	p->buffertype=-1;
#if QM
        p->counter=1;
#else
	p->counter=0;
#endif
	p->x=t->index;
	p->y=t->index;
	s->com=p;
	sleft=sleft->next;
      } else {
      p=(Comp *)malloc(sizeof(Comp));
      m_used+=sizeof(Comp);
      p->left=sleft->com;
      p->right=sright->com;
      if (p->left!=(Comp *)NULL)
	p->left->counter++;
      if (p->right!=(Comp *)NULL)
	p->right->counter++;
      p->buffertype=-1;    // steiner merge point, indeed no buffer here
#if QM
      p->counter=1;
#else
      p->counter=0;
#endif
      p->x=t->index;
      p->y=t->index;
      s->com=p;
      sright=sright->next;					
    }

    if (total_solution-reduce_solution+m_used-m_reduced>max_space)
      max_space=total_solution-reduce_solution+m_used-m_reduced;
    s->cap=c;
    s->time=q;		
  }
  s->next=(RLnode *)NULL;    
	
  list_delete( slefthead );
  list_delete( srighthead );
  return dummy.next;	
}

/* create list */
RLnode *create_list(FBI_Node *t)
{
  RLnode *last;
  last = (RLnode *)malloc(sizeof(RLnode)); 
  total_solution += sizeof(RLnode);
  if (total_solution - reduce_solution + m_used - m_reduced > max_space )
    max_space = total_solution - reduce_solution + m_used - m_reduced;
  last->cap = t->cap;
  last->time = t->time;
  last->next = (RLnode *) NULL;
  last->com = (Comp *) NULL;
  return last;
}

/* add wire then buffer from previor subnode. */
RLnode *add_buffer(double k,RLnode *list, int px, int py)
{
  RLnode *x, *y, *z[MAXBUFFERTYPE], *pred, dummy;
  double q, qmax;
  int i, btype;
  Comp *p;

  /* if the wire has length 0, then quit */
#if TREATZERO
  if ( k == 0 )    
    return list;    
#endif
  /* get the solution after the edge from the current node to its child */
  x = list;
  while ( x != (RLnode *)NULL ) {
    x->time = x->time - 0.5*r0*c0*k*k - r0*k*x->cap;
    x->cap = x->cap + k*c0;
#if !QM      
    p = (Comp *)malloc( sizeof(Comp) );
    m_used += sizeof(Comp);
    p->left = x->com;
    p->right = (Comp *)NULL;
    if ( p->left != (Comp *)NULL)
      p->left->counter++;
    p->buffertype = 0;
    p->counter = 0;
    p->x = px;
    p->y = py;
    x->com = p;
#endif
    x = x->next;
  }
  dummy.next = list;
  /*  consider the buffer solution */
  for ( i=n_buffertype-1;i>-1;i-- ) {
    y = dummy.next;
    qmax = -INFINITY;
    while ( y != (RLnode *)NULL ) {
      q = y->time-tb[i]-rb[i]*y->cap;
      if ( q > qmax ) {
	qmax = q;
	btype = i+1;
	x = y;
      }
      y = y->next;
    }
    z[i] = (RLnode *)malloc(sizeof(RLnode));
    total_solution += sizeof(RLnode);
    z[i]->cap = cb[i];
    z[i]->time = qmax;

    p = (Comp *)malloc(sizeof(Comp));
    m_used += sizeof(Comp);
#if QM
    p->left = x->com;
    if ( p->left )
      p->left->counter++;
    p->right = (Comp *)NULL;
    p->counter = 1;
#else
    p->left = x->com->left;
    p->right = x->com->right;
    if ( p->left != (Comp *)NULL )
      p->left->counter++;
    if ( p->right != (Comp *)NULL )
      p->right->counter++;
    p->counter = 0;
#endif
    p->buffertype = btype;
    p->x = px;
    p->y = py;
    z[i]->com = p;
  }
	
  /* add buffer solution answer to the list */
  for ( i=n_buffertype-1;i>-1;i-- ) {
    y = dummy.next; pred = &dummy;
    while ( y!=(RLnode *)NULL ) {
      if ( fabs(z[i]->cap - y->cap )<1e-10) {
	if ( z[i]->time > y->time ) {
	  z[i]->next = y->next;
	  pred->next = z[i];
#if QM
          if ( y->com )
            y->com->counter--;
#endif
	  comp_delete( y->com );
	  free( y );
	  reduce_solution += sizeof(RLnode);
	} else {
#if QM
          if ( z[i]->com )
            z[i]->com->counter--;
#endif
	  comp_delete( z[i]->com );
	  free( z[i] );
	  reduce_solution += sizeof(RLnode);
	}
	break; 
      }
      if ( z[i]->cap < y->cap ) {
	z[i]->next = y;
	pred->next = z[i];					
	break;
      } else {
	if ( z[i]->cap > y->cap ) {
	  y = y->next;
	  pred = pred->next;
	} 
      }
    }
    if ( y==(RLnode *)NULL ) {
      z[i]->next = (RLnode *)NULL;
      pred->next = z[i];
    }		
  }
  if (total_solution-reduce_solution+m_used-m_reduced>max_space)
    max_space=total_solution-reduce_solution+m_used-m_reduced;
  return dummy.next;
}


/* remove redundent realizations */
/* list is sorted as increasing capacitance and arrival time */
RLnode *redundent(RLnode *list)
{
  RLnode dummy, *pred, *r;
  double x;

  dummy.cap = 0;
  dummy.time = -INFINITY;

  dummy.com = (Comp *)NULL;
  dummy.next = (RLnode *)NULL;
  dummy.next = list;

  pred = &dummy;
  r = list;

  while(r != (RLnode *) NULL) 
    {
#if PREDICT
      x= (r->time-rb[n_buffertype-1]*r->cap) - (pred->time-rb[n_buffertype-1]*pred->cap);
#else
      x= r->time-pred->time;
#endif
      if ( x < eps ) {
	pred->next = r->next;
#if QM
        if ( r->com )
          r->com->counter--;
#endif
	comp_delete(r->com);
	free(r);
	reduce_solution+=sizeof(RLnode);
      } 
      else
	pred = r;
      r = pred->next;
    }

  return dummy.next; 
}

void list_delete(RLnode *l)
{
  RLnode *tmp;

  while (l != (RLnode *) NULL) {
    tmp = l->next;
#if QM
    if ( l->com )
      l->com->counter--;
#endif
    comp_delete(l->com);
    free(l);
    reduce_solution+=sizeof(RLnode);
    l = tmp;
  }
}

void comp_delete(Comp *com)
{
  if (com == (Comp *) NULL)
    return;
  if (com->counter==0) {
    if (com->left!=(Comp *)NULL) {
      com->left->counter--;
      comp_delete(com->left);
    }
    if (com->right!=(Comp *)NULL) {
      com->right->counter--;
      comp_delete(com->right);
    }
    free(com);
    m_reduced+=sizeof(Comp);
  }
	
}
/* print list */
void print(RLnode *x)
{
  int i;
  if (x == (RLnode *)NULL) {
    printf("empty tree\n");
    return;
  }
  printf("\tq=%10.10g\tc=%10.10g\n", x->time, x->cap*1000);
  if (debug)
    printbuffer(x->com, &i);	
  if (x->next != (RLnode *)NULL) 
    print(x->next);	 
}
/* print location */
void printbuffer(Comp *x, int *i)
{
  if (x == (Comp *)NULL) {
    //fprintf(fp, "empty tree\n");
    return;
  }
  if ( x->buffertype!=0 && x->buffertype!=-1 ) {
    //fprintf(fp, "start %d  end %d  buffertype %d\n", x->x, x->y, x->buffertype);
    *i=*i+1;
  }
  if (x->left != (Comp *)NULL)
    printbuffer(x->left, i);	
  if (x->right != (Comp *)NULL)
    printbuffer(x->right, i);
}
