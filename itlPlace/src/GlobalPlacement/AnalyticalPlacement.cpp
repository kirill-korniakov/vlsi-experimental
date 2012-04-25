#include "WeightsRoutines.h"
#include "ObjectivesConstraints.h"
#include "PlacementQualityAnalyzer.h"
#include "AnalyticalPlacement.h"
#include "AnalyticalPlacementReporting.h"
#include "GPBuffering.h"

using namespace std;

timetype expTime = 0;
timetype lseTime = 0;
timetype lseGradTime = 0;
timetype calcPotentialsTime = 0;
timetype quadraticSpreading = 0;
timetype quadraticSpreadingGradTime = 0;

using namespace AnalyticalGlobalPlacement;

void GlobalPlacement(HDesign& hd, std::string cfgContext)
{
    ConfigContext ctx = hd.cfg.OpenContext(cfgContext);

    WRITELINE("");
    ALERT("ANALYTICAL PLACEMENT STARTED");

    ClusteringInformation ci(hd);
    //ci.affinityFunction = Affinity;

    Clustering(hd, ci);

    ConfigContext ctx1 = hd.cfg.OpenContext("GlobalPlacement");

    //set initial placement
    if (hd.cfg.ValueOf(".placeToTheCenter", false))
    {
        PlaceToTheCenterIntially(hd, ci);
    }
    else
    {
        WriteCellsCoordinates2Clusters(hd, ci);
    }

    //perform placement of clustered netlist
    Relaxation(hd, ci, 1);

    ClusteringLogIterator clusteringLogIterator = ci.clusteringLog.rbegin();
    std::vector<HClusteredNets*>::reverse_iterator netLevelsIterator = hd.NetLevels.GetNetLelev()->rbegin();
    if (netLevelsIterator != hd.NetLevels.GetNetLelev()->rend()) 
        ++netLevelsIterator;

    int metaIterationNumber = 2;
    for (; clusteringLogIterator != ci.clusteringLog.rend(); ++clusteringLogIterator, ++netLevelsIterator)
    {
        hd.ClustersNetList = *(*netLevelsIterator);
        UnclusterLevelUp(hd, ci, clusteringLogIterator);
        Interpolation(hd, ci);
        Relaxation(hd, ci, metaIterationNumber);
        metaIterationNumber++;
    }

    WRITELINE("");
    ALERT("ANALYTICAL PLACEMENT FINISHED");
}

int TaoInit(const char* taoCmd)
{
    int argc = 1;
    char** argv = 0;

    //calculate number of arguments
    if (strlen(taoCmd)) 
    {
        argc++;  
        char* ptr = (char*)taoCmd;
        while(ptr = strchr(ptr+1, ' '))
            argc++;
    }

    //initialize array of arguments
    argv = new char*[argc+1];
    int i = 0;

    char path[_MAX_PATH];
    GetModuleFileName(NULL, path, _MAX_PATH);
    argv[i++] = path;

    char seps[] = " \n";
    char *token;  
    token = strtok((char*)taoCmd, seps);  
    while (token != NULL)
    {
        argv[i++] = token;
        token = strtok(NULL, seps);
    }
    argv[i] = NULL;

    PetscInitialize(&argc, &argv, (char *)0, NULL);
    TaoInitialize(&argc, &argv, (char *)0, NULL);

    return OK;
}

void AnalyticalGlobalPlacement::PlaceToTheCenterIntially(HDesign& hd, ClusteringInformation& ci)
{
    ALERT("SET INITIAL STATE");

    double minX = hd.Circuit.PlacementMinX();
    double maxX = hd.Circuit.PlacementMaxX();
    double minY = hd.Circuit.PlacementMinY();
    double maxY = hd.Circuit.PlacementMaxY();
    double shufflePercent = hd.cfg.ValueOf(".shufflePercent", 0.0);

    int clusterIdx = 0;
    /*for (HClusters::ClustersEnumeratorW cluster = ci.design.Cluster.GetEnumeratorW(); cluster.MoveNext(); clusterIdx++)
    {
        while (cluster.IsFake())
        {
            cluster.MoveNext();
            clusterIdx++;
        }*/
    for (HClusters::ClustersNotFakeEnumeratorW cluster = ci.design.Cluster.GetNotFakeEnumeratorW(); cluster.MoveNext(); clusterIdx++)
    {
        cluster.SetXCoord((minX + maxX) / 2.0);
        cluster.SetXCoord(cluster.XCoord() + 2.0*((double)rand()/(double)RAND_MAX - 0.5)*
            shufflePercent/100.0*hd.Circuit.PlacementWidth());
        cluster.SetYCoord( (minY + maxY) / 2.0);
        cluster.SetYCoord(cluster.YCoord() + 2.0*((double)rand()/(double)RAND_MAX - 0.5)*
            shufflePercent/100.0*hd.Circuit.PlacementHeight());
    }
    UpdateCellsCoordinates(hd, ci);

    //int netListSize = static_cast<int>(ci.netList.size());
    //for (int i = 0; i < netListSize; i++)
    for (HClusteredNets::ClusteredNetsEnumeratorW i = ci.design.ClustersNetList.GetEnumeratorW(); i.MoveNext(); )
    {
        i.Setk(0.0);
    }
}

