#include "Utils.h"
#include "Clustering.h"

string ClusteringInformation::GetClusteringInformationFileName(HDesign& hd)
{
    string fileName = hd.cfg.ValueOf(".Clustering.clusteringInformationLoadFileName");
    if (fileName == "")
    {
        fileName = Aux::Format(".\\ClusteringInformation\\%s_%s_%d.ci",
            (const char*)hd.cfg.ValueOf("params.techname", "IWLS"),
            hd.Circuit.Name().c_str(),
            hd.Cells.CellsCount());
    }
    return fileName;
}

bool ClusteringInformation::LoadFromFile(const char* fileName, const char* benchName, HDesign& hd)
{
  FILE* resultFile = fopen(fileName, "r");
  char bmName[128];

  if (resultFile)
  {
    fscanf(resultFile, "%s\n", bmName);
    if (strcmp(bmName, benchName))
    {
      GLOGERROR(LOGINPLACE, "Information in %s doesn't correspond to the design", fileName);
      return false;
    }

    int lClusterRatio;
    int lDesiredFinalNumberOfClusters;
    double lClustersAreaTolerance;
    int lCurrentNumberOfClusters;

    if (0 == fscanf(resultFile, "a %d %d %I64X %d\n",
                    &lClusterRatio, 
                    &lDesiredFinalNumberOfClusters, 
                    &lClustersAreaTolerance,
                    &lCurrentNumberOfClusters)
      || lClusterRatio != mClusterRatio
      || lDesiredFinalNumberOfClusters != mDesiredFinalNumberOfClusters
      || lClustersAreaTolerance != mClustersAreaTolerance)
    {
      GLOGERROR(LOGINPLACE, "Information in %s doesn't correspond to the design", fileName);
      return false;
    }

    mCurrentNumberOfClusters = lCurrentNumberOfClusters;
    LoadClustersFromFile(resultFile, hd);
    LoadClusteringLogFromFile(resultFile);
    LoadNetLevelsFromFile(resultFile, hd);
    LoadCurrTableOfAdjacentNetsFromFile(resultFile);

    fclose(resultFile);
    return true;
  }
  else
  {
    ALERT("Error during opening clustering information import file");
    return false;
  }
}

void ClusteringInformation::LoadClustersFromFile(FILE* rf, HDesign& hd)
{
  char  buffer[256];
  int   clustersCellsSize;
  double area;

  do
  {
    fgets(buffer, 255, rf);
  }while (strcmp(buffer, "Clusters\n"));
  fgets(buffer, 255, rf); // skip '{'

  for (HClusters::ClustersEnumeratorW  cluster = hd.Cluster.GetEnumeratorW(); cluster.MoveNext();)
  //for (unsigned int i = 0; i < clusters.size(); ++i)
  {
    fscanf(rf, "%d : %I64X %s", &clustersCellsSize, &area, buffer);
    cluster.SetArea(area);
    cluster.Cells()->resize(clustersCellsSize);
    if (!strcmp(buffer, "true"))
    {
      cluster.SetIsFake(true);
    }
    else
    {
      cluster.SetIsFake(false);
    }

    for (int j = 0; j < clustersCellsSize; ++j)
    {
      fscanf(rf, "%s", buffer);
      (*cluster.Cells())[j] = Utils::FindCellByName(hd, buffer);
    }
  }

  do
  {
    fgets(buffer, 255, rf);
  }while (strcmp(buffer, "ClustersUnreal\n"));

  int id = 0;
  int countUnreal = 0;
  fscanf(rf, "%s\n", buffer);
  countUnreal = atoi(buffer);
  for (int i = 0; i < countUnreal; i++)
  {
      hd.Cluster.AllocateClusterUnreal();
  }
  for (HClusters::ClustersUnrealEnumeratorW clusterU = hd.Cluster.GetUnrealEnumeratorW(); clusterU.MoveNext();)
  {
    fscanf(rf, "%d : %I64X : %d %s", &clustersCellsSize, &area, &id, buffer);

      clusterU.SetArea(area);
      clusterU.Cells()->resize(clustersCellsSize);
      clusterU.Setid(id);
      if (!strcmp(buffer, "true"))
      {
          clusterU.SetIsFake(true);
      }
      else
      {
          clusterU.SetIsFake(false);
      }
      fscanf(rf, "%s\n", buffer);
      if (!strcmp(buffer, "true"))
      {
          clusterU.SetIsPrimary(true);
      }
      else
      {
          clusterU.SetIsPrimary(false);
      }
      fscanf(rf, "%s\n", buffer);
      if (!strcmp(buffer, "true"))
      {
          clusterU.SetIsTerminals(true);
      }
      else
      {
          clusterU.SetIsTerminals(false);
      }

      for (int j = 0; j < clustersCellsSize; ++j)
      {
          fscanf(rf, "%s", buffer);
          (*clusterU.Cells())[j] = Utils::FindCellByName(hd, buffer);
      }
  }
}

