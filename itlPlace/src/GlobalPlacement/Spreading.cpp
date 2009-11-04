#include "Spreading.h"

extern timetype calcPotentialsTime;
extern timetype quadraticSpreading;
extern timetype quadraticSpreadingGradTime;

void MoveBinIndexesIntoBorders(AppCtx* context, int& min_col, int& min_row, int& max_col, int& max_row)
{
  min_col = std::min(std::max(0, min_col), context->binGrid.nBinCols-1);
  min_row = std::min(std::max(0, min_row), context->binGrid.nBinRows-1);
  max_col = std::max(0, std::min(max_col, context->binGrid.nBinCols-1));
  max_row = std::max(0, std::min(max_row, context->binGrid.nBinRows-1));
}

void DetermineBordersOfClusterPotential(int& min_col, int& max_col, 
                                        int& min_row, int& max_row,
                                        int i, PetscScalar* x, AppCtx* context)
{
  min_col = (Aux::cool_dtoi((x[2*i+0] - context->hd->Circuit.PlacementMinX() - 
    context->potentialRadiusX) / context->binGrid.binWidth));
  max_col = (Aux::cool_dtoi((x[2*i+0] - context->hd->Circuit.PlacementMinX() + 
    context->potentialRadiusX) / context->binGrid.binWidth));
  min_row = (Aux::cool_dtoi((x[2*i+1] - context->hd->Circuit.PlacementMinY() - 
    context->potentialRadiusY) / context->binGrid.binHeight));    
  max_row = (Aux::cool_dtoi((x[2*i+1] - context->hd->Circuit.PlacementMinY() + 
    context->potentialRadiusY) / context->binGrid.binHeight));

  MoveBinIndexesIntoBorders(context, min_col, min_row, max_col, max_row);
}

double CalcBellShapedFunction(AppCtx* context, int solutionIdx, int clusterIdx, 
                              int colIdx, int rowIdx,
                              PetscScalar* x)
{
  double potX  = 0;
  double potY  = 0;

  double dx = x[2*solutionIdx+0] - context->binGrid.bins[rowIdx][colIdx].xCoord;
  double dy = x[2*solutionIdx+1] - context->binGrid.bins[rowIdx][colIdx].yCoord;

  double fabsdx = fabs(dx);
  double fabsdy = fabs(dy);

  if (fabsdx > context->potentialRadiusX || 
    fabsdy > context->potentialRadiusY)
  {//bin out of potential
    return 0;
  }

  //calculate x-potential
  if (fabsdx <= context->potentialRadiusX/2)
  {
    potX = 1 - 2 * dx * dx * context->invPSX;
  }
  else
  {
    potX = 2 * (fabsdx - context->potentialRadiusX) * (fabsdx - context->potentialRadiusX) * context->invPSX;
  }

  //calculate y-potential
  if (fabsdy <= context->potentialRadiusY/2)
  {
    potY = 1 - 2 * dy * dy * context->invPSY;
  }
  else
  {
    potY = 2 * (fabsdy - context->potentialRadiusY) * (fabsdy - context->potentialRadiusY) * context->invPSY;
  }

  return potX * potY;
}

