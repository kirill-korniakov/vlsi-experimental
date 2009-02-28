/* 
 * random_numbers_generation.h
 * this is a part of itlAnalyticalPlacer
 * Copyright (C) 2005, 2006 ITLab, Kornyakov
 * email: kirillkornyakov@yandex.ru
 */

#ifndef _RANDOM_NUMBERS_GENERATION_H_
#define _RANDOM_NUMBERS_GENERATION_H_

#include "mkl_vsl.h"
#include <stdlib.h>

//#define RAND
//#define SHAKED_RAND
#define VSL

void InitializeStream(unsigned int initVal = 0);

void DeinitializeStream();

void GetIntegerRandomNumbers( int* arr, int arrSize, int lowerBound, int highBound );

void GetFloatRandomNumbers( float* arr, int arrSize, float lowerBound, float highBound );

#endif