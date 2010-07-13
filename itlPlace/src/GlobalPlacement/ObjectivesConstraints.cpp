#include "ObjectivesConstraints.h"
#include "LogSumExp.h"
#include "SumOfDelays.h"
#include "LagrangianRelaxation.h"
#include "Spreading.h"

#include "time.h"
#include "omp.h"

void SetGradientToZero(double* gradient, int nComponents)
{
    for (int i = 0; i < nComponents; i++)
    {
        gradient[i] = 0.0;
    }
}

void NullCriteriaValues(AppCtx* context) 
{
    context->criteriaValues.objective = 0.0;
    context->criteriaValues.lse = 0.0;
    context->criteriaValues.lr = 0.0;
    context->criteriaValues.sod = 0.0;
    context->criteriaValues.spr = 0.0;
    context->criteriaValues.gLSENorm = 0.0;
    context->criteriaValues.gLRNorm = 0.0;
    context->criteriaValues.gSprNorm = 0.0;

    SetGradientToZero(context->criteriaValues.gLSE, context->nVariables);
    SetGradientToZero(context->criteriaValues.gSOD, context->nVariables);
    SetGradientToZero(context->criteriaValues.gLR,  context->nVariables);
    SetGradientToZero(context->criteriaValues.gQS,  context->nVariables);
}

void PrintClusterCoordinates(AppCtx* context)
{
    int clusterIdx = -1;
    while (GetNextActiveClusterIdx(context->ci, clusterIdx))
    {
        ALERT("Cluster %d coordinates: [%.3f, %.3f]", clusterIdx,
            context->ci->clusters[clusterIdx].xCoord, context->ci->clusters[clusterIdx].yCoord);
    }

    WRITELINE("");
}

void PrintDerivatives(int nClusterVariables, int nBufferVariables, double* gradient,
                      bool clusterX, bool clusterY, bool ki)
{
    const std::string format = "%.10f ";

    if (clusterX)
    {
        ALERT("Cluster X derivatives:");
        for (int i = 0; i < nClusterVariables/2; i++)
        {
            WRITE(format.c_str(), gradient[2*i]);
            if ((i+1) % 10 == 0)
            {
                WRITELINE("");
            }
        }
        WRITELINE("");
    }

    if (clusterY)
    {
        ALERT("Cluster Y derivatives:");
        for (int i = 0; i < nClusterVariables/2; i++)
        {
            WRITE(format.c_str(), gradient[2*i+1]);
            if ((i+1) % 10 == 0)
            {
                WRITELINE("");
            }
        }
        WRITELINE("");
    }

    if (ki)
    {
        ALERT("ki derivatives:");
        for (int i = nClusterVariables; i < nClusterVariables + nBufferVariables; i++)
        {
            WRITE(format.c_str(), gradient[i]);
            if ((i - nClusterVariables + 1) % 10 == 0)
            {
                WRITELINE("");
            }
        }
        WRITELINE("");
    }
}

void ScaleBufferGradients(int nClusterVariables, int nVariables, AppCtx*& context)
{
    double meanCoordGradient = 0.0;
    double meanBuffGradient = 0.0;
    double scale = 0.0;

    for (int i = 0; i < nClusterVariables; i++)
    {
        meanCoordGradient += context->criteriaValues.gSOD[i];
    }
    meanCoordGradient /= double(nClusterVariables);

    for (int i = nClusterVariables; i < nVariables; i++)
    {
        meanBuffGradient += context->criteriaValues.gSOD[i];
    }
    meanBuffGradient /= double(nVariables - nClusterVariables);

    scale = fabs(context->SODdata.gradientBalance * meanCoordGradient / meanBuffGradient);

    for (int i = nClusterVariables; i < nVariables; i++)
    {
        context->criteriaValues.gSOD[i] *= scale;
    }
}