//REFACTOR: use CalcBellShapedFunc() function
void CalcBellShapedFuncAndDerivative(AppCtx* userData, int solutionIdx, int clusterIdx, 
                                     int colIdx, int rowIdx,
                                     PetscScalar* x,
                                     //double &potX, 
                                     double &gradX, 
                                     //double &potY, 
                                     double &gradY)
{
  double potX  = 0;
  gradX = 0;
  double _potX = 0;
  double potY  = 0;
  gradY = 0;
  double _potY = 0;

  double dx = x[2*solutionIdx+0] - userData->binGrid.bins[rowIdx][colIdx].xCoord;
  double dy = x[2*solutionIdx+1] - userData->binGrid.bins[rowIdx][colIdx].yCoord;

  double fabsdx = fabs(dx);
  double fabsdy = fabs(dy);
  double sigx4 = 4 * Aux::sign(dx);
  double sigy4 = 4 * Aux::sign(dy);

  double multiplier;
  double residual;

  if (fabsdx > userData->potentialRadiusX || fabsdy > userData->potentialRadiusY)
  {//cluster doesn't affect this bin
    return;
  } 

  // calculate x-potential
  if (2 * fabsdx <= userData->potentialRadiusX)
  {
    potX  = 1 - 2 * dx * dx * userData->invPSX;
    _potX = -sigx4 * fabsdx * userData->invPSX;
  }
  else
  {
    potX  = 2 * (fabsdx - userData->potentialRadiusX) * (fabsdx - userData->potentialRadiusX) * userData->invPSX;
    _potX = sigx4 * (fabsdx - userData->potentialRadiusX)  * userData->invPSX;
  }

  // calculate y-potential
  if (2 * fabsdy <= userData->potentialRadiusY)
  {
    potY  = 1 - 2 * dy * dy * userData->invPSY;
    _potY = -sigy4 * fabsdy * userData->invPSY;
  }
  else
  {
    potY  = 2 * (fabsdy - userData->potentialRadiusY) * (fabsdy - userData->potentialRadiusY) * userData->invPSY;
    _potY = sigy4 * (fabsdy - userData->potentialRadiusY) * userData->invPSY;
  }

  residual = userData->binGrid.bins[rowIdx][colIdx].sumPotential - userData->desiredCellsAreaAtEveryBin;
  if (userData->useUnidirectSpreading)
  {
    if (residual > 0.0)
      multiplier = 2 * residual *
      userData->ci->clusters[clusterIdx].potentialMultiplier;
    else
      multiplier = 0.0;
  }
  else
    multiplier = 2 * residual *
    userData->ci->clusters[clusterIdx].potentialMultiplier;

  gradX = multiplier * _potX * potY;
  gradY = multiplier * _potY * potX;
}

// calculate every cluster's potential
void CalculatePotentials(AppCtx* userData, PetscScalar* x)
{
  //null the penalties
  for (int i = 0; i < userData->binGrid.nBinRows; ++i)
  {
    for (int j = 0; j < userData->binGrid.nBinCols; ++j)
    {
      userData->binGrid.bins[i][j].sumPotential = 0.0;
    }
  }

  // each cell must have total potential equal to its area
  // this variable are needed to control this issue
  double currClusterTotalPotential = 0.0;

  // we take every cluster and consider only bins 
  // which are affected by this cluster potential
  int idxInSolutionVector = 0;
  int clusterIdx = -1;
  int min_row, min_col, max_row, max_col; // area affected by cluster potential 

  while (GetNextActiveClusterIdx(userData->ci, clusterIdx))
  {
    idxInSolutionVector = userData->clusterIdx2solutionIdxLUT[clusterIdx];
    currClusterTotalPotential = 0.0;

    DetermineBordersOfClusterPotential(min_col, max_col, min_row, max_row, idxInSolutionVector, x, userData);

    // loop over affected bins - now we just precalculate potential
    // later we will scale it so that currClusterTotalPotential = cluster area
    //#pragma omp parallel for reduction(+:currClusterTotalPotential)
    for (int rowIdx = min_row; rowIdx <= max_row; ++rowIdx)
    {
      for (int colIdx = min_col; colIdx <= max_col; ++colIdx)
      {
        double bsf = CalcBellShapedFunction(userData, idxInSolutionVector, clusterIdx, colIdx, rowIdx, x);

        userData->clusterPotentialOverBins[rowIdx-min_row][colIdx-min_col] = bsf;
        currClusterTotalPotential += bsf;
      }
    }// loop over affected bins

    // scale the potential
    if (currClusterTotalPotential != 0)
    {
      userData->ci->clusters[clusterIdx].potentialMultiplier = 
        userData->ci->clusters[clusterIdx].area / currClusterTotalPotential;
    } 
    else
    {
      userData->ci->clusters[clusterIdx].potentialMultiplier = 0;
    }

    // add scaled cluster potential 
    for (int k = min_row; k <= max_row; ++k)
      for (int j = min_col; j <= max_col; ++j)
        userData->binGrid.bins[k][j].sumPotential += 
        userData->ci->clusters[clusterIdx].potentialMultiplier * 
        userData->clusterPotentialOverBins[k-min_row][j-min_col];
  }

  double totalPotential = 0.0;
  for (int i = 0; i < userData->binGrid.nBinRows; ++i)
  {
    for (int j = 0; j < userData->binGrid.nBinCols; ++j)
    {
      totalPotential += userData->binGrid.bins[i][j].sumPotential;
    }
  }
  //ALERTFORMAT(("Total Potential   = %f", totalPotential));
  //ALERTFORMAT(("Desired Potential = %f", 
  //  user->desiredCellsAreaAtEveryBin * user->binGrid.nBins));
}

