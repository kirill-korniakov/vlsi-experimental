/*  
 * multilevel_framework.cpp
 * this is a part of itlAnalyticalPlacer
 * Copyright (C) 2007, ITLab, Zhivoderov Artem
 * email: zhivoderov.a@gmail.com
 */

#include "..\include\multilevel_framework.h"
#include "..\include\output.h"
#include "math.h"
#include "tao.h"
#include <algorithm>

using namespace MultilevelFramework;

void PrecalculateNetListSizes(NetList& netList, int* netListSizes)
{
  int netListSize = static_cast<int>(netList.size());
  for (int i = 0; i < netListSize; i++)
  {
    netListSizes[i] = static_cast<int>(netList[i].clusterIdxs.size());
  }
}

void MultilevelFramework::SetInitialState(vector<Cluster>& clusters, Circuit& circuit, const int& nClusters)
{
  float* rndCoords = new float[2 * nClusters];
  GetFloatRandomNumbers(rndCoords, nClusters * 2, -1.0, 1.0);

  int idx = 0;
  for (int i = 0; i < static_cast<int>(clusters.size()); ++i)
  {
    if (clusters[i].isFake == false)
    {
      clusters[i].xCoord = circuit.width  / 2/* + 0.2*circuit.width  * rndCoords[2*idx+0]*/;
      clusters[i].yCoord = circuit.height / 2/* + 0.2*circuit.height * rndCoords[2*idx+1]*/;
      idx++;
    }
  }

  delete [] rndCoords;
}

void PrintTerminationReason(TaoTerminateReason reason)
{
  char message[256];

  switch(reason)
  {
    case 2:
      strcpy(message, "(residual of optimality conditions <= absolute tolerance)");
  	  break;
    case 3:
      strcpy(message, "(residual of optimality conditions / initial residual of optimality conditions <= relative tolerance)");
  	  break;
    case 4:
      strcpy(message, "(current trust region size <= trtol)");  
      break;
    case 5:
      strcpy(message, "(function value <= fmin)");  
      break;
    case -2:
  	  strcpy(message, "(its > maxits)");
      break;
    case -4:
  	  strcpy(message, "(numerical problems)");
      break;
    case -5:
      strcpy(message, "(number of function evaluations > maximum number of function evaluations)");
      break;
    case -6:
      strcpy(message, "(line search failure)");
      break;
    default:
      strcpy(message, "(unrecognized error)");  
      break;
  }
  PetscPrintf(MPI_COMM_WORLD,"reason = %d ", reason);
  PetscPrintf(MPI_COMM_WORLD, message);
  PetscPrintf(MPI_COMM_WORLD, "\n");
}

void UpdateNetWeights(double* netWeights, NetList& netList)
{
  int netListSize = static_cast<int>(netList.size());
  for (int i = 0; i < netListSize; ++i)
  {
  }
}

void MultilevelFramework::Merge(vector<int>& a, vector<int>& b, int result[])
{
  long size1 = static_cast<int>(a.size());
  long size2 = static_cast<int>(b.size());
  // текущая позиция чтения из первой последовательности
  long pos1=0;

  // текущая позиция чтения из второй последовательности
  long pos2=0;

  // текущая позиция записи в temp
  long pos3=0;

  // идет слияние, пока есть хоть один элемент в каждой последовательности
  while (pos1 < size1 && pos2 < size2) {
    if (a[pos1] < b[pos2])
      result[pos3++] = a[pos1++];
    else
      result[pos3++] = b[pos2++];
  }

  // одна последовательность закончилась - 
  // копировать остаток другой в конец буфера 
  while (pos2 < size2)   // пока вторая последовательность непуста 
    result[pos3++] = b[pos2++];
  while (pos1 < size1)  // пока первая последовательность непуста
    result[pos3++] = a[pos1++];
}

bool PredicateClusterDataGreater(ClusterData elem1, ClusterData elem2)
{
  return elem1.score > elem2.score;
}

bool PredicateNetListLess(NetWithWeight elem1, NetWithWeight elem2)
{
  int size1 = static_cast<int>(elem1.clusterIdxs.size());
  int size2 = static_cast<int>(elem2.clusterIdxs.size());
  
  if (size1 < size2)
    return true;
  else 
    if (size1 > size2)
      return false;
  
  for (int i = 0; i < size2; ++i)
  {
    if (elem1.clusterIdxs[i] < elem2.clusterIdxs[i])
      return true;
    else
      if (elem1.clusterIdxs[i] > elem2.clusterIdxs[i])
        return false;
  }

  // сортировка по весам в обратном порядке
  // при применении функции unique автоматически будет выбран первый нет
  // при таком способе сортировки у этого нета будет максимальный вес
  if (elem1.weight > elem2.weight)
    return true;
  else
    if (elem1.weight < elem2.weight)
      return false;
  
  return false;
}

bool IsEqualNetListBinaryPredicate(NetWithWeight elem1, NetWithWeight elem2)
{
  int size1 = static_cast<int>(elem1.clusterIdxs.size());
  int size2 = static_cast<int>(elem2.clusterIdxs.size());
  
  if (size1 != size2)
    return false;
  
  for (int i = 0; i < size2; ++i)
  {
    if (elem1.clusterIdxs[i] != elem2.clusterIdxs[i])
      return false;
  }
  
  return true;
}

void MultilevelFramework::UpdateCoords(Circuit& circuit, vector<Cluster>& clusters)
{
  int cellIdx;

  for (int i = 0; i < static_cast<int>(clusters.size()); ++i)
  {
    if (clusters[i].isFake == true)
      continue;
    for (int j = 0; j < static_cast<int>(clusters[i].cellIdxs.size()); ++j)
    {
      cellIdx = clusters[i].cellIdxs[j];
      circuit.placement[cellIdx].xCoord = clusters[i].xCoord;
      circuit.placement[cellIdx].yCoord = clusters[i].yCoord;
    }
  }
}

MULTIPLACER_ERROR MultilevelFramework::Interpolation(Circuit& circuit, vector<Cluster>& clusters,
                                                       vector<ConnectionsList>& currTableOfConnections, NetList& netList)
{
  // todo : consider terminals during interpolation
  vector<ClusterData> clustersData(clusters.size());
  list<ClusterData> clustersDataList;
  list<ClusterData>::iterator clustersDataIterator;
  bool* isCPoint = new bool[clusters.size()];  // C-point is a cluster which location is fixed during interpolation
  double part = 0.5;  // part of clusters to be initially fixed during interpolation (part of C-points)
  int currClusterIdx;
  int neighborIdx;
  int netIdx;
  int nNodes = circuit.nNodes;
  vector<int> currCPoints;
  double sumX;
  double sumY;
  double area;

  int* netListSizes = new int[netList.size()];//todo: delete this memory
  PrecalculateNetListSizes(netList, netListSizes);

  for (int i = 0; i < static_cast<int>(clusters.size()); ++i)
  {
    clustersData[i].clusterIdx = i;
    clustersData[i].score      = 0.0;
    
    if (clusters[i].isFake == true)
      continue;
    
    CalculateScore(i, currTableOfConnections, netList, netListSizes,
                   NULL, clusters, clustersData[i].score, clustersData[i].bestNeighborIdx, false, AffinityInterp, nNodes);
    isCPoint[i] = false;
    //cout << clustersData[i].score << endl;
  }
  delete[] netListSizes;
  sort(clustersData.begin(), clustersData.end(), PredicateClusterDataGreater);
  
  for (int i = 0; i < static_cast<int>(clusters.size()); ++i)
  {
    if (clustersData[i].score >= 1.5)
      clustersDataList.push_back(clustersData[i]);
    else
      break;
  }
  clustersDataIterator = clustersDataList.begin();
  for (int i = 0; i < static_cast<int>(clustersDataList.size() * part); ++i)
  {
    isCPoint[clustersDataIterator->clusterIdx] = true;
    ++clustersDataIterator;
  }
  
  for (int i = static_cast<int>(clustersDataList.size() * part); i < static_cast<int>(clustersDataList.size()); ++i)
  {
    currClusterIdx = clustersDataIterator->clusterIdx;
    currCPoints.clear();
    for (int j = 0; j < static_cast<int>(currTableOfConnections[currClusterIdx].size()); ++j)
    {
      netIdx = currTableOfConnections[currClusterIdx][j];
      for (int k = 0; k < static_cast<int>(netList[netIdx].clusterIdxs.size()); ++k)
      {
        neighborIdx = netList[netIdx].clusterIdxs[k];
        if (!IsNotTerminal(neighborIdx) || isCPoint[neighborIdx] == false)
          continue;
        // remember all placed (fixed) neighbors of current cluster
        currCPoints.push_back(neighborIdx);
      }
    }
    isCPoint[currClusterIdx] = true;
    if (currCPoints.size() == 0)
      continue;
    sumX = sumY = area = 0.0;
    for (int j = 0; j < static_cast<int>(currCPoints.size()); ++j)
    {
      sumX += clusters[currCPoints[j]].area * clusters[currCPoints[j]].xCoord;
      sumY += clusters[currCPoints[j]].area * clusters[currCPoints[j]].yCoord;
      area += clusters[currCPoints[j]].area;
    }
    clusters[currClusterIdx].xCoord = sumX / area;
    clusters[currClusterIdx].yCoord = sumY / area;
    ++clustersDataIterator;
  }
  
  UpdateCoords(circuit, clusters);

  delete[] isCPoint;

  return OK;
}

