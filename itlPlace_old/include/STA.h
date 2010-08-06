#ifndef __STA_H__
#define __STA_H__

#include "HDesign.h"

const bool DO_NOT_REPORT = false;

void STA(HDesign& design, bool doReport = true, bool reroute = true);
void NoWireDelaySTA(HDesign& design, bool doReport = true);

#endif //__STA_H__