double SpreadingPenalty(AppCtx* user, PetscScalar* x)
{
  timetype start_local, finish_local;
  start_local = GET_TIME_METHOD();
  CalculatePotentials(user, x);
  finish_local = GET_TIME_METHOD();
  calcPotentialsTime += finish_local - start_local;

  double spreadingPenalty = 0.0;
  double residual;

  if (user->useLRSpreading)
  {
    int binIdx;

    for (int i = 0; i < user->binGrid.nBinRows; ++i)
    {
      for (int j = 0; j < user->binGrid.nBinCols; ++j)
      {
        binIdx = i * user->binGrid.nBinCols + j;
        residual = user->binGrid.bins[i][j].sumPotential - user->desiredCellsAreaAtEveryBin;

        if (user->useUnidirectSpreading)
        {
          if (residual > 0.0)
          {
            user->binsPenaltyValues[binIdx] = residual * residual;
          }
        }
        else
        {
          user->binsPenaltyValues[binIdx] = residual * residual;
        }

        spreadingPenalty += user->binsPenaltyValues[binIdx];
      }
    } 
  }
  else
  {
    for (int i = 0; i < user->binGrid.nBinRows; ++i)
    {
      for (int j = 0; j < user->binGrid.nBinCols; ++j)
      {
        residual = user->binGrid.bins[i][j].sumPotential - user->desiredCellsAreaAtEveryBin;
        if (user->useUnidirectSpreading)
        {
          if (residual > 0.0)
          {
            spreadingPenalty += residual * residual;
          }
        }
        else
        {
          spreadingPenalty += residual * residual;
        }
      }
    }
  }

  return spreadingPenalty;
}

void AddSpreadingPenaltyGradient(AppCtx* user, PetscScalar* x, PetscScalar* grad)
{
  /* double potX;
  double potY;*/


  int idxInSolutionVector = 0;
  int clusterIdx = -1;
  double muBinsPen;

  if (user->useLRSpreading)
  {
    while (GetNextActiveClusterIdx(user->ci, clusterIdx))
    {
      idxInSolutionVector = user->clusterIdx2solutionIdxLUT[clusterIdx];

      int min_row, min_col, max_row, max_col; // area affected by cluster potential 
      DetermineBordersOfClusterPotential(min_col, max_col, min_row, max_row, idxInSolutionVector, x, user);

      double gX = 0;
      double gY = 0;
      //#pragma omp parallel for reduction(+:gX) reduction(+:gY)
      for (int rowIdx = min_row; rowIdx <= max_row; ++rowIdx)
      {
        for (int colIdx = min_col; colIdx <= max_col; ++colIdx)
        {
          double gradX;
          double gradY;
          CalcBellShapedFuncAndDerivative(user, idxInSolutionVector, clusterIdx, colIdx, rowIdx, x, 
            /* potX,*/ gradX,/* potY,*/ gradY);

          muBinsPen = user->muBinsPen[rowIdx * user->binGrid.nBinCols + colIdx];
          gX += muBinsPen * gradX;
          gY += muBinsPen * gradY;
        }
      }

      grad[2*idxInSolutionVector+0] = gX;
      grad[2*idxInSolutionVector+1] = gY;
    }
  }
  else
  {
    while (GetNextActiveClusterIdx(user->ci, clusterIdx))
    {
      idxInSolutionVector = user->clusterIdx2solutionIdxLUT[clusterIdx];

      int min_row, min_col, max_row, max_col; // area affected by cluster potential 
      DetermineBordersOfClusterPotential(min_col, max_col, min_row, max_row, idxInSolutionVector, x, user);

      double gX = 0;
      double gY = 0;

      //#pragma omp parallel for reduction(+:gX) reduction(+:gY)
      for (int rowIdx = min_row; rowIdx <= max_row; ++rowIdx)
      {
        for (int colIdx = min_col; colIdx <= max_col; ++colIdx)
        {
          double gradX;
          double gradY;
          CalcBellShapedFuncAndDerivative(user, idxInSolutionVector, clusterIdx, colIdx, rowIdx, x, 
            /*potX,*/ gradX, /*potY,*/ gradY);
          gX += user->muSpreading * gradX;
          gY += user->muSpreading * gradY;
        }
      }

      grad[2*idxInSolutionVector+0] = gX;
      grad[2*idxInSolutionVector+1] = gY;
    }
  }
}