int* AnalyticalGlobalPlacement::InitIdxs(int nVariables, int shift)
{
    int* idxs = new int[nVariables];
    for (int i = 0; i < nVariables; ++i)
    {
        idxs[i] = shift + i;
    }
    return idxs;
}

void AnalyticalGlobalPlacement::SetClustersCoords(ClusteringInformation& ci, Vec& x)
{
    PetscScalar* initValues = new PetscScalar[2*ci.mCurrentNumberOfClusters];
    int idx = 0;
    int clusterIdx = 0;
    //for (HClusters::ClustersEnumeratorW cluster = ci.design.Cluster.GetEnumeratorW(); cluster.MoveNext(); clusterIdx++)
    //{
    //    while (cluster.IsFake())
    //    {
    //        cluster.MoveNext();
    //        clusterIdx++;
    //    }
    for (HClusters::ClustersNotFakeEnumeratorW cluster = ci.design.Cluster.GetNotFakeEnumeratorW(); cluster.MoveNext(); clusterIdx++)
    {
        initValues[2*idx+0] = cluster.XCoord();
        initValues[2*idx+1] = cluster.YCoord();
        idx++;
    }

    int* idxs = InitIdxs(2*ci.mCurrentNumberOfClusters, 0);

    VecSetValues(x, 2*ci.mCurrentNumberOfClusters, idxs, initValues, INSERT_VALUES);

    delete[] initValues;
    delete[] idxs;
}

void AnalyticalGlobalPlacement::SetVariablesValues(ClusteringInformation & ci, Vec& x)
{
    SetClustersCoords(ci, x);
    SetKValues(ci, x);
}

void AnalyticalGlobalPlacement::SetBounds(HDesign& hd, ClusteringInformation& ci, AppCtx& context, Vec& xl, Vec& xu)
{
    PetscScalar* initValues = new PetscScalar[context.nVariables];
    int* idxs = InitIdxs(context.nVariables, 0);
    int idx;
    int clusterIdx;

    double xMargin = context.sprData.binGrid.binWidth;
    double yMargin = context.sprData.binGrid.binHeight;

    //TODO: change borders according cluster sizes, we have to calculate width
    //and height of each cluster and shift slightly borders for each cluster
    idx = 0;
    clusterIdx = 0;
    //for (HClusters::ClustersEnumeratorW cluster = ci.design.Cluster.GetEnumeratorW(); cluster.MoveNext(); clusterIdx++)
    //{
    //    while (cluster.IsFake())
    //    {
    //        cluster.MoveNext();
    //        clusterIdx++;
    //    }
    for (HClusters::ClustersNotFakeEnumeratorW cluster = ci.design.Cluster.GetNotFakeEnumeratorW(); cluster.MoveNext(); clusterIdx++)
    {
        initValues[2*idx+0] = hd.Circuit.PlacementMinX() + xMargin;
        initValues[2*idx+1] = hd.Circuit.PlacementMinY() + yMargin;
        idx++;
    }
    for (int i = 2*idx; i < context.nVariables; i++)
    {//set lower borders for ki variables
        initValues[i] = 0.0;
    }
    VecSetValues(xl, context.nVariables, idxs, initValues, INSERT_VALUES);

    idx = 0;
    clusterIdx = 0;
    /*for (HClusters::ClustersEnumeratorW cluster = ci.design.Cluster.GetEnumeratorW(); cluster.MoveNext(); clusterIdx++)
    {
        while (cluster.IsFake())
        {
            cluster.MoveNext();
            clusterIdx++;
        }*/
    for (HClusters::ClustersNotFakeEnumeratorW cluster = ci.design.Cluster.GetNotFakeEnumeratorW(); cluster.MoveNext(); clusterIdx++)
    {
        initValues[2*idx+0] = hd.Circuit.PlacementMaxX() - xMargin;
        initValues[2*idx+1] = hd.Circuit.PlacementMaxY() - yMargin;
        idx++;
    }
    for (int i = 2*idx; i < context.nVariables; i++)
    {//set upper borders for ki variables
        initValues[i] = hd.cfg.ValueOf(".bufferCountUpperBound", 100.0); //TODO: set correct upper bound
    }
    VecSetValues(xu, context.nVariables, idxs, initValues, INSERT_VALUES);

    delete [] initValues;
    delete [] idxs;
}