void CalcGradValues( AppCtx* context )
{
    double gLSE = 0;
    double gLR = 0;
    double gSpreading = 0;

    for (int i = 0; i < context->nVariables; i++)
    {
        gLSE += context->criteriaValues.gLSE[i] * context->criteriaValues.gLSE[i];
        gLR += context->criteriaValues.gLR[i] * context->criteriaValues.gLR[i];
        gSpreading += context->criteriaValues.gQS[i] * context->criteriaValues.gQS[i];
    }

    context->criteriaValues.gLSENorm = sqrt(gLSE);
    context->criteriaValues.gLRNorm = sqrt(gLR);
    context->criteriaValues.gSprNorm = sqrt(gSpreading);

    //ALERT("LSE gradient value: %.3e", context->criteriaValues.gHPWL);
    //ALERT("LR  gradient value: %.3e", context->criteriaValues.gLR);
    //ALERT("Spr gradient value: %.3e", context->criteriaValues.gSpr);
}

void ReportDebugInfo(AppCtx* context, PetscScalar * solution, PetscScalar * gradient)
{
    //PrintCoordinates(nClusterVariables, nBufferVariables, solution);
    //PrintDerivatives(nClusterVariables, nBufferVariables, gradient, true, true, false);

    //ALERT("Corr gLSE&gLR %f\t", Aux::correlation(context->gLSE, context->gLR, context->nVariables));
    //ALERT("Corr gLSE&gQS %f\t", Aux::correlation(context->gLSE, context->gQS, context->nVariables));
    //ALERT("Corr gLR&gQS %f\t", Aux::correlation(context->gLR, context->gQS, context->nVariables));

    static bool plotSolverState = context->hd->cfg.ValueOf("GlobalPlacement.plotSolverState", false);
    if (plotSolverState)
    {
        context->hd->Plotter.VisualizeState(context->ci->mCurrentNumberOfClusters, 
            context->sprData.binGrid.nBinRows, context->sprData.binGrid.nBinCols, 
            context->ci->netList.size(), 
            (double*)solution, 
            context->criteriaValues.gLSE, context->criteriaValues.gSOD, context->criteriaValues.gLR, context->criteriaValues.gQS, 
            gradient);

        if (context->useSumOfDelays)
        {
            context->hd->Plotter.ClearHistogram();
            context->hd->Plotter.PlotKi(context->ci->mCurrentNumberOfClusters, context->ci->netList.size(), 
                (double*)solution, Color_Violet);
            context->hd->Plotter.RefreshHistogram();
        }
    }
}

int AnalyticalObjectiveAndGradient(TAO_APPLICATION taoapp, Vec X, double* f, Vec G, void* ctx)
{
    AppCtx* context = (AppCtx*)ctx;

    PetscScalar *solution, *gradient;
    iCHKERRQ VecGetArray(X, &solution);
    iCHKERRQ VecGetArray(G, &gradient);

    NullCriteriaValues(context);
    SetGradientToZero((double*)gradient, context->nVariables);    

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            if (context->useLogSumExp)
            {
                LSE_AddObjectiveAndGradient(context, solution);
            }
            if (context->useSumOfDelays)
            {
                SOD_AddObjectiveAndGradient(context, solution);
            }
            if (context->useLR)
            {
                LR_AddObjectiveAndGradient(context, solution);
            }
        }

        #pragma omp section
        {
            if (context->useQuadraticSpreading)
            {
                QS_AddObjectiveAndGradient(context, solution);
            }
            if (context->useLRSpreading)
            {
                LRS_AddObjectiveAndGradient(context, solution);
            }
        }
    }

    context->criteriaValues.objective = context->criteriaValues.lse + 
        context->criteriaValues.lr + 
        context->criteriaValues.sod + 
        context->criteriaValues.spr;

    *f = context->criteriaValues.objective;

    //normalize gradients
    if (context->useSumOfDelays)
        ScaleBufferGradients(2 * context->ci->mCurrentNumberOfClusters, context->nVariables, context);

    for (int i = 0; i < context->nVariables; i++)
    {
        gradient[i] += context->criteriaValues.gLSE[i];
        gradient[i] += context->criteriaValues.gSOD[i];
        gradient[i] += context->criteriaValues.gLR[i];
        gradient[i] += context->criteriaValues.gQS[i];
    }

    CalcGradValues(context);

    iCHKERRQ VecRestoreArray(X, &solution);
    iCHKERRQ VecRestoreArray(G, &gradient);

    ReportDebugInfo(context, solution, gradient);

    return OK;
}