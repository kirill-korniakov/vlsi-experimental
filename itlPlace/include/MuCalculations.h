#ifndef __MU_CALCULATIONS__
#define __MU_CALCULATIONS__

#include "HDesign.h"

void InitializeMuSourceVector(HDesign& design, std::vector<double>& muSource, int size);
void InitializeMuNetVector(HDesign& design, std::vector<double>& muNet, int size);

#endif