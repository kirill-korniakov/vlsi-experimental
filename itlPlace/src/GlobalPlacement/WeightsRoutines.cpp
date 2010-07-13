#include "OptimizationContext.h"
#include "LogSumExp.h"
#include "SumOfDelays.h"
#include "LagrangianRelaxation.h"
#include "Spreading.h"

void InitWeights(double* x, AppCtx* context)
{
    context->weights.lseW = context->hd->cfg.ValueOf("GlobalPlacement.LagrangianRelaxation.alphaTWL", 1.0);
    context->weights.sodW = 1.0;
    context->weights.lrW  = 1.0;

    //The following coefficient greatly affects quality of placement
    //because it determines initial distribution of cells over bins. 
    //We have to choose small value to keep good HPWL.
    double sprWInitialMultiplier = context->hd->cfg.ValueOf("TAOOptions.spreadingWeightInitialMultiplier", 0.1);
    PrecalcExponents(context, x);

    NullCriteriaValues(context);

    double criteriaValue = 0.0;
    double penaltyValue  = 0.0;
    if (context->useLogSumExp)
    {
        LSE_AddObjectiveAndGradient(context, x);
        criteriaValue = context->criteriaValues.lse;
    }
    else if (context->useSumOfDelays)
    {
        SOD_AddObjectiveAndGradient(context, x);
        criteriaValue = context->criteriaValues.sod;
    }
    else if (context->useLR)
    {
        LR_AddObjectiveAndGradient(context, x);
        criteriaValue = context->criteriaValues.lse + context->criteriaValues.lr;
    }

    penaltyValue = SpreadingPenalty(context, x);

    context->sprData.sprWInitial = sprWInitialMultiplier * criteriaValue / penaltyValue;
    context->weights.sprW = context->sprData.sprWInitial;

    if (context->useLRSpreading)
    {
        for (int i = 0; i < context->sprData.binGrid.nBins; ++i)
        {
            context->sprData.muBinsPen[i] = context->sprData.sprWInitial;
        }
    }
}

void UpdateWeights( HDesign& hd, AppCtx& context, int iterate )
{
    context.weights.sprW *= hd.cfg.ValueOf("TAOOptions.muSpreadingMultiplier", 2.0);

    UpdateLRSpreadingMu(hd, context, 32); //TODO: ISSUE 18 COMMENT 12

    if (context.useLR)
    {
        context.LRdata.UpdateMultipliers(hd);
        context.weights.lseW *= hd.cfg.ValueOf("GlobalPlacement.LagrangianRelaxation.alphaTWLMultiplier", 1.0);
    }
}

void ReportWeights(AppCtx& context) 
{
    if (context.useQuadraticSpreading)
        ALERT("sprW = " + Aux::SciFormat, context.weights.sprW);
    if (context.useLR)
        ALERT("lseW = " + Aux::SciFormat, context.weights.lseW);
}