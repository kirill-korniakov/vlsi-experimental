#include "ObjectivesConstraints.h"
//#include "mkl_vml.h"
#include "time.h"
#include "omp.h"

extern timetype expTime;
extern timetype lseTime;
extern timetype lseGradTime;
extern timetype calcPotentialsTime;
extern timetype quadraticSpreading;
extern timetype quadraticSpreadingGradTime;

extern timetype start;
extern timetype finish;

void PrecalcExponents(AppCtx* userData, PetscScalar* coordinates);

void GetClusterCoordinates(int clusterIdx, PetscScalar *coordinates, 
                           AppCtx* userData, double& x, double& y)
{
  if (IsMovableCell(clusterIdx))
  {
    int clusterIdxInCoordinatesArray = (userData->clusterIdx2solutionIdxLUT)[clusterIdx];
    CRITICAL_ASSERT(clusterIdxInCoordinatesArray != -1);
    x = coordinates[2*clusterIdxInCoordinatesArray+0];
    y = coordinates[2*clusterIdxInCoordinatesArray+1];
  }
  else if (IsTerminal(clusterIdx))
  {
    HCell terminal = userData->ci->terminalCells[clusterIdx - SHIFT_NUMBER_FOR_TERMINALS];
    x = userData->hd->GetDouble<HCell::X>(terminal);
    y = userData->hd->GetDouble<HCell::Y>(terminal);
  }
  else
  {
    HPin pin = userData->ci->primaryPins[clusterIdx - SHIFT_NUMBER_FOR_PRIMARY_PINS];
    x = userData->hd->GetDouble<HPin::X>(pin);
    y = userData->hd->GetDouble<HPin::Y>(pin);
  }
}

int GetPrecalcedExponentsIdx(AppCtx* context, int clusterIdx)
{
  if (IsMovableCell(clusterIdx))
  {
    return context->clusterIdx2solutionIdxLUT[clusterIdx];
  }
  else if (IsTerminal(clusterIdx))
  {
    return clusterIdx - SHIFT_NUMBER_FOR_TERMINALS + context->ci->mCurrentNumberOfClusters;
  }
  else
  {
    return clusterIdx - SHIFT_NUMBER_FOR_PRIMARY_PINS + context->ci->mCurrentNumberOfClusters
      + context->ci->terminalCells.size();
  }
}

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

double GetHeightOfLowestRow(AppCtx* userData)
{
  double heightOfLowestRow = 0;
  double yOfLowestRow = userData->hd->Circuit.PlacementMaxY();
  for (HPlacementRows::EnumeratorW rIter = userData->hd->PlacementRows.GetEnumeratorW(); rIter.MoveNext(); )
  {
    if (rIter.Y() <= yOfLowestRow)
    {
      heightOfLowestRow = rIter.SiteHeight();
      yOfLowestRow = rIter.Y();
    }
  }
  return heightOfLowestRow;
}

double BorderPenalty(AppCtx* userData, PetscScalar* x)
{
  double borderPenalty = 0.0;
  double deviation = 0.0;
  double penaltyVal = 0.0;
  static double minX = userData->hd->Circuit.PlacementMinX();
  static double maxX = userData->hd->Circuit.PlacementMaxX();
  static double minY = userData->hd->Circuit.PlacementMinY() + GetHeightOfLowestRow(userData) / 2.0;
  static double maxY = userData->hd->Circuit.PlacementMaxY();
  int idxInSolutionVector;
  int nCellsInCluster = 0;
  int clusterIdx = -1;
  while (GetNextActiveClusterIdx(userData->ci, clusterIdx))
  {
    idxInSolutionVector = userData->clusterIdx2solutionIdxLUT[clusterIdx];
    nCellsInCluster = static_cast<int>(userData->ci->clusters[clusterIdx].cells.size());

    deviation = 0.0;
    if (x[2*idxInSolutionVector+0] < minX)
    {
      deviation = minX - x[2*idxInSolutionVector+0];
    }
    else if (x[2*idxInSolutionVector+0] > maxX)
    {
      deviation = x[2*idxInSolutionVector+0] - maxX;
    }
    penaltyVal = nCellsInCluster * pow(deviation, 2);
    borderPenalty += penaltyVal;

    deviation = 0.0;
    if (x[2*idxInSolutionVector+1] < minY)
    {
      deviation = minY - x[2*idxInSolutionVector+1];
    }
    else if (x[2*idxInSolutionVector+1] > maxY)
    {
      deviation = x[2*idxInSolutionVector+1] - maxY;
    }
    penaltyVal = nCellsInCluster * pow(deviation, 2);
    borderPenalty += penaltyVal;
  }

  userData->borderPenaltyVal = borderPenalty;

  return borderPenalty;
}

