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
    context->weights.sprW = 1.0;

    //The following coefficient greatly affects quality of placement
    //because it determines initial distribution of cells over bins. 
    //We have to choose small value to keep good HPWL.
    double sprWInitialMultiplier = context->hd->cfg.ValueOf("TAOOptions.spreadingWeightInitialMultiplier", 0.1);
    PrecalcExponents(context, x);

    CalculateCostAndGradients(context, x);

    double criteriaValue = context->criteriaValues.lse + context->criteriaValues.lr + context->criteriaValues.sod;
    double penaltyValue = context->criteriaValues.spr;

    context->sprData.sprWInitial = sprWInitialMultiplier * criteriaValue / penaltyValue;
    context->weights.sprW = context->sprData.sprWInitial;
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

void UpdateWeights(HDesign& hd, AppCtx& context, int iterate)
{
    context.weights.sprW *= hd.cfg.ValueOf("TAOOptions.muSpreadingMultiplier", 2.0);

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