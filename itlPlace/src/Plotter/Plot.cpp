#include "HPlotter.h"
#include "PlotterData.h" //
#include "HDesign.h"
#include "Auxiliary.h" //
#include "AdaptiveRoute.h"
#include "VanGinnekenTreeNode.h"

#include "Utils.h" //
#include "OptimizationContext.h"

void HPlotter::PlotCell(HCell cell, Color col)
{
    if (!IsEnabled())
        return;

    double startX  = m_hd.GetDouble<HCell::X>(cell);
    double startY  = m_hd.GetDouble<HCell::Y>(cell);
    double finishX = m_hd.GetDouble<HCell::X>(cell) + m_hd.GetDouble<HCell::Width>(cell);
    double finishY = m_hd.GetDouble<HCell::Y>(cell) + m_hd.GetDouble<HCell::Height>(cell);

    DrawFilledRectangleWithBorder(startX, startY, finishX, finishY, Color_Blue, col, false);
}

void HPlotter::PlotSites()
{
    if (!IsEnabled())
        return;

    for (HPlacementRows::EnumeratorW row = m_hd.PlacementRows.GetEnumeratorW(); row.MoveNext(); )
    {
        for (int i = 0; i < row.VertSitesCount(); ++i)
            for (int j = 0; j < row.HorizSitesCount(); ++j)
            {
                double x1 = row.X() + row.SiteWidth() * j;
                double y1 = row.Y() + row.SiteHeight() * i;
                double x2 = row.X() + row.SiteWidth() * (j + 1);
                double y2 = row.Y() + row.SiteHeight() * (i + 1);

                DrawRectangle(x1, y1, x2, y2, Color_Red, false);
            }
    }
}



void HPlotter::PlotCriticalPath(HCriticalPath aPath)
{
    if (!IsEnabled())
        return;

    HCriticalPathWrapper criticalPathW = m_hd[aPath];

    HCriticalPath::PointsEnumeratorW i = criticalPathW.GetEnumeratorW();
    i.MoveNext();

    CvPoint start, finish;
    start.x = 10;
    start.y = 10;
    CvFont font;
    cvInitFont( &font, CV_FONT_HERSHEY_COMPLEX, 1, 1, 0.0, 1, 1 ); 

    HPinWrapper pin1 = m_hd[m_hd.Get<HTimingPoint::Pin, HPin>(i.TimingPoint())];
    start.x = DesignX2ImageX(pin1.X());
    start.y = DesignY2ImageY(pin1.Y());
    cvCircle(IMG, start, 3, GetCvColor(Color_Peru), 3);
    int index = 0;
    for(; i.MoveNext(); index++)
    {
        HPinWrapper pin2 = m_hd[m_hd.Get<HTimingPoint::Pin, HPin>(i.TimingPoint())]; 

        start.x = DesignX2ImageX(pin1.X());
        start.y = DesignY2ImageY(pin1.Y());
        finish.x = DesignX2ImageX(pin2.X());
        finish.y = DesignY2ImageY(pin2.Y());

        if (i.SignalDirection() == SignalDirection_Fall)
            cvLine(IMG, start, finish, GetCvColor(Color_Black), m_hd.cfg.ValueOf("CriticalPaths.thicknessLines", 1));
        else
            cvLine(IMG, start, finish, GetCvColor(Color_Red), m_hd.cfg.ValueOf("CriticalPaths.thicknessLines", 1));
        if (index <=1 )
            cvCircle(IMG, finish, 2, GetCvColor(Color_Plum), 2);
        else
            cvCircle(IMG, finish, 2, GetCvColor(Color_Yellow), 2);
        pin1 = pin2;
    }
}

