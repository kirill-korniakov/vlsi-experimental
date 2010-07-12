#include "ObjectivesConstraints.h"
#include "LogSumExp.h"
#include "SumOfDelays.h"
#include "LagrangianRelaxation.h"
#include "Spreading.h"
#include "BorderPenalty.h"

#include "time.h"
#include "omp.h"

void NullCriteriaValues(AppCtx* context) 
{
    context->criteriaValues.objective = 0.0;
    context->criteriaValues.hpwl = 0.0;
    context->criteriaValues.lr = 0.0;
    context->criteriaValues.sod = 0.0;
    context->criteriaValues.spreading = 0.0;
    context->criteriaValues.gHPWL = 0.0;
    context->criteriaValues.gLR = 0.0;
    context->criteriaValues.gSpr = 0.0;
}

//TODO: think about more artificial method
void CalcMuInitial(PetscScalar *x, AppCtx* context)
{
    //The following coefficient greatly affects quality of placement
    //because it determines initial distribution of cells over bins. 
    //We have to choose small value to keep good HPWL.
    double spreadingWeightInitialMultiplier = context->hd->cfg.ValueOf("TAOOptions.spreadingWeightInitialMultiplier", 0.1);
    PrecalcExponents(context, x);

    double criteriaValue = 0.0;

    NullCriteriaValues(context);

    if (context->useLogSumExp)
    {
        LSE_AddObjectiveAndGradient(context, x);
        criteriaValue = context->criteriaValues.hpwl;
    }
    else if (context->useSumOfDelays)
    {
        SOD_AddObjectiveAndGradient(context, x);
        criteriaValue = context->criteriaValues.sod;
    }
    else if (context->useLR)
    {
        LR_AddObjectiveAndGradient(context, x);
        criteriaValue = context->criteriaValues.hpwl + context->criteriaValues.lr;
    }

    double penaltyValue  = 0.0;
    penaltyValue = SpreadingPenalty(context, x);
    //ALERT("penaltyValue = %f", penaltyValue));

    //calculate mu value
    context->spreadingData.spreadingWeightInitial = 
        spreadingWeightInitialMultiplier * criteriaValue / penaltyValue;
    context->spreadingData.spreadingWeight = context->spreadingData.spreadingWeightInitial;

    if (context->useLRSpreading)
    {
        for (int i = 0; i < context->spreadingData.binGrid.nBins; ++i)
        {
            context->spreadingData.muBinsPen[i] = context->spreadingData.spreadingWeightInitial;
        }
    }

    context->muBorderPenalty = context->spreadingData.spreadingWeightInitial;
}

void SetGradientToZero(double* gradient, int nComponents)
{
    for (int i = 0; i < nComponents; i++)
    {
        gradient[i] = 0;
    }
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
        meanCoordGradient += context->gSOD[i];
    }
    meanCoordGradient /= double(nClusterVariables);

    for (int i = nClusterVariables; i < nVariables; i++)
    {
        meanBuffGradient += context->gSOD[i];
    }
    meanBuffGradient /= double(nVariables - nClusterVariables);

    scale = fabs(context->SODdata.gradientBalance * meanCoordGradient / meanBuffGradient);

    for (int i = nClusterVariables; i < nVariables; i++)
    {
        context->gSOD[i] *= scale;
    }
}

void CalcGradValues( AppCtx* context )
{
    double gLSE = 0;
    double gLR = 0;
    double gSpreading = 0;

    for (int i = 0; i < context->nVariables; i++)
    {
        gLSE += context->gLSE[i] * context->gLSE[i];
        gLR += context->gLR[i] * context->gLR[i];
        gSpreading += context->gQS[i] * context->gQS[i];
    }

    context->criteriaValues.gHPWL = sqrt(gLSE);
    context->criteriaValues.gLR = sqrt(gLR);
    context->criteriaValues.gSpr = sqrt(gSpreading);

    //ALERT("LSE gradient value: %.3e", context->criteriaValues.gHPWL);
    //ALERT("LR  gradient value: %.3e", context->criteriaValues.gLR);
    //ALERT("Spr gradient value: %.3e", context->criteriaValues.gSpr);
}