void AnalyticalGlobalPlacement::GetClusterCoordinates(ClusteringInformation& ci, Vec x)
{
    PetscScalar* values = new PetscScalar[2*ci.mCurrentNumberOfClusters];
    int* idxs = InitIdxs(2*ci.mCurrentNumberOfClusters, 0);

    VecGetValues(x, PetscInt(2*ci.mCurrentNumberOfClusters), idxs, values);
    int idx = 0;
    int clusterIdx = 0;
    //for (HClusters::ClustersEnumeratorW cluster = ci.design.Cluster.GetEnumeratorW(); cluster.MoveNext(); clusterIdx++)
    //{
    //    while (cluster.IsFake())
    //    {
    //        cluster.MoveNext();
    //        clusterIdx++;
    //    }
    for (HClusters::ClustersNotFakeEnumeratorW cluster = ci.design.Cluster.GetNotFakeEnumeratorW(); cluster.MoveNext(); clusterIdx++)
    {
        cluster.SetXCoord(values[2*idx+0]);
        cluster.SetYCoord(values[2*idx+1]);
        idx++;
    }

    delete[] values;
    delete[] idxs;
}

void AnalyticalGlobalPlacement::GetVariablesValues(ClusteringInformation& ci, Vec x)
{
    GetClusterCoordinates(ci, x);
    GetKValues(ci, x);
}

void AnalyticalGlobalPlacement::UpdateCellsCoordinates(HDesign& hd, ClusteringInformation& ci)
{
    int clusterIdx = 0;
    //for (HClusters::ClustersEnumeratorW cluster = ci.design.Cluster.GetEnumeratorW(); cluster.MoveNext(); clusterIdx++)
    //{
    //    while (cluster.IsFake())
    //    {
    //        cluster.MoveNext();
    //        clusterIdx++;
    //    }
    for (HClusters::ClustersNotFakeEnumeratorW cluster = ci.design.Cluster.GetNotFakeEnumeratorW(); cluster.MoveNext(); clusterIdx++)
    {
        //for (int j = 0; j < static_cast<int>(cluster.Cells()->size()); ++j)
        for (std::vector<HCell>::iterator cellIter = cluster.Cells()->begin(); cellIter <  cluster.Cells()->end(); cellIter++)
        {
            HCell cell = *cellIter; //Utils::FindCellByName(hd, ci.clusters[clusterIdx].cells[j]);
            hd.Set<HCell::X>(cell, cluster.XCoord() - hd.GetDouble<HCell::Width>(cell)/2.0);
            hd.Set<HCell::Y>(cell, cluster.YCoord() - hd.GetDouble<HCell::Height>(cell)/2.0);
        }
    }
}

void AnalyticalGlobalPlacement::WriteCellsCoordinates2Clusters(HDesign& hd, ClusteringInformation& ci)
{
    int clusterIdx = 0;
    //for (HClusters::ClustersEnumeratorW cluster = ci.design.Cluster.GetEnumeratorW(); cluster.MoveNext(); clusterIdx++)
    //{
    //    while (cluster.IsFake())
    //    {
    //        cluster.MoveNext();
    //        clusterIdx++;
    //    }
    for (HClusters::ClustersNotFakeEnumeratorW cluster = ci.design.Cluster.GetNotFakeEnumeratorW(); cluster.MoveNext(); clusterIdx++)
    {
        HCell cell = *cluster.Cells()->begin(); //Utils::FindCellByName(hd, ci.clusters[clusterIdx].cells[0]);
        cluster.SetXCoord(hd.GetDouble<HCell::X>(cell) + hd.GetDouble<HCell::Width>(cell)/2.0);
        cluster.SetYCoord(hd.GetDouble<HCell::Y>(cell) + hd.GetDouble<HCell::Height>(cell)/2.0);
    }
}