int CalcMaxAffectedArea(double potentialSize, double binSize)
{
  return 1+2*static_cast<int>(ceil(potentialSize/binSize));
}

void MultilevelFramework::InitializeOptimizationProblemParameters(AppCtx &user, Circuit& circuit, 
                                                                  vector<Cluster>& clusters, NetList& netList, 
                                                                  int nClusters, PetscInt** lookUpTable, 
                                                                  vector<ConnectionsList>& currTableOfConnections)
{
  LogEnter("MultilevelFramework::InitializeOptimizationProblemParameters");
  user.circuit = &circuit;
  user.clusters = &clusters;
  user.netList = &netList;
  user.nClusters = nClusters;
  user.lookUpTable = *lookUpTable;
  user.currTableOfConnections = &currTableOfConnections;

  // Calculate current bin grid
  CalcBinGrid(circuit, clusters, nClusters, 
    user.nBinRows, user.nBinCols, 
    user.binWidth, user.binHeight);

  //todo: probably we can choose this parameter better
  //user.alpha = 25;
  user.alpha = user.binWidth / 2;

  // todo: correct this potential radius calculation
  double potentialRatio = 2.1; // must be greater than 0.5
  user.potentialRadiusX = potentialRatio*user.binWidth;
  user.potentialRadiusY = potentialRatio*user.binHeight;
  cout << "Potential radius X: " << user.potentialRadiusX 
       << "\tPotential radius Y: " << user.potentialRadiusY << endl;

  // todo: check if circuit width and height come here initialized
  // todo: embed max
  int maxAffectedRows = CalcMaxAffectedArea(user.potentialRadiusY, user.binHeight);
  int maxAffectedCols = CalcMaxAffectedArea(user.potentialRadiusX, user.binWidth);
  cout << "maxAffectedRows " << maxAffectedRows << endl;
  cout << "maxAffectedCols " << maxAffectedCols << endl;
  user.clusterPotentialOverBins = new double*[maxAffectedRows];
  for (int i=0; i<maxAffectedRows; i++)
  {
    user.clusterPotentialOverBins[i] = new double[maxAffectedCols];
  }

  user.binPenalties = new Bin*[user.nBinRows];
  for (int j = 0; j < user.nBinRows; ++j)
  {
    user.binPenalties[j] = new Bin[user.nBinCols];
  }
  for (int i = 0; i < user.nBinRows; ++i)
  {
    for (int j = 0; j < user.nBinCols; ++j)
    {
      user.binPenalties[i][j].xCoord = (j + 0.5) * user.binWidth;
      user.binPenalties[i][j].yCoord = (i + 0.5) * user.binHeight;
      //printf("%.1f,%.1f\t", user.binPenalties[i][j].xCoord, user.binPenalties[i][j].yCoord);
    }
    //cout << endl;
  }
  user.totalCellArea = 0.0;
  for (int i = 0; i < static_cast<int>((*(user.clusters)).size()); ++i)
  {
    if ((*user.clusters)[i].isFake == false)
    {
      user.totalCellArea += (*user.clusters)[i].area;
    }
  }
  user.meanBinArea = user.totalCellArea / user.nBinCols / user.nBinRows;
  
  /*ComputeNetWeights(circuit, clusters, netList, netWeights);*/
  user.netWeights = circuit.netWeights;

  LogExit("MultilevelFramework::InitializeOptimizationProblemParameters");
}


// todo: check if all the memory released
void MultilevelFramework::DeinitializeOptimizationProblemParameters(AppCtx &user)
{
  LogEnter("MultilevelFramework::DeinitializeOptimizationProblemParameters");
  for (int j = 0; j < user.nBinRows; ++j)
  {
    delete []user.binPenalties[j];
  }
  delete []user.binPenalties;

  int maxAffectedRows = CalcMaxAffectedArea(user.potentialRadiusY, user.binHeight);
  for (int i=0; i<maxAffectedRows; i++)
  {
    delete [] user.clusterPotentialOverBins[i];
  }
  delete [] user.clusterPotentialOverBins;
  LogExit("MultilevelFramework::DeinitializeOptimizationProblemParameters");
}

int AnalyticalObjectiveAndGradient(TAO_APPLICATION taoapp, Vec X, double *f, Vec G, void *ptr)
{
  LogEnter("AnalyticalObjectiveAndGradient");
  AppCtx *user = (AppCtx*)ptr;
  int    info;
  PetscScalar *x, *g;

  // Get pointers to vector data
  info = VecGetArray(X, &x); CHKERRQ(info);
  info = VecGetArray(G, &g); CHKERRQ(info);
  
  for (int i = 0; i < user->nClusters; ++i)
  {
    g[2*i+0] = 0;
    g[2*i+1] = 0;
  }

  *f = LogSumExpForClusters(x, user) + user->mu * CalcPenalty(x, user);
  CalcLogSumExpForClustersGrad(x, g, user);
  CalcPenaltyGrad(x, g, user);

  LogExit("AnalyticalObjectiveAndGradient");
  return 0;
}

MULTIPLACER_ERROR MultilevelFramework::ReadClustersCoords(Vec x, int nClusters, int* idxs, PetscScalar* initValues,
                       vector<Cluster> &clusters)
{
  VecGetValues(x, PetscInt(2*nClusters), idxs, initValues);
  Trace("solution read", 1);
  int idx = 0;
  for (int i = 0; i < static_cast<int>(clusters.size()); ++i)
  {
    if (clusters[i].isFake == false)
    {
      clusters[i].xCoord = initValues[2*idx+0];
      clusters[i].yCoord = initValues[2*idx+1];
      idx++;
    }
  }	
  
  return OK;
}

MULTIPLACER_ERROR MultilevelFramework::Relaxation(Circuit& circuit, vector<Cluster>& clusters, NetList& netList,
                                                  vector<ConnectionsList>& currTableOfConnections,
                                                  int& nInnerIters, int& nOuterIters)
{
  LogEnter("MultilevelFramework::Relaxation");
  int nClusters = static_cast<int>(clusters.size());

  for (int i = 0; i < static_cast<int>(clusters.size()); ++i)
  {
    if (clusters[i].isFake == true)
      --nClusters;
  }

  PetscInt *lookUpTable = new PetscInt[clusters.size()]; //todo: move this memory work from function
  int j = 0;  
  for(int i=0; i < static_cast<int>(clusters.size());i++)
  {
    if (clusters[i].isFake == false)
    {
      lookUpTable[i] = j;
      ++j;
    }
    else
      lookUpTable[i] = -1;
  }

  int             info;       // used to check for functions returning nonzeros
  Vec             x;          // solution vector
  TAO_SOLVER      tao;        // TAO_SOLVER solver context
  TAO_APPLICATION taoapp;     // TAO application context
  TaoTerminateReason reason;  // terminate reason
  AppCtx          user;       // user-defined application context
  PetscScalar *solution;
  double discrepancy;
  double currentWL;
  double* netWeights = NULL;
  
  /* Initialize optimization problem parameters */
  InitializeOptimizationProblemParameters(user, circuit, clusters, netList, nClusters, 
                                          &lookUpTable, currTableOfConnections);

  /* Allocate vectors for the solution and gradient */
  info = VecCreateSeq(PETSC_COMM_SELF, nClusters * 2, &x); CHKERRQ(info);

  /* The TAO code begins here */
  /* Create TAO solver with desired solution method */
  //info = TaoCreate(PETSC_COMM_SELF, "tao_blmvm", &tao); CHKERRQ(info);
  info = TaoCreate(PETSC_COMM_SELF, "tao_cg", &tao); CHKERRQ(info);
  info = TaoApplicationCreate(PETSC_COMM_SELF,&taoapp); CHKERRQ(info);

  /* Set solution vec and an initial guess */
  int idx = 0;  
  PetscScalar* initValues = new PetscScalar[2 * nClusters];
  for (int i = 0; i < static_cast<int>(clusters.size()); ++i)
  {
    if (clusters[i].isFake == false)
    {
      initValues[2*idx+0] = clusters[i].xCoord;
      initValues[2*idx+1] = clusters[i].yCoord;
      idx++;
    }
  }

  int* idxs = new int[2 * nClusters];
  for (int i = 0; i < 2 * nClusters; ++i)
  {
    idxs[i] = i;
  }

  VecSetValues(x, 2 * nClusters, idxs, initValues, INSERT_VALUES);

  info = TaoAppSetInitialSolutionVec(taoapp, x); CHKERRQ(info); 

  /* Set routines for function and gradient evaluation */
  info = TaoAppSetObjectiveAndGradientRoutine(taoapp, AnalyticalObjectiveAndGradient, (void *)&user); 
  CHKERRQ(info);

  //todo: we may play with number of iterations
  //static int nIters = 32; //todo: find appropriate value
  //static int nInnerIters = 16;
  //if (nClusters == circuit.nNodes) 
  //{
  //  nIters = 32;
  //  nIters = 16;
  //}
  nOuterIters /= 2;
  nInnerIters *= 2;
  info = TaoSetMaximumIterates(tao, nInnerIters);
  //CHKERRQ(info);
  //todo: choose correct values
  double fatol = 1.0e-12;
  double frtol = 1.0e-12;
  double catol = 0.000001;
  double crtol = 0.000001;
  info = TaoSetTolerances(tao, fatol, frtol, catol, crtol);
  CHKERRQ(info);

  /* Check for TAO command line options */
  info = TaoSetOptions(taoapp, tao); CHKERRQ(info);

  double targetDisc = 2.0;

  /* Get the mu value */
  VecGetArray(x, &solution);
  CalcMu0(solution, &user);
  user.mu = user.mu0/* = 0.000001*/;//fixme: delete this dummy value
  printf("mu0 = %.20f\n", user.mu0);
  //PetscPrintf(MPI_COMM_WORLD, "mu = %.16f\n", user.mu);
  
  VecRestoreArray(x, &solution);

  int iter = 0;
  do
  {
    cout << "CGM iter# " << iter++ << endl;
    // Tao solve the application
    info = TaoSolveApplication(taoapp, tao); CHKERRQ(info);
    //TaoView(tao);/*LineSearch*/
    
    /* Get termination information */
    int iterate;
    double f;
    double gnorm;
    double cnorm;
    double xdiff;
    info = TaoGetSolutionStatus(tao, &iterate, &f, &gnorm, &cnorm, &xdiff, &reason); CHKERRQ(info);

    PrintTerminationReason(reason);
    Trace("termination reason printed", 1);

    VecGetArray(x, &solution);
    PetscPrintf(MPI_COMM_WORLD, "mu = %.20f\t", user.mu);
    //fixme: uncomment
    user.mu *= 2;
    discrepancy = GetDiscrepancy(solution, &user);
    cout << "discrepancy = " << discrepancy << endl;
    // read solution vector to array of clusters coordinates
    if (iterate > 1)
    {
      ReadClustersCoords(x, nClusters, idxs, initValues, clusters);
      UpdateCoords(circuit, clusters);
      currentWL = cf_recalc_all(UPDATE_NETS_WLS, circuit.nNets, circuit.nets, circuit.placement);
      cout << "currWL = " << currentWL << endl;
      
      char str[128];
      char *pVal;
      strcpy(str, gOptions.benchmarkName);
      pVal = strrchr(str, '\\');
      if (pVal) strcpy(str, ++pVal);
      pVal = strrchr(str, '.');
      if (pVal) pVal[0] = '\0';
      strcat(str, "_CGM_.pl");
      PrintToPL(str, circuit, 0, 0, user.nBinRows, user.nBinCols);
      //PrintToPL(strcat("CGM_", gOptions.benchmarkName), circuit, 0, 0, user.nBinRows, user.nBinCols);
    }

    /*if (iter % 30 == 0)
    {
      CalcMu0(solution, &user);
      user.mu0 = 1/user.mu0;
      user.mu = user.mu0;
      PetscPrintf(MPI_COMM_WORLD, "mu0 recalculated\n");
    }*/

    //VecRestoreArray(x, &solution);
  } while(discrepancy > targetDisc && iter < nOuterIters);//todo: make stopping criteria stronger, like in APlace
  // we have to stop ONLY if discrepancy is small enough

  /* Free TAO data structures */
  info = TaoDestroy(tao); CHKERRQ(info);
  info = TaoAppDestroy(taoapp); CHKERRQ(info);

  /* Free PETSc data structures */
  info = VecDestroy(x); CHKERRQ(info);

  Trace("TAO&PETSc data structures destroyed", 1);

  delete[] lookUpTable;
  delete[] initValues;
  delete[] idxs;
  delete[] netWeights;

  DeinitializeOptimizationProblemParameters(user);
  Trace("todo: after deinit", 1);
  LogExit("MultilevelFramework::Relaxation");
  Trace("todo: after log print", 1);

  return OK;
}

