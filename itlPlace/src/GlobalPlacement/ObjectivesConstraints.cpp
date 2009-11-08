#include "ObjectivesConstraints.h"
#include "LogSumExp.h"
#include "SumOfDelays.h"
#include "LagrangianRelaxation.h"
#include "Spreading.h"
#include "BorderPenalty.h"

#include "time.h"
#include "omp.h"

//TODO: think about more artificial method
void CalcMuInitial(PetscScalar *x, AppCtx* context)
{
  //The following coefficient greatly affects quality of placement
  //because it determines initial distribution of cells over bins. 
  //We have to choose small value to keep good HPWL.
  double muInitialmultiplier = context->hd->cfg.ValueOf("TAOOptions.muInitialMultiplier", 0.1);
  PrecalcExponents(context, x);

  //calculate criteria value
  double* criteriaValue = new double;
  *criteriaValue = 0.0;
  if (context->useLogSumExp)
    LSE_AddObjectiveAndGradient(context, x, criteriaValue);
  else if (context->useSumOfDelays)
    SOD_AddObjectiveAndGradient(context, x, criteriaValue);
  else if (context->useLR)
    LR_AddObjectiveAndGradient(context, x, criteriaValue);
  //ALERTFORMAT(("criteriaValue = %f", criteriaValue));

  //calculate penalty value
  double penaltyValue  = 0.0;
  penaltyValue = SpreadingPenalty(context, x);
  //ALERTFORMAT(("penaltyValue = %f", penaltyValue));

  //calculate mu value
  context->spreadingData.muInitial = muInitialmultiplier * (*criteriaValue) / penaltyValue;
  context->spreadingData.muSpreading = context->spreadingData.muInitial;

  if (context->useLRSpreading)
  {
    for (int i = 0; i < context->spreadingData.binGrid.nBins; ++i)
    {
      context->spreadingData.muBinsPen[i] = context->spreadingData.muInitial;
    }
  }

  context->muBorderPenalty = context->spreadingData.muInitial;
}

void SetGradientToZero(double* gradient, int nComponents)
{
  for (int i = 0; i < nComponents; i++)
  {
    gradient[i] = 0;
  }
}

void PrintCoordinates(int nClusterVariables, int nBufferVariables, double* solution)
{
  ALERT("Cluster X coordinates:");
  for (int i = 0; i < nClusterVariables/2; i++)
  {
    WRITE("%.2f ", solution[2*i]);
    if ((i+1) % 10 == 0)
    {
      WRITELINE("");
    }
  }
  WRITELINE("");

  ALERT("Cluster Y coordinates:");
  for (int i = 0; i < nClusterVariables/2; i++)
  {
    WRITE("%.2f ", solution[2*i+1]);
    if ((i+1) % 10 == 0)
    {
      WRITELINE("");
    }
  }
  WRITELINE("");

  ALERT("Buffer counts:");
  for (i = nClusterVariables; i < nClusterVariables + nBufferVariables; i++)
  {
    WRITE("%.2f ", solution[i]);
    if ((i - nClusterVariables + 1) % 10 == 0)
    {
      WRITELINE("");
    }
  }
  WRITELINE("");
}

void PrintDerivatives(int nClusterVariables, int nBufferVariables, double* gradient)
{
  ALERT("Cluster X derivatives:");
  for (int i = 0; i < nClusterVariables/2; i++)
  {
    WRITE("%.2f ", gradient[2*i]);
    if ((i+1) % 10 == 0)
    {
      WRITELINE("");
    }
  }
  WRITELINE("");

  //ALERT("Cluster Y derivatives:");
  //for (int i = 0; i < nClusterVariables/2; i++)
  //{
  //  WRITE("%.2f ", gradient[2*i+1]);
  //  if ((i+1) % 10 == 0)
  //  {
  //    WRITELINE("");
  //  }
  //}
  //WRITELINE("");

  ALERT("Buffer counts derivatives:");
  for (i = nClusterVariables; i < nClusterVariables + nBufferVariables; i++)
  {
    WRITE("%.2f ", gradient[i]);
    if ((i - nClusterVariables + 1) % 10 == 0)
    {
      WRITELINE("");
    }
  }
  WRITELINE("");
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

int AnalyticalObjectiveAndGradient(TAO_APPLICATION taoapp, Vec X, double* f, Vec G, void* ctx)
{
  AppCtx* context = (AppCtx*)ctx;

  int nClusterVariables = 2 * context->ci->mCurrentNumberOfClusters;
  int nBufferVariables = context->ci->netList.size();
  int nVariables = nClusterVariables + nBufferVariables;

  //get pointers to vector data
  int info;
  PetscScalar *solution, *gradient;
  info = VecGetArray(X, &solution); CHKERRQ(info);
  info = VecGetArray(G, &gradient); CHKERRQ(info);

  *f = 0.0;
  SetGradientToZero((double*)gradient, nVariables);
  SetGradientToZero(context->gLSE, nVariables);
  SetGradientToZero(context->gSOD, nVariables);
  SetGradientToZero(context->gLR,  nVariables);
  SetGradientToZero(context->gQS,  nVariables);

  //timing
  timetype start = 0;
  timetype finish = 0;

#pragma omp parallel sections
  {
#pragma omp section
    {
      if (context->useLogSumExp)
      {
        LSE_AddObjectiveAndGradient(context, solution, f);
      }

      if (context->useSumOfDelays)
      {
        SOD_AddObjectiveAndGradient(context, solution, f);
      }

      if (context->useLR)
      {
        LR_AddObjectiveAndGradient(context, solution, f);
      }
    }

#pragma omp section
    {
      if (context->useQuadraticSpreading)
      {
        QS_AddObjectiveAndGradient(context, solution, f);
      }

      if (context->useLRSpreading)
      {
        LRS_AddObjectiveAndGradient(context, solution, f);
      }
    }
  }

  if (context->useBorderPenalty)
  {
    AddBPwithGrad(context, solution, f, gradient);
  }

  //normalize gradients
  if (context->useSumOfDelays)
    ScaleBufferGradients(nClusterVariables, nVariables, context);

  for (int i = 0; i < nVariables; i++)
  {
    gradient[i] += context->gLSE[i];
    gradient[i] += context->gSOD[i];
    gradient[i] += context->gLR[i];
    gradient[i] += context->gQS[i];
  }

  //PrintCoordinates(nClusterVariables, nBufferVariables, solution);
  //PrintDerivatives(nClusterVariables, nBufferVariables, gradient);

  static bool plotGradients = context->hd->cfg.ValueOf("GlobalPlacement.plotGradients", false);
  if (plotGradients)
  {
    static double scaling = context->hd->cfg.ValueOf("GlobalPlacement.gradientScaling", 1.0);
    static int waitTime = context->hd->cfg.ValueOf("GlobalPlacement.plotWait", 1);
    context->hd->Plotter.ShowGradients(context->ci->mCurrentNumberOfClusters, 
      context->spreadingData.binGrid.nBinRows, context->spreadingData.binGrid.nBinCols, context->ci->netList.size(), 
      (double*)solution, context->gLSE, context->gSOD, context->gLR, context->gQS, gradient, 
      scaling, (HPlotter::WaitTime)waitTime);
  }

  info = VecRestoreArray(X, &solution); CHKERRQ(info);
  info = VecRestoreArray(G, &gradient); CHKERRQ(info);

  return 0;
}