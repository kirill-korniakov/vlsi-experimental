/* Released Software: Fast Buffer Insertion for Interconnect Optimization */
/*                      Version 2.3: 2003 - 2007 */
/* Feb 21, 2007 */
/* Author:  Zhuo Li                       -- zhybear@gmail.com  */
/* Author:  Weiping Shi                   -- wshi@ee.tamu.edu */

/* Permission to use for non-commercial purposes is granted provided */
/* that proper acknowledgments are given. */
/* For information about commercial use, please contact authors */

#include "def.h"

/* delete whole tree without rebalancing */
void tree_delete(RTnode *x)
{
  if (x == NIL)
    return;
  if (x->com != (Comp *) NULL)
    x->com->counter--;
  comp_delete(x->com);
  if (x->coma!= (Comp *) NULL)
    x->coma->counter--;
  comp_delete(x->coma);
  tree_delete(x->left);
  tree_delete(x->right);		
  free(x);
  reduce_solution+=sizeof(RTnode);
}

/* update the fields of arrival time and capacitance */
void update(RTnode *x)
{
  Comp *p;
  x->left->da += x->da+x->ra*x->left->ca;
  x->left->ra += x->ra;
  x->left->ca += x->ca;

  if (x->coma != (Comp *) NULL) {
    if (x->left != NIL)	{
      if (x->left->coma == (Comp *) NULL) {
	x->left->coma=x->coma;
	x->left->coma->counter++;
      } else {
	p=(Comp *)malloc(sizeof(Comp));
	m_used+=sizeof(Comp);
	p->right=x->left->coma;
	p->left=x->coma;
	if (p->left!=(Comp *)NULL)
	  p->left->counter++;
	p->counter=1;
	p->buffertype=-2;
	x->left->coma=p;
      }
    }
  }
  x->left->dirty = 1;

  x->right->da += x->da+x->ra*x->right->ca;
  x->right->ra += x->ra;
  x->right->ca+=x->ca;
	
  if (x->coma != (Comp *) NULL) {
    if (x->right != NIL) {
      if (x->right->coma == (Comp *) NULL) {
	x->right->coma=x->coma;
	x->right->coma->counter++;
      } else {
	p=(Comp *)malloc(sizeof(Comp));
	m_used+=sizeof(Comp);
	p->right=x->right->coma;
	p->left=x->coma;
	if (p->left!=(Comp *)NULL)
	  p->left->counter++;
	p->counter=1;
	p->buffertype=-2;
	x->right->coma=p;
      }
    }
  }
  x->right->dirty = 1;

  x->time = x->time-x->da-x->ra*x->cap;
  x->cap += x->ca;
  x->da = x->ca = x->ra = 0;
	
  if (x->coma != (Comp *) NULL) {
    if (x->com == (Comp *) NULL) 
      x->com=x->coma;				
    else {
      p=(Comp *)malloc(sizeof(Comp));
      m_used+=sizeof(Comp);
      p->right=x->com;
      p->left=x->coma;
      p->counter=1;
      p->buffertype=-2;
      x->com=p;
    }
  }
  x->coma=(Comp *)NULL;
	
  x->dirty = 0;

  if (total_solution-reduce_solution+m_used-m_reduced>max_space)
    max_space=total_solution-reduce_solution+m_used-m_reduced;

}

/* find minimum height node in tree */
RTnode *minimum(RTnode *x)
{
  while (x->left != NIL) {
    x = x->left;
    if (x->dirty)
      update(x);
  }
  return x;
}

/* find maximum height node in tree */
RTnode *maximum(RTnode *x)
{
  while (x->right != NIL) {
    x = x->right;
    if (x->dirty)
      update(x);
  }
  return x;
}

/* find successor of node x */
RTnode *node_succ(RTnode *x)
{
  RTnode *y;

  if (x->right != NIL) {
    if (x->right->dirty)
      update(x->right);
    return minimum(x->right);
  }

  for (y = x->parent; y != NIL && x == y->right;) {
    x = y;
    y = y->parent;
  }
  return y;
}