void MultilevelFramework::UnclusterLevelUp(vector<Cluster>& clusters, vector<ConnectionsList>& currTableOfConnections,
                                             NetList& netList, ClusteringLogIterator clusteringLogIterator, const int& nNodes)
{
  list<ClusteredNodes>::reverse_iterator iter = clusteringLogIterator->rbegin();
  
  for (; iter != clusteringLogIterator->rend(); ++iter)
  {
    clusters[iter->firstClusterIdx].cellIdxs.resize(iter->size);
    clusters[iter->firstClusterIdx].area -= clusters[iter->secondClusterIdx].area;
    clusters[iter->secondClusterIdx].isFake = false;
    clusters[iter->secondClusterIdx].xCoord = clusters[iter->firstClusterIdx].xCoord;
    clusters[iter->secondClusterIdx].yCoord = clusters[iter->firstClusterIdx].yCoord;
  }
  
  CreateTableOfConnections(clusters, currTableOfConnections, netList, nNodes);
}

MULTIPLACER_ERROR MultilevelFramework::Clusterize(Circuit& circuit, vector<Cluster>& clusters, NetList& netList,
                                                    list<NetList>& netLevels, affinityFunc Affinity, 
                                                    list<ClusteringInfoAtEachLevel>& clusteringLog, int& currNClusters)
{  
  int i = 0;
  
  int targetnClusters;
  int netIdx = 0;
  int currClusterIdx = 0;
  int nClusters = 0;
  double totalCellArea = 0.0;
  double targetClusterArea;
  vector<ConnectionsList> currTableOfConnections(clusters.size());
  vector<ClusterData> clustersData(clusters.size());
  list<ClusterData> clustersDataList;
  list<ClusterData>::iterator clustersDataIterator;
  ClusteringInfoAtEachLevel clusteringLogAtEachLevel;  // инфа, к-ая сохраняется по ходу кластеризации на каждом уровне
                                                       // и затем кладется в clusteringLog
  double* netAreas;
  double currNetArea = 0.0;
  //double min, max;

  int bestNeighborIdx;
  int mergeCount = 0;
  ClusteredNodes buffer;

  int nNodes = circuit.nNodes;

  nClusters = static_cast<int>(clusters.size());
  netAreas = new double[netList.size()];
  
  for (int i = 0; i < circuit.nNodes; ++i)
  {
    totalCellArea += clusters[i].area;
  }

  while (nClusters > FINAL_NCLUSTERS)
  {
    targetnClusters = static_cast<int>(nClusters / CLUSTER_RATIO);
    targetClusterArea   = (totalCellArea / targetnClusters) * CLUSTERS_AREA_TOLERANCE;
    
    //targetnClusters = min(targetnClusters, FINAL_NCLUSTERS);

    CreateTableOfConnections(clusters, currTableOfConnections, netList, circuit.nNodes);

    /*for (int i = 0; i < static_cast<int>(netList.size()); ++i)
    { 
      currNetArea = 0.0;
      for (int j = 0; j < static_cast<int>(netList[i].size()); ++j)
      {
        currNetArea += clusters[netList[i][j]].area;
      }
      netAreas[i] = currNetArea;
    }*/
    int* netListSizes = new int[netList.size()];//todo: delete this memory
    PrecalculateNetListSizes(netList, netListSizes);
    
    for (int i = 0; i < static_cast<int>(clusters.size()); ++i)
    {
      clustersData[i].clusterIdx = i;
      clustersData[i].score      = 0.0;
      
      if (clusters[i].isFake == true)
        continue;
      
      CalculateScore(i, currTableOfConnections, netList, netListSizes,
                     netAreas, clusters, clustersData[i].score, clustersData[i].bestNeighborIdx,
                     DONT_MARK_NEIGHBORS_INVALID, Affinity, nNodes);
      if (i % 10000 == 0)
        cout << "i = " << i << endl;
    }
    sort(clustersData.begin(), clustersData.end(), PredicateClusterDataGreater);
    
    clustersDataList.clear();
    clusteringLogAtEachLevel.clear();
    for (int i = 0; i < static_cast<int>(clusters.size()); ++i)
    {
      if (clustersData[i].score != 0)
        clustersDataList.push_back(clustersData[i]);
    }
    
    mergeCount = 0;
    while (clustersDataList.size() && nClusters - mergeCount > targetnClusters)
    {
      clustersDataIterator = clustersDataList.begin();
      currClusterIdx  = clustersDataIterator->clusterIdx;
      bestNeighborIdx = clustersDataIterator->bestNeighborIdx;
      
      if (!IsNotTerminal(currClusterIdx) || !IsNotTerminal(bestNeighborIdx))
      {
        clustersDataList.pop_front();
        continue;
      }

      if ( clusters[currClusterIdx].isValid && 
          (clusters[currClusterIdx].area + clusters[bestNeighborIdx].area <= targetClusterArea))
      {
        buffer.firstClusterIdx  = currClusterIdx;
        buffer.secondClusterIdx = bestNeighborIdx;
        buffer.size             = clusters[currClusterIdx].cellIdxs.size();
        
        clusteringLogAtEachLevel.push_back(buffer);
        
        MergeClusters(currClusterIdx, bestNeighborIdx, clusters, netList, currTableOfConnections, netAreas);
        PrecalculateNetListSizes(netList, netListSizes);
        ++mergeCount;
        
        ++clustersDataIterator;
        for (; clustersDataIterator != clustersDataList.end(); ++clustersDataIterator)
        {
          if (clustersDataIterator->clusterIdx == bestNeighborIdx)
          {
            clustersDataList.erase(clustersDataIterator);
            break;
          }
        }
      }
      else if (!clusters[currClusterIdx].isValid)
      {
        CalculateScore(currClusterIdx, currTableOfConnections, netList, netListSizes, 
                       netAreas, clusters, clustersDataIterator->score, 
                       clustersDataIterator->bestNeighborIdx,
                       DONT_MARK_NEIGHBORS_INVALID, Affinity, nNodes);
        InsertInHeap(clustersDataList);
        clusters[currClusterIdx].isValid = true;
        continue;
      }
      else
      {
        clustersDataList.pop_front();
        continue;
      }
      
      clustersDataIterator = clustersDataList.begin();
      CalculateScore(currClusterIdx, currTableOfConnections, netList, netListSizes,
                     netAreas, clusters, clustersDataIterator->score, 
                     clustersDataIterator->bestNeighborIdx,
                     MARK_NEIGHBORS_INVALID, Affinity, nNodes);
      // insert the new object into the heap
      InsertInHeap(clustersDataList);
    }
    
    //clusters.resize(clustersDataList.size());
    //clusters.clear();
    /*vector<Cluster> buffer;
    clustersDataIterator = clustersDataList.begin();
    for (int i = 0; i < static_cast<int>(clusters.size()); ++i)
    {
      if (clusters[i].isFake == false)
      {
        buffer.push_back(clusters[i]);
      }
      else
        clusters[i].isFake = false;
      clusters[i].isValid = true;
    }
    clusters.clear();
    clusters.resize(buffer.size());
    copy(buffer.begin(), buffer.end(), clusters.begin());*/
    int numOfFakes = 0;
    for (int i = 0; i < static_cast<int>(clusters.size()); ++i)
    {
      clusters[i].isValid = true;
      if (clusters[i].isFake == true)
        ++numOfFakes;
    }
    
    delete[] netListSizes;

    // update netList again
    NetList::iterator netListIterator;
    sort(netList.begin(), netList.end(), PredicateNetListLess);
    netListIterator = unique(netList.begin(), netList.end(), IsEqualNetListBinaryPredicate);
    netList.resize(netListIterator - netList.begin());

    netListIterator = netList.begin();
    while (netListIterator->clusterIdxs.size() < 2) ++netListIterator;
    netList.erase(netList.begin(), netListIterator);
    
    netLevels.push_back(netList);
    clusteringLog.push_back(clusteringLogAtEachLevel);

    nClusters = static_cast<int>(clusters.size() - numOfFakes);
    currNClusters = nClusters;
    
    cout << "Currently there are " << nClusters << " clusters\n";
    /*cout << "targetClusterArea = " << targetClusterArea << "\n";
    min = totalCellArea, max = 0.0;
    
    for (int i = 0; i < static_cast<int>(clusters.size()); ++i)
    {
      if (clusters[i].isFake == false)
      {
        if (min > clusters[i].area)
          min = clusters[i].area;
        if (max < clusters[i].area)
          max = clusters[i].area;
      }
    }
    cout << "min area is " << min << endl;
    cout << "max area is " << max << endl << endl;*/
  }
  delete[] netAreas;
  
  //clustersDataIterator = clustersDataList.begin();
  
  
  /*int denominator = 1000;
  int size = (int)((max - min) / denominator) + 1;
  int* stat = new int[size];
  for (int i = 0; i < size; ++i)
  {
    stat[i] = 0;
  }
  for (int i = 0; i < static_cast<int>(clusters.size()); ++i)
  {
    if (clusters[i].isFake == false)
    {
      int idx = (int)(clusters[i].area - min) / denominator;
      if (idx < 0 || idx >= size)
        continue;
      stat[idx]++;
    }
  }
  FILE* f = fopen("stat.xls", "w");
  char str[128];
  for (int i = 0; i < size; ++i)
  {
    sprintf(str, "%d\t%d\n", (int)min(denominator * (i + 1), (int)max), stat[i]);
    fputs(str, f);
  }
  fclose(f);
  delete[] stat;*/
  
  return OK;
}

