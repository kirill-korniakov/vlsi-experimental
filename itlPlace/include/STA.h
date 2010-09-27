#ifndef __STA_H__
#define __STA_H__

#include "HDesign.h"

const bool DO_NOT_REPORT = false;

void STA(HDesign& design, bool doReport = true, bool reroute = true);
void NoWireDelaySTA(HDesign& design, bool doReport = true);

double B5CTiming(HDesign& hd, double x1, double x2, double x3, double x4, double x5, bool printTable = true);
void SolveB5C(HDesign& hd);

#endif //__STA_H__