void AddBorderPenaltyGradient(AppCtx* user, PetscScalar* x, PetscScalar* grad)
{
  double deviation = 0.0;
  static double minX = user->hd->Circuit.PlacementMinX();
  static double maxX = user->hd->Circuit.PlacementMaxX();
  static double minY = user->hd->Circuit.PlacementMinY();
  static double maxY = user->hd->Circuit.PlacementMaxY();

  int idxInSolutionVector;
  int nCells = 0;
  int clusterIdx = -1;
  while (GetNextActiveClusterIdx(user->ci, clusterIdx))
  {
    idxInSolutionVector = user->clusterIdx2solutionIdxLUT[clusterIdx];
    nCells = static_cast<int>(user->ci->clusters[clusterIdx].cells.size());

    deviation = 0.0;
    if (x[2*idxInSolutionVector+0] < minX)
    {
      deviation = x[2*idxInSolutionVector+0] - minX;
    }
    else if (x[2*idxInSolutionVector+0] > maxX)
    {
      deviation = x[2*idxInSolutionVector+0] - maxX;
    }
    grad[2*idxInSolutionVector+0] += /*context->muBorderPenalty * */nCells * 2 * deviation; //HACK: we really need this multiplier

    deviation = 0.0;
    if (x[2*idxInSolutionVector+1] < minY)
    {
      deviation = x[2*idxInSolutionVector+1] - minY;
    }
    else if (x[2*idxInSolutionVector+1] > maxY)
    {
      deviation = x[2*idxInSolutionVector+1] - maxY;
    }
    grad[2*idxInSolutionVector+1] += /*context->muBorderPenalty * */nCells * 2 * deviation; //HACK: we really need this multiplier
  }
}

double CalcNetLSE(AppCtx* data, PetscScalar* coordinates, int netIdx)
{
  double sumExp1 = 0.0;
  double sumExp2 = 0.0;
  double sumExp3 = 0.0;
  double sumExp4 = 0.0;
  
  int precalcedExponentsIdx;
  int realClusterIdx;

  int netSize = data->netListSizes[netIdx];
  
  for (int j = 0; j < netSize; ++j)
  {
    realClusterIdx = data->ci->netList[netIdx].clusterIdxs[j];
    precalcedExponentsIdx = GetPrecalcedExponentsIdx(data, realClusterIdx);

    sumExp1 += data->precalcedExponents[4*precalcedExponentsIdx + 0];
    sumExp2 += data->precalcedExponents[4*precalcedExponentsIdx + 1];
    sumExp3 += data->precalcedExponents[4*precalcedExponentsIdx + 2];
    sumExp4 += data->precalcedExponents[4*precalcedExponentsIdx + 3];
  }
  data->SUM1[netIdx] = sumExp1;
  data->SUM2[netIdx] = sumExp2;
  data->SUM3[netIdx] = sumExp3;
  data->SUM4[netIdx] = sumExp4;

  return data->alpha * log(sumExp1 * sumExp2 * sumExp3 * sumExp4);
}

double LogSumExp(void* data, PetscScalar* coordinates)
{
  AppCtx* context = (AppCtx*)data;

  double logSumExp = 0.0;
  int netListSize = static_cast<int>(context->ci->netList.size());

  for (int i = 0; i < netListSize; ++i)
  {
    logSumExp += CalcNetLSE(context, coordinates, i) * context->ci->netList[i].weight;
    //ALERTFORMAT(("%d logSumExp =\t%f", i, logSumExp));
  }

  return logSumExp;
}