void MultilevelFramework::InitializeDataStructures(Circuit& circuit, vector<Cluster>& clusters, 
                                                   NetList& netList, const int& nNodes, int& nClusters)
{
  int cellIdx;

  for (int i = 0; i < circuit.nNodes; ++i)
  {
    clusters[i].cellIdxs.push_back(i);
    clusters[i].area = circuit.nodes[i].width * circuit.nodes[i].height;
  }
  
  for (int i = 0; i < circuit.nNets; ++i)
  {
    if (circuit.nets[i].numOfPins > 10000)
      continue;
    
    for (int j = 0; j < circuit.nets[i].numOfPins; ++j)
    {
      cellIdx = circuit.nets[i].arrPins[j].cellIdx;
      //if (IsNotTerminal(cellIdx))
      netList[i].clusterIdxs.push_back(cellIdx);
    }
  }
  if (gOptions.isLEFDEFinput)
  {
    for (int i = 0; i < circuit.nNets; ++i)
    {
      netList[i].weight = circuit.netWeights[i];
    }
  }

  for (int i = 0; i < static_cast<int>(netList.size()); ++i)
  {
    if (netList[i].clusterIdxs.size() == 1)
    {
      netList[i] = netList.back();
      netList.pop_back();
      --i;
    }
  }
  nClusters = nNodes;
}

MULTIPLACER_ERROR MultilevelFramework::InsertInHeap(list<ClusterData>& clustersDataList)
{
  list<ClusterData>::iterator clustersDataIterator;
  list<ClusterData>::iterator iteratorEnd = clustersDataList.end();
  clustersDataIterator = clustersDataList.begin();
  double initScore = clustersDataIterator->score;
  ++clustersDataIterator;
  for (; clustersDataIterator != iteratorEnd; ++clustersDataIterator)
  {
    if (clustersDataIterator->score <= initScore)
      break;
  }
  clustersDataList.insert(clustersDataIterator, (*clustersDataList.begin()));
  clustersDataList.pop_front();
  
  return OK;
}

MULTIPLACER_ERROR MultilevelFramework::MergeClusters(const int& firstClusterIdx, const int& secondClusterIdx, 
                                                            vector<Cluster>& clusters, NetList& netList,
                                                            vector<ConnectionsList>& currTableOfConnections, 
                                                            double* netAreas)
// the result is stored in firstClusterIdx
{
  ConnectionsList tempVector(currTableOfConnections[firstClusterIdx].size() + 
                             currTableOfConnections[secondClusterIdx].size());
  ConnectionsList::iterator newEnd;
  
  merge (currTableOfConnections[firstClusterIdx].begin(), currTableOfConnections[firstClusterIdx].end(), 
         currTableOfConnections[secondClusterIdx].begin(), currTableOfConnections[secondClusterIdx].end(),
         tempVector.begin());
  newEnd = unique(tempVector.begin(), tempVector.end());
  currTableOfConnections[firstClusterIdx].resize(newEnd - tempVector.begin());
  copy(tempVector.begin(), newEnd, currTableOfConnections[firstClusterIdx].begin());
  
  clusters[firstClusterIdx].area += clusters[secondClusterIdx].area;
  
  tempVector.resize(clusters[firstClusterIdx].cellIdxs.size() + clusters[secondClusterIdx].cellIdxs.size());
  copy(clusters[firstClusterIdx].cellIdxs.begin(), clusters[firstClusterIdx].cellIdxs.end(), tempVector.begin());
  copy(clusters[secondClusterIdx].cellIdxs.begin(), clusters[secondClusterIdx].cellIdxs.end(), 
       tempVector.begin() + clusters[firstClusterIdx].cellIdxs.size());
  clusters[firstClusterIdx].cellIdxs.resize(tempVector.size());
  copy(tempVector.begin(), tempVector.end(), clusters[firstClusterIdx].cellIdxs.begin());
  
  //update netList
  int netIdx = 0;
  vector<int>::iterator Iter;
  for (int j = 0; j < static_cast<int>(currTableOfConnections[secondClusterIdx].size()); ++j)
  {
    netIdx = currTableOfConnections[secondClusterIdx][j];
    if (netList[netIdx].clusterIdxs.size() == 0)
      continue;
    Iter = find(netList[netIdx].clusterIdxs.begin(), netList[netIdx].clusterIdxs.end(), secondClusterIdx);
    (*Iter) = firstClusterIdx;
    sort(netList[netIdx].clusterIdxs.begin(), netList[netIdx].clusterIdxs.end());
    Iter = unique(netList[netIdx].clusterIdxs.begin(), netList[netIdx].clusterIdxs.end());
    netList[netIdx].clusterIdxs.resize(Iter - netList[netIdx].clusterIdxs.begin());
    if (netList[netIdx].clusterIdxs.size() == 2 && (netList[netIdx].clusterIdxs[0] - netList[netIdx].clusterIdxs[1] == 0))
    {
      netList[netIdx].clusterIdxs.clear();
      netAreas[netIdx] = 0;
    }
  }
  /*double currNetArea = 0.0;
  for (int i = 0; i < static_cast<int>(currTableOfConnections[firstClusterIdx].size()); ++i)
  {
    netIdx = currTableOfConnections[firstClusterIdx][i];
    if (netList[netIdx].size() == 0)
      continue;
    currNetArea = 0.0;
    for (int j = 0; j < static_cast<int>(netList[netIdx].size()); ++j)
    {
      currNetArea += clusters[netList[netIdx][j]].area;
    }
    netAreas[netIdx] = currNetArea;
  }*/
  
  clusters[firstClusterIdx].xCoord = (clusters[firstClusterIdx ].xCoord * clusters[firstClusterIdx ].area + 
                                      clusters[secondClusterIdx].xCoord * clusters[secondClusterIdx].area) / 
                                     (clusters[firstClusterIdx ].area + clusters[secondClusterIdx].area);
  clusters[firstClusterIdx].yCoord = (clusters[firstClusterIdx ].yCoord * clusters[firstClusterIdx ].area + 
                                      clusters[secondClusterIdx].yCoord * clusters[secondClusterIdx].area) / 
                                     (clusters[firstClusterIdx ].area + clusters[secondClusterIdx].area);

  clusters[secondClusterIdx].isFake = true;
  
  return OK;
}