void ClusteringInformation::LoadClusteringLogFromFile(FILE* rf)
{
  char  buffer[256];
  int   nLevels;
  int   nMerges;
  ClusteringLog thisLevelClusteringLog;

  do 
  {
    fscanf(rf, "%s %d", buffer, &nLevels);
  } while (strcmp(buffer, "ClusteringLog"));

  fgets(buffer, 255, rf); // skip '{'
  fgets(buffer, 255, rf); // skip '{'

  for (int levelCounter = 0; levelCounter < nLevels; ++levelCounter)
  {
    ClusteringLog thisLevelClusteringLog;
    fscanf(rf, "%s %d", buffer, &nMerges);
    if (strcmp(buffer, "next_level"))
    {
      LOGCRITICAL("Wrong string in clustering file");
      return;
    }

    for (int j = 0; j < nMerges; ++j)
    {
      MergedCluster mergedCluster;
      int cluster1 = 0;
      int cluster2 = 0;
      fscanf(rf, "\t%d %d %u\n", &cluster1, &cluster2, &mergedCluster.nCellsInCluster1);
      mergedCluster.cluster1Idx = design.Cluster.GetClusterByIndex(cluster1 + 1);
      mergedCluster.cluster2Idx = design.Cluster.GetClusterByIndex(cluster2 + 1);
      thisLevelClusteringLog.push_back(mergedCluster);
    }
    clusteringLog.push_back(thisLevelClusteringLog);
  }
}

void ClusteringInformation::LoadNetListFromFile(FILE* rf, HClusteredNets* nl, HDesign& hd)
{
  char  buffer[256];
  unsigned int netListSize;
  unsigned int clusterIdxsSize;
  int clusterIdx;
  double weight;

  do
  {
    fscanf(rf, "\t%s %u\n", buffer, &netListSize);
  } while (strcmp(buffer, "NetList"));
  nl->Resize(netListSize);
  for (HClusteredNets::ClusteredNetsEnumeratorW i = hd.ClustersNetList.GetEnumeratorW(); i.MoveNext(); )
  //for (unsigned int i = 0; i < netListSize; ++i)
  {
    //while(!fscanf(rf, "\t%u :\n", &clusterIdxsSize));
    do
    {
      fgets(buffer, 128, rf);
    } while (!sscanf(buffer, "\t%u :\n", &clusterIdxsSize));
    i.clusterIdxs()->resize(clusterIdxsSize);

    for (unsigned int j = 0; j < clusterIdxsSize; ++j)
    {
      fscanf(rf, "\t\t%d\n", &clusterIdx);
      //for (HClusters::ClustersEnumeratorW cluster = hd.Cluster.GetEnumeratorW(); cluster.MoveNext();)
      //    if (clusterIdx != ::ToID(cluster))
      //        cluster.MoveNext();
      (*i.clusterIdxs())[j] =  design.Cluster.GetClusterByIndex(clusterIdx + 1);
    }
    fscanf(rf, "\tweight = %I64X\n", &weight);
    i.Setweight(weight);
    //i.weight = weight;
  }
  // This line should be equal to "}; // NetList"
  // we don't need it
  fgets(buffer, 128, rf);
}

void ClusteringInformation::LoadNetLevelsFromFile(FILE* rf, HDesign& hd)
{
  char  buffer[256];
  unsigned int netLevelsSize;
  unsigned int netLevelsCounter;

  do
  {
    fscanf(rf, "\t%s %u\n", buffer, &netLevelsSize);
  } while (strcmp(buffer, "NetLevels"));
  //netLevels.clear();
  hd.NetLevels.GetNetLelev()->clear();
  //hd.NetLevels.Resize(netLevelsSize);
  
  for (netLevelsCounter = 0; netLevelsCounter < netLevelsSize; ++netLevelsCounter)
  {
    HClusteredNets* netList = hd.NetLevels.AllocateNetLevel(); //new HClusteredNets(&hd);
    LoadNetListFromFile(rf, netList, hd);
    //hd.NetLevels.GetNetLelev()->push_back(netList);
    //hd.ClustersNetList = netList;
  }
  // This line should be equal to "}; // NetLevels"
  // we don't need it
  fgets(buffer, 128, rf);
}

