/* Released Software: Fast Buffer Insertion for Interconnect Optimization */
/*                      Version 2.3: 2003 - 2007 */

/* Author:  Zhuo Li                       -- zhybear@gmail.com  */
/* Author:  Weiping Shi                   -- wshi@ee.tamu.edu */

/* Permission to use for non-commercial purposes is granted provided */
/* that proper acknowledgments are given. */
/* For information about commercial use, please contact authors */

#define TREATZERO 1
#include "def_b.h"
/* Van Ginekin's O(n^2) time algorithm */
/* main subroutine */
#define QM 1
#define PREDICT 1 /* predictive pruning */
RLnode *van(t)
     Node *t;
{
  RLnode dummy, *sleft, *sright, *s, *x, *slefthead, *srighthead;
  double k1, k2, c, q;
  Comp *p;

  if ( t->type==SINK )
    return create_list(t);
    
  sleft = van(t->left); 	
  k1 = distance(t,t->left);
  if ( t->right != (Node *)NULL ) {
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
    if ( t->right == (Node *)NULL ) {			
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
    s->next->prev = s;
    s = s->next;
		
    if (sleft->time<sright->time) {
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
  dummy.next->prev = (RLnode *)NULL;	
  list_delete( slefthead );
  list_delete( srighthead );
  return dummy.next;	
}

/* create list */
RLnode *create_list(t)
     Node *t;
{
  RLnode *last;
  last = (RLnode *)malloc(sizeof(RLnode)); 
  total_solution += sizeof(RLnode);
  if (total_solution - reduce_solution + m_used - m_reduced > max_space )
    max_space = total_solution - reduce_solution + m_used - m_reduced;
  last->cap = t->cap;
  last->time = t->time;
  last->next = (RLnode *) NULL;
  last->prev = (RLnode *) NULL;
  last->com = (Comp *) NULL;
  return last;
}

/* add wire then buffer from previor subnode. */
RLnode *add_buffer(k,list, px, py)
     double k;
     RLnode *list;
     int px, py;
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
  y = cover( list );
  for ( i = 0 ; i < n_buffertype ; i++ ) {
    while ( y->listnext != (RLnode *) NULL ) {
      if ( y->time - rb[i]*y->cap < y->listnext->time - 
	   rb[i]*y->listnext->cap ) {
	y = y->listnext;                              
      } else
	break;
    }   
    z[i] = (RLnode *)malloc(sizeof(RLnode));
    total_solution += sizeof(RLnode);
    z[i]->cap = cb[i];
    z[i]->time = y->time - tb[i] - rb[i] * y->cap;
      
    p = (Comp *)malloc(sizeof(Comp));
    m_used += sizeof(Comp);
#if QM
    p->left = y->com;
    if ( p->left )
      p->left->counter++;
    p->right = (Comp *)NULL;
    p->counter = 1;
#else
    p->left = y->com->left;
    p->right = y->com->right;
    if ( p->left != (Comp *)NULL ) 
      p->left->counter++;             
    if ( p->right != (Comp *)NULL )
      p->right->counter++;
    p->counter = 0;
#endif
    p->buffertype = i + 1;
    p->x = px;
    p->y = py;
    z[i]->com = p;
  }    
    
  /* add buffer solution answer to the list */
  y = dummy.next; pred = &dummy;
  for ( i = 0; i < n_buffertype; i++ ) {
    while ( y!=(RLnode *)NULL ) {
      if ( fabs(z[i]->cap - y->cap )<1e-10) {
	if ( z[i]->time > y->time ) {
	  z[i]->next = y->next;
	  if ( y->next != (RLnode *)NULL )
	    y->next->prev = z[i];
	  pred->next = z[i];
	  z[i]->prev = pred;
#if QM
          if ( y->com )
            y->com->counter--;
#endif
	  comp_delete( y->com );
	  free( y );
	  reduce_solution += sizeof(RLnode);
	  y = z[i];
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
	y->prev = z[i];
	pred->next = z[i];
	z[i]->prev = pred;
	pred = z[i];
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
      z[i]->prev = pred;
      y = z[i];
    }		
  }
  dummy.next->prev = (RLnode *)NULL;
  if (total_solution-reduce_solution+m_used-m_reduced>max_space)
    max_space=total_solution-reduce_solution+m_used-m_reduced;
  return dummy.next;
}


/* remove redundent realizations */
/* list is sorted as increasing capacitance and arrival time */
RLnode *redundent(list)
     RLnode *list;
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
  while(r != (RLnode *) NULL) {
#if PREDICT
    x = P(r, rb[n_buffertype-1]) - P(pred, rb[n_buffertype-1]);
#else
    x = r->time - pred->time;
#endif
    if ( x < eps ) {
      pred->next = r->next;
      if ( r->next != (RLnode *) NULL )
	r->next->prev = pred;
#if QM
      if ( r->com )
        r->com->counter--;
#endif
      comp_delete(r->com);
      free(r);
      reduce_solution+=sizeof(RLnode);
    } else
      pred = r;
    r = pred->next;
  }
  dummy.next->prev = (RLnode *)NULL;
  return dummy.next; 
}

void list_delete(l)
     RLnode *l;
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

void comp_delete(com)
     Comp *com;
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
void print(x)
     RLnode *x;
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
void printbuffer(x, i)
     Comp *x;
     int *i;
{
  if (x == (Comp *)NULL) {
    fprintf(fp, "empty tree\n");
    return;
  }
  if ( x->buffertype!=0 && x->buffertype!=-1 ) {
    fprintf(fp, "start %d  end %d  buffertype %d\n", x->x, x->y, x->buffertype);
    *i=*i+1;
  }
  if (x->left != (Comp *)NULL)
    printbuffer(x->left, i);	
  if (x->right != (Comp *)NULL)
    printbuffer(x->right, i);
}
/* 2004 09/08, new cover code to find the cover of (q,c) list. */
RLnode *cover( list )
     RLnode *list;
{
  RLnode dummy, *pred, *r;    
  double z;
  for ( r = list ; r != (RLnode *)NULL ; r = r->next ) {
    r->listnext = r->next;
    if ( r->listnext != NULL)
      r->listnext->listprev = r;
  }
  dummy.listnext = list;
  list->listprev = &dummy;
  dummy.cap = 0;
  dummy.time = -INFINITY;
    
  if ( list->listnext != (RLnode *)NULL )
    r=list->listnext;
  else
    return list;
  while ( r != (RLnode *) NULL ) {
    if ( r->listnext != (RLnode *)NULL ) {
      z = ( r->time - r->listprev->time ) * ( r->listnext->cap -  r->cap ) -
	( r->listnext->time - r->time ) * ( r->cap - r->listprev->cap );
      if ( z < eps ) {
	pred = r->listprev;
	r->listprev->listnext = r->listnext;
	r->listnext->listprev = r->listprev;
	r = pred;                                     
      } else {
	r = r->listnext;                                                      
      }
    } else         
      break;                
  }
  return dummy.listnext;
}
double P(RLnode *solution, double res)
{
  return solution->time - res * solution->cap;
}


