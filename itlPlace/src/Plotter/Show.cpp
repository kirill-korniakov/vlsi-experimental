#include "HPlotter.h"
#include "PlotterData.h"
#include "AdaptiveRoute.h"

#include "OpenCVPlotter.h"

//Show methods
void HPlotter::ShowPlacement(WaitTime waitTime)
{
    Clear();
    PlotPlacement();
    Refresh(waitTime);
}

void OpenCVPlotter::AutoShowPlacement(WaitTime waitTime)
{
    if (!IsEnabled())
        return;

    static int i = 0;
    if (i++ % m_Data->autoRefreshStep == 0)
    {
        ShowPlacement(waitTime);
    }
}

void HPlotter::ShowGlobalPlacement(bool plotWires, int nBinRows, int nBinCols, WaitTime waitTime)
{
    Clear();
    PlotBinGrid(nBinRows, nBinCols);
    if (plotWires)
        PlotFullWires();
    PlotPlacement();
    Refresh(waitTime);
}

void HPlotter::VisualizeState(int nClusters, int nBinRows, int nBinCols, int nNets,
                              double* x, double* gLSE, double* gSOD, double* gLR, double* gQS, double* g)
{
    double scaling = m_hd.cfg.ValueOf("GlobalPlacement.Plotting.gradientScaling", 1.0);
    int waitTime = m_hd.cfg.ValueOf("GlobalPlacement.Plotting.plotWait", 1);

    Clear();
    PlotBinGrid(nBinRows, nBinCols);
    PlotPlacement();
    PlotAntiGradients(nClusters, (double*)x, gLSE, scaling, Color_Red);
    PlotAntiGradients(nClusters, (double*)x, gSOD, scaling, Color_Brown);
    PlotAntiGradients(nClusters, (double*)x, gLR, scaling, Color_LimeGreen);
    PlotAntiGradients(nClusters, (double*)x, gQS, scaling, Color_Orange);
    PlotAntiGradients(nClusters, (double*)x, g, scaling, Color_Black);
    Refresh((HPlotter::WaitTime)waitTime);
}

void HPlotter::ShowLegalizationState(WaitTime waitTime, bool drawSites)
{
    Clear();
    if (drawSites)
        PlotSites();
    PlotPlacement();
    Refresh(waitTime);
}

void HPlotter::ShowSteinerForest(Color color, WaitTime waitTime)
{
    if (!IsEnabled()) return;

    for (HNets::ActiveNetsEnumeratorW anew = m_hd.Nets.GetActiveNetsEnumeratorW(); anew.MoveNext();)
    {
        AdaptiveRoute(m_hd, anew);
        PlotNetSteinerTree(anew, color);
    }

    Refresh(waitTime);
    SaveMilestoneImage("SteinerForest");
}

void HPlotter::ShowNetSteinerTree(HNet net, Color color, bool isShowPlacement, WaitTime waitTime, double textSize)
{
    if (!IsEnabled()) 
        return;

    if (isShowPlacement)
        ShowPlacement();

    DrawText(m_hd.Nets.GetString<HNet::Name>(net), textSize);
    PlotNetSteinerTree(net, color);  
    Refresh(waitTime);
}

void HPlotter::ShowCriticalPathSteinerTree(HCriticalPath path, Color color, bool isShowPlacement, WaitTime waitTime, double textSize)
{
    if (!IsEnabled()) 
        return;

    if (isShowPlacement)
        ShowPlacement();

    char bufferName[256];
    sprintf(bufferName, "Critical Path id = %d\tpoint count = %d", ::ToID(path), m_hd.CriticalPaths.GetInt<HCriticalPath::PointsCount>(path));
    DrawText(string(bufferName), textSize);
    PlotCriticalPathSteinerTree(path, color);  
    Refresh(waitTime);
}

void HPlotter::ShowVGTree(HNet net, VanGinnekenTreeNode* tree, Color LineColor, bool isShowPlacement, WaitTime waitTime, Color VGNodeColor, double textSize)
{  
    if (!IsEnabled()) 
        return;

    if (isShowPlacement)
        ShowPlacement();

    DrawText(m_hd.Nets.GetString<HNet::Name>(net), textSize);
    PlotVGTree(tree, LineColor, VGNodeColor); 
    Refresh(waitTime);
}

void HPlotter::ShowNet(HNet net, bool isShowPlacement, WaitTime waitTime, Color color, double textSize)
{
    if (!IsEnabled()) 
        return;

    if (isShowPlacement)
        ShowPlacement();

    DrawText(m_hd.Nets.GetString<HNet::Name>(net), textSize);
    PlotNet(net, color);
    Refresh(waitTime);
}