double SumOfDelays(void* data, PetscScalar* coordinates)
{
  AppCtx* context = (AppCtx*)data;

  double sumOfDelays = 0.0;
  int netListSize = static_cast<int>(context->ci->netList.size());
  int nClusterCoordinates = 2 * context->ci->mCurrentNumberOfClusters;
  double ki = 0.0;
  
  //FIXME: debugging variables
  double kopt = 0.0;
  double x = 0.0;
  double y = 0.0;

  for (int i = 0; i < netListSize; ++i)
  {
    ki = coordinates[nClusterCoordinates + i]; //WARNING: this index is correct
    kopt = (CalcNetLSE(context, coordinates, i) + context->ci->netList[i].Lnet - context->Lbuf) / context->Dbuf - 1;

    context->ci->netList[i].braces = CalcNetLSE(context, coordinates, i) + 
      context->ci->netList[i].Lnet + ki * context->Lbuf;

    x = context->ci->netList[i].braces;
    x = x * x;
    x = x / (ki + 1);
    y = context->DbufLbufDifferenceOfSquares;
    y = y * ki;
    
    sumOfDelays = sumOfDelays + x + y;

    //sumOfDelays += context->ci->netList[i].braces * context->ci->netList[i].braces / (ki + 1) +
    //  ki * context->DbufLbufDifferenceOfSquares;
  }

  return sumOfDelays;
}

//TODO: think about more artificial method
void CalcMuInitial(PetscScalar *x, AppCtx* context)
{
  //The following coefficient greatly affects quality of placement
  //because it determines initial distribution of cells over bins. 
  //We have to choose small value to keep good HPWL.
  double muInitialmultiplier = context->hd->cfg.ValueOf("TAOOptions.muInitialMultiplier", 0.1);
  PrecalcExponents(context, x);

  double criteriaValue;
  double penaltyValue;

  //calculate criteria value
  if (context->useLogSumExp)
    criteriaValue = LogSumExp((void*)context, x);
  else if (context->useSumOfDelays)
    criteriaValue = SumOfDelays((void*)context, x);
  else
    criteriaValue = LogSumExp((void*)context, x);

  //calculate penalty value
  //ALERTFORMAT(("criteriaValue = %f", criteriaValue));
  penaltyValue = SpreadingPenalty(context, x);
  //ALERTFORMAT(("penaltyValue = %f", penaltyValue));

  //calculate mu value
  context->muInitial = muInitialmultiplier * criteriaValue / penaltyValue;
  context->muSpreading = context->muInitial;

  if (context->useLRSpreading)
  {
    for (int i = 0; i < context->binGrid.nBins; ++i)
    {
      context->muBinsPen[i] = context->muInitial;
    }
  }

  context->muBorderPenalty = context->muInitial;
}

void SetGradientToZero(double* grad, int nComponents)
{
  for (int i = 0; i < nComponents; i++)
  {
    grad[i] = 0;
  }
}

double CalcNetLSEGradient(AppCtx* userData, int netIdx, int idxInSolutionVector)
{
  double sum1 = 0.0;
  double sum2 = 0.0;
  //int precalcedExponentsIdx;

  //int netSize = userData->netListSizes[netIdx];
  //int netSize = static_cast<int>(userData->ci->netList[netIdx].clusterIdxs.size());
  
  //for (int k = 0; k < netSize; ++k)
  //{
  //  int realClusterIdx = userData->ci->netList[netIdx].clusterIdxs[k];
  //  precalcedExponentsIdx = GetPrecalcedExponentsIdx(userData, realClusterIdx);

  //  //if (idxInSolutionVector % 2 == 0)
  //  {
  //    sum1 += userData->precalcedExponents[4*precalcedExponentsIdx + 0 + 2 * (idxInSolutionVector % 2)];
  //    sum2 += userData->precalcedExponents[4*precalcedExponentsIdx + 1 + 2 * (idxInSolutionVector % 2)];
  //  }
  //  /*else
  //  {
  //    sum1 += userData->precalcedExponents[4*precalcedExponentsIdx + 2];
  //    sum2 += userData->precalcedExponents[4*precalcedExponentsIdx + 3];
  //  }*/
  //}
  if (idxInSolutionVector % 2 == 0)
  {
    sum1 = userData->SUM1[netIdx];
    sum2 = userData->SUM2[netIdx];
  }
  else
  {
    sum1 = userData->SUM3[netIdx];
    sum2 = userData->SUM4[netIdx];
  }

  //TODO [issue 131]: we need alpha here, but parameters tuning will follow
  return /*userData->alpha * */
    (userData->precalcedExponents[2*idxInSolutionVector + 0] / sum1 - 
     userData->precalcedExponents[2*idxInSolutionVector + 1] / sum2) * userData->ci->netList[netIdx].weight;
}

