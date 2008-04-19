/* Released Software: Fast Buffer Insertion for Interconnect Optimization */
/*                      Version 2.3: 2003 - 2007 */

/* Author:  Zhuo Li                       -- zhybear@gmail.com  */
/* Author:  Weiping Shi                   -- wshi@ee.tamu.edu */

/* Permission to use for non-commercial purposes is granted provided */
/* that proper acknowledgments are given. */
/* For information about commercial use, please contact authors */

#include "def.h"

/* insert l into tree */
RTnode* insert_swap(RTnode **root, double q, double c)
{
  RTnode *x, *y, *z;
  int i;

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
  x->qc_option= NIL;
  x->com=(Comp *)NULL;
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

  return x;
  /* does not check redundency */	
}

/* delete node z from swap_list */
void node_delete_swap(RTnode **root, RTnode  *z)
{
  RTnode *x, *y;
  int i;

  i = (*root)->size;

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
    z->com= y->com;
    z->qc_option=y->qc_option;
    y->qc_option->swap_option=z;
		
  }
  if (y->color == BLACK)
    fixup(root, x);
  (*root)->size = i-1;
  free(y);	
  reduce_solution+=sizeof(RTnode);
	
}


void tree_delete_swap(RTnode *x)
{
  if (x == NIL)
    return;
  tree_delete(x->left);
  tree_delete(x->right);	
  free(x);
  reduce_solution+=sizeof(RTnode);
}