void AnalyticalGlobalPlacement::ExportNetWeights(ClusteringInformation &ci, int &i ) 
{
    FILE* resultFile = fopen("Before relax", "a");
    i = 0;
    for (HClusteredNets::ClusteredNetsEnumeratorW j = ci.design.ClustersNetList.GetEnumeratorW(); j.MoveNext() && i < 10; ++i)
    //for (netListIter = ci.netList.begin(), i = 0; netListIter != ci.netList.end() && i < 10; ++netListIter, ++i)
    {
        fprintf(resultFile, "%f\n", j.weight());
    }
    fprintf(resultFile, "END\n");
    fclose(resultFile);
}

int AnalyticalGlobalPlacement::Interpolation(HDesign& hd, ClusteringInformation& ci)
{
    //TODO: consider terminals during interpolation??
    vector<MergeCandidate> clustersData(hd.Cluster.ClustersCount());
    list<MergeCandidate> clustersDataList;
    list<MergeCandidate>::iterator clustersDataIterator;
    //bool* isCPoint = new bool[hd.Cluster.ClustersCount()];  // C-point is a cluster which location is fixed during interpolation
    double part = 0.5;  // part of ci.clusters to be initially fixed during interpolation (part of C-points)
    HCluster currClusterIdx;
    HCluster neighborIdx;
    HClusteredNet netIdx;
    vector<HCluster> currCPoints;
    double sumX;
    double sumY;
    double area;

    //int* netListSizes = new int[ci.netList.size()]; //TODO: free this memory
    //CalculateNetListSizes(ci.netList, netListSizes);

    CalculateScores(hd, ci, clustersData);
    int clusterIdx = 0;
    //for (HClusters::ClustersEnumeratorW cluster = ci.design.Cluster.GetEnumeratorW(); cluster.MoveNext(); clusterIdx++)
    //{
    //    while (cluster.IsFake())
    //    {
    //        cluster.MoveNext();
    //        clusterIdx++;
    //    }
    for (HClusters::ClustersNotFakeEnumeratorW cluster = ci.design.Cluster.GetNotFakeEnumeratorW(); cluster.MoveNext(); clusterIdx++)
    {
        cluster.SetisCPoint(false);
        //isCPoint[clusterIdx] = false;
    }

    //delete[] netListSizes;
    sort(clustersData.begin(), clustersData.end(), PredicateMergePairGreater);

    for (int i = 0; i < static_cast<int>(hd.Cluster.ClustersCount()); ++i)
    {
        if (clustersData[i].score >= 1.5)
            clustersDataList.push_back(clustersData[i]);
        else
            break;
    }
    clustersDataIterator = clustersDataList.begin();
    for (int i = 0; i < static_cast<int>(clustersDataList.size() * part); ++i)
    {
        //isCPoint[::ToID(clustersDataIterator->clusterIdx) - 1] = true;
        hd[clustersDataIterator->clusterIdx].SetisCPoint(true);
        ++clustersDataIterator;
    }

    for (int i = static_cast<int>(clustersDataList.size() * part); i < static_cast<int>(clustersDataList.size()); ++i)
    {
        currClusterIdx = clustersDataIterator->clusterIdx;
        currCPoints.clear();
        for (int j = 0; j < static_cast<int>(ci.design[currClusterIdx].tableOfAdjacentNets()->size()); ++j)
        {
            netIdx = (*ci.design[currClusterIdx].tableOfAdjacentNets())[j];
            for (int k = 0; k < static_cast<int>(ci.design[netIdx].clusterIdxs()->size()); ++k)
            {
                neighborIdx = (*ci.design[netIdx].clusterIdxs())[k];
                //TODO: probably better to consider terminals (primary pins) also
                if (ci.design[neighborIdx].IsTerminals() || ci.design[neighborIdx].IsPrimary() || (ci.design[neighborIdx].isCPoint() == false))
                    continue;
                // remember all placed (fixed) neighbors of current cluster
                currCPoints.push_back(neighborIdx);
            }
        }
        hd[currClusterIdx].SetisCPoint(true);
        //isCPoint[::ToID(currClusterIdx) - 1] = true;
        if (currCPoints.size() == 0)
            continue;
        sumX = sumY = area = 0.0;
        for (int j = 0; j < static_cast<int>(currCPoints.size()); ++j)
        {
            sumX += ci.design[currCPoints[j]].Area() * ci.design[currCPoints[j]].XCoord();
            sumY += ci.design[currCPoints[j]].Area() * ci.design[currCPoints[j]].YCoord();
            area += ci.design[currCPoints[j]].Area();
        }
        ci.design[currClusterIdx].SetXCoord(sumX / area);
        ci.design[currClusterIdx].SetYCoord(sumY / area);
        ++clustersDataIterator;
    }

    UpdateCellsCoordinates(hd, ci);

    //delete[] isCPoint;

    return OK;
}


