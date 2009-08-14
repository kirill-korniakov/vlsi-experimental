/* Released Software: Fast Buffer Insertion for Interconnect Optimization */
/*                      Version 2.3: 2003 - 2007 */

/* Author:  Zhuo Li                       -- zhybear@gmail.com  */
/* Author:  Weiping Shi                   -- wshi@ee.tamu.edu */

/* Permission to use for non-commercial purposes is granted provided */
/* that proper acknowledgments are given. */
/* For information about commercial use, please contact authors */

#define TREATZERO 1
#include "def_mn.h"
/* 05/14/05 New O(mn) time algorithm for two-pin net */
/* main subroutine */

/* pointer used for O(bn) algorithm to access alpha_1, ..., alpha_B, 
   beta_1, ..., beta_B */
#define PREDICT 1 /* predictive pruning */
RLnode **van(Node *t, Bp **bestpointer)
{
  RLnode dummy, **sleftwhole, **srightwhole, *sleft, *sright, *s, *x, *slefthead, *srighthead, **sfinal;
  double k1, k2, c, q;
  Comp *p;
  Bp *leftbest, *rightbest;
  int i;

  if ( t->type==SINK )
    return create_list(t, bestpointer);

  sleftwhole = van(t->left, &leftbest); 	
  k1 = distance(t,t->left);
  if ( t->right != (Node *)NULL ) {
    srightwhole = van(t->right, &rightbest);
    k2 = distance(t,t->right);		
  }

  if ( t->type == SOURCE ) {
    van_answer=0-INFINITY;
    x=sleftwhole[1];
    while ( x && x->next ) {
      if ( x->time - (r0*k1 + rd + leftbest->ra) * x->cap >
           x->next->time - (r0*k1 + rd + leftbest->ra) * x->next->cap )
        break;
      else
        x = x->next;
    }
    van_answer = x->time - leftbest->da - leftbest->ra * x->cap - r0*k1*0.5*c0*k1
      - (r0*k1 + rd) * (x->cap + leftbest->ca) - rd*k1*c0;
    final_location_van = x->com;

    /*      while( x ) { */
    /*        update_nblist(x); */
    /*        q = x->time-0.5*r0*c0*k1*k1-(r0*k1+rd)*x->cap-rd*k1*c0; */
    /*        if ( q > van_answer ) { */
    /*  	van_answer = q; */
    /*  	final_location_van = x->com; */
    /*        } */
    /*        x = x->next; */
    /*      } */
    /*      ra = da = ca = 0; */

    *bestpointer = leftbest;
    return sleftwhole;
  } else {
    sleftwhole = add_buffer(k1, sleftwhole, t->index, t->left->index, leftbest );
    /* if the node has only one child (default left), only 
       return update solution of left child */
    if ( t->right == (Node *)NULL ) {			
      if ( debug ) {
	printf("Debug Information: qc list at %d \n", t->index);
	print( sleftwhole[0], leftbest );
        leftbest->ra = leftbest->da = leftbest->ca = 0;
	printf("\n");
      }
      *bestpointer = leftbest;
      return sleftwhole;
    } else
      srightwhole = add_buffer(k2, srightwhole, t->index, t->right->index, rightbest);
  }

  sleftwhole[0] = redundent(sleftwhole[0], leftbest);
  srightwhole[0] = redundent(srightwhole[0], rightbest);
  leftbest->ra = leftbest->da = leftbest->ca = 0;
  rightbest->ra = rightbest->da = rightbest->ca = 0;

  if ( debug ) {
    printf("Debug Information: qc list at left %d \n", t->index);
    print( sleftwhole[0], leftbest );
    printf("Debug Information: qc list at right %d \n", t->index);
    print( srightwhole[0], rightbest );
    printf("\n");
  }
  sfinal = (RLnode **)malloc(2*sizeof(RLnode *));
  //  *bestpointer = (Bp *)malloc(sizeof(Bp));
  total_solution += 2*sizeof(RLnode *) + sizeof(Bp);
  /* merge two list */
  s=&dummy;
  dummy.cap=0;
  dummy.time=0;
  dummy.com=(Comp *)NULL;
  dummy.next=(RLnode *)NULL;
  sleft = slefthead = sleftwhole[0];
  sright = srighthead = srightwhole[0];
 
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
      p->counter=1;
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
      p->counter=1;
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
  sfinal[0] = dummy.next;
  list_delete( sleftwhole[0] ); list_delete( sleftwhole[1] );
  list_delete( srightwhole[0] ); list_delete( srightwhole[1] );
  free(sleftwhole); free(srightwhole);

  *bestpointer = leftbest;
  free(rightbest);
  
  reduce_solution += 4 * sizeof(RLnode *);
  reduce_solution += 2 * sizeof(Bp);
  sfinal[1] = cover_bn(sfinal[0], bestpointer);
  sfinal[1]->prev = (RLnode *)NULL;
  return sfinal;
}

