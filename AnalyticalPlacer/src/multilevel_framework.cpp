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

void MultilevelFramework :: SetInitialState(vector<Cluster>& clusters, Circuit& circuit, const int& numOfClusters)
{
  float* rndCoords = new float[2 * numOfClusters];
  GetFloatRandomNumbers(rndCoords, numOfClusters * 2, 0.0, 1.0);
  /*GetFloatRandomNumbers(rndCoords, numOfClusters / 2, 0.0, 1.0);
  GetFloatRandomNumbers(rndCoords + numOfClusters / 2, 1.5 * numOfClusters, 0.0, 0.2);*/

  int idx = 0;
  for (int i = 0; i < static_cast<int>(clusters.size()); ++i)
  {
    if (clusters[i].isFake == false)
    {
      clusters[i].xCoord = circuit.width  * rndCoords[2*idx+0];
      clusters[i].yCoord = circuit.height * rndCoords[2*idx+1];
      idx++;
    }
  }

  delete [] rndCoords;
}

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

  for (int i = 0; i < static_cast<int>(clusters.size()); ++i)
  {
    clustersData[i].clusterIdx = i;
    clustersData[i].score      = 0.0;
    
    if (clusters[i].isFake == true)
      continue;
    
    CalculateScore(i, currTableOfConnections, netList, 
                   NULL, clusters, clustersData[i].score, clustersData[i].bestNeighborIdx, false, AffinityInterp, nNodes);
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
  float alpha;
  PetscInt* lookUpTable;
  vector<ConnectionsList>* currTableOfConnections;
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

int AnalyticalObjectiveAndGradient(TAO_APPLICATION taoapp, Vec X, double *f, Vec G, void *ptr)
{
  AppCtx *user = (AppCtx*)ptr;
  int    info;
  PetscScalar *x,*g;

  /* Get pointers to vector data */
  info = VecGetArray(X, &x); CHKERRQ(info);
  info = VecGetArray(G, &g); CHKERRQ(info);

  *f = LogSumExpForClusters(x, user);
  //cout << "before GradientLogSumExpForClusters" << endl;
  GradientLogSumExpForClusters(x, g, user);
  //cout << "after GradientLogSumExpForClusters" << endl;
  //*f = TestObjectiveFunc(x, user);

/* Restore vectors */
  info = VecRestoreArray(X,&x); CHKERRQ(info);
  info = VecRestoreArray(G,&g); CHKERRQ(info);
  //info = PetscLogFlops(nn*15); CHKERRQ(info);
  return 0;
}

MULTIPLACER_ERROR MultilevelFramework :: Relaxation(Circuit& circuit, vector<Cluster>& clusters, NetList& netList,
                                                    vector<ConnectionsList>& currTableOfConnections)
{
  int numOfClusters = static_cast<int>(clusters.size());

  for (int i = 0; i < static_cast<int>(clusters.size()); ++i)
  {
    if (clusters[i].isFake == true)
      --numOfClusters;
  }

  static PetscInt *lookUpTable = new PetscInt[clusters.size()]; //todo: move this memory work from function
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

  /* Initialize problem parameters */
  user.circuit = &circuit;
  user.clusters = &clusters;
  user.netList = &netList;
  user.numOfClusters = numOfClusters;
  user.lookUpTable = lookUpTable;
  user.alpha = 100; //todo: make this correct parameter
  user.currTableOfConnections = &currTableOfConnections;

  /* Check for command line arguments to override defaults */
  //info = PetscOptionsGetInt(PETSC_NULL,"-n",&user.n,&flg); CHKERRQ(info);
  //info = PetscOptionsGetReal(PETSC_NULL,"-alpha",&user.alpha,&flg); CHKERRQ(info);

  /* Allocate vectors for the solution and gradient */
  info = VecCreateSeq(PETSC_COMM_SELF, numOfClusters * 2, &x); CHKERRQ(info);

  /* The TAO code begins here */
  /* Create TAO solver with desired solution method */
  info = TaoCreate(PETSC_COMM_SELF, "tao_cg", &tao); CHKERRQ(info);
  info = TaoApplicationCreate(PETSC_COMM_SELF,&taoapp); CHKERRQ(info);

  /* Set solution vec and an initial guess */
  int idx = 0;  
  PetscScalar* initValues = new PetscScalar[2 * numOfClusters];
  for (int i = 0; i < static_cast<int>(clusters.size()); ++i)
  {
    if (clusters[i].isFake == false)
    {
      initValues[2*idx+0] = clusters[i].xCoord;
      initValues[2*idx+1] = clusters[i].yCoord;
      idx++;
    }
  }

  int* idxs = new int[2 * numOfClusters];
  for (int i = 0; i < 2 * numOfClusters; ++i)
  {
    idxs[i] = i;
  }

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
  {
    PetscPrintf(MPI_COMM_WORLD,"Try a different TAO method, adjust some parameters, or check the function evaluation routines\n");
    PetscPrintf(MPI_COMM_WORLD,"reason = %d\n", reason);
  }

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
  //PrintToTmpPL(circuit);


  /* Free TAO data structures */
  info = TaoDestroy(tao); CHKERRQ(info);
  info = TaoAppDestroy(taoapp); CHKERRQ(info);

  /* Free PETSc data structures */
  info = VecDestroy(x); CHKERRQ(info);

  /* Finalize TAO */
  TaoFinalize();
  PetscFinalize();

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
                                                    list<NetList>& netLevels, affinityFunc Affinity, 
                                                    list<ClusteringInfoAtEachLevel>& clusteringLog, int& currNClusters)
{  
  int i = 0;
  
  int targetNumOfClusters;
  int netIdx = 0;
  int currClusterIdx = 0;
  int numOfClusters = 0;
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
  double min, max;

  int bestNeighborIdx;
  int mergeCount = 0;
  ClusteredNodes buffer;

  int nNodes = circuit.nNodes;

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
    
    //targetNumOfClusters = min(targetNumOfClusters, FINAL_NCLUSTERS);

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
    
    for (int i = 0; i < static_cast<int>(clusters.size()); ++i)
    {
      clustersData[i].clusterIdx = i;
      clustersData[i].score      = 0.0;
      
      if (clusters[i].isFake == true)
        continue;
      
      CalculateScore(i, currTableOfConnections, netList, 
                     netAreas, clusters, clustersData[i].score, clustersData[i].bestNeighborIdx,
                     DONT_MARK_NEIGHBORS_INVALID, Affinity, nNodes);
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
      CalculateScore(currClusterIdx, currTableOfConnections, netList, 
                     netAreas, clusters, clustersDataIterator->score, clustersDataIterator->bestNeighborIdx,
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
    currNClusters = numOfClusters;
    
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
      //if (IsNotTerminal(cellIdx))
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

MULTIPLACER_ERROR MultilevelFramework :: CalculateScore(const int& currClusterIdx,
                                                        vector<ConnectionsList>& currTableOfConnections, NetList& netList, 
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
    for (int k = 0; k < static_cast<int>(netList[netIdx].size()); ++k)
    {
      neighborIdx = netList[netIdx][k];
      if (neighborIdx == currClusterIdx || !IsNotTerminal(currClusterIdx) || !IsNotTerminal(neighborIdx))
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
      if (IsNotTerminal(cellIdx))
      {
        if (clusters[cellIdx].isFake == true)
          continue;
        currTableOfConnections[cellIdx].push_back(i);
      }
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

double MultilevelFramework :: LogSumExpForClusters(PetscScalar *coordinates, void* data)
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
    for (int j = 0; j < static_cast<int>((*userData->netList)[i].size()); ++j)
    {
      realClusterIdx = (*userData->netList)[i][j];
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

    sum += log(logsum1) + log(logsum2) + log(logsum3) + log(logsum4);
  }
  
  //PetscPrintf(PETSC_COMM_SELF,"\nExit LogSumExp func\n");
  return alpha * sum;
}

void MultilevelFramework :: GradientLogSumExpForClusters(PetscScalar *coordinates, PetscScalar *grad, void* data)
{
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

  for (int i = 0; i < userData->numOfClusters; ++i, ++clusterIdx)
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
      for (int k = 0; k < static_cast<int>((*userData->netList)[netIdx].size()); ++k)
      {
        //PetscPrintf(PETSC_COMM_SELF,"\nk = %d\t", k);
        realClusterIdx = (*userData->netList)[netIdx][k];
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
      grad[2*i+0] += exp(+coordinates[2*i+0] / alpha) / sum1 - 
                     exp(-coordinates[2*i+0] / alpha) / sum2;
      grad[2*i+1] += exp(+coordinates[2*i+1] / alpha) / sum3 - 
                     exp(-coordinates[2*i+1] / alpha) / sum4;
    }
  }
}

double MultilevelFramework :: TestObjectiveFunc(PetscScalar *coordinates, void* data)
{
  double sum = 0.0;
  int realClusterIdx;
  int clusterIdxInCoordinatesArray;

  AppCtx* userData = (AppCtx*)data;

  for (int i = 0; i < static_cast<int>(userData->netList->size()); ++i)
  {
    for (int j = 0; j < static_cast<int>((*userData->netList)[i].size()); ++j)
    {
      realClusterIdx = (*userData->netList)[i][j];
      clusterIdxInCoordinatesArray = (userData->lookUpTable)[realClusterIdx];
      
      sum += pow((coordinates[clusterIdxInCoordinatesArray] - userData->circuit->height / 2), 2);
    }
  }

  return sum;
}