void QS_AddObjectiveAndGradient(AppCtx* context, PetscScalar * solution, double* &f)
{
  timetype start = GET_TIME_METHOD();
  *f += context->muSpreading * SpreadingPenalty(context, solution);
  //ALERTFORMAT(("SPR = %f", addValue));
  timetype finish = GET_TIME_METHOD();
  quadraticSpreading += finish - start;

  start = GET_TIME_METHOD();
  AddSpreadingPenaltyGradient(context, solution, context->gQS);
  finish = GET_TIME_METHOD();
  quadraticSpreadingGradTime += finish - start;
}

void LRS_AddObjectiveAndGradient(AppCtx* context, PetscScalar* solution, double*& f)
{
  SpreadingPenalty(context, solution);
  *f += Aux::ScalarProduct(context->muBinsPen, context->binsPenaltyValues, context->binGrid.nBins);
  //ALERTFORMAT(("LRS = %f", addValue));
  AddSpreadingPenaltyGradient(context, solution, context->gQS); //FIXME:HACK: use proper gradient
}

double CalculateDiscrepancy(Vec& x, void* data)
{
  AppCtx* user = (AppCtx*)data;

  double maximum = 0;
  double dimension;  // половина стороны квадрата, площадь которого равна площади текущего кластера
  int min_col;
  int max_col;
  int min_row;
  int max_row;
  // доли, на которые делится бинами данный кластер (от 0 до 1)
  double leftRatio  = 0.0;  
  double rightRatio = 0.0;
  double lowerRatio = 0.0;
  double upperRatio = 0.0;

  PetscScalar* solution;
  VecGetArray(x, &solution);

  double *clustersAreasInBins = new double[user->binGrid.nBins];
  for (int i = 0; i < user->binGrid.nBins; ++i)
  {
    clustersAreasInBins[i] = 0;
  }

  int idxInSolutionVector;
  for (int i = 0; i < static_cast<int>(user->ci->clusters.size()); ++i)
  {
    if (user->ci->clusters[i].isFake)
      continue;

    idxInSolutionVector = user->clusterIdx2solutionIdxLUT[i];
    dimension = sqrt((double)user->ci->clusters[i].area) / 2;
    min_col = static_cast<int>((solution[2*idxInSolutionVector+0]-dimension) / user->binGrid.binWidth);
    max_col = static_cast<int>((solution[2*idxInSolutionVector+0]+dimension) / user->binGrid.binWidth);
    min_row = static_cast<int>((solution[2*idxInSolutionVector+1]-dimension) / user->binGrid.binHeight);    
    max_row = static_cast<int>((solution[2*idxInSolutionVector+1]+dimension) / user->binGrid.binHeight);

    MoveBinIndexesIntoBorders(user, min_col, min_row, max_col, max_row);

    if (min_col != max_col)
    {
      leftRatio  = (user->binGrid.binWidth * (min_col + 1) - solution[2*idxInSolutionVector+0] + dimension) / (2*dimension);
    }
    else
    {
      leftRatio = 0.0;
    }
    rightRatio = 1 - leftRatio;

    if (min_row != max_row)
    {
      lowerRatio = (user->binGrid.binHeight * (min_row + 1) - solution[2*idxInSolutionVector+1] + dimension) / (2*dimension);
    } 
    else
    {
      lowerRatio = 0.0;
    }
    upperRatio = 1 - lowerRatio;

    if (fabs(leftRatio + rightRatio + lowerRatio + upperRatio - 2) > 0.001) 
    {
      break;
    }

    clustersAreasInBins[min_row * user->binGrid.nBinCols + min_col] += 
      user->ci->clusters[i].area * leftRatio * lowerRatio;
    clustersAreasInBins[min_row * user->binGrid.nBinCols + max_col] += 
      user->ci->clusters[i].area * rightRatio * lowerRatio;
    clustersAreasInBins[max_row * user->binGrid.nBinCols + min_col] += 
      user->ci->clusters[i].area * leftRatio * upperRatio;
    clustersAreasInBins[max_row * user->binGrid.nBinCols + max_col] += 
      user->ci->clusters[i].area * rightRatio * upperRatio;
  }

  for (int i = 0; i < user->binGrid.nBins; ++i)
  {
    if (maximum < clustersAreasInBins[i])
    {
      maximum = clustersAreasInBins[i];
    }

    if (user->useLRSpreading)
    {
      user->individualBinsDiscrepancy[i] = clustersAreasInBins[i] / user->desiredCellsAreaAtEveryBin;
    }
  }

  delete [] clustersAreasInBins;
  VecRestoreArray(x, &solution);

  return maximum / user->desiredCellsAreaAtEveryBin;
}