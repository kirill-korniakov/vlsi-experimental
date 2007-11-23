/*  
 * multilevel_framework.cpp
 * this is a part of itlDragon
 * Copyright (C) 2007, ITLab, Zhivoderov Artem
 * email: zhivoderov.a@gmail.com
 */

#include "..\include\multilevel_framework.h"
#include "..\include\output.h"
#include "math.h"
#include "tao.h"
#include <algorithm>

using namespace MultilevelFramework;

void MultilevelFramework :: Merge(vector<int>& a, vector<int>& b, int result[])
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

bool PredicateNetListLess(vector<int> elem1, vector<int> elem2)
{
  int size1 = static_cast<int>(elem1.size());
  int size2 = static_cast<int>(elem2.size());
  
  if (size1 < size2)
    return true;
  else 
    if (size1 > size2)
      return false;
  
  for (int i = 0; i < size2; ++i)
  {
    if (elem1[i] < elem2[i])
      return true;
    else
      if (elem1[i] > elem2[i])
        return false;
  }
  
  return false;
}

bool IsEqualNetListBinaryPredicate(vector<int> elem1, vector<int> elem2)
{
  int size1 = static_cast<int>(elem1.size());
  int size2 = static_cast<int>(elem2.size());
  
  if (size1 != size2)
    return false;
  
  for (int i = 0; i < size2; ++i)
  {
    if (elem1[i] != elem2[i])
      return false;
  }
  
  return true;
}


void MultilevelFramework :: UpdateCoords(Circuit& circuit, vector<Cluster>& clusters)
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

