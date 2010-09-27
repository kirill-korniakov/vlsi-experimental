#include "HPlotter.h"
#include "PlotterData.h"
#include "Utils.h"
#include "AdaptiveRoute.h"
#include "OptimizationContext.h"

void HPlotter::PlotAntiGradients(int nClusters, double* coordinates, double* gradients, 
                                 double scaling, Color color)
{
    if (IsEnabled())
    {
        CvPoint start, finish;

        for (int i = 0; i < nClusters; i++)
        {
            start.x = DesignX2ImageX(coordinates[2*i+0]);
            start.y = DesignY2ImageY(coordinates[2*i+1]);
            finish.x = DesignX2ImageX(coordinates[2*i+0] - scaling*gradients[2*i+0]);
            finish.y = DesignY2ImageY(coordinates[2*i+1] - scaling*gradients[2*i+1]);
            cvDrawLine(IMG, start, finish, GetCvColor(color));
            cvDrawCircle(IMG, finish, 1, GetCvColor(color));
        }
    }
}

void HPlotter::PlotMostCriticalPaths(HDesign& design, int n)
{
    Utils::IterateMostCriticalPaths(design, n, (Utils::CriticalPathHandler)HPlotter::PlotPath);
}

void HPlotter::PlotKi(int nClusters, int nNets, double* x, Color color)
{
    double maxKi = m_hd.cfg.ValueOf("GlobalPlacement.bufferCountUpperBound", 0.0);
    if (!IsEnabled() || maxKi <= 0.0) 
        return;

    //FIXME: find proper place
    InitializeHistogramWindow();

    CvPoint start, finish;
    double meanK = 0.0;

    for (int i = 0; i < nNets; i++)
    {
        meanK += x[2*nClusters + i];
        start.x = HNormalX2ImageX((double)i/(double)nNets);
        start.y = HNormalY2ImageY(0.0);
        finish.x = start.x;
        finish.y = HNormalY2ImageY(x[2*nClusters + i]/maxKi);
        cvDrawLine(m_Data->histogramImg, start, finish, GetCvColor(color));
    }

    //plot one line
    start.x  = HNormalX2ImageX(0.0);
    finish.x = HNormalX2ImageX(1.0);
    start.y  = HNormalY2ImageY(1.0/maxKi);
    finish.y = HNormalY2ImageY(1.0/maxKi);
    cvDrawLine(m_Data->histogramImg, start, finish, GetCvColor(Color_Black), 1);

    //plot mean ki
    meanK /= nNets;
    start.x  = HNormalX2ImageX(0.0);
    finish.x = HNormalX2ImageX(1.0);
    start.y  = HNormalY2ImageY(meanK/maxKi);
    finish.y = HNormalY2ImageY(meanK/maxKi);
    cvDrawLine(m_Data->histogramImg, start, finish, GetCvColor(Color_Black), 1);
}


//Show methods
void HPlotter::ShowPlacement(WaitTime waitTime)
{
    Clear();
    PlotPlacement();
    Refresh(waitTime);
}

void HPlotter::AutoShowPlacement(WaitTime waitTime)
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

void HPlotter::PlotPlacement()
{
    if (!IsEnabled())
        return;

    CvPoint center;
    CvPoint start, finish;

    for (HCells::PlaceableCellsEnumeratorW cell = m_hd.Cells.GetPlaceableCellsEnumeratorW(); cell.MoveNext(); )
    {
        PlotCell(cell, _GetCellColor(cell));
    }

    CvScalar terminalColor = cvScalar(0.0, 0.0, 255.0);
    CvScalar terminalBackColor = cvScalar(64.0, 0.0, 0.0);

    for (HCells::TerminalsEnumeratorW terminal = m_hd.Cells.GetTerminalsEnumeratorW(); terminal.MoveNext(); )
    {
        //draw border of cells
        start.x = DesignX2ImageX(terminal.X());
        start.y = DesignY2ImageY(terminal.Y());
        finish.x = DesignX2ImageX(terminal.X() + terminal.Width());
        finish.y = DesignY2ImageY(terminal.Y() + terminal.Height());
        if (start.x + 1 < finish.x && start.y + 1 < finish.y)
        {
            cvRectangle(IMG, start, finish, terminalColor);
            //fill
            start.x++; start.y++;
            finish.x--; finish.y--;
            if (start.x < finish.x && start.y < finish.y)
                cvRectangle(IMG, start, finish, terminalBackColor, CV_FILLED);
        }
        else
            cvCircle(IMG, start, 2, terminalBackColor, CV_FILLED);
    }

    //draw primary pins
    CvScalar PIColor = cvScalar(0.0, 255.0, 0.0); //green
    CvScalar POColor = cvScalar(0.0, 0.0, 255.0); //red
    for (HPins::PrimariesEnumeratorW pin = m_hd.Pins.GetEnumeratorW(); pin.MoveNext(); )
    {
        if (!::IsNull(pin.Net()))
        {
            center.x = DesignX2ImageX(pin.X());
            center.y = DesignY2ImageY(pin.Y());
            if (pin.IsPrimaryInput())
                cvCircle(IMG, center, 3, PIColor, CV_FILLED);
            else
                cvCircle(IMG, center, 3, POColor, CV_FILLED);
        }
    }

    //draw placement region
    CvScalar plBorderColor = cvScalar(0.0, 0.0, 255.0);

    start.x = DesignX2ImageX(m_hd.Circuit.PlacementMinX());
    start.y = DesignY2ImageY(m_hd.Circuit.PlacementMinY());
    finish.x = DesignX2ImageX(m_hd.Circuit.PlacementMinX());
    finish.y = DesignY2ImageY(m_hd.Circuit.PlacementMaxY());
    cvDrawLine(IMG, start, finish, plBorderColor);

    start = finish;
    finish.x = DesignX2ImageX(m_hd.Circuit.PlacementMaxX());
    finish.y = DesignY2ImageY(m_hd.Circuit.PlacementMaxY());
    cvDrawLine(IMG, start, finish, plBorderColor);

    start = finish;
    finish.x = DesignX2ImageX(m_hd.Circuit.PlacementMaxX());
    finish.y = DesignY2ImageY(m_hd.Circuit.PlacementMinY());
    cvDrawLine(IMG, start, finish, plBorderColor);

    start = finish;
    finish.x = DesignX2ImageX(m_hd.Circuit.PlacementMinX());
    finish.y = DesignY2ImageY(m_hd.Circuit.PlacementMinY());
    cvDrawLine(IMG, start, finish, plBorderColor);

    //draw placement center
    CvScalar centerColor = cvScalar(0.0, 0.0, 0.0);
    center.x = DesignX2ImageX((m_hd.Circuit.PlacementMinX() + m_hd.Circuit.PlacementMaxX()) / 2.0);
    center.y = DesignY2ImageY((m_hd.Circuit.PlacementMinY() + m_hd.Circuit.PlacementMaxY()) / 2.0);
    cvCircle(IMG, center, 4, centerColor, 1);

    CvPoint startTextLine, finishTextLine;
    startTextLine.x = 0;
    startTextLine.y = m_Data->textSpaceHeight;
    finishTextLine.x = IMG->width;
    finishTextLine.y = m_Data->textSpaceHeight;
    cvLine(IMG, startTextLine, finishTextLine, GetCvColor(Color_Black), 1);
}