/* create list */
RLnode **create_list(Node *t, Bp **pointer)
{
  RLnode **last;
  int i;
  last = (RLnode **)malloc(2*sizeof(RLnode *));
  last[0] = (RLnode *) malloc(sizeof(RLnode));
  last[1] = (RLnode *) malloc(sizeof(RLnode));
  total_solution += 2*sizeof(RLnode *) + 2 * sizeof(RLnode);
  if (total_solution - reduce_solution + m_used - m_reduced > max_space )
    max_space = total_solution - reduce_solution + m_used - m_reduced;
  for ( i = 0; i < 2; i ++ ) { 
    last[i]->cap = t->cap;
    last[i]->time = t->time;
    last[i]->next = (RLnode *) NULL;
    last[i]->prev = (RLnode *) NULL;
    last[i]->com = (Comp *) NULL;
  }
  *pointer = (Bp *)malloc(sizeof(Bp));
  total_solution += sizeof(Bp);
  (*pointer)->ra = (*pointer)->da = (*pointer)->ca = 0;
  for (i=0;i<n_buffertype;i++) {
    (*pointer)->alpha[i] = last[1];
    (*pointer)->beta[i] = last[1];
    (*pointer)->betaD[i] = last[0];
  }
  return last;
}
/* update Q and C field */
void update_nblist(RLnode *solution, Bp *pointer)
{
  solution->time = solution->time - pointer->da - pointer->ra * solution->cap;
  solution->cap += pointer->ca;
}

