/*
 *     This  file  is  part  of  the  Cadence  LEF/DEF  Open   Source
 *  Distribution,  Product Version 5.7, and is subject to the Cadence
 *  LEF/DEF Open Source License Agreement.   Your  continued  use  of
 *  this file constitutes your acceptance of the terms of the LEF/DEF
 *  Open Source License and an agreement to abide by its  terms.   If
 *  you  don't  agree  with  this, you must remove this and any other
 *  files which are part of the distribution and  destroy any  copies
 *  made.
 *
 *     For updates, support, or to become part of the LEF/DEF Community,
 *  check www.openeda.org for details.
 */


#ifndef FEF_mallocs
#define FEF_mallocs

/*****************************************************************
 * FEF_malloc.h:  memory management basic routines
 ******************************************************************
 */
#define FEF_malloc LEF_FEF_malloc
#define FEF_calloc LEF_FEF_calloc
#define FEF_realloc LEF_FEF_realloc
#define FEF_free_then_malloc LEF_FEF_free_then_malloc
#define FEF_relax LEF_FEF_relax
#define FEF_free LEF_FEF_free
#define NoMemory LEF_NoMemory
#define memory_malloc LEF_memory_malloc
#define memory_free LEF_memory_free

void *FEF_malloc(int num_bytes,const char *comment);
void *FEF_calloc(int num_elem,int elem_size,const char *comment);
void *FEF_realloc(void *ptr,int num_bytes,const char *comment);
void *FEF_free_then_malloc(void *ptr,int num_bytes,const char *comment);
void *FEF_relax(void *ptr,int num_bytes,const char *comment);
void  FEF_free(void *ptr,const char *comment);
void NoMemory();
char *memory_malloc(unsigned n);
void memory_free();

#endif