void HPlotter::PlotPath(HDesign& design, HCriticalPath path, int pathNumber)
{
    ASSERT(design.CanDoTiming());
    char fileName[32];
    char signature[256];
    sprintf(fileName, "critical_path_%d", pathNumber);
    sprintf(signature, "critical path #%d %f", pathNumber, design.GetDouble<HCriticalPath::Criticality>(path));

    design.Plotter.Clear();
    design.Plotter.PlotPlacement();
    design.Plotter.PlotCriticalPath(path);
    design.Plotter.DrawText(signature);
    design.Plotter.Refresh("CriticalPaths.plotWait");
    design.Plotter.SaveImage(fileName,
        design.cfg.ValueOf("plotter.pixDirectory", ".\\") + design.Circuit.Name() + "_critical_path\\");
}

void HPlotter::PlotBinGrid(int nBinRows, int nBinCols)
{
    if (IsEnabled())
    {
        CvPoint start, finish;

        Color color = Color_GrayText;
        start.y  = DesignY2ImageY(m_hd.Circuit.PlacementMinY());
        finish.y = DesignY2ImageY(m_hd.Circuit.PlacementMaxY());

        //vertical lines
        for (int i = 1; i < nBinCols; i++)
        {
            start.x = finish.x
                = DesignX2ImageX(m_hd.Circuit.PlacementMinX() + i * m_hd.Circuit.PlacementWidth() / nBinCols);
            cvDrawLine(IMG, start, finish, GetCvColor(color));
        }

        start.x  = DesignX2ImageX(m_hd.Circuit.PlacementMinX());
        finish.x = DesignX2ImageX(m_hd.Circuit.PlacementMaxX());

        //horizontal lines
        for (int i = 1; i < nBinRows; i++)
        {
            start.y = finish.y
                = DesignY2ImageY(m_hd.Circuit.PlacementMinY() + i * m_hd.Circuit.PlacementHeight() / nBinRows);
            cvDrawLine(IMG, start, finish, GetCvColor(color));
        }
    }
}

void HPlotter::PlotFillBinGrid(AppCtx* context)
{
    if (!IsEnabled()) return;

    CvPoint start, finish;

    for (int i = 0; i < context->sprData.binGrid.nBinCols; i++)
        for (int j = 0; j < context->sprData.binGrid.nBinRows; j++)
        {
            start.x = DesignX2ImageX(context->sprData.binGrid.bins[i][j].xCoord - context->sprData.binGrid.binWidth / 2.0);
            start.y = DesignY2ImageY(context->sprData.binGrid.bins[i][j].yCoord - context->sprData.binGrid.binHeight / 2.0);
            finish.x = DesignX2ImageX(context->sprData.binGrid.bins[i][j].xCoord + context->sprData.binGrid.binWidth / 2.0);
            finish.y = DesignY2ImageY(context->sprData.binGrid.bins[i][j].yCoord + context->sprData.binGrid.binHeight / 2.0);
            cvRectangle(IMG, start, finish, GetCvColor(Color_LightPink), 1);

            char str1[255], str2[255];
            sprintf(str1, Aux::SciFormat.c_str(), context->sprData.binGrid.bins[i][j].sumBufPotential);
            sprintf(str2, Aux::SciFormat.c_str(), context->sprData.binGrid.bins[i][j].sumPotential);

            DrawTextInPoint(string(str1), 
                context->sprData.binGrid.bins[i][j].xCoord - context->sprData.binGrid.binWidth / 2.0, 
                context->sprData.binGrid.bins[i][j].yCoord - context->sprData.binGrid.binHeight / 2.5, 0.3);
            DrawTextInPoint(string(str2), 
                context->sprData.binGrid.bins[i][j].xCoord - context->sprData.binGrid.binWidth / 2.0, 
                context->sprData.binGrid.bins[i][j].yCoord + context->sprData.binGrid.binHeight / 3.2, 0.3);
        }
}