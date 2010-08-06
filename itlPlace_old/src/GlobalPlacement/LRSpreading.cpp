#include "Spreading.h"

void LRS_AddObjectiveAndGradient(AppCtx* context, PetscScalar* solution)
{
  SpreadingPenalty(context, solution);
  context->criteriaValues.spr += Aux::ScalarProduct(context->sprData.muBinsPen, 
    context->sprData.binsPenaltyValues, 
    context->sprData.binGrid.nBins);
  AddSpreadingPenaltyGradient(context, solution, context->criteriaValues.gQS); //FIXME:HACK: use proper gradient
}