MULTIPLACER_ERROR MultilevelFramework::AffinityInterp(const int& firstClusterIdx, const int& secondClusterIdx, 
                                                             vector<Cluster>& clusters, NetList& netList, int* netListSizes,
                                                             vector<ConnectionsList>& currTableOfConnections, 
                                                             double* netAreas, double& result)
{
  result = 0.0;
  
  vector<int> commonNetsIdxs;
  int netIdx = 0;
  int theSize = static_cast<int>(currTableOfConnections[firstClusterIdx].size() + currTableOfConnections[secondClusterIdx].size());
  int* tmpIdxsInt = new int[theSize];
  
  // looking for common nets for these clusters:
  Merge(currTableOfConnections[firstClusterIdx], currTableOfConnections[secondClusterIdx], tmpIdxsInt);
  for (int i = 0; i < theSize - 1; ++i)
  {
    if (tmpIdxsInt[i] == tmpIdxsInt[i+1] && netList[tmpIdxsInt[i]].clusterIdxs.size() > 1)
      commonNetsIdxs.push_back(tmpIdxsInt[++i]);
  }
  
  for (int i = 0; i < static_cast<int>(commonNetsIdxs.size()); ++i)
  {
    netIdx = commonNetsIdxs[i];
    result += 1 / ((netList[netIdx].clusterIdxs.size() - 1));
  }
  
  delete[] tmpIdxsInt;
  return OK;
}

MULTIPLACER_ERROR MultilevelFramework::CalculateScore(const int& currClusterIdx,
                                                        vector<ConnectionsList>& currTableOfConnections, 
                                                        NetList& netList, int* netListSizes,
                                                        double* netAreas, vector<Cluster>& clusters, double& score,
                                                        int& bestNeighborIdx, bool isToMark,
                                                        affinityFunc Affinity, int nNodes)
{
  int netIdx;
  double result;
  int neighborIdx;
  //int nNodes = circuit->nNodes;
  
  score = 0.0;
  for (int j = 0; j < static_cast<int>(currTableOfConnections[currClusterIdx].size()); ++j)
  {
    netIdx = currTableOfConnections[currClusterIdx][j];
    for (int k = 0; k < static_cast<int>(netList[netIdx].clusterIdxs.size()); ++k)
    {
      neighborIdx = netList[netIdx].clusterIdxs[k];
      if (neighborIdx == currClusterIdx || !IsNotTerminal(currClusterIdx) || !IsNotTerminal(neighborIdx))
        continue;
      Affinity(currClusterIdx, neighborIdx, clusters, netList, netListSizes, 
               currTableOfConnections, netAreas, result);
      if (Affinity != AffinityInterp && score < result)
      {
        score           = result;
        bestNeighborIdx = neighborIdx;
      }
      else
        if (Affinity == AffinityInterp)
          score += result;
      // mark the neighbors of the new object as invalid
      if (isToMark) clusters[neighborIdx].isValid = false;
    }
  }
  return OK;
}

void MultilevelFramework::CreateTableOfConnections(vector<Cluster>& clusters, vector<ConnectionsList>& currTableOfConnections,
                                                     NetList& netList, const int& nNodes)
{
  int cellIdx;

  currTableOfConnections.clear();
  currTableOfConnections.resize(clusters.size());
  for (int i = 0; i < static_cast<int>(netList.size()); ++i)
  {
    for (int j = 0; j < static_cast<int>(netList[i].clusterIdxs.size()); ++j)
    {
      cellIdx = netList[i].clusterIdxs[j];
      if (IsNotTerminal(cellIdx))
      {
        if (clusters[cellIdx].isFake == true)
          continue;
        currTableOfConnections[cellIdx].push_back(i);
      }
    }
  }
}

MULTIPLACER_ERROR MultilevelFramework::Affinity(const int& firstClusterIdx, const int& secondClusterIdx, 
                                                vector<Cluster>& clusters, NetList& netList, int* netListSizes,
                                                vector<ConnectionsList>& currTableOfConnections, 
                                                double* netAreas, double& result)
{
  result = 0.0;
  
  static vector<int> commonNetsIdxs(10000000);
  //vector<int> tmpIdxs;
  int netIdx = 0;
  int theSize = static_cast<int>(currTableOfConnections[firstClusterIdx].size() + currTableOfConnections[secondClusterIdx].size());
  static int* tmpIdxsInt = new int[10000000];
  
  // looking for common nets for these clusters:
  Merge(currTableOfConnections[firstClusterIdx], currTableOfConnections[secondClusterIdx], tmpIdxsInt);
  //commonNetsIdxs.reserve(theSize);// resize(theSize);
  int counter = 0;
  for (int i = 0; i < theSize - 1; ++i)
  {
    if (tmpIdxsInt[i] == tmpIdxsInt[i+1] && netListSizes[tmpIdxsInt[i]]/*netList[tmpIdxsInt[i]].size()*/ > 1)
      //commonNetsIdxs.push_back(tmpIdxsInt[++i]);
      commonNetsIdxs[counter++] = tmpIdxsInt[++i];
  }
  
  for (int i = 0; i < counter; ++i)
  {
    netIdx = commonNetsIdxs[i];
    result += 1 / ((netListSizes[netIdx]/*netList[netIdx].size()*/ - 1) * clusters[firstClusterIdx].area * clusters[secondClusterIdx].area);
  }
  
  //delete[] tmpIdxsInt;
  return OK;
}

double dist(const int& firstClusterIdx, const int& secondClusterIdx, vector<Cluster>& clusters)
{
  return sqrt(pow(clusters[firstClusterIdx].xCoord - clusters[secondClusterIdx].xCoord, 2) + 
              pow(clusters[firstClusterIdx].yCoord - clusters[secondClusterIdx].yCoord, 2));
}

MULTIPLACER_ERROR MultilevelFramework::AffinitySP(const int& firstClusterIdx, const int& secondClusterIdx, 
                                                  vector<Cluster>& clusters, NetList& netList, int* netListSizes,
                                                  vector<ConnectionsList>& currTableOfConnections, double* netAreas, double& result)
{
  result = 0.0;
  
  vector<int> commonNetsIdxs;
  //vector<int> tmpIdxs;
  int netIdx = 0;
  int theSize = static_cast<int>(currTableOfConnections[firstClusterIdx].size() + currTableOfConnections[secondClusterIdx].size());
  int* tmpIdxsInt = new int[theSize];
  
  // looking for common nets for these clusters:
  Merge(currTableOfConnections[firstClusterIdx], currTableOfConnections[secondClusterIdx], tmpIdxsInt);
  for (int i = 0; i < theSize - 1; ++i)
  {
    if (tmpIdxsInt[i] == tmpIdxsInt[i+1] && netList[tmpIdxsInt[i]].clusterIdxs.size() > 1)
      commonNetsIdxs.push_back(tmpIdxsInt[++i]);
  }
  
  for (int i = 0; i < static_cast<int>(commonNetsIdxs.size()); ++i)
  {
    netIdx = commonNetsIdxs[i];
    result += 1 / ((netList[netIdx].clusterIdxs.size() - 1) * clusters[firstClusterIdx].area * 
              clusters[secondClusterIdx].area * dist(firstClusterIdx, secondClusterIdx, clusters));
  }
  
  delete[] tmpIdxsInt;
  return OK;
}

double MultilevelFramework::LogSumExpForClusters(PetscScalar *coordinates, void* data)
{
  AppCtx* userData = (AppCtx*)data;
  
  double sum = 0.0;
  double logsum1 = 0.0;
  double logsum2 = 0.0;
  double logsum3 = 0.0;
  double logsum4 = 0.0;
  double x = 0.0;
  double y = 0.0;
  double alpha = userData->alpha;
  
  int nNodes = userData->circuit->nNodes;
  int realClusterIdx;
  int clusterIdxInCoordinatesArray;

  //PetscPrintf(PETSC_COMM_SELF,"\nEnter LogSumExp func\n");

  for (int i = 0; i < static_cast<int>(userData->netList->size()); ++i)
  {
    logsum1 = 0.0;
    logsum2 = 0.0;
    logsum3 = 0.0;
    logsum4 = 0.0;
    //PetscPrintf(PETSC_COMM_SELF,"\n%d\n", i);
    for (int j = 0; j < static_cast<int>((*userData->netList)[i].clusterIdxs.size()); ++j)
    {
      realClusterIdx = (*userData->netList)[i].clusterIdxs[j];
      if (IsNotTerminal(realClusterIdx))
      {
        clusterIdxInCoordinatesArray = (userData->lookUpTable)[realClusterIdx];
        //PetscPrintf(PETSC_COMM_SELF,"\n%d\n", idx);
        x = coordinates[2*clusterIdxInCoordinatesArray+0];
        y = coordinates[2*clusterIdxInCoordinatesArray+1];
      }
      else
      {
        x = 1 * userData->circuit->placement[realClusterIdx].xCoord;
        y = 1 * userData->circuit->placement[realClusterIdx].yCoord;
      }
      logsum1 += exp(+x / alpha);
      logsum2 += exp(-x / alpha);
      logsum3 += exp(+y / alpha);
      logsum4 += exp(-y / alpha);
    }

    sum += userData->netWeights[i] * (log(logsum1) + log(logsum2) + log(logsum3) + log(logsum4));
  }
  
  //PetscPrintf(PETSC_COMM_SELF,"\nExit LogSumExp func\n");
  return alpha * sum;
}