int AnalyticalGlobalPlacement::InitializeTAO(HDesign& hd, ClusteringInformation &ci, AppCtx &context, 
                                             Vec& x, Vec& xl, Vec& xu, TAO_SOLVER& tao, TAO_APPLICATION& taoapp)
{
    const char* taoCmd = hd.cfg.ValueOf(".TAOOptions.commandLine");
    TaoInit(taoCmd);

    /* Create TAO solver with desired solution method */
    iCHKERRQ TaoCreate(PETSC_COMM_SELF, hd.cfg.ValueOf(".TAOOptions.method"), &tao);
    iCHKERRQ TaoApplicationCreate(PETSC_COMM_SELF, &taoapp);

    // Allocate vectors for the solution and gradient
    iCHKERRQ VecCreateSeq(PETSC_COMM_SELF, context.nVariables, &x);
    // Set solution vec and an initial guess
    SetVariablesValues(ci, x);

    hd.Plotter->ShowGlobalPlacement(false, context.sprData.binGrid.nBinRows, 
        context.sprData.binGrid.nBinCols, HPlotter::WAIT_1_SECOND);

    iCHKERRQ TaoAppSetInitialSolutionVec(taoapp, x);

    //set bounds
    if (context.useBorderBounds)
    {
        iCHKERRQ VecCreateSeq(PETSC_COMM_SELF, context.nVariables, &xl);
        iCHKERRQ VecCreateSeq(PETSC_COMM_SELF, context.nVariables, &xu);
        SetBounds(hd, ci, context, xl, xu);
        iCHKERRQ TaoAppSetVariableBounds(taoapp, xl, xu);
    }

    iCHKERRQ TaoAppSetObjectiveAndGradientRoutine(taoapp, AnalyticalObjectiveAndGradient, (void*)&context);

    int nInnerIterations = hd.cfg.ValueOf(".TAOOptions.nInnerIterations", 1000);
    iCHKERRQ TaoSetMaximumIterates(tao, nInnerIterations);

    double fatol = hd.cfg.ValueOf(".TAOOptions.fatol", 1.0e-14);
    double frtol = hd.cfg.ValueOf(".TAOOptions.frtol", 1.0e-14);
    double catol = hd.cfg.ValueOf(".TAOOptions.catol", 1.0e-8);
    double crtol = hd.cfg.ValueOf(".TAOOptions.crtol", 1.0e-8);
    iCHKERRQ TaoSetTolerances(tao, fatol, frtol, catol, crtol);

    /* Check for TAO command line options */
    iCHKERRQ TaoSetOptions(taoapp, tao);

    PetscScalar* solution;
    VecGetArray(x, &solution);
    InitWeights(solution, &context);
    VecRestoreArray(x, &solution);

    return OK;
}

int AnalyticalGlobalPlacement::Relaxation(HDesign& hd, ClusteringInformation& ci, int metaIteration)
{
    Vec             x;          // solution vector
    Vec             xl, xu;     // bounds vectors
    AppCtx          context;    // context-defined application context
    TAO_SOLVER      tao;        // TAO_SOLVER solver context
    TAO_APPLICATION taoapp;     // TAO application context

    int i;

    //INITIALIZE OPTIMIZATION PROBLEM PARAMETERS
    context.Initialize(hd, ci);
    iCHKERRQ InitializeTAO(hd, ci, context, x, xl, xu, tao, taoapp);

    ReportIterationInfo(ci, context);
    ReportBinGridInfo(context);   
    ExportNetWeights(ci, i);

    //SOLVE THE PROBLEM
    iCHKERRQ Solve(hd, ci, context, taoapp, tao, x, metaIteration);
    ReportTimes();

    //Free TAO&PETSc data structures
    iCHKERRQ TaoDestroy(tao);
    iCHKERRQ TaoAppDestroy(taoapp);
    iCHKERRQ VecDestroy(x);
    if (context.useBorderBounds)
    {
        iCHKERRQ VecDestroy(xl);
        iCHKERRQ VecDestroy(xu);
    }
    context.FreeMemory();

    return OK;
}