double correlation(double* x, double* y, int n)
{
    double result;
    int i;
    double xmean;
    double ymean;
    double s;
    double xv;
    double yv;
    double t1;
    double t2;

    xv = 0;
    yv = 0;
    if( n<=1 )
    {
        result = 0;
        return result;
    }

    //
    // Mean
    //
    xmean = 0;
    ymean = 0;
    for(i = 0; i <= n-1; i++)
    {
        xmean = xmean+x[i];
        ymean = ymean+y[i];
    }
    xmean = xmean/n;
    ymean = ymean/n;

    //
    // numerator and denominator
    //
    s = 0;
    xv = 0;
    yv = 0;
    for(i = 0; i <= n-1; i++)
    {
        t1 = x[i]-xmean;
        t2 = y[i]-ymean;
        xv = xv + t1*t1;
        yv = yv + t2*t2;
        s = s+t1*t2;
    }
    double epsilon = 1.0e-100;
    if( fabs(xv) < epsilon || fabs(yv) < epsilon )
    {
        result = 0;
    }
    else
    {
        result = s/(sqrt(xv)*sqrt(yv));
    }
    return result;
}

void ReportDebugInfo(AppCtx* context, PetscScalar * solution, PetscScalar * gradient)
{
    //PrintCoordinates(nClusterVariables, nBufferVariables, solution);
    //PrintDerivatives(nClusterVariables, nBufferVariables, gradient, true, true, false);
    //ReportGradValues(context);

    //ALERT("Corr gLSE&gLR %f\t", correlation(context->gLSE, context->gLR, context->nVariables));
    //ALERT("Corr gLSE&gQS %f\t", correlation(context->gLSE, context->gQS, context->nVariables));
    //ALERT("Corr gLR&gQS %f\t", correlation(context->gLR, context->gQS, context->nVariables));

    static bool plotSolverState = context->hd->cfg.ValueOf("GlobalPlacement.plotSolverState", false);
    if (plotSolverState)
    {
        context->hd->Plotter.VisualizeState(context->ci->mCurrentNumberOfClusters, 
            context->spreadingData.binGrid.nBinRows, context->spreadingData.binGrid.nBinCols, 
            context->ci->netList.size(), 
            (double*)solution, context->gLSE, context->gSOD, context->gLR, context->gQS, gradient);

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

    int nClusterVariables = 2 * context->ci->mCurrentNumberOfClusters;
    int nBufferVariables = context->ci->netList.size();

    //get pointers to vector data
    PetscScalar *solution, *gradient;
    iCHKERRQ VecGetArray(X, &solution);
    iCHKERRQ VecGetArray(G, &gradient);

    NullCriteriaValues(context);

    SetGradientToZero((double*)gradient, context->nVariables);
    SetGradientToZero(context->gLSE, context->nVariables);
    SetGradientToZero(context->gSOD, context->nVariables);
    SetGradientToZero(context->gLR,  context->nVariables);
    SetGradientToZero(context->gQS,  context->nVariables);

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

    context->criteriaValues.objective = context->criteriaValues.hpwl + 
        context->criteriaValues.lr + 
        context->criteriaValues.sod + 
        context->criteriaValues.spreading;

    *f = context->criteriaValues.objective;

    //normalize gradients
    if (context->useSumOfDelays)
        ScaleBufferGradients(nClusterVariables, context->nVariables, context);

    for (int i = 0; i < context->nVariables; i++)
    {
        gradient[i] += context->gLSE[i];
        gradient[i] += context->gSOD[i];
        gradient[i] += context->gLR[i];
        gradient[i] += context->gQS[i];
    }

    iCHKERRQ VecRestoreArray(X, &solution);
    iCHKERRQ VecRestoreArray(G, &gradient);

    ReportDebugInfo(context, solution, gradient);

    return OK;
}