/*  
 * global_placement.cpp
 * this is a part of itlDragon
 * Copyright (C) 2005-2008, ITLab, Aleksey Bader, Kirill Kornyakov, Zhivoderov Artem
 * email: bader@newmail.ru
 * email: kirillkornyakov@yandex.ru
 * email: zhivoderov.a@gmail.com
 */

#include "..\include\global_placement.h"
#include "..\include\output.h"
#include "..\include\optimal_region_determination.h"
#include "math.h"
#include <conio.h>

#include <iostream>   // for output additional information
#include <time.h>     // for timing
#include <stdlib.h>   // for random numbers
#include <algorithm>

using namespace MultilevelFramework;

MULTIPLACER_ERROR GlobalPlacement(Circuit& circuit)
{
  vector<Cluster> clusters(circuit.nNodes);
  NetList netList(circuit.nNets);    // vector of nets of clusters (cluster indexes are stored in netList)
  vector<ConnectionsList> currTableOfConnections(clusters.size());
  list<ClusteringInfoAtEachLevel> clusteringLog;  // здесь сохраняется информация, 
                                                  // необходимая для перехода на предыдущие уровни кластеризации
  ClusteringLogIterator clusteringLogIterator;    // reverse_iterator по clusteringLog'у
  list<NetList> netLevels;  // список из нетлистов на каждом уровне кластеризации
  list<NetList>::reverse_iterator netLevelsIterator;

  // initial filling
  InitializeDataStructures(circuit, clusters, netList, circuit.nNodes);
  netLevels.push_back(netList);
  
  // First V-cycle
  Clusterize(circuit, clusters, netList, currTableOfConnections, netLevels, Affinity, clusteringLog);
  cout << netList[2][1] << endl;
  Relaxation(circuit, clusters, netList);
  PrintToTmpPL(circuit);
  double currentWL = cf_recalc_all(UPDATE_NETS_WLS, circuit.nNets, circuit.nets, circuit.placement);
  cout << "currWL on the coarsest level = " << currentWL << endl;

  netLevelsIterator = netLevels.rbegin();
  clusteringLogIterator = clusteringLog.rbegin();
  if (netLevelsIterator != netLevels.rend()) ++netLevelsIterator;

  for (; clusteringLogIterator != clusteringLog.rend(); ++clusteringLogIterator, ++netLevelsIterator)
  {
    UnclusterLevelUp(clusters, currTableOfConnections, (*netLevelsIterator), clusteringLogIterator, circuit.nNodes);
    Interpolation(circuit, clusters, currTableOfConnections, (*netLevelsIterator));
    PrintToTmpPL(circuit);
    //Relaxation(circuit, clusters);
    currentWL = cf_recalc_all(UPDATE_NETS_WLS, circuit.nNets, circuit.nets, circuit.placement);
    cout << "currWL = " << currentWL << endl;
  }
  
  // Second V-cycle
  netList = netLevels.front();
  netLevels.resize(1);
  clusteringLog.clear();
  Clusterize(circuit, clusters, netList, currTableOfConnections, netLevels, AffinitySP, clusteringLog);
  UpdateCoords(circuit, clusters);
  PrintToTmpPL(circuit);

  netLevelsIterator = netLevels.rbegin();
  clusteringLogIterator = clusteringLog.rbegin();
  if (netLevelsIterator != netLevels.rend()) ++netLevelsIterator;

  for (; clusteringLogIterator != clusteringLog.rend(); ++clusteringLogIterator, ++netLevelsIterator)
  {
    UnclusterLevelUp(clusters, currTableOfConnections, (*netLevelsIterator), clusteringLogIterator, circuit.nNodes);
    Interpolation(circuit, clusters, currTableOfConnections, (*netLevelsIterator));
    PrintToTmpPL(circuit);
    //Relaxation(circuit, clusters);
    currentWL = cf_recalc_all(UPDATE_NETS_WLS, circuit.nNets, circuit.nets, circuit.placement);
    cout << "currWL = " << currentWL << endl;
  }

  return OK;
}
