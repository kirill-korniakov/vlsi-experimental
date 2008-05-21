/*  
 * global_placement.h
 * this is a part of itlAnalyticalPlacer
 * Copyright (C) 2005-2008, ITLab, Aleksey Bader, Kirill Kornyakov, Zhivoderov Artem
 * email: bader@newmail.ru
 * email: kirillkornyakov@yandex.ru
 * email: zhivoderov.a@gmail.com
 */

#ifndef _GLOBAL_PLACEMENT_H_
#define _GLOBAL_PLACEMENT_H_

#include "..\include\data_structures.h"
#include "..\include\cost_function.h"
#include "..\include\errors.h"
#include "..\include\check_legality.h"
#include "..\include\optimal_region_determination.h"
#include "..\include\random_numbers_generation.h"
//#include "..\include\overlap_removing.h"
#include "..\include\global.h"
#include "..\include\multilevel_framework.h"

//#define RECURSIVE_BISECTION

MULTIPLACER_ERROR GlobalPlacement(Circuit& circuit);

int RecursiveBisection(Circuit& circuit, Statistics& statistics);

int SplitCircuit(bool doCut, pBin oldCirc, pBin firstCirc, pBin secondCirc);

int TerminalPropagation(Circuit& circuit, pBin **bins, bool isHorizontal);

// function from libhmetis for hypergraph partitioning
extern "C"{
  extern void HMETIS_PartRecursive(int nvtxs, int nhedges, int *vwgts, int *eptr,
    int *eind, int *hewgts, int nparts, 
    int ubfactor, int *options, int *part, 
    int *edgecut);
}

#endif