void MultilevelFramework::CalcLogSumExpForClustersGrad(PetscScalar *coordinates, PetscScalar *grad, void* data)
{
  LogEnter("MultilevelFramework::CalcLogSumExpForClustersGrad");
  AppCtx* userData = (AppCtx*)data;
  
  double sum = 0.0;
  double sum1 = 0.0;
  double sum2 = 0.0;
  double sum3 = 0.0;
  double sum4 = 0.0;
  double x = 0.0;
  double y = 0.0;
  double alpha = userData->alpha;
  
  int realClusterIdx;
  int clusterIdxInCoordinatesArray;
  int clusterIdx = 0;
  int netIdx;
  int nNodes = userData->circuit->nNodes;

  for (int i = 0; i < userData->nClusters; ++i, ++clusterIdx)
  {
    grad[2*i+0] = 0;
    grad[2*i+1] = 0;
    while ((*userData->clusters)[clusterIdx].isFake == true) ++clusterIdx;
    //PetscPrintf(PETSC_COMM_SELF,"\ni = %d\n", i);
    //PetscPrintf(PETSC_COMM_SELF,"\nsize = %d\n", static_cast<int>((*userData->currTableOfConnections)[i].size()));
    for (int j = 0; j < static_cast<int>((*userData->currTableOfConnections)[clusterIdx].size()); ++j)
    {     
      sum1 = 0.0;
      sum2 = 0.0;
      sum3 = 0.0;
      sum4 = 0.0;
      netIdx = (*userData->currTableOfConnections)[clusterIdx][j];
      //PetscPrintf(PETSC_COMM_SELF,"\nnetIdx = %d\t", netIdx);
      //PetscPrintf(PETSC_COMM_SELF,"netSize = %d\n", (*userData->netList)[netIdx].size());
      for (int k = 0; k < static_cast<int>((*userData->netList)[netIdx].clusterIdxs.size()); ++k)
      {
        //PetscPrintf(PETSC_COMM_SELF,"\nk = %d\t", k);
        realClusterIdx = (*userData->netList)[netIdx].clusterIdxs[k];
        //PetscPrintf(PETSC_COMM_SELF,"realClusterIdx = %d\n", realClusterIdx);
        if (IsNotTerminal(realClusterIdx))
        {
          clusterIdxInCoordinatesArray = (userData->lookUpTable)[realClusterIdx];
          x = coordinates[2*clusterIdxInCoordinatesArray+0];
          y = coordinates[2*clusterIdxInCoordinatesArray+1];
        }
        else
        {
          x = 1 * userData->circuit->placement[realClusterIdx].xCoord;
          y = 1 * userData->circuit->placement[realClusterIdx].yCoord;
        }
        sum1 += exp(+x / alpha);
        sum2 += exp(-x / alpha);
        sum3 += exp(+y / alpha);
        sum4 += exp(-y / alpha);
      }
      grad[2*i+0] += (exp(+coordinates[2*i+0] / alpha) / sum1 - 
                      exp(-coordinates[2*i+0] / alpha) / sum2) * 
                      userData->netWeights[netIdx];
      grad[2*i+1] += (exp(+coordinates[2*i+1] / alpha) / sum3 - 
                      exp(-coordinates[2*i+1] / alpha) / sum4) * 
                      userData->netWeights[netIdx];
    }
  }
  LogExit("MultilevelFramework::CalcLogSumExpForClustersGrad");
}

double MultilevelFramework::TestObjectiveFunc(PetscScalar *coordinates, void* data)
{
  double sum = 0.0;
  int realClusterIdx;
  int clusterIdxInCoordinatesArray;

  AppCtx* userData = (AppCtx*)data;

  for (int i = 0; i < static_cast<int>(userData->netList->size()); ++i)
  {
    for (int j = 0; j < static_cast<int>((*userData->netList)[i].clusterIdxs.size()); ++j)
    {
      realClusterIdx = (*userData->netList)[i].clusterIdxs[j];
      clusterIdxInCoordinatesArray = (userData->lookUpTable)[realClusterIdx];
      
      sum += pow((coordinates[clusterIdxInCoordinatesArray] - userData->circuit->height / 2), 2);
    }
  }

  return sum;
}

void MultilevelFramework::CalcBinGrid(Circuit& circuit, vector<Cluster>& clusters, const int& nClusters,
                                      int& nBinRows, int& nBinCols,
                                      double& binWidth, double& binHeight)
{
  double circuitAspectRatio = circuit.width / circuit.height;
  int desiredNumberOfClustersInEveryBin = 10;
  nBinRows = static_cast<int>(sqrt(circuitAspectRatio * nClusters/desiredNumberOfClustersInEveryBin));
  if (nBinRows%2 == 0) nBinRows++; //we need odd number, because cells will not move otherwise
  nBinCols = (int)dtoi(circuitAspectRatio * nBinRows);
  if (nBinCols%2 == 0) nBinCols++; //we need odd number, because cells will not move otherwise
  binWidth = circuit.width / nBinCols;
  binHeight = circuit.height / nBinRows;

  cout << "\ncircuitAspectRatio = " << circuitAspectRatio << endl;
  cout << "Current bin grid: " << nBinRows << " x " << nBinCols << endl;
  cout << "Bin width: " << binWidth 
       << "\tBin height: " << binHeight << endl;

  //user.binWidth  = circuit.width;
  //user.binHeight = CLUSTER_RATIO * circuit.height / dtoi((double)circuit.nRows / (circuit.nNodes / nClusters));

  /*double average = 0.0;

  for (int i = 0; i < static_cast<int>(clusters.size()); ++i)
  {
    if (clusters[i].isFake == false)
      average += sqrt(clusters[i].area);
  }
  cout << "average = " << average << endl;
  average /= sqrt((double)nClusters);*/

  /*binWidth  = average * (circuit.width  / circuit.height);
  binHeight = average * (circuit.height / circuit.width );
  cout << "average = " << average << endl;
  cout << binWidth << "\t" << binHeight << endl;*/

  //binWidth  /= 4.0;
  //binHeight /= CLUSTER_RATIO;
}

void MultilevelFramework::DetermineBordersOfClusterPotential(int &min_col, int &max_col, int &min_row, int &max_row,
                                        int i, PetscScalar * x, AppCtx* userData)
{
  LogEnter("MultilevelFramework::DetermineBordersOfClusterPotential", 0);
  min_col = static_cast<int>(dtoi((x[2*i+0]-userData->potentialRadiusX) / userData->binWidth));
  max_col = static_cast<int>(floor((x[2*i+0]+userData->potentialRadiusX) / userData->binWidth));
  min_row = static_cast<int>(dtoi((x[2*i+1]-userData->potentialRadiusY) / userData->binHeight));    
  max_row = static_cast<int>(floor((x[2*i+1]+userData->potentialRadiusY) / userData->binHeight));

  min_col = min(max(0, min_col), userData->nBinCols-1);
  min_row = min(max(0, min_row), userData->nBinRows-1);
  max_col = max(0, min(max_col, userData->nBinCols-1));
  max_row = max(0, min(max_row, userData->nBinRows-1));

  //cout << "min_col = " << min_col << endl;
  //cout << "min_row = " << min_row << endl;
  //cout << "max_col = " << max_col << endl;
  //cout << "max_row = " << max_row << endl;

  LogExit("MultilevelFramework::DetermineBordersOfClusterPotential", 0);
}

void MultilevelFramework::CalcBellShapedFuncAndDerivative(AppCtx* userData, int solutionIdx, int clusterIdx, int colIdx, int rowIdx,
                                                          PetscScalar * x, double &potX, double &gradX, 
                                                          double &potY, double &gradY)
{
  LogEnter("MultilevelFramework::CalcBellShapedFuncAndDerivative", 0);
  
  potX  = 0;
  gradX = 0;
  double _potX = 0;
  potY  = 0;
  gradY = 0;
  double _potY = 0;

  double dx = x[2*solutionIdx+0] - userData->binPenalties[rowIdx][colIdx].xCoord;
  double dy = x[2*solutionIdx+1] - userData->binPenalties[rowIdx][colIdx].yCoord;

  //cout << "dx" << dx << endl;
  //cout << "dy" << dy << endl;

  if (fabs(dx) > userData->potentialRadiusX|| 
      fabs(dy) > userData->potentialRadiusY)
  {
    LogExit("MultilevelFramework::CalcBellShapedFuncAndDerivative -- bin out of potential", 0);
    return;
  } // else this cluster affects this bin
  
  // calculate x-potential
  if (fabs(dx) <= userData->potentialRadiusX/2)
  {
    potX  = 1 - 2 * pow(dx / userData->potentialRadiusX, 2);
    _potX = -4 * sign(dx) * fabs(dx) / userData->potentialRadiusX / userData->potentialRadiusX;
  }
  else
  {
    potX  = 2 * pow((fabs(dx) - userData->potentialRadiusX) / userData->potentialRadiusX, 2);
    _potX = 4 * sign(dx) * (fabs(dx) - userData->potentialRadiusX) / userData->potentialRadiusX / userData->potentialRadiusX;
  }

  // calculate y-potential
  if (fabs(dy) <= userData->potentialRadiusY/2)
  {
    potY  = 1 - 2 * pow(dy / userData->potentialRadiusY, 2);
    _potY = -4 * sign(dy) * fabs(dy) / userData->potentialRadiusY / userData->potentialRadiusY;
  }
  else
  {
    potY  = 2 * pow((fabs(dy) - userData->potentialRadiusY) / userData->potentialRadiusY, 2);
    _potY = 4 * sign(dy) * (fabs(dy) - userData->potentialRadiusY) / userData->potentialRadiusY / userData->potentialRadiusY;
  }

  gradX = 2 * (userData->binPenalties[rowIdx][colIdx].sumPotential - userData->meanBinArea) *
    (*(userData->clusters))[clusterIdx].scalingCoefficient * _potX * potY;
  gradY = 2 * (userData->binPenalties[rowIdx][colIdx].sumPotential - userData->meanBinArea) * 
    (*(userData->clusters))[clusterIdx].scalingCoefficient * _potY * potX;
  LogExit("MultilevelFramework::CalcBellShapedFuncAndDerivative", 0);
}

