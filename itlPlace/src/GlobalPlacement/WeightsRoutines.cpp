#include "ObjectivesConstraints.h"
#include "WeightsRoutines.h"

void InitWeights(double* x, AppCtx* context)
{
    context->weights.lseW = 1.0;
    context->weights.sodW = 1.0;
    context->weights.lrW  = 1.0;
    context->weights.sprW = 1.0;

    TotalCostAndGradients(context, x);

    double lseInitialRatio = context->hd->cfg.ValueOf("GlobalPlacement.Weights.lseInitialRatio", 1.0);
    double sprInitialRatio = context->hd->cfg.ValueOf("GlobalPlacement.Weights.sprInitialRatio", 1.0);
    double lrInitialRatio = context->hd->cfg.ValueOf("GlobalPlacement.Weights.lrInitialRatio", 1.0);

    double bigValue = 1.0e+6;

    if (context->useLogSumExp)
        context->weights.lseW = bigValue * lseInitialRatio / context->criteriaValues.lse;
    else
        context->weights.lseW = 0.0;

    if (context->useSpreading)
        context->weights.sprW = context->sprData.sprWInitial = bigValue * sprInitialRatio / context->criteriaValues.spr;
    else
        context->weights.sprW = context->sprData.sprWInitial = 0.0;
    
    if (context->useLR)
        context->weights.lrW = bigValue * lrInitialRatio / context->criteriaValues.lr;
    else
        context->weights.lrW = 0.0;

    ReportWeights(*context);
}

void ApplyWeights(AppCtx* context)
{
    context->criteriaValues.lse *= context->weights.lseW;
    context->criteriaValues.sod *= context->weights.sodW;
    context->criteriaValues.lr  *= context->weights.lrW;
    context->criteriaValues.spr *= context->weights.sprW;

    for (int i = 0; i < context->nVariables; i++)
    {
        context->criteriaValues.gLSE[i] *= context->weights.lseW;
        context->criteriaValues.gSOD[i] *= context->weights.sodW;
        context->criteriaValues.gLR[i]  *= context->weights.lrW;
        context->criteriaValues.gQS[i]  *= context->weights.sprW;
    }
}

void UpdateWeights(HDesign& hd, AppCtx& context)
{
    if (context.useLR)
    {
        context.LRdata.UpdateMultipliers(hd);

        double sprUpdateMultiplier = hd.cfg.ValueOf("GlobalPlacement.Weights.sprUpdateMultiplier", 1.0);
        double lseUpdateMultiplier = hd.cfg.ValueOf("GlobalPlacement.Weights.lseUpdateMultiplier", 1.0);
        
        context.weights.lseW *= lseUpdateMultiplier;
        context.weights.sprW *= sprUpdateMultiplier;
    }
    else
    {
        double sprUpdateMultiplier = hd.cfg.ValueOf("GlobalPlacement.Weights.sprUpdateMultiplier", 2.0);
        context.weights.sprW *= sprUpdateMultiplier;
    }
}

void ReportWeights(AppCtx& context) 
{
    ALERT("Cost terms weights:");
    ALERT("  lseW = " + Aux::SciFormat, context.weights.lseW);
    ALERT("  lrW  = " + Aux::SciFormat, context.weights.lrW);
    ALERT("  sprW = " + Aux::SciFormat, context.weights.sprW);
}