/* find predcessor of node x */
RTnode *node_pred(RTnode *x)
{
  RTnode *y;

  if (x->left != NIL) {
    if (x->left->dirty)
      update(x->left);
    return maximum(x->left);
  }

  for (y = x->parent; y != NIL && x == y->left;) {
    x = y;
    y = y->parent;
  }
  return y;
}
/*
 * Find the realization whose arrival time equals q,
 * or the maximum over all that are less than q.
 */
RTnode *pred(RTnode *root, double q)
{
  RTnode *x, *y;

  for (x = root;;) {
    if (x->dirty)
      update(x);
    if (fabs(q - x->time)<eps)
      return x;	/* found */
    if (q > x->time) {
      if (x->right != NIL)
	x = x->right;
      else
	return x;
    } else {
      if (x->left != NIL)
	x = x->left;
      else
	break;
    }
  }

  for (y = x->parent; y != NIL && x == y->left;) {
    x = y;
    y = y->parent;
  }
  return y;
}

/*
 * Find the realization whose arrival time equals q. 
 */
RTnode *search(RTnode *root, double q)
{
  RTnode *x;

  for (x = root;;) {
    if (x->dirty)
      update(x);
    if (fabs(q - x->time)<eps)
      return x;	/* found */
    if (q > x->time) {
      if (x->right != NIL)
	x = x->right;
      else
	return NIL;
    } else {
      if (x->left != NIL)
	x = x->left;
      else
	return NIL;
    }
  }
}


/*
 * Find the realization whose arrival time equals q,
 * or the minimum over all that are greater than q.
 */
RTnode *succ(RTnode *root, double q)
{
  RTnode *x, *y;

  for (x = root;;) {
    if (x->dirty)
      update(x);
    if ( fabs(q - x->time)<eps)
      return x;	/* found */
    if (q < x->time) {
      if (x->left != NIL)
	x = x->left;
      else
	return x;
    } else {
      if (x->right != NIL)
	x = x->right;
      else
	break;
    }
  }

  for (y = x->parent; y != NIL && x == y->right;) {
    x = y;
    y = y->parent;
  }
  return y;
}