double MultilevelFramework::CalcPenalty(PetscScalar *x, void* data)
{
  LogEnter("MultilevelFramework::CalcPenalty");
  AppCtx* userData = (AppCtx*)data;

  double potX;
  double potY;
  // kornyakov todo: they are dummy here i've extracted method
  // plan to merge penalty and its gradient calculation
  double gradX;
  double gradY;
  int clusterIdx = 0;

  // each cell must have total potential equal to its area
  // these variables are needed to control this issue
  double currClusterTotalPotential = 0.0;

  double totalPenalty = 0.0;

  // null the penalties
  for (int i = 0; i < userData->nBinRows; ++i)
  {
    for (int j = 0; j < userData->nBinCols; ++j)
    {
      userData->binPenalties[i][j].sumPotential = 0.0;
      //cout << "userData->binPenalties[i][j].sumPotential = " 
      //     << userData->binPenalties[i][j].sumPotential << "\n";
    }
  }

  //cout << "userData->potentialRadiusY = " << userData->potentialRadiusY << endl;
  //cout << "userData->potentialRadiusX = " << userData->potentialRadiusX << endl;

  // calculate every cluster's potential
  // we take every cluster and consider only bins 
  // which are affected by this cluster potential
  for (int i = 0; i < userData->nClusters; ++i, ++clusterIdx)
  {
    //cout << "cluster#\t" << i << endl;

    while ((*userData->clusters)[clusterIdx].isFake == true) ++clusterIdx;

    currClusterTotalPotential = 0.0;
    (*(userData->clusters))[clusterIdx].scalingCoefficient = 1.0;
    int min_row, min_col, max_row, max_col; // area affected by cluster potential 
    //cout << i << "\t" << x[2*i+0] << "\t" << x[2*i+1] << "\n";
    DetermineBordersOfClusterPotential(min_col, max_col, min_row, max_row, i, x, userData);

    // loop over affected bins - now we just precalculate potential
    // later we will scale it so that currClusterTotalPotential = cluster area
    for (int rowIdx = min_row; rowIdx <= max_row; ++rowIdx)
    {
      for (int colIdx = min_col; colIdx <= max_col; ++colIdx)
      {
        CalcBellShapedFuncAndDerivative(userData, i, clusterIdx, colIdx, rowIdx, x, 
                                        potX, gradX, potY, gradY);
        //cout << "rowIdx " << rowIdx << endl;
        //cout << "colIdx " << colIdx << endl;
        //cout << "min_row " << min_row << endl;
        //cout << "min_col " << min_col << endl;

        //printf("%.18f\t%.18f\n", potX, potY);
        userData->clusterPotentialOverBins[rowIdx-min_row][colIdx-min_col] = potX*potY;
        //cout << "stored user data\n";
        currClusterTotalPotential += userData->clusterPotentialOverBins[rowIdx-min_row][colIdx-min_col];        
        //cout << "currClusterTotalPotential updated\n";
      }
    }// loop over affected bins

//     cout << "currClusterTotalPotential = " << currClusterTotalPotential << endl; 
//     for (int k = min_row; k <= max_row; ++k)
//     {
//       for (int j = min_col; j <= max_col; ++j)
//       {
//         cout << userData->clusterPotentialOverBins[k-min_row][j-min_col] << "\t";
//       }
//       cout << endl;
//     }

    // scale the potential
    //cout << "currClusterTotalPotential = " << currClusterTotalPotential << endl;
    if (currClusterTotalPotential <= 0.0)
    {
      //Trace("current cluster potential is less than 0!");
      //cout << i << "\t" << x[2*i+0] << "\t" << x[2*i+1] << "\n";
      (*(userData->clusters))[clusterIdx].scalingCoefficient = 0;
    }
    else
    {
      (*(userData->clusters))[clusterIdx].scalingCoefficient = (*(userData->clusters))[clusterIdx].area / currClusterTotalPotential;
    }

    // add scaled cluster potential 
    for (int k = min_row; k <= max_row; ++k)
      for (int j = min_col; j <= max_col; ++j)
        userData->binPenalties[k][j].sumPotential += 
          userData->clusterPotentialOverBins[k-min_row][j-min_col] * (*(userData->clusters))[clusterIdx].scalingCoefficient;
  }// calculate every cluster's potential

  // calculate result penalty and return
  totalPenalty = 0.0;
  //double sum = 0.0;
  for (int i = 0; i < userData->nBinRows; ++i)
  {
    //cout << "\ni = " << i << endl;
    for (int j = 0; j < userData->nBinCols; ++j)
    {
      //cout << "j = " << j << "\t";
      totalPenalty += pow(userData->binPenalties[i][j].sumPotential - userData->meanBinArea, 2);
      //sum += userData->binPenalties[i][j].sumPotential;
      //cout << "SP["<<i<<"]["<<j<<"]=" << userData->binPenalties[i][j].sumPotential << "\t";
    }
    //cout << endl;
  }
  //cout << endl << endl;
  /*cout << "totalCellArea = " << userData->totalCellArea << endl;
  cout << "sumPotential  = " << sum << endl;*/

  //fixme: try to move clusters into bound
  //float bigPenalty = (float)(totalPenalty / 100.0);
  //for (int i = 0; i < userData->nClusters; ++i)
  //{
  //  // x-penalty
  //  double scconst = 0.01;
  //  if (x[2*i+0] < 0)
  //  {
  //    //totalPenalty += bigPenalty;
  //    //totalPenalty += pow(-x[2*i+0], 1);
  //    totalPenalty += -x[2*i+0]*scconst;
  //  }
  //  else if (x[2*i+0] > userData->circuit->width)
  //  {
  //    totalPenalty += (x[2*i+0] - userData->circuit->width)*scconst;
  //  }
  //  // y-penalty
  //  if (x[2*i+1] < 0)
  //  {
  //    //totalPenalty += bigPenalty;
  //    totalPenalty += -x[2*i+1]*scconst;
  //  }
  //  else if (x[2*i+1] > userData->circuit->height)
  //  {
  //    totalPenalty += (x[2*i+1] - userData->circuit->height)*scconst;
  //  }
  //}
#ifdef USE_BORDER_PENALTY
  double halfWidth;
  double halfHeight;
  int solIdx;  // solutionIdx
  for (int i = 0; i < userData->circuit->nNodes; ++i)
  {
    solIdx = userData->lookUpTable[i];
    if (solIdx < 0) continue;
    
    halfWidth  = 0.5 * userData->circuit->nodes[i].width;
    halfHeight = 0.5 * userData->circuit->nodes[i].height;
    if (x[2*solIdx+0] - halfWidth < 0 || x[2*solIdx+0] + halfWidth > userData->circuit->width)
    {
      totalPenalty += pow(fabs(x[2*solIdx+0] - userData->circuit->width/2) + halfWidth - userData->circuit->width/2, 3);
    }
    if (x[2*solIdx+1] - halfHeight < 0 || x[2*solIdx+1] + halfHeight > userData->circuit->height)
    {
      totalPenalty += pow(fabs(x[2*solIdx+1] - userData->circuit->height/2) + halfHeight - userData->circuit->height/2, 3);
    }
  }
#endif

  LogExit("MultilevelFramework::CalcPenalty");
  return totalPenalty;
}