void ClusteringInformation::LoadCurrTableOfAdjacentNetsFromFile(FILE* rf)
{
  char  buffer[256];
  unsigned int ctoanSize;
  unsigned int connectionsVectorSize;
  int netIdx;

  do
  {
    fscanf(rf, "%s %u\n", buffer, &ctoanSize);
  } while (strcmp(buffer, "CurrTableOfAdjacentNets"));
  //tableOfAdjacentNets.resize(ctoanSize);

  ASSERT(design.Cluster.ClustersCount() == ctoanSize);

  for (HClusters::ClustersEnumeratorW i = design.Cluster.GetEnumeratorW(); i.MoveNext(); )
  //for (unsigned int i = 0; i < ctoanSize; ++i)
  {
    //while(!fscanf(rf, "\t%u :\n", &clusterIdxsSize));
    do
    {
      fgets(buffer, 128, rf);
    } while (!sscanf(buffer, "%u :\n", &connectionsVectorSize));
    i.tableOfAdjacentNets()->resize(connectionsVectorSize);
    //tableOfAdjacentNets[i].resize(connectionsVectorSize);

    for (unsigned int j = 0; j < connectionsVectorSize; ++j)
    {
      fscanf(rf, "\t%d\n", &netIdx);
      (*i.tableOfAdjacentNets())[j] = design.ClustersNetList.GetClusteredNetByIndex(netIdx);
    }
  }
  // This line should be equal to "}; // NetList"
  // we don't need it
  fgets(buffer, 128, rf);
}

void ClusteringInformation::SaveToFile(const char* fileName, const char* benchName, HDesign& hd)
{
  FILE* resultFile = fopen(fileName, "w");

  if (resultFile)
  {
    fprintf(resultFile, "%s\n", benchName);
    fprintf(resultFile, "a %d %d %I64X %d\n", mClusterRatio, mDesiredFinalNumberOfClusters, 
      mClustersAreaTolerance, mCurrentNumberOfClusters);
    SaveClustersToFile(resultFile, hd);
    SaveClusteringLogToFile(resultFile);
    SaveNetLevelsToFile(resultFile);
    SaveCurrTableOfAdjacentNetsToFile(resultFile);

    fclose(resultFile);
  }
  else
  {
    LOGCRITICAL("Error during opening clustering information export file");
  }
}

void ClusteringInformation::SaveClustersToFile(FILE* rf, HDesign& hd)
{
  unsigned int clustersSize = hd.Cluster.ClustersCount();
  unsigned int clustersCellsSize;

  fprintf(rf, "\nClusters\n{\n");

  for (HClusters::ClustersEnumeratorW cluster = hd.Cluster.GetEnumeratorW(); cluster.MoveNext();)
  {
    clustersCellsSize = cluster.Cells()->size();

    fprintf(rf, "%u : %I64X", clustersCellsSize, cluster.Area());

    if (cluster.IsFake())
      fprintf(rf, " true\n");
    else
      fprintf(rf, " false\n");

    for (unsigned int j = 0; j < clustersCellsSize; ++j)
    {
      fprintf(rf, "\t%s\n", hd[(*cluster.Cells())[j]].Name().c_str());
      //std::cout << clusters[i].cells[j].Name << std::endl;
    }
  }
  int countUnreal = 0;
  for (HClusters::ClustersUnrealEnumeratorW clusterU = hd.Cluster.GetUnrealEnumeratorW(); clusterU.MoveNext();)
      countUnreal++;
  fprintf(rf, "\nClustersUnreal\n");
  fprintf(rf, "%d\n", countUnreal);
  for (HClusters::ClustersUnrealEnumeratorW clusterU = hd.Cluster.GetUnrealEnumeratorW(); clusterU.MoveNext();)
  {
      clustersCellsSize = clusterU.Cells()->size();

      fprintf(rf, "%u : %I64X : %u %", clustersCellsSize, clusterU.Area(), clusterU.id());

      if (clusterU.IsFake())
          fprintf(rf, " true\n");
      else
          fprintf(rf, " false\n");

      if (clusterU.IsPrimary())
          fprintf(rf, " true\n");
      else
          fprintf(rf, " false\n");

      if (clusterU.IsTerminals())
          fprintf(rf, " true\n");
      else
          fprintf(rf, " false\n");   

      for (unsigned int j = 0; j < clustersCellsSize; ++j)
      {
            fprintf(rf, "\t%s\n", hd[(*cluster.Cells())[j]].Name().c_str()); 
          //std::cout << clusters[i].cells[j].Name << std::endl;
      }
  }

  fputs("}; // Clusters\n", rf);
}

