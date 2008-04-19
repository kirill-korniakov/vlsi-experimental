/* 
 * random_numbers_generation.cpp
 * this is a part of itlAnalyticalPlacer
 * Copyright (C) 2005, 2006 ITLab, Kornyakov
 * email: kirillkornyakov@yandex.ru
 */

#include "..\include\random_numbers_generation.h"
#include <time.h>

#define BRNG  VSL_BRNG_MCG59
#define METHOD  0
#define SHAKE_FREQ (RAND_MAX/2)

VSLStreamStatePtr stream;

void InitializeStream(unsigned int initVal)
{
  if (initVal == 0)
    vslNewStream(&stream, BRNG, (unsigned)time(NULL));
  else
    vslNewStream(&stream, BRNG, initVal);
}

void DeinitializeStream()
{
  vslDeleteStream( &stream );
}

void GetIntegerRandomNumbers(int* arr, int arrSize, int lowerBound, int highBound)
{
#ifdef VSL
  viRngUniform(METHOD, stream, arrSize, arr, lowerBound, highBound);
#endif

#ifdef RAND
  for (int i = 0; i < arrSize; ++i)
    arr[i] = lowerBound + rand()%highBound;
#endif

#ifdef SHAKED_RAND
  for (int i = 0; i < arrSize; ++i)
  {
    if (i%SHAKE_FREQ == 0) srand(static_cast<unsigned>(time(NULL)));
    arr[i] = lowerBound + rand()%highBound;
  }
#endif
}

void GetFloatRandomNumbers(float* arr, int arrSize, float lowerBound, float highBound)
{
#ifdef VSL
  vsRngUniform(METHOD, stream, arrSize, arr, lowerBound, highBound);
#endif

#ifdef RAND
  for (int i = 0; i < arrSize; ++i)
    arr[i] = lowerBound + static_cast<float>(rand())/static_cast<float>(RAND_MAX) * (highBound - lowerBound);
#endif

#ifdef SHAKED_RAND
  for (int i = 0; i < arrSize; ++i)
  {
    if (i%SHAKE_FREQ == 0) srand(static_cast<unsigned>(time(NULL)));
    arr[i] = lowerBound + static_cast<float>(rand())/static_cast<float>(RAND_MAX) * (highBound - lowerBound);
  }
#endif
}