// todo: check if grad already nulled or we have to null it
void MultilevelFramework::CalcPenaltyGrad(PetscScalar *x, PetscScalar *grad, void* data)
{
  LogEnter("MultilevelFramework::CalcPenaltyGrad");
  AppCtx* userData = (AppCtx*)data;

  double potX;
  double potY;
  double gradX;
  double gradY;
  int clusterIdx = 0;

  for (int i = 0; i < userData->nClusters; ++i, ++clusterIdx)
  {
    int min_row, min_col, max_row, max_col; // area affected by cluster potential 

    while ((*userData->clusters)[clusterIdx].isFake == true) ++clusterIdx;
   
    DetermineBordersOfClusterPotential(min_col, max_col, min_row, max_row, i, x, userData);

    for (int rowIdx = min_row; rowIdx <= max_row; ++rowIdx)
    {
      for (int colIdx = min_col; colIdx <= max_col; ++colIdx)
      {
        CalcBellShapedFuncAndDerivative(userData, i, clusterIdx, colIdx, rowIdx, x, 
                                        potX, gradX, potY, gradY);

        //fixme: delete rand
        grad[2*i+0] += userData->mu*gradX;
        grad[2*i+1] += userData->mu*gradY;
        //grad[2*i+0] += userData->mu*(rand()-RAND_MAX/2);
        //grad[2*i+1] += userData->mu*(rand()-RAND_MAX/2);
      }
    }
  }

  //fixme: gradient for border penalties
  /*for (int i = 0; i < userData->nClusters; ++i)
  {
    double scconst = 0.01;
    if (x[2*i+0] < 0)
    {
      totalPenalty += bigPenalty;
      grad[2*i+0] -= 1*pow(-x[2*i+0], 0);
      grad[2*i+0] += -scconst;
    }
    else if (x[2*i+0] > userData->circuit->width)
    {
      grad[2*i+0] += scconst;
    }

    if (x[2*i+1] < 0)
    {
      totalPenalty += bigPenalty;
      grad[2*i+1] += -scconst;
    }
    else if (x[2*i+1] > userData->circuit->height)
    {
      grad[2*i+1] += scconst;
    }
  }*/
#ifdef USE_BORDER_PENALTY
  double halfWidth;
  double halfHeight;
  int solIdx; // solutionIdx
  for (int i = 0; i < userData->circuit->nNodes; ++i)
  {
    solIdx = userData->lookUpTable[i];
    if (solIdx < 0) continue;

    halfWidth  = 0.5 * userData->circuit->nodes[i].width;
    halfHeight = 0.5 * userData->circuit->nodes[i].height;

    if (x[2*solIdx+0] - halfWidth < 0 || x[2*solIdx+0] + halfWidth > userData->circuit->width)
    {
      grad[2*solIdx+0] += 3*pow(fabs(x[2*solIdx+0] - userData->circuit->width/2) + halfWidth - userData->circuit->width/2, 2) *
                     sign(x[2*solIdx+0] - userData->circuit->width/2);
    }
    if (x[2*solIdx+1] - halfHeight < 0 || x[2*solIdx+1] + halfHeight > userData->circuit->height)
    {
      grad[2*solIdx+1] += 3*pow(fabs(x[2*solIdx+1] - userData->circuit->height/2) + halfHeight - userData->circuit->height/2, 2) *
                     sign(x[2*solIdx+1] - userData->circuit->height/2);
    }
  }
#endif

  LogExit("MultilevelFramework::CalcPenaltyGrad");
}

void MultilevelFramework::CalcMu0(PetscScalar *x, void* data)
{
  LogEnter("MultilevelFramework::CalcMu0");
//#define USE_GRADIENTS
#ifdef USE_GRADIENTS
  AppCtx* user = (AppCtx*)data;
 
  PetscScalar *gradPen = new PetscScalar[2*user->nClusters];
  PetscScalar *gradWL  = new PetscScalar[2*user->nClusters];
  
  double potX;
  double potY;
  double gradX;
  double gradY;
  double sumSDModXY = 0.0;
  double sumNumerator = 0.0;
  double sumDenominator = 0.0;
  int clusterIdx = 0;

  for (int i = 0; i < 2*user->nClusters; ++i)
  {
    gradPen[i] = 0;
  }

  CalcLogSumExpForClustersGrad(x, gradWL, user);
  CalcPenalty(x, user);
  for (int k = 0; k <= user->nBinRows - 1; ++k)
  {
    for (int j = 0; j <= user->nBinCols - 1; ++j)
    {
      sumSDModXY = 0.0;
      for (int i = 0; i < user->nClusters; ++i)
      {    
        while ((*user->clusters)[clusterIdx].isFake == true) ++clusterIdx;
        CalcBellShapedFuncAndDerivative(user, i, clusterIdx, j, k, x, 
                                        potX, gradX, potY, gradY);
        // todo: solve problem with infinity and this magic number
        sumSDModXY += fabs(min(gradX, 100000)) + fabs(min(gradY, 100000));
        //cout << "gradX\t" << gradX << "\tgradY\t" << gradY << endl;
      }
      sumSDModXY *= fabs(user->binPenalties[k][j].sumPotential - user->meanBinArea);
      //cout << "sumSDModXY\t" << sumSDModXY << endl;
      sumNumerator += sumSDModXY;
    }
  }
  for (int i = 0; i < user->nClusters; ++i)
  {
    sumDenominator += fabs(gradWL[2*i+0]) + fabs(gradWL[2*i+1]);
  }
  //CalcPenaltyGrad(x, gradPen, &user);

  delete[] gradPen;
  delete[] gradWL;

  user->mu0 = 0.5 * sumNumerator / sumDenominator;
  user->mu0 = 1 / user->mu0;
  //cout << "mu0 = " << user->mu0 << endl;
  cout << "sumNumerator\t" << sumNumerator << endl;
  cout << "sumDenominator\t" << sumDenominator << endl;
#else
  AppCtx* user = (AppCtx*)data;
  user->mu0 = CalcPenalty(x, data) / LogSumExpForClusters(x, data);
  double interestingConstant = 1.0f/16.0f; //this coefficient greatly affect quality of placement
                                    //because it determines initial distribution of cells
                                    //over bins. We have to choose small value to keep good HPWL                  
  user->mu0 = interestingConstant / user->mu0;
  //cout << "mu0 = " << user->mu0 << endl;
#endif
  LogExit("MultilevelFramework::CalcMu0");
}

double MultilevelFramework::GetDiscrepancy(PetscScalar *x, void* data)
{
  AppCtx* user = (AppCtx*)data;

  //double binArea = user->binHeight * user->binWidth;
  double *clustersAreasInBins = new double[user->nBinRows * user->nBinCols];
  double max = 0;
  double dimension;  // половина стороны квадрата, площадь которого равна площади текущего кластера
  int min_col;
  int max_col;
  int min_row;
  int max_row;
  double leftRatio;   //
  double rihgtRatio;  //
  double lowerRatio;  //
  double upperRatio;  // доли, на которые делится бинами данный кластер (от 0 до 1)
  double area;        // current cluster area
  int solutionIdx;

  for (int i = 0; i < user->nBinRows * user->nBinCols; ++i)
  {
    clustersAreasInBins[i] = 0;
  }

  for (int i = 0; i < static_cast<int>(user->clusters->size()); ++i)
  {
    if ((*user->clusters)[i].isFake == true)
    {
      continue;
    }

    solutionIdx = user->lookUpTable[i];
    area = (*user->clusters)[i].area;
    dimension = sqrt(area) / 2;
    min_col = static_cast<int>(dtoi((x[2*solutionIdx+0]-dimension) / user->binWidth));
    max_col = static_cast<int>(dtoi((x[2*solutionIdx+0]+dimension) / user->binWidth));
    min_row = static_cast<int>(dtoi((x[2*solutionIdx+1]-dimension) / user->binHeight));    
    max_row = static_cast<int>(dtoi((x[2*solutionIdx+1]+dimension) / user->binHeight));

    min_col = min(max(0, min_col), user->nBinCols-1);
    min_row = min(max(0, min_row), user->nBinRows-1);
    max_col = max(0, min(max_col, user->nBinCols-1));
    max_row = max(0, min(max_row, user->nBinRows-1));

    if (min_col != max_col)
    {
      leftRatio  = (user->binWidth * (min_col + 1) - x[2*solutionIdx+0] + dimension) / (2*dimension);
    }
    else
    {
      leftRatio = 0.0;
    }
    rihgtRatio = 1 - leftRatio;

    if (min_row != max_row)
    {
      lowerRatio = (user->binHeight * (min_row + 1) - x[2*solutionIdx+1] + dimension) / (2*dimension);
    } 
    else
    {
      lowerRatio = 0.0;
    }
    upperRatio = 1 - lowerRatio;

    clustersAreasInBins[min_row * user->nBinCols + min_col] += area * leftRatio * lowerRatio;
    clustersAreasInBins[min_row * user->nBinCols + max_col] += area * rihgtRatio * lowerRatio;
    clustersAreasInBins[max_row * user->nBinCols + min_col] += area * leftRatio * upperRatio;
    clustersAreasInBins[max_row * user->nBinCols + max_col] += area * rihgtRatio * upperRatio;
  }

  for (int i = 0; i < user->nBinRows * user->nBinCols; ++i)
  {
    if (max < clustersAreasInBins[i])
    {
      max = clustersAreasInBins[i];
    }
  }

  delete []clustersAreasInBins;

  return max / user->meanBinArea;
}

void MultilevelFramework::MakeClustersFromBins(Circuit& circuit, vector<Cluster>& clusters, NetList& netList)
{
  int firstClusterIdx;
  int secondClusterIdx;

  vector<ConnectionsList> currTableOfConnections(clusters.size());

  CreateTableOfConnections(clusters, currTableOfConnections, netList, circuit.nNodes);

  for (int i = 0; i < circuit.nBinRows; ++i)
  {
    for (int j = 0; j < circuit.nBinCols; ++j)
    {
      firstClusterIdx = circuit.arrOfBins[i][j]->nodes[0];
      for (int k = 1; k < static_cast<int>(circuit.arrOfBins[i][j]->nodes.size()); ++k)
      {
        secondClusterIdx = circuit.arrOfBins[i][j]->nodes[k];
        MergeClusters(firstClusterIdx, secondClusterIdx, clusters, netList, currTableOfConnections, NULL);
      }
    }
  }
}