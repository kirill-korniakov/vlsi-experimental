#include "BorderPenalty.h"

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

void AddBPwithGrad(AppCtx* context, PetscScalar* solution, double*& f, PetscScalar* gradient )
{
  *f += context->muBorderPenalty * BorderPenalty(context, solution);
  //ALERTFORMAT(("BorderPenalty = %f", addValue));
  AddBorderPenaltyGradient(context, solution, gradient);
}