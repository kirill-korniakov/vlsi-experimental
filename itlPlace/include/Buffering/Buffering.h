#ifndef __Buffering_h__
#define __Buffering_h__

#include "HDesign.h"

void NetlistBuffering(HDesign&);
void TestBuffering(HDesign&);
void BufferingAndReport(HDesign& design);

void InsertRepeaters(HDesign& design);

#endif