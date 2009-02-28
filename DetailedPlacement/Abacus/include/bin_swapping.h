/*  
 * bin_swapping.h
 * this is a part of itlDragon
 * Copyright (C) 2005, ITLab,Aleksey Bader,Svetlana Gagarinova
 * email: bader@newmail.ru
 * email: svetlana.gagarinova@cs.vmk.unn.ru
 * last modified 25 February 2005
 */

#ifndef _BIN_SWAPPING_H_
#define _BIN_SWAPPING_H_

#include "..\include\data_structures.h"
#include "..\include\cost_function.h"
#include "..\include\random_numbers_generation.h"
#include "..\include\global.h"

// bin-swapping 
double BinSwapping(Circuit& circuit, Statistics& statistics, double* params);
#endif