/* red-black tree left rotation */
void left_rotate(RTnode **root, RTnode *x)
{
  RTnode *y;
	
  if (x->dirty)
    update(x);
	

  y = x->right;
  if (y->dirty)
    update(y);
  x->right = y->left;

  if (y->left != NIL)
    y->left->parent = x;

  y->parent = x->parent;
  if (x->parent == NIL)
    *root = y;
  else {
    if (x == x->parent->left)
      x->parent->left = y;
    else
      x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}

/* red-black tree right rotation */
void right_rotate(RTnode **root, RTnode *y)
{
  RTnode *x;
	
  if (y->dirty)
    update(y);
    
  x = y->left;
  if (x->dirty)
    update(x);
  y->left= x->right;

  if (x->right != NIL)
    x->right->parent = y;

  x->parent = y->parent;
  if (y->parent == NIL)
    *root = x;
  else if (y == y->parent->right)
    y->parent->right = x;
  else
    y->parent->left = x;

  x->right = y;
  y->parent = x;
}

/* balance after insertion */
void balance(RTnode **root, RTnode *x)
{
  RTnode *y;

  while(x != *root && x->parent->color == RED) {
    if (x->parent == x->parent->parent->left) {
      y = x->parent->parent->right;
      if (y->color == RED) {
	x->parent->color = BLACK;
	y->color = BLACK;
	x->parent->parent->color = RED;
	x = x->parent->parent;
      } else {
	if (x == x->parent->right) {
	  x = x->parent;
	  left_rotate(root, x);
	}
	x->parent->color = BLACK;
	x->parent->parent->color = RED;
	right_rotate(root, x->parent->parent);
      }
    } else {	/* same as above, with left right exchanged */
      y = x->parent->parent->left;
      if (y->color == RED) {
	x->parent->color = BLACK;
	y->color = BLACK;
	x->parent->parent->color = RED;
	x = x->parent->parent;
      } else {
	if (x == x->parent->left) {
	  x = x->parent;
	  right_rotate(root, x);
	}
	x->parent->color = BLACK;
	x->parent->parent->color = RED;
	left_rotate(root, x->parent->parent);
      }
    }
  }
  (*root)->color = BLACK;
}
 
/* insert (q,c) into tree */   
RTnode * insert(RTnode **root,  double q,  double c, Comp *com, RTnode **list)
{
  RTnode *x, *y, *z, *a;
  int i;
  double pre_c;

  x = (RTnode *) malloc(sizeof(RTnode));

  total_solution+=sizeof(RTnode);
  if (total_solution-reduce_solution+m_used-m_reduced>max_space)
    max_space=total_solution-reduce_solution+m_used-m_reduced;
  x->time = q;
  x->cap =  c;
  x->da = 0;
  x->ca = 0;
  x->ra=0;
  x->dirty = 0;
  x->left = NIL;
  x->right = NIL;
  x->swap_option = NIL;
  x->qc_option=NIL;
  x->com=com;
  if (x->com != (Comp *) NULL)
    x->com->counter++;
  x->coma=(Comp *)NULL;


  if (*root == NIL) {
    x->parent = NIL;
    x->color = BLACK;
    x->size = 1;
    *root = x;
    return x;
  }

  for (y = *root; y != NIL;) {
    if (y->dirty)
      update(y);
    z = y;
    if (q < y->time)
      y = y->left;
    else
      y = y->right;
  }

  x->color = RED;
  x->parent = z;
  if (q < z->time)
    z->left = x;
  else
    z->right = x;

  i = (*root)->size;
  balance(root, x);
  (*root)->size  = i+1;

  /* check redundency */

  while ( (y = node_pred(x)) != NIL ) {
    pre_c = x->time;
    if ( x->cap <= y->cap ) {
      if ( (a=y->swap_option)!=NIL ) {
	y->swap_option = NIL;
	node_delete_swap( list, a );
      }				
      node_delete( root, y );
    } else
      break;
    x=search( *root, pre_c );
  }
  if ( (y = node_succ(x)) != NIL ) 
    if ( x->cap >= y->cap ) {
      if ( (a=x->swap_option)!=NIL ) {
	x->swap_option = NIL; /////changed
	  node_delete_swap( list, a );
      }
      node_delete(root, x);
      x = NIL;
    }
  return x;		
}

/* delete node z from tree */
void node_delete(RTnode **root, RTnode *z)
{
  RTnode *x, *y;
  int i;

  i = (*root)->size;
    
  if (z->dirty)
    update(z);

  if (z->left == NIL || z->right == NIL)
    y = z;
  else
    y = node_succ(z);

  if (y->left != NIL)
    x = y->left;
  else
    x = y->right;

  if (x!= NIL && x->dirty)
    update(x);

  x->parent = y->parent;

  if (y->parent == NIL)
    *root = x;
  else {
    if (y == y->parent->left)
      y->parent->left = x;
    else
      y->parent->right = x;
  }
  if (y != z) {	/* swap y and z */
    z->time = y->time;
    z->cap = y->cap;
    if (z->com != (Comp *) NULL)
      z->com->counter--;
    comp_delete(z->com);
    z->com = y->com;	
    z->swap_option=y->swap_option;
    y->swap_option->qc_option=z;
  } else {
    if (y->com != (Comp *) NULL)
      y->com->counter--;
    comp_delete(y->com);	
  }
  if (y->color == BLACK)
    fixup(root, x);
  (*root)->size = i-1;
  free(y);
  reduce_solution += sizeof(RTnode);	
}

/* balance tree after deletion */
void fixup(RTnode **root, RTnode *x)
{
  RTnode *w;

  while (x != *root && x->color == BLACK) {
    if (x == x->parent->left) {
      w = x->parent->right;
      if (w->color == RED) {
	w->color = BLACK;
	x->parent->color = RED;
	left_rotate(root, x->parent);
	w = x->parent->right;
      }
      if (w->left->color == BLACK &&
	  w->right->color == BLACK) {
	w->color = RED;
	x = x->parent;
      } else {
	if (w->right->color == BLACK) {
	  w->left->color =BLACK;
	  w->color = RED;
	  if (w->dirty)
	    update(w);
	  right_rotate(root, w);
	  w = x->parent->right;
	}
	w->color = x->parent->color;
	x->parent->color = BLACK;
	w->right->color = BLACK;
	left_rotate(root, x->parent);
	x = *root;
      }
    }
    else {
      w = x->parent->left;
      if (w->color == RED) {
	w->color = BLACK;
	x->parent->color = RED;
	right_rotate(root, x->parent);
	w = x->parent->left;
      }
      if (w->right->color == BLACK &&
	  w->left->color == BLACK) {
	w->color = RED;
	x = x->parent;
      } else {
	if (w->left->color == BLACK) {
	  w->right->color = BLACK;
	  w->color = RED;
	  if (w->dirty)
	    update(w);
	  left_rotate(root, w);
	  w = x->parent->left;
	}
	w->color = x->parent->color;
	x->parent->color = BLACK;
	w->left->color = BLACK;
	right_rotate(root, x->parent);
	x = *root;
      }
    }
  }
  x->color = BLACK;
}

/* print candidate tree */
void travel(RTnode *x)
{
  int i = 0;
  if ( x == NIL ) {
    printf("empty tree\n");
    return;
  }
  if ( x->dirty == 1 ) {
    update(x);
  }
  if ( x->left != NIL )
    travel( x->left );
  printf("\tq=%10.10g\tc=%10.10g\n", x->time, x->cap*1000);
  if ( debug )
    printbuffer_shili( x->com, &i );
  if ( x->right != NIL )
    travel(x->right);
}
/* print buffer location */
void printbuffer_shili(Comp *x, int *i)
{
  if ( x == (Comp *)NULL ) 	{
    fprintf(fp, "empty tree\n");
    return;
  }
  if ( x->buffertype!=-2 && x->buffertype!=-1 ) {
    fprintf(fp, "start %d  end %d  buffertype %d\n", x->x, x->y, x->buffertype);
    *i=*i+1;
  }
  if ( x->left != (Comp *)NULL )
    printbuffer_shili( x->left, i );
  if ( x->right != (Comp *)NULL )
    printbuffer_shili( x->right, i );	
}

void travel_update( RTnode *root, RTnode *x )
{
  RTnode *k;
  int *path, i, j, dirty=0;
  path = (int *)malloc(root->size*sizeof(int));
  total_solution += root->size*sizeof(int);
  i = 0;
  k = x->parent;
  while ( k!=NIL ) {
    if ( x == k->left )
      path[i] = 0;
    else
      path[i] = 1;
    i ++;
    if ( x->dirty )
      dirty++; 
    k = k->parent;
    x = x->parent;
  }
  if (dirty==0) {
    free( path );
    reduce_solution += root->size*sizeof(int);
    return;
  }
  k = root;
  for ( j=i-1;j>-1;j-- ) {
    if ( k->dirty )
      update( k );
    if ( path[j]==1 )
      k = k->right;
    else
      k = k->left;		
  }
  if ( k!=NIL && k->dirty )
    update( k );
  free( path );
  reduce_solution += root->size*sizeof(int);
}

/*****************************************************************
02/21/2007: Return the address of inserted node to insert function.
Also, if the qc tree is ordered by c, the time of search q again after deleteing the prev(x) could be saved
*****************************************************************/
