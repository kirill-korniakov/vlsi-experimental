/*  
 * directed_bin_swapping.h
 * this is a part of itlDragon
 * Copyright (C) 2006, ITLab, Kirill Kornyakov
 * email: kirillkornyakov@yandex.ru
 */

#ifndef _DIRECTED_BIN_SWAPPING_H_
#define _DIRECTED_BIN_SWAPPING_H_

#include "..\include\data_structures.h"
#include "..\include\cost_function.h"
#include "..\include\optimal_region_determination.h"
#include "..\include\errors.h"
#include "..\include\check_legality.h"

int DirectedBinSwapping(Circuit& circuit, Statistics& statistics, double* paramsOfDBS);

#endif