/*  
 * overlap_removing.h
 * this is a part of itlDragon
 * Copyright (C) 2005, ITLab, Kornyakov
 * email: kirillkornyakov@yandex.ru
 */

#ifndef _OVERLAP_REMOVING_H_
#define _OVERLAP_REMOVING_H_

#include "..\include\data_structures.h"
#include "..\include\cost_function.h"
#include "..\include\optimal_region_determination.h"
#include "..\include\check_legality.h"
#include "..\include\output.h"

MULTIPLACER_ERROR OverlapRemoving(Circuit& circuit);

MULTIPLACER_ERROR LegalizeRow(Circuit& circuit, int rowIdx);

MULTIPLACER_ERROR EqualizeRowLengths(Circuit& circuit);

int Juggle(Circuit& circuit, double* desiredRowLength, int direction);

int EqualizeRow(Circuit& circuit, double desiredRowLength, int rowIdx, int direction);

#endif
