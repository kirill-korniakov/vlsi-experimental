#include "Spreading.h"

void LRS_AddObjectiveAndGradient(AppCtx* context, PetscScalar* solution)
{
  SpreadingPenalty(context, solution);
  context->criteriaValues.spreading += Aux::ScalarProduct(context->spreadingData.muBinsPen, 
    context->spreadingData.binsPenaltyValues, 
    context->spreadingData.binGrid.nBins);
  AddSpreadingPenaltyGradient(context, solution, context->gQS); //FIXME:HACK: use proper gradient
}

void UpdateLRSpreadingMu(HDesign& hd, AppCtx& context, int iterate)
{
  context.spreadingData.spreadingWeight *= hd.cfg.ValueOf("TAOOptions.muSpreadingMultiplier", 2.0);

  if (context.useLRSpreading)
  {
    BinGrid& binGrid = context.spreadingData.binGrid;

    for (int i = 0; i < binGrid.nBinRows; ++i)
    {
      for (int j = 0; j < binGrid.nBinCols; ++j)
      {
        context.spreadingData.muBinsPen[i * binGrid.nBinCols + j] += 
          context.spreadingData.spreadingWeightInitial 
          * pow(context.spreadingData.spreadingWeightInitial, 0.2) 
          / pow(2.0, iterate)
          * (binGrid.bins[i][j].sumPotential - context.spreadingData.desiredCellsAreaAtEveryBin)
          * (binGrid.bins[i][j].sumPotential - context.spreadingData.desiredCellsAreaAtEveryBin);
      }
    }
  }
}