void HPlotter::PlotNetSteinerTree(HNet net, Color color)
{
    if (!IsEnabled()) return;

    AdaptiveRoute(m_hd, net);

    HNetWrapper netw = m_hd[net];
    HPinWrapper src =  m_hd[netw.Source()];
    HSteinerPointWrapper srcPoint = m_hd[m_hd.SteinerPoints[src]];
    HSteinerPointWrapper nextPoint = srcPoint;

    m_hd.Plotter.DrawCircle(srcPoint.X(), srcPoint.Y(), 4, color, false);

    TemplateTypes<HSteinerPoint>::stack points;
    points.push(srcPoint);
    while (!points.empty())
    {
        srcPoint = points.top();
        points.pop();

        if (srcPoint.HasLeft())
        {
            nextPoint = srcPoint.Left();
            m_hd.Plotter.DrawLine(srcPoint.X(), srcPoint.Y(), nextPoint.X(), nextPoint.Y(), color, false);
            points.push(nextPoint);

            if (srcPoint.HasRight())
            {
                nextPoint = srcPoint.Right();
                m_hd.Plotter.DrawLine(srcPoint.X(), srcPoint.Y(), nextPoint.X(), nextPoint.Y(), color, false);
                points.push(nextPoint);
            }
        }
        else
        {
            m_hd.Plotter.DrawCircle(srcPoint.X(), srcPoint.Y(), 1, color, false);
        }
    }
}

void HPlotter::PlotCriticalPathSteinerTree(HCriticalPath path, Color color)
{
    HCriticalPathWrapper pointW = m_hd[path];
    HCriticalPath::PointsEnumeratorW pointsEnumeratorW = pointW.GetEnumeratorW();
    pointsEnumeratorW.MoveNext();
    
    HSteinerPointWrapper srcPoint = m_hd[m_hd.SteinerPoints[m_hd.TimingPoints.Get<HTimingPoint::Pin, HPin>(m_hd.CriticalPathPoints.Get<HCriticalPathPoint::TimingPoint, HTimingPoint>(pointsEnumeratorW))]];
    HSteinerPointWrapper nextPoint = srcPoint;

    m_hd.Plotter.DrawCircle(srcPoint.X(), srcPoint.Y(), 4, color, false);
    int pointCount = pointW.PointsCount();
    int pc = 1;
    TemplateTypes<HSteinerPoint>::stack points;
    points.push(srcPoint);
    while (!points.empty())
    {
        srcPoint = points.top();
        points.pop();

        if (srcPoint.HasLeft())
        {
            nextPoint = srcPoint.Left();
            m_hd.Plotter.DrawLine(srcPoint.X(), srcPoint.Y(), nextPoint.X(), nextPoint.Y(), color, false);
            points.push(nextPoint);

            if (srcPoint.HasRight())
            {
                nextPoint = srcPoint.Right();
                m_hd.Plotter.DrawLine(srcPoint.X(), srcPoint.Y(), nextPoint.X(), nextPoint.Y(), color, false);
                points.push(nextPoint);
            }
        }
        else
        {
          HCriticalPath::PointsEnumeratorW nextCriticalPathPointW = pointsEnumeratorW;
          nextCriticalPathPointW.MoveNext();
          HSteinerPointWrapper nextSteinerPoint = m_hd[m_hd.SteinerPoints[m_hd.TimingPoints.Get<HTimingPoint::Pin, HPin>(nextCriticalPathPointW.TimingPoint())]];

          bool isCriticalPathLeaf = true;

          if (nextSteinerPoint == srcPoint)
          {
            if (pointsEnumeratorW.MoveNext())
            {
              if (pointsEnumeratorW.MoveNext())
              {
                pc+=2;
                isCriticalPathLeaf = false;
                srcPoint = m_hd[m_hd.SteinerPoints[m_hd.TimingPoints.Get<HTimingPoint::Pin, HPin>(pointsEnumeratorW.TimingPoint())]];
                m_hd.Plotter.DrawCircle(srcPoint.X(), srcPoint.Y(), 4, color, false);
                bool f = false;
                if (srcPoint.HasLeft())
                {
                  f = true;
                    nextPoint = srcPoint.Left();
                    m_hd.Plotter.DrawLine(srcPoint.X(), srcPoint.Y(), nextPoint.X(), nextPoint.Y(), color, false);
                    points.push(nextPoint);

                    if (srcPoint.HasRight())
                    {
                        nextPoint = srcPoint.Right();
                        m_hd.Plotter.DrawLine(srcPoint.X(), srcPoint.Y(), nextPoint.X(), nextPoint.Y(), color, false);
                        points.push(nextPoint);
                    }
                }
                if (!f)
                  ALERT("Error source not left!!!");
              }
            }
          }
          if (isCriticalPathLeaf)
          {
            m_hd.Plotter.DrawCircle(srcPoint.X(), srcPoint.Y(), 1, color, false);
          }
              
        }
    }
    if (pc != (pointCount - 1))
      ALERT("error point count in !!!!!!!!pc = %d\tpoint count = %d", pc, pointCount);
}