bool IsTimeToExit(HDesign& hd, ClusteringInformation& ci, AppCtx& context, 
                  PlacementQualityAnalyzer* QA, int iteration) 
{
    if (hd.cfg.ValueOf(".useQAClass", false))
    {
        QA->AnalyzeQuality(iteration, &context.criteriaValues, 
            hd.cfg.ValueOf(".improvementTreshold", 0.0));

        if (hd.cfg.ValueOf(".earlyExit", false))
        {
            if (QA->GetNumIterationsWithoutGain() >= hd.cfg.ValueOf(".nTolerantIterations", 2))
            {
                QA->RestoreBestAchievedPlacement();
                WriteCellsCoordinates2Clusters(hd, ci);
                ALERT(Color_Green, "Reached maximum tolerant iteration number.");
                return true;
            }
        }
        if (QA->GetConvergeIterationsNumber() >= hd.cfg.ValueOf(".nConvergedIterations", 2))
        {
            QA->RestoreBestAchievedPlacement();
            WriteCellsCoordinates2Clusters(hd, ci);
            ALERT(Color_Green, "Method converged");
            return true;
        }
    }

    const int nOuterIters = hd.cfg.ValueOf(".TAOOptions.nOuterIterations", 32);
    if (iteration >= nOuterIters)
    {
        ALERT(Color_Green, "Iterations finished");
        return true;
    }

    return false;
}

int AnalyticalGlobalPlacement::Solve(HDesign& hd, ClusteringInformation& ci, AppCtx& context, 
                                     TAO_APPLICATION taoapp, TAO_SOLVER tao, Vec x, int metaIteration)
{
    PlacementQualityAnalyzer* QA = 0;
    if(hd.cfg.ValueOf(".useQAClass", false))
    {
        Vec g;
        iCHKERRQ VecCreateSeq(PETSC_COMM_SELF, context.nVariables, &g);
        AnalyticalObjectiveAndGradient(taoapp, x, &context.criteriaValues.objective, g, &context);

        QA = new PlacementQualityAnalyzer(hd, hd.cfg.ValueOf(".QAcriteria",
            PlacementQualityAnalyzer::GetMetricName(PlacementQualityAnalyzer::MetricHPWLleg)));
        QA->AnalyzeQuality(0, &context.criteriaValues);
    }

    int iteration = 1;
    while (1)
    {
        ReportPreIterationInfo(hd, context, metaIteration, iteration);
        //FIXME: finish the try to implement multiple spreading grids
        //ConstructBinGrid(hd, context, ci.clusters.size() / pow(4.0, (double)iteration));
        //PetscScalar* solution;
        //VecGetArray(x, &solution);
        //InitWeights(solution, &context);
        //VecRestoreArray(x, &solution);

        // Tao solve the application
        iCHKERRQ TaoSolveApplication(taoapp, tao);
        int innerTAOIterations = 0;
        iCHKERRQ ReportTerminationReason(tao, innerTAOIterations);
        ALERT("Number of iterations made by TAO = %d", innerTAOIterations);

        if (innerTAOIterations >= 1)
        {// if found any solution, apply it
            GetVariablesValues(ci, x);
            UpdateCellsCoordinates(hd, ci);
        }

        ReportPostIterationInfo(hd, context, metaIteration, iteration);

        bool plotBinOccupancy = hd.cfg.ValueOf("LSE.GlobalPlacement.Plotting.BinOccupancyMap.plotBinOccupancy", false);
        if (plotBinOccupancy)
        {
            hd.Plotter->binGridForPlotting.SetBinGrid(context.sprData.binGrid);
            hd.Plotter->PlotBinOccupancyMap("GP");
        }

        if (IsTimeToExit(hd, ci, context, QA, iteration))
            break;

        UpdateWeights(hd, context, QA, iteration);
        
        if (hd.cfg.ValueOf(".UseBuffering", false))
        {
            if (metaIteration >= hd.cfg.ValueOf(".New_Buffering.NumberMetaIterationStartBuffering", 0))
            {
                //ALERT("------------GP---------------totalBufferArea %f", context.sprData.totalBufferArea);
                GPBuffering bufferer(hd);
                bufferer.DoBuffering(context, QA->GetBackHPWL(), QA->GetBackLHPWL());

                int nBins = context.sprData.binGrid.nBins;
                context.sprData.desiredCellsAreaAtEveryBin = (context.sprData.totalCellArea + context.sprData.totalBufferArea)/nBins;
                //ALERT("------------BF---------------totalBufferArea %f", context.sprData.totalBufferArea);
            }
        }

        iteration++;
    }

    if (QA != 0)
    {
        QA->Report("Placement Quality Analysis Table");
        delete QA;
    }

    return OK;
}