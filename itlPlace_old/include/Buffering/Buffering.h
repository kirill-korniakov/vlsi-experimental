#ifndef __Buffering_h__
#define __Buffering_h__

#include "HDesign.h"

void BufferingAndReport(HDesign& design);
void InsertRepeaters(HDesign& design);
void InsertRepeaters2(HDesign& design, double slackTreshold);
void InsertRepeaters2(HDesign& design, int iterations, double bufferedPercent);

#endif