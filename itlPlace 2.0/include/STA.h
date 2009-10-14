#ifndef __STA_H__
#define __STA_H__

#include "HDesign.h"
#include "AdaptiveRoute.h"
#include "DelayCalculation.h"
#include "Timing.h"
#include "Reporting.h"

const bool DO_NOT_REPORT = false;

void STA(HDesign& design, bool doReport = true);

#endif //__STA_H__
