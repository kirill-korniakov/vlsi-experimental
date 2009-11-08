#include "MuCalculations.h"

void InitializeMuSourceVector(HDesign& design, std::vector<double>& muSource, int size)
{ //FIXME: use proper value
  double muInitialValue = design.cfg.ValueOf(".LagrangianRelaxation.muLR", 0.99);

  for (int i = 0; i < size; i++)
  {
    muSource.push_back(muInitialValue);
  }
}

void InitializeMuNetVector(HDesign& design, std::vector<double>& muNet, int size)
{ //FIXME: use proper value
  double muInitialValue = design.cfg.ValueOf(".LagrangianRelaxation.muLR", 0.99);

  for (int i = 0; i < size; i++)
  {
    muNet.push_back(muInitialValue);
  }
}