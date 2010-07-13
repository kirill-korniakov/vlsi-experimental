#include "Spreading.h"

void LRS_AddObjectiveAndGradient(AppCtx* context, PetscScalar* solution)
{
  SpreadingPenalty(context, solution);
  context->criteriaValues.spr += Aux::ScalarProduct(context->sprData.muBinsPen, 
    context->sprData.binsPenaltyValues, 
    context->sprData.binGrid.nBins);
  AddSpreadingPenaltyGradient(context, solution, context->criteriaValues.gQS); //FIXME:HACK: use proper gradient
}

void UpdateLRSpreadingMu(HDesign& hd, AppCtx& context, int iterate)
{
  context.weights.sprW *= hd.cfg.ValueOf("TAOOptions.muSpreadingMultiplier", 2.0);

  if (context.useLRSpreading)
  {
    BinGrid& binGrid = context.sprData.binGrid;

    for (int i = 0; i < binGrid.nBinRows; ++i)
    {
      for (int j = 0; j < binGrid.nBinCols; ++j)
      {
        context.sprData.muBinsPen[i * binGrid.nBinCols + j] += 
          context.sprData.sprWInitial 
          * pow(context.sprData.sprWInitial, 0.2) 
          / pow(2.0, iterate)
          * (binGrid.bins[i][j].sumPotential - context.sprData.desiredCellsAreaAtEveryBin)
          * (binGrid.bins[i][j].sumPotential - context.sprData.desiredCellsAreaAtEveryBin);
      }
    }
  }
}