void AddLogSumExpGradient(AppCtx* context, int nCoordinates, PetscScalar* coordinates, PetscScalar* grad)
{
//#pragma omp parallel for
  for (int idxInSolutionVector = 0; idxInSolutionVector < nCoordinates; idxInSolutionVector++)
  {
    int clusterIdx = context->solutionIdx2clusterIdxLUT[idxInSolutionVector];
    int tableSize = static_cast<int>(context->ci->tableOfAdjacentNets[clusterIdx].size());
    //consider all adjacent nets
    for (int j = 0; j < tableSize; ++j)
    {
      grad[idxInSolutionVector] += CalcNetLSEGradient(context, 
                                                      context->ci->tableOfAdjacentNets[clusterIdx][j],
                                                      idxInSolutionVector);
    }
  }
}

void AddSumOfDelaysGradient(AppCtx* context, PetscScalar* coordinates, PetscScalar* gradient)
{
  int nAdjacentNets;
  int netIdx;
  int netListSize = static_cast<int>(context->ci->netList.size());

  int nClusterCoordinates = 2*context->ci->mCurrentNumberOfClusters;

  for (int i = 0; i < nClusterCoordinates; i++)
  {
    int clusterIdx = context->solutionIdx2clusterIdxLUT[i];
    nAdjacentNets = static_cast<int>(context->ci->tableOfAdjacentNets[clusterIdx].size());
    
    for (int j = 0; j < nAdjacentNets; ++j)
    {//consider all adjacent nets
      netIdx = context->ci->tableOfAdjacentNets[clusterIdx][j];
      gradient[i] += 2 * context->ci->netList[netIdx].braces * 
                 CalcNetLSEGradient(context, netIdx, i);
    }
  }

  double ki = 0.0;
    
  for (int i = nClusterCoordinates; i < nClusterCoordinates + netListSize; i++)
  {
    ki = coordinates[i]; //WARNING: this index is correct
    netIdx = i - nClusterCoordinates;
    gradient[i] = context->DbufLbufDifferenceOfSquares + 
      (2 * context->ci->netList[netIdx].braces * context->Lbuf * (ki + 1) - 
       context->ci->netList[netIdx].braces * context->ci->netList[netIdx].braces) / 
       ((ki + 1) * (ki + 1));
  }
}

void PerformArgsAssignment(AppCtx* userData, PetscScalar* coordinates, double* arguments)
{
  int i;
  int nClusters    = userData->ci->mCurrentNumberOfClusters;
  int nTerminals   = userData->ci->terminalCells.size();
  int nPrimaryPins = userData->ci->primaryPins.size();
  int realClusterIdx;
  double x = 0.0;
  double y = 0.0;
  double alpha = userData->alpha;

  for (i = 0; i < nClusters; ++i)
  {
    realClusterIdx = userData->solutionIdx2clusterIdxLUT[2 * i];
    GetClusterCoordinates(realClusterIdx, coordinates, userData, x, y);
    arguments[4*i + 0] = (+x / alpha);
    arguments[4*i + 1] = (-x / alpha);
    arguments[4*i + 2] = (+y / alpha);
    arguments[4*i + 3] = (-y / alpha);
  }

  for (i = 0; i < nTerminals; ++i)
  {
    GetClusterCoordinates(i + SHIFT_NUMBER_FOR_TERMINALS, coordinates, userData, x, y);
    arguments[4*(i + nClusters) + 0] = (+x / alpha);
    arguments[4*(i + nClusters) + 1] = (-x / alpha);
    arguments[4*(i + nClusters) + 2] = (+y / alpha);
    arguments[4*(i + nClusters) + 3] = (-y / alpha);
  }

  for (i = 0; i < nPrimaryPins; ++i)
  {
    GetClusterCoordinates(i + SHIFT_NUMBER_FOR_PRIMARY_PINS, coordinates, userData, x, y);
    arguments[4*(i + nClusters + nTerminals) + 0] = (+x / alpha);
    arguments[4*(i + nClusters + nTerminals) + 1] = (-x / alpha);
    arguments[4*(i + nClusters + nTerminals) + 2] = (+y / alpha);
    arguments[4*(i + nClusters + nTerminals) + 3] = (-y / alpha);
  }
}

