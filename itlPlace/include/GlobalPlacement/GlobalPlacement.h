#ifndef __GLOBAL_PLACEMENT__
#define __GLOBAL_PLACEMENT__

#include "HDesign.h"

void PinPlacement(HDesign&);

void GlobalPlacement(HDesign&, string cfgContext);

void RandomPlacement(HDesign&);

#endif