void ClusteringInformation::SaveClusteringLogToFile(FILE* rf)
{
  unsigned int clusteringLogSize = clusteringLog.size();
  std::list<ClusteringLog>::iterator clustersLogIterator;

  fprintf(rf, "\nClusteringLog %u\n{\n", clusteringLogSize);

  for (clustersLogIterator = clusteringLog.begin(); clustersLogIterator != clusteringLog.end(); ++clustersLogIterator)
  {
    fprintf(rf, "next_level %u\n", clustersLogIterator->size());
    ClusteringLog::iterator cii; // clusteringInfoIterator
    for (cii = clustersLogIterator->begin();
      cii != clustersLogIterator->end(); ++cii)
    {
        fprintf(rf, "\t%d %d %u\n", ::ToID(cii->cluster1Idx) - 1, ::ToID(cii->cluster2Idx) - 1, cii->nCellsInCluster1);
    }
  }

  fputs("}; // ClusteringLog\n", rf);
}

void ClusteringInformation::SaveNetListToFile(FILE* rf, HClusteredNets* nl)
{
  unsigned int netListSize = nl->ClusteredNetCount();
  unsigned int clusterIdxsSize;

  fprintf(rf, "\tNetList %u\n\t{\n", netListSize);
  for (HClusteredNets::ClusteredNetsEnumeratorW i = design.ClustersNetList.GetEnumeratorW(); i.MoveNext(); )
  //for (unsigned int i = 0; i < netListSize; ++i)
  {
    clusterIdxsSize = i.clusterIdxs()->size();
    fprintf(rf, "\t%u :\n", clusterIdxsSize);

    for (unsigned int j = 0; j < clusterIdxsSize; ++j)
    {
        fprintf(rf, "\t\t%d\n", ::ToID((*i.clusterIdxs())[j]) - 1);
    }
    fprintf(rf, "\tweight = %I64X\n", i.weight());
  }

  fputs("\t}; // NetList\n", rf);
}

void ClusteringInformation::SaveNetLevelsToFile(FILE* rf)
{
  std::vector<HClusteredNets*>::iterator netLevelsIterator;


  fprintf(rf, "\nNetLevels %u\n{\n", design.NetLevels.GetNetLelev()->size());

  for (netLevelsIterator = design.NetLevels.GetNetLelev()->begin(); netLevelsIterator != 
      design.NetLevels.GetNetLelev()->end(); ++netLevelsIterator)
  {
    SaveNetListToFile(rf, *netLevelsIterator);
  }

  fputs("}; // NetLevels\n", rf);
}

void ClusteringInformation::SaveCurrTableOfAdjacentNetsToFile(FILE* rf)
{
  unsigned int ctoanSize = design.Cluster.ClustersCount();// tableOfAdjacentNets.size();
  unsigned int connectionsVectorSize;

  fprintf(rf, "\nCurrTableOfAdjacentNets %u\n{\n", ctoanSize);


  for (HClusters::ClustersEnumeratorW i = design.Cluster.GetEnumeratorW(); i.MoveNext(); )
  //for (unsigned int i = 0; i < ctoanSize; ++i)
  {
    connectionsVectorSize = i.tableOfAdjacentNets()->size();
    fprintf(rf, "%u :\n", connectionsVectorSize);

    for (unsigned int j = 0; j < connectionsVectorSize; ++j)
    {
      fprintf(rf, "\t%d\n", (*i.tableOfAdjacentNets())[j]);
    }
  }

  fputs("}; // CurrTableOfAdjacentNets\n", rf);
}