void PrecalcExponents(AppCtx* userData, PetscScalar* coordinates)
{
  int i;
  int nClusters    = userData->ci->mCurrentNumberOfClusters;
  int nTerminals   = userData->ci->terminalCells.size();
  int nPrimaryPins = userData->ci->primaryPins.size();

  int count = (nClusters + nTerminals + nPrimaryPins) * 4; 
  double* arguments = userData->argsForPrecalcedExponents;

  PerformArgsAssignment(userData, coordinates, arguments);

#ifdef __MKL_VML_H__
  int batchSize = userData->batchSize;
  for (i = 0; i < count / batchSize; ++i)
    vdExp(batchSize, &arguments[i * batchSize], &userData->precalcedExponents[i * batchSize]);
  vdExp(count % batchSize, &arguments[count - count % batchSize], &userData->precalcedExponents[count - count % batchSize]);
#else
  for (i = 0; i < count; ++i)
    userData->precalcedExponents[i] = exp(arguments[i]);
#endif
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
  
  scale = fabs(context->gradientBalance * meanCoordGradient / meanBuffGradient);

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
  double addValue = 0.0;
  SetGradientToZero((double*)gradient, nVariables);
  SetGradientToZero(context->gLSE, nVariables);
  SetGradientToZero(context->gSOD, nVariables);
  SetGradientToZero(context->gQS, nVariables);

  //timing
  timetype start = 0;
  timetype finish = 0;

#pragma omp parallel sections
  {
#pragma omp section
    {
      if (context->useLogSumExp)
      {
        start = GET_TIME_METHOD();
        PrecalcExponents(context, solution);
        finish = GET_TIME_METHOD();
        expTime += finish - start;

        start = GET_TIME_METHOD();
        addValue = LogSumExp(context, solution);
        finish = GET_TIME_METHOD();
        lseTime += finish - start;

        //ALERTFORMAT(("LSE = %f", addValue));
        *f += addValue;

        start = GET_TIME_METHOD();
        AddLogSumExpGradient(context, nClusterVariables, solution, context->gLSE);
        finish = GET_TIME_METHOD();
        lseGradTime += finish - start;
      }

      if (context->useSumOfDelays)
      {
        start = GET_TIME_METHOD();
        PrecalcExponents(context, solution);
        finish = GET_TIME_METHOD();
        expTime += finish - start;

        addValue = SumOfDelays(context, solution);
        //ALERTFORMAT(("SOD = %f", addValue));
        *f += addValue;
        AddSumOfDelaysGradient(context, solution, context->gSOD);
      }
    }

#pragma omp section
    {
      if (context->useQuadraticSpreading)
      {
        timetype start1 = GET_TIME_METHOD();
        addValue = context->muSpreading * SpreadingPenalty(context, solution);
        timetype finish1 = GET_TIME_METHOD();
        quadraticSpreading += finish1 - start1;
        
        //ALERTFORMAT(("SPR = %f", addValue));
        *f += addValue;

        start = GET_TIME_METHOD();
        AddSpreadingPenaltyGradient(context, solution, context->gQS);
        finish = GET_TIME_METHOD();
        quadraticSpreadingGradTime += finish - start;
      }

      if (context->useLRSpreading)
      {
        SpreadingPenalty(context, solution);
        addValue = Aux::ScalarProduct(context->muBinsPen, context->binsPenaltyValues, context->binGrid.nBins);
        //ALERTFORMAT(("LRS = %f", addValue));
        *f += addValue;
        AddSpreadingPenaltyGradient(context, solution, context->gQS); //FIXME: use proper gradient
      }
    }
  }

  if (context->useBorderPenalty)
  {
    addValue = context->muBorderPenalty * BorderPenalty(context, solution);
    //ALERTFORMAT(("BorderPenalty = %f", addValue));
    *f += addValue;
    AddBorderPenaltyGradient(context, solution, gradient);
  }
  //WRITELINE("");

  //normalize gradients
  if (context->useSumOfDelays)
    ScaleBufferGradients(nClusterVariables, nVariables, context);

  for (int i = 0; i < nVariables; i++)
  {
    gradient[i] += context->gLSE[i];
    gradient[i] += context->gSOD[i];
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
      context->binGrid.nBinRows, context->binGrid.nBinCols, context->ci->netList.size(), 
      (double*)solution, context->gLSE, context->gSOD, context->gQS, gradient, scaling, (HPlotter::WaitTime)waitTime);
  }

  info = VecRestoreArray(X, &solution); CHKERRQ(info);
  info = VecRestoreArray(G, &gradient); CHKERRQ(info);

  return 0;
}