/* add wire then buffer from previor subnode. */
RLnode **add_buffer(double k, RLnode **list, int px, int py, Bp *pointer)
{
  RLnode *y, *z[2][MAXBUFFERTYPE], dummy, *x;
  int i, btype, j, type;
  Comp *p;
  int listindex;

  /* if the wire has length 0, then quit */
#if TREATZERO
  if ( k == 0 )    
    return list;    
#endif
  /* Use ra, da, ca to add wire */
  pointer->da += r0 * k * ( 0.5 * c0 * k +  pointer->ca );
  pointer->ra += r0 * k;
  pointer->ca += c0 * k;

 
  /*  consider the buffer solution */
  for (i = n_buffertype - 1; i > -1; i-- ) {
    /* the order decide alpha[i] must be at left side of alpha[i+1] */
    if ( i < n_buffertype - 1 && pointer->alpha[i]->cap > pointer->alpha[i+1]->cap )
      pointer->alpha[i] = pointer->alpha[i+1];

    while ( pointer->alpha[i]->prev ) {
      if ( pointer->alpha[i]->time - (pointer->ra + rb[i])*pointer->alpha[i]->cap < pointer->alpha[i]->prev->time - (pointer->ra + rb[i])*pointer->alpha[i]->prev->cap )
	pointer->alpha[i] = pointer->alpha[i]->prev;
      else
	break;
    }
    for ( listindex = 0; listindex < 2; listindex ++ ) {
      z[listindex][i] = (RLnode *)malloc(sizeof(RLnode));
      total_solution += sizeof(RLnode);
      /* computation of z[i]->cap and z[i]->time considers the interconnect
	 reduction */

      z[listindex][i]->cap = cb[i] - pointer->ca;;
      z[listindex][i]->time = pointer->alpha[i]->time - tb[i] - (pointer->ra + rb[i]) * ( pointer->ca + pointer->alpha[i]->cap )
	+ pointer->ra * cb[i];
    }

    p = (Comp *)malloc(sizeof(Comp));
    m_used += sizeof(Comp);
    p->left = pointer->alpha[i]->com;
    if ( p->left != (Comp *)NULL )
      p->left->counter++;             
    p->right = (Comp *)NULL;
    p->buffertype = i + 1;
    p->counter = 2;
    p->x = px;
    p->y = py;
    z[0][i]->com = p;
    z[1][i]->com = p;
  }    

  /* add buffer solution answer to the convex list */
  for ( listindex = 0; listindex < 2; listindex ++ ) {
    dummy.next = list[listindex];
    list[listindex]->prev = &dummy;
    dummy.cap = -INFINITY;
    dummy.prev = (RLnode *)NULL;
    for ( i = n_buffertype - 1; i > -1; i-- ) {
      if ( listindex == 0 ) {
	if ( i < n_buffertype - 1 && pointer->betaD[i]->cap > pointer->betaD[i+1]->cap )
	  pointer->betaD[i] = pointer->betaD[i+1];
      } else {
        if ( i < n_buffertype - 1 && pointer->beta[i]->cap > pointer->beta[i+1]->cap )
	  pointer->beta[i] = pointer->beta[i+1];
      }
      y = ( listindex == 0 ) ? pointer->betaD[i] : pointer->beta[i];
      while ( y ) {
	if ( fabs(z[listindex][i]->cap - y->cap ) < 1e-10) {
	  if ( z[listindex][i]->time > y->time ) {
	    z[listindex][i]->next = y->next;
	    if ( y->next )
	      y->next->prev = z[listindex][i];
	    z[listindex][i]->prev = y->prev;
	    if ( y->prev )
	      y->prev->next = z[listindex][i];
	    /* need to update the other pointers pointing to beta[i] */
	    for ( j = 0; j< n_buffertype; j++ ) {
              if ( listindex ==0 ) {
	        if ( pointer->betaD[j] == y )
                  pointer->betaD[j] = z[listindex][i];
	      } else {
	        if ( pointer->alpha[j] == y )
		  pointer->alpha[j] = z[listindex][i];
	        if ( pointer->beta[j] == y )
		  pointer->beta[j] = z[listindex][i];
	      }
	    }
	    /********************************************************/
	    if ( y->com )
	      y->com->counter--;
	    comp_delete( y->com );
	    free( y );
	    reduce_solution += sizeof(RLnode);
	    y = z[listindex][i];
	  } else {
	    z[listindex][i]->com->counter--;
	    comp_delete( z[listindex][i]->com );
	    free( z[listindex][i] );
	    reduce_solution += sizeof(RLnode);
	  }
	  break;
	}
	if ( z[listindex][i]->cap > y->cap ) {
	  z[listindex][i]->next = y->next;
	  if ( y->next )
	    y->next->prev = z[listindex][i];
	  z[listindex][i]->prev = y;
	  y->next = z[listindex][i];
	  y = z[listindex][i];
	  break;
	} else
          y = y->prev;
      }
      if ( y == (RLnode *) NULL )
	error("Error code 3: dummy list is not functioned!");
      if ( listindex == 0 ) {
        pointer->betaD[i] = y;
        x = y->next;
        while ( x ) {
#if PREDICT
          if ( P(y, pointer->ra + rb[n_buffertype-1]) 
               > P(x, pointer->ra + rb[n_buffertype-1]) ) {
#else
	    if ( y->time - ( pointer->ra  ) * y->cap
		 > x->time - ( pointer->ra ) * x->cap ) {
#endif
	      y->next = x->next;
	      if ( x->next )
		x->next->prev = y;
	      for ( type = 0; type < n_buffertype; type ++ ) {
		if ( pointer->betaD[type] == x )
		  pointer->betaD[type] = (x->next) ? x->next : y;
	      }
	      if ( x->com )
		x->com->counter--;
	      comp_delete(x->com);
	      free(x);
	      reduce_solution += sizeof(RLnode);
	      x = y->next;
	    } else
	      break;
	  } 
	} else {
	  pointer->beta[i] = y;
	  cover(dummy.next, i, pointer);
	  if ( dummy.next->prev != &dummy )
	    dummy.next->prev = &dummy;
	}
      }
      if (total_solution-reduce_solution+m_used-m_reduced>max_space)
	max_space=total_solution-reduce_solution+m_used-m_reduced;
      dummy.next->prev = (RLnode *)NULL;
      list[listindex] = dummy.next;
    }
    return list;
  }


  /* remove redundent realizations */
  /* list is sorted as increasing capacitance and arrival time */
  RLnode *redundent(RLnode *list, Bp *pointer)
    {

      RLnode dummy, *pred, *r;
      double x;

      dummy.cap = 0;
      dummy.time = -INFINITY;
      dummy.com = (Comp *)NULL;
      dummy.next = list;
      list->prev = &dummy;
      pred = &dummy;
      r = list;
      while(r != (RLnode *) NULL) {
	update_nblist(r, pointer);
#if PREDICT
	x = P(r, rb[n_buffertype-1]) - P(pred, rb[n_buffertype - 1]);
#else
	x = r->time - pred->time;
#endif
	if ( x < eps ) {
	  pred->next = r->next;
	  if ( r->next )
	    r->next->prev = pred;
	  if ( r->com )
	    r->com->counter--;
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

  void list_delete(RLnode *l)
  {
    RLnode *tmp; 
    while (l != (RLnode *) NULL) {
      tmp = l->next;
      if (l->com != (Comp *)NULL )
	l->com->counter--;
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
  void print(RLnode *x, Bp *pointer)
  {
    int i;
    if (x == (RLnode *)NULL) {
      printf("empty tree\n");
      return;
    }
    update_nblist(x, pointer);
    printf("\tq=%10.10g\tc=%10.10g\n", x->time, x->cap*1000);
    if (debug)
      printbuffer(x->com, &i);	
    if (x->next != (RLnode *)NULL) 
      print(x->next, pointer);	 
  }

  /* print location */
  void printbuffer(Comp *x, int *i)
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

  /* 2004 05/12, cover code to find the cover of (q,c) list 
     at pointer beta[i]. */
  void cover( RLnode *list, int i, Bp * pointer)
  {
    RLnode dummy, *r, *pred;    
    double z;
    int j;

    dummy.next = list;
    list->prev = &dummy;
    dummy.cap = list->cap;
    dummy.time = -INFINITY;
  
    if ( list->next == (RLnode *) NULL )
      return;
    if ( pointer->beta[i]->prev != &dummy )
      r = pointer->beta[i]->prev;
    else
      r = pointer->beta[i];
    while ( r && r->next ) {
      z = ( r->time - r->prev->time ) * ( r->next->cap - r->cap ) -
	( r->next->time - r->time ) * ( r->cap - r->prev->cap );
      if ( z < eps ) {
	pred = r->prev;
	r->prev->next = r->next;
	r->next->prev = r->prev;
	for ( j = 0; j < n_buffertype; j ++ ) {
	  /* beta pointers need to move right only when it is freeed 
	     since when new solution is inserted, it stops when it is 
	     bigger than beta pointer. Genearlly both beta and alpha 
	     pointers move left. */ 
	  if ( pointer->beta[j] == r )
	    pointer->beta[j] = r->next;
	  if ( pointer->alpha[j] == r )
	    pointer->alpha[j] = r->next;
	}
	if ( r->com ) 
	  r->com->counter--;
	comp_delete(r->com);
	free(r);
	reduce_solution += sizeof(RLnode);
	r = pred;
      } else {
	if ( r == pointer->beta[i]->next ) 
	  break;
	else
	  r = r->next;                                                 
      }                
    }
  }
  void error(char *msg)
  {
    printf("%s\n", msg);
    exit(0);
  }

  /* cover idea from bn */
  RLnode *cover_bn( RLnode *list, Bp **bestpointer )
    {
      RLnode dummy, *pred, *r, *s, *newlist;    
      double z;
      int i;

      s = &dummy;
      for ( r = list ; r != (RLnode *)NULL ; r = r->next ) {
	s->next = (RLnode *)malloc(sizeof(RLnode));
	total_solution+=sizeof(RLnode);
	if (total_solution-reduce_solution+m_used-m_reduced>max_space)
	  max_space=total_solution-reduce_solution+m_used-m_reduced;
	s->next->prev = s;
	s = s->next;
	s->cap = r->cap;
	s->time = r->time;
	s->com = r->com;
	if ( r->com )
	  r->com->counter++;
      }
      s->next = (RLnode *)NULL;
      newlist = dummy.next;

      dummy.cap = newlist->cap;
      dummy.time = -INFINITY;
    
      if ( newlist->next ) {
	r=newlist->next;
	while ( r && r->next ) {
	  z = ( r->time - r->prev->time ) * ( r->next->cap -  r->cap ) -
	    ( r->next->time - r->time ) * ( r->cap - r->prev->cap );
	  if ( z < eps ) {
	    pred = r->prev;
	    r->prev->next = r->next;
	    r->next->prev = r->prev;
	    if ( r->com )
	      r->com->counter--;
	    comp_delete(r->com);
	    free(r);
	    reduce_solution += sizeof(RLnode);
	    r = pred;                                     
	  } else {
	    r = r->next;                                                      
	  }
	}
      }
      (*bestpointer)->ra = (*bestpointer)->da = (*bestpointer)->ca = 0;
      r = list;
      while ( r->next )
	r = r->next;
      for ( i=0; i< n_buffertype; i++)
	(*bestpointer)->betaD[i] = r;
      r = newlist;
      while ( r->next )
	r = r->next;
      for ( i = 0; i < n_buffertype; i ++ ) {
	(*bestpointer)->beta[i] = r;
	(*bestpointer)->alpha[i] = r;
      }
      // The following seems to be a faster implementation, but results are same
      /*    r = list; i = 0; */
      /*    while (i < n_buffertype ) { */
      /*      if ( r->cap > cb[i] || r->next == (RLnode *) NULL ) { */
      /*        (*bestpointer)->betaD[i] = r; */
      /*        i++; */
      /*      } else */
      /*        r = r->next; */
      /*    } */
      /*    r = newlist; i = 0; */
      /*    while (i < n_buffertype) { */
      /*      if ( r->cap > cb[i] || r->next == (RLnode *)NULL ) { */
      /*        (*bestpointer)->beta[i] = r; */
      /*        i++; */
      /*      } else */
      /*        r = r->next; */
      /*    } */
      /*    r = newlist; i = 0; */
      /*    while (i < n_buffertype) { */
      /*      if ( r->next == (RLnode *)NULL || P(r, i) > P(r->next, i) ) { */
      /*        (*bestpointer)->alpha[i] = r; */
      /*        i++; */
      /*      } else */
      /*        r = r->next; */
      /*    } */
      if ( dummy.next )
	dummy.next->prev = (RLnode *) NULL;
      return newlist;
    }
  double P(RLnode *solution, double res)
  {
    return solution->time - res * solution->cap;
  }