MULTIPLACER_ERROR MultilevelFramework :: Interpolation(Circuit& circuit, vector<Cluster>& clusters,
                                                       vector<ConnectionsList>& currTableOfConnections, NetList& netList)
{
  vector<ClusterData> clustersData(clusters.size());
  list<ClusterData> clustersDataList;
  list<ClusterData>::iterator clustersDataIterator;
  bool* isCPoint = new bool[clusters.size()];  // C-point is a cluster which location is fixed during interpolation
  double part = 0.5;  // part of clusters to be initially fixed during interpolation (part of C-points)
  int currClusterIdx;
  int neighborIdx;
  int netIdx;
  vector<int> currCPoints;
  double sumX;
  double sumY;
  double area;

  for (int i = 0; i < static_cast<int>(clusters.size()); ++i)
  {
    clustersData[i].clusterIdx = i;
    clustersData[i].score      = 0.0;
    
    if (clusters[i].isFake == true)
      continue;
    
    CalculateScore(i, currTableOfConnections, netList, 
                   NULL, clusters, clustersData[i].score, clustersData[i].bestNeighborIdx, false, AffinityInterp);
    isCPoint[i] = false;
  }
  sort(clustersData.begin(), clustersData.end(), PredicateClusterDataGreater);
  
  for (int i = 0; i < static_cast<int>(clusters.size()); ++i)
  {
    if (clustersData[i].score >= 0.25)
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
      for (int k = 0; k < static_cast<int>(netList[netIdx].size()); ++k)
      {
        neighborIdx = netList[netIdx][k];
        if (isCPoint[neighborIdx] == false)
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
    //clusters[currClusterIdx].xCoord = 0;
    //clusters[currClusterIdx].yCoord = 0;
    ++clustersDataIterator;
  }
  
  UpdateCoords(circuit, clusters);

  delete[] isCPoint;

  return OK;
}

/* 
User-defined application context - contains data needed by the 
application-provided call-back routines that evaluate the function,
gradient, and hessian.
*/
typedef struct {
  Circuit* circuit;
  vector<Cluster>* clusters;
  NetList* netList;
  int numOfClusters;
} AppCtx;

static  char help[] = "This example demonstrates use of the TAO package to \n\
                      solve an unconstrained minimization problem on a single processor.  We \n\
                      minimize the extended Rosenbrock function: \n\
                      sum_{i=0}^{n/2-1} ( alpha*(x_{2i+1}-x_{2i}^2)^2 + (1-x_{2i})^2 ) \n";

//int FormFunctionGradient(TAO_APPLICATION taoapp,Vec X,double *f, Vec G,void *ptr)
//{
//  AppCtx *user = (AppCtx *) ptr;  
//  int    i,info,nn=user->n/2;
//  double ff=0,t1,t2,alpha=user->alpha;
//  PetscScalar *x,*g;
//
//  /* Get pointers to vector data */
//  info = VecGetArray(X,&x); CHKERRQ(info);
//  info = VecGetArray(G,&g); CHKERRQ(info);
//
//  /* Compute G(X) */
//  for (i=0; i<nn; i++){
//    t1 = x[2*i+1]-x[2*i]*x[2*i]; t2= 1-x[2*i];
//    ff += alpha*t1*t1 + t2*t2;
//    g[2*i] = -4*alpha*t1*x[2*i]-2.0*t2;
//    g[2*i+1] = 2*alpha*t1;
//  }
//
//  /* Restore vectors */
//  info = VecRestoreArray(X,&x); CHKERRQ(info);
//  info = VecRestoreArray(G,&g); CHKERRQ(info);
//  *f=ff;
//
//  info = PetscLogFlops(nn*15); CHKERRQ(info);
//  return 0;
//}

int AnalyticalObjectiveAndGradient(TAO_APPLICATION taoapp,Vec X,double *f, Vec G,void *ptr)
{
  AppCtx *user = (AppCtx *) ptr;
  int    info,nn = user->circuit->nNodes * 2;
  double ff=0;
  PetscScalar *x,*g;

  /* Get pointers to vector data */
  info = VecGetArray(X,&x); CHKERRQ(info);
  info = VecGetArray(G,&g); CHKERRQ(info);

  /* Compute G(X) */
  // TODO: implement

  /* Restore vectors */
  info = VecRestoreArray(X,&x); CHKERRQ(info);
  info = VecRestoreArray(G,&g); CHKERRQ(info);
  const double alpha = 0.1;
  *f = LogSumExpForClusters(X, user->numOfClusters, user->netList, user->clusters, alpha);

  info = PetscLogFlops(nn*15); CHKERRQ(info);
  return 0;
}

MULTIPLACER_ERROR MultilevelFramework :: Relaxation(Circuit& circuit, vector<Cluster>& clusters, NetList& netList)
{
  int numOfClusters = static_cast<int>(clusters.size());

  for (int i = 0; i < static_cast<int>(clusters.size()); ++i)
  {
    if (clusters[i].isFake == true)
      --numOfClusters;
  }

  int        info;                  /* used to check for functions returning nonzeros */
  //PetscScalar zero=0.0;
  Vec        x;                     /* solution vector */
  Mat        H;                     /* Hessian matrix */
  TAO_SOLVER tao;                   /* TAO_SOLVER solver context */
  TAO_APPLICATION taoapp;           /* TAO application context */
  //PetscTruth  flg;
  int        size,rank;                  /* number of processes running */
  TaoTerminateReason reason;
  AppCtx     user;                  /* user-defined application context */

  info = MPI_Comm_size(PETSC_COMM_WORLD,&size); CHKERRQ(info);
  info = MPI_Comm_rank(PETSC_COMM_WORLD,&rank); CHKERRQ(info);

  if (size >1) {
    if (rank == 0)
      PetscPrintf(PETSC_COMM_SELF,"This example is intended for single processor use!\n");
    SETERRQ(1,"Incorrect number of processors");
  }


  /* Initialize problem parameters */
  user.circuit = &circuit;
  user.clusters = &clusters;
  user.netList = &netList;
  user.numOfClusters = numOfClusters;

  /* Check for command line arguments to override defaults */
  //info = PetscOptionsGetInt(PETSC_NULL,"-n",&user.n,&flg); CHKERRQ(info);
  //info = PetscOptionsGetReal(PETSC_NULL,"-alpha",&user.alpha,&flg); CHKERRQ(info);

  /* Allocate vectors for the solution and gradient */
  info = VecCreateSeq(PETSC_COMM_SELF, numOfClusters * 2, &x); CHKERRQ(info);

  /* 
  Allocate storage space for Hessian matrix; 
  Hessian information is optional -- unless a Newton method is selected
  */
  info = MatCreateSeqBDiag(PETSC_COMM_SELF, numOfClusters * 2, numOfClusters * 2, 0, 2, 0, 0, &H);CHKERRQ(info);
  info = MatSetOption(H,MAT_SYMMETRIC); CHKERRQ(info);

  /* The TAO code begins here */

  /* Create TAO solver with desired solution method */
  info = TaoCreate(PETSC_COMM_SELF, "tao_nm", &tao); CHKERRQ(info);
  info = TaoApplicationCreate(PETSC_COMM_SELF,&taoapp); CHKERRQ(info);

  /* Set solution vec and an initial guess */
  //info = VecSet(x, zero); CHKERRQ(info);

  float* rndCoords = new float[2 * numOfClusters];
  int* idxs = new int[2 * numOfClusters];
  PetscScalar* initValues = new PetscScalar[2 * numOfClusters];

  GetFloatRandomNumbers(rndCoords, 2 * numOfClusters, 0.0, 1.0);

  for (int i = 0; i < 2 * numOfClusters; ++i)
  {
    idxs[i] = i;
    initValues[i] = rndCoords[i];
  }

  int idx = 0;
  for (int i = 0; i < static_cast<int>(clusters.size()); ++i)
  {
    if (clusters[i].isFake == false)
    {
      clusters[i].xCoord = circuit.width  * initValues[2*idx+0];
      clusters[i].yCoord = circuit.height * initValues[2*idx+1];
      idx++;
    }
  }

  UpdateCoords(circuit, clusters);
  PrintToTmpPL(circuit);

  VecSetValues(x, 2 * numOfClusters, idxs, initValues, INSERT_VALUES);

  info = TaoAppSetInitialSolutionVec(taoapp, x); CHKERRQ(info); 

  /* Set routines for function, gradient, hessian evaluation */
  info = TaoAppSetObjectiveAndGradientRoutine(taoapp, AnalyticalObjectiveAndGradient,(void *)&user); 
  CHKERRQ(info);

  /* Check for TAO command line options */
  info = TaoSetOptions(taoapp, tao); CHKERRQ(info);

  /* SOLVE THE APPLICATION */
  info = TaoSolveApplication(taoapp, tao); CHKERRQ(info);

  /* Get termination information */
  info = TaoGetTerminationReason(tao, &reason); CHKERRQ(info);
  if (reason <= 0)
    PetscPrintf(MPI_COMM_WORLD,"Try a different TAO method, adjust some parameters, or check the function evaluation routines\n");

  // read solution vector to array of clusters coordinates
  VecGetValues(x, PetscInt(2*numOfClusters), idxs, initValues);
  idx = 0;
  for (int i = 0; i < static_cast<int>(clusters.size()); ++i)
  {
    if (clusters[i].isFake == false)
    {
      clusters[i].xCoord = initValues[2*idx+0];
      clusters[i].yCoord = initValues[2*idx+1];
      idx++;
    }
  }
  
  UpdateCoords(circuit, clusters);
  PrintToTmpPL(circuit);

  //PetscInt ni = numOfClusters*2;
  //static PetscInt *idxs = new PetscInt[numOfClusters*2];
  //for(int i=0; i<numOfClusters*2;i++)
  //{
  //  idxs[i] = i;
  //}
  //static PetscScalar *coordinates = new PetscScalar[numOfClusters*2];

  //VecGetValues(X, ni, idxs, coordinates);


  /* Free TAO data structures */
  info = TaoDestroy(tao); CHKERRQ(info);
  info = TaoAppDestroy(taoapp); CHKERRQ(info);

  /* Free PETSc data structures */
  info = VecDestroy(x); CHKERRQ(info);
  info = MatDestroy(H); CHKERRQ(info);

  /* Finalize TAO */
  TaoFinalize();
  PetscFinalize();

  delete[] rndCoords;
  delete[] initValues;
  delete[] idxs;

  return OK;
}

void MultilevelFramework :: UnclusterLevelUp(vector<Cluster>& clusters, vector<ConnectionsList>& currTableOfConnections,
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

MULTIPLACER_ERROR MultilevelFramework :: Clusterize(Circuit& circuit, vector<Cluster>& clusters, NetList& netList,
                                         vector<ConnectionsList>& currTableOfConnections, list<NetList>& netLevels,
                                         affinityFunc Affinity, list<ClusteringInfoAtEachLevel>& clusteringLog)
{  
  int i = 0;
  
  int targetNumOfClusters;
  int netIdx = 0;
  int currClusterIdx = 0;
  int numOfClusters = 0;
  double totalCellArea = 0.0;
  double targetClusterArea;
  vector<ClusterData> clustersData(clusters.size());
  list<ClusterData> clustersDataList;
  list<ClusterData>::iterator clustersDataIterator;
  ClusteringInfoAtEachLevel clusteringLogAtEachLevel;  // инфа, к-ая сохраняется по ходу кластеризации на каждом уровне
                                                       // и затем кладется в clusteringLog
  double* netAreas;
  double currNetArea = 0.0;
  double min, max;

  int bestNeighborIdx;
  int mergeCount = 0;
  ClusteredNodes buffer;

  numOfClusters = static_cast<int>(clusters.size());
  netAreas = new double[netList.size()];
  
  for (int i = 0; i < circuit.nNodes; ++i)
  {
    totalCellArea += clusters[i].area;
  }

  while (numOfClusters > FINAL_NCLUSTERS)
  {
    targetNumOfClusters = static_cast<int>(numOfClusters / CLUSTER_RATIO);
    targetClusterArea   = (totalCellArea / targetNumOfClusters) * CLUSTERS_AREA_TOLERANCE;
    
    CreateTableOfConnections(clusters, currTableOfConnections, netList, circuit.nNodes);

    for (int i = 0; i < static_cast<int>(netList.size()); ++i)
    { 
      currNetArea = 0.0;
      for (int j = 0; j < static_cast<int>(netList[i].size()); ++j)
      {
        currNetArea += clusters[netList[i][j]].area;
      }
      netAreas[i] = currNetArea;
    }
    
    for (int i = 0; i < static_cast<int>(clusters.size()); ++i)
    {
      clustersData[i].clusterIdx = i;
      clustersData[i].score      = 0.0;
      
      if (clusters[i].isFake == true)
        continue;
      
      CalculateScore(i, currTableOfConnections, netList, 
                     netAreas, clusters, clustersData[i].score, clustersData[i].bestNeighborIdx,
                     DONT_MARK_NEIGHBORS_INVALID, Affinity);
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
    while (clustersDataList.size() && numOfClusters - mergeCount > targetNumOfClusters)
    {
      clustersDataIterator = clustersDataList.begin();
      currClusterIdx  = clustersDataIterator->clusterIdx;
      bestNeighborIdx = clustersDataIterator->bestNeighborIdx;
      if ( clusters[currClusterIdx].isValid && 
          (clusters[currClusterIdx].area + clusters[bestNeighborIdx].area <= targetClusterArea))
      {
        buffer.firstClusterIdx  = currClusterIdx;
        buffer.secondClusterIdx = bestNeighborIdx;
        buffer.size             = clusters[currClusterIdx].cellIdxs.size();
        
        clusteringLogAtEachLevel.push_back(buffer);
        
        MergeClusters(currClusterIdx, bestNeighborIdx, clusters, netList, currTableOfConnections, netAreas);
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
        CalculateScore(currClusterIdx, currTableOfConnections, netList, 
                       netAreas, clusters, clustersDataIterator->score, clustersDataIterator->bestNeighborIdx,
                       DONT_MARK_NEIGHBORS_INVALID, Affinity);
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
      CalculateScore(currClusterIdx, currTableOfConnections, netList, 
                     netAreas, clusters, clustersDataIterator->score, clustersDataIterator->bestNeighborIdx,
                     MARK_NEIGHBORS_INVALID, Affinity);
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
    
    // update netList again
    NetList::iterator netListIterator;
    sort(netList.begin(), netList.end(), PredicateNetListLess);
    netListIterator = unique(netList.begin(), netList.end(), IsEqualNetListBinaryPredicate);
    netList.resize(netListIterator - netList.begin());
    
    netListIterator = netList.begin();
    while (netListIterator->size() < 2) ++netListIterator;
    netList.erase(netList.begin(), netListIterator);
    
    netLevels.push_back(netList);
    clusteringLog.push_back(clusteringLogAtEachLevel);

    numOfClusters = static_cast<int>(clusters.size() - numOfFakes);
    
    cout << "Currently there are " << numOfClusters << " clusters\n";
    cout << "targetClusterArea = " << targetClusterArea << "\n";
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
    cout << "max area is " << max << endl << endl;
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

void MultilevelFramework :: InitializeDataStructures(Circuit& circuit, vector<Cluster>& clusters, NetList& netList, const int& nNodes)
{
  int cellIdx;

  for (int i = 0; i < circuit.nNodes; ++i)
  {
    clusters[i].cellIdxs.push_back(i);
    clusters[i].area = circuit.nodes[i].width * circuit.nodes[i].height;
  }
  for (int i = 0; i < circuit.nNets; ++i)
  {
    for (int j = 0; j < circuit.nets[i].numOfPins; ++j)
    {
      cellIdx = circuit.nets[i].arrPins[j].cellIdx;
      if (IsNotTerminal(cellIdx))
        netList[i].push_back(cellIdx);
    }
  }
  for (int i = 0; i < static_cast<int>(netList.size()); ++i)
  {
    if (netList[i].size() == 1)
    {
      netList[i] = netList.back();
      netList.pop_back();
      --i;
    }
  }
}

inline MULTIPLACER_ERROR MultilevelFramework :: InsertInHeap(list<ClusterData>& clustersDataList)
{
  list<ClusterData>::iterator clustersDataIterator;
  clustersDataIterator = clustersDataList.begin();
  double initScore = clustersDataIterator->score;
  ++clustersDataIterator;
  for (; clustersDataIterator != clustersDataList.end(); ++clustersDataIterator)
  {
    if (clustersDataIterator->score <= initScore)
      break;
  }
  clustersDataList.insert(clustersDataIterator, (*clustersDataList.begin()));
  clustersDataList.pop_front();
  
  return OK;
}

inline MULTIPLACER_ERROR MultilevelFramework :: MergeClusters(const int& firstClusterIdx, const int& secondClusterIdx, vector<Cluster>& clusters, NetList& netList,
                                            vector<ConnectionsList>& currTableOfConnections, double* netAreas)
// the result is stored in firstClusterIdx
{
  ConnectionsList tempVector(currTableOfConnections[firstClusterIdx].size() + currTableOfConnections[secondClusterIdx].size());
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
  copy(clusters[secondClusterIdx].cellIdxs.begin(), clusters[secondClusterIdx].cellIdxs.end(), tempVector.begin() + clusters[firstClusterIdx].cellIdxs.size());
  clusters[firstClusterIdx].cellIdxs.resize(tempVector.size());
  copy(tempVector.begin(), tempVector.end(), clusters[firstClusterIdx].cellIdxs.begin());
  
  //update netList
  int netIdx = 0;
  vector<int>::iterator Iter;
  for (int j = 0; j < static_cast<int>(currTableOfConnections[secondClusterIdx].size()); ++j)
  {
    netIdx = currTableOfConnections[secondClusterIdx][j];
    if (netList[netIdx].size() == 0)
      continue;
    Iter = find(netList[netIdx].begin(), netList[netIdx].end(), secondClusterIdx);
    (*Iter) = firstClusterIdx;
    sort(netList[netIdx].begin(), netList[netIdx].end());
    Iter = unique(netList[netIdx].begin(), netList[netIdx].end());
    netList[netIdx].resize(Iter - netList[netIdx].begin());
    if (netList[netIdx].size() == 2 && (netList[netIdx][0] - netList[netIdx][1] == 0))
    {
      netList[netIdx].clear();
      netAreas[netIdx] = 0;
    }
  }
  double currNetArea = 0.0;
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
  }
  
  clusters[firstClusterIdx].xCoord = (clusters[firstClusterIdx ].xCoord * clusters[firstClusterIdx ].area + 
                                      clusters[secondClusterIdx].xCoord * clusters[secondClusterIdx].area) / 
                                     (clusters[firstClusterIdx ].area + clusters[secondClusterIdx].area);
  clusters[firstClusterIdx].yCoord = (clusters[firstClusterIdx ].yCoord * clusters[firstClusterIdx ].area + 
                                      clusters[secondClusterIdx].yCoord * clusters[secondClusterIdx].area) / 
                                     (clusters[firstClusterIdx ].area + clusters[secondClusterIdx].area);

  clusters[secondClusterIdx].isFake = true;
  
  return OK;
}

inline MULTIPLACER_ERROR MultilevelFramework :: AffinityInterp(const int& firstClusterIdx, const int& secondClusterIdx, vector<Cluster>& clusters, NetList& netList, 
                                       vector<ConnectionsList>& currTableOfConnections, double* netAreas, double& result)
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
    if (tmpIdxsInt[i] == tmpIdxsInt[i+1] && netList[tmpIdxsInt[i]].size() > 1)
      commonNetsIdxs.push_back(tmpIdxsInt[++i]);
  }
  
  for (int i = 0; i < static_cast<int>(commonNetsIdxs.size()); ++i)
  {
    netIdx = commonNetsIdxs[i];
    result += 1 / ((netList[netIdx].size() - 1));
  }
  
  delete[] tmpIdxsInt;
  return OK;
}

MULTIPLACER_ERROR MultilevelFramework :: CalculateScore(const int& currClusterIdx, vector<ConnectionsList>& currTableOfConnections, NetList& netList, 
                                             double* netAreas, vector<Cluster>& clusters, double& score, int& bestNeighborIdx, bool isToMark,
                                             affinityFunc Affinity)
{
  int netIdx;
  double result;
  int neighborIdx;
  
  score = 0.0;
  for (int j = 0; j < static_cast<int>(currTableOfConnections[currClusterIdx].size()); ++j)
  {
    netIdx = currTableOfConnections[currClusterIdx][j];
    for (int k = 0; k < static_cast<int>(netList[netIdx].size()); ++k)
    {
      neighborIdx = netList[netIdx][k];
      if (neighborIdx == currClusterIdx)
        continue;
      Affinity(currClusterIdx, neighborIdx, clusters, netList, currTableOfConnections, netAreas, result);
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

void MultilevelFramework :: CreateTableOfConnections(vector<Cluster>& clusters, vector<ConnectionsList>& currTableOfConnections,
                                                     NetList& netList, const int& nNodes)
{
  int cellIdx;

  currTableOfConnections.clear();
  currTableOfConnections.resize(clusters.size());
  for (int i = 0; i < static_cast<int>(netList.size()); ++i)
  {
    for (int j = 0; j < static_cast<int>(netList[i].size()); ++j)
    {
      cellIdx = netList[i][j];
      if (clusters[cellIdx].isFake == true)
        continue;
      // do not consider terminals
      if (IsNotTerminal(cellIdx))
        currTableOfConnections[cellIdx].push_back(i);
    }
  }
}

MULTIPLACER_ERROR MultilevelFramework :: Affinity(const int& firstClusterIdx, const int& secondClusterIdx, vector<Cluster>& clusters, NetList& netList, 
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
    if (tmpIdxsInt[i] == tmpIdxsInt[i+1] && netList[tmpIdxsInt[i]].size() > 1)
      commonNetsIdxs.push_back(tmpIdxsInt[++i]);
  }
  
  for (int i = 0; i < static_cast<int>(commonNetsIdxs.size()); ++i)
  {
    netIdx = commonNetsIdxs[i];
    result += 1 / ((netList[netIdx].size() - 1) * clusters[firstClusterIdx].area * clusters[secondClusterIdx].area);
  }
  
  delete[] tmpIdxsInt;
  return OK;
}

double dist(const int& firstClusterIdx, const int& secondClusterIdx, vector<Cluster>& clusters)
{
  return sqrt(pow(clusters[firstClusterIdx].xCoord - clusters[secondClusterIdx].xCoord, 2) + pow(clusters[firstClusterIdx].yCoord - clusters[secondClusterIdx].yCoord, 2));
}

MULTIPLACER_ERROR MultilevelFramework :: AffinitySP(const int& firstClusterIdx, const int& secondClusterIdx, vector<Cluster>& clusters, NetList& netList, 
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
    if (tmpIdxsInt[i] == tmpIdxsInt[i+1] && netList[tmpIdxsInt[i]].size() > 1)
      commonNetsIdxs.push_back(tmpIdxsInt[++i]);
  }
  
  for (int i = 0; i < static_cast<int>(commonNetsIdxs.size()); ++i)
  {
    netIdx = commonNetsIdxs[i];
    result += 1 / ((netList[netIdx].size() - 1) * clusters[firstClusterIdx].area * clusters[secondClusterIdx].area * dist(firstClusterIdx, secondClusterIdx, clusters));
  }
  
  delete[] tmpIdxsInt;
  return OK;
}

double MultilevelFramework :: LogSumExpForClusters(Vec X, int nClusters, NetList* netList,
                                                   vector<Cluster>* clusters, const double& alpha)
{
  int idx;
  double sum = 0.0;
  double logsum1 = 0.0;
  double logsum2 = 0.0;
  double logsum3 = 0.0;
  double logsum4 = 0.0;

  //nClusters = clusters->size();
  
  //PetscPrintf(PETSC_COMM_SELF,"\nEnter LogSumExp func\n");
  PetscInt ni = nClusters*2;
  static PetscInt *idxs = new PetscInt[nClusters];
  static PetscInt *ix = new PetscInt[nClusters*2];
  static PetscInt *lookUpTable = new PetscInt[clusters->size()];
  int j = 0;
  int realIdx = 0;
  for(int i=0; i < nClusters*2; i++)
  {
    ix[i] = i;
  }
  for(int i=0; i < static_cast<int>(clusters->size());i++)
  {
    if (!(*clusters)[i].isFake)
    {
      lookUpTable[i] = j;
      idxs[j++] = i;
    }
    else
      lookUpTable[i] = -1;
  }
  static PetscScalar *coordinates = new PetscScalar[nClusters*2];
  
  //PetscPrintf(PETSC_COMM_SELF,"\nEnter VecGetValues func\n");
  VecGetValues(X, ni, ix, coordinates);
  //PetscPrintf(PETSC_COMM_SELF,"\nExit VecGetValues func\n");

  for (int i = 0; i < static_cast<int>(netList->size()); ++i)
  {
    logsum1 = 0.0;
    logsum2 = 0.0;
    logsum3 = 0.0;
    logsum4 = 0.0;
    //PetscPrintf(PETSC_COMM_SELF,"\n%d\n", i);
    for (int j = 0; j < static_cast<int>((*netList)[i].size()); ++j)
    {
      realIdx = (*netList)[i][j];
      idx = lookUpTable[realIdx];
      //PetscPrintf(PETSC_COMM_SELF,"\n%d\n", idx);
      logsum1 += exp(+(float)coordinates[2*idx+0] / alpha);
      logsum2 += exp(-(float)coordinates[2*idx+0] / alpha);
      logsum3 += exp(+(float)coordinates[2*idx+1] / alpha);
      logsum4 += exp(-(float)coordinates[2*idx+1] / alpha);
    }

    sum += log(logsum1) + log(logsum2) + log(logsum3) + log(logsum4);
  }
  
  //PetscPrintf(PETSC_COMM_SELF,"\nExit LogSumExp func\n");
  return alpha * sum;
}