void HPlotter::PlotVGTree(VanGinnekenTreeNode* tree, Color LineColor, Color VGNodeColor)
{
    if (!IsEnabled()) return;

    VanGinnekenTreeNode* srcPoint = tree;
    VanGinnekenTreeNode* nextPoint = srcPoint;

    m_hd.Plotter.DrawCircle(srcPoint->x, srcPoint->y, 4, LineColor, false);

    TemplateTypes<VanGinnekenTreeNode*>::stack points;
    points.push(srcPoint);
    while (!points.empty())
    {
        srcPoint = points.top();
        points.pop();

        if (srcPoint->HasLeft())
        {
            nextPoint = srcPoint->GetLeft();
            m_hd.Plotter.DrawLine(srcPoint->x, srcPoint->y, nextPoint->x, nextPoint->y, LineColor, false);
            m_hd.Plotter.DrawCircle(srcPoint->x, srcPoint->y, 1, VGNodeColor, false);
            //m_hd.Plotter.DrawRectangle(nextPoint->x, nextPoint->y, nextPoint->x + DesignX2ImageX(5), nextPoint->y + DesignY2ImageY(5),  Color(color + 1));
            points.push(nextPoint);

            if (srcPoint->HasRight())
            {
                nextPoint = srcPoint->GetRight();
                m_hd.Plotter.DrawLine(srcPoint->x, srcPoint->y, nextPoint->x, nextPoint->y, LineColor, false);
                m_hd.Plotter.DrawCircle(srcPoint->x, srcPoint->y, 1, VGNodeColor, false);
                //m_hd.Plotter.DrawRectangle(nextPoint->x, nextPoint->y, nextPoint->x + DesignX2ImageX(5), nextPoint->y + DesignY2ImageY(5),  Color(color + 1));
                points.push(nextPoint);
            }
        }
        else
        {
            m_hd.Plotter.DrawCircle(srcPoint->x, srcPoint->y, 1, LineColor, false);
        }
    }
}

void HPlotter::PlotNet(HNet net, Color color)
{
    CvPoint start, finish;
    HPinWrapper pin1 = m_hd[m_hd[net].Source()];
    start.x = DesignX2ImageX(pin1.X());
    start.y = DesignY2ImageY(pin1.Y());
    cvCircle(IMG, start, 1, GetCvColor(color), 3);
    for (HNet::SinksEnumeratorW sew = m_hd[net].GetSinksEnumeratorW(); sew.MoveNext(); )
    {
        start.x = DesignX2ImageX(pin1.X());
        start.y = DesignY2ImageY(pin1.Y());
        finish.x = DesignX2ImageX(sew.X());
        finish.y = DesignY2ImageY(sew.Y());
        cvCircle(IMG, finish, 1, GetCvColor(Color_Yellow), 2);
        cvLine(IMG, start, finish, GetCvColor(Color_Red), 1, 1);
    }
}

void HPlotter::PlotFullWires()
{
    if (IsEnabled())
    {
        for ( HNets::ActiveNetsEnumeratorW net = m_hd.Nets.GetActiveNetsEnumeratorW(); net.MoveNext(); )
        {
            PlotNet(net);
        }
    }
}

void HPlotter::Plot2PinNets()
{
    if (IsEnabled())
    {
        for ( HNets::ActiveNetsEnumeratorW net = m_hd.Nets.GetActiveNetsEnumeratorW(); net.MoveNext(); )
        {
            if (net.PinsCount() == 2)
            {
                PlotNet(net);
            }
        }
    }
}

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