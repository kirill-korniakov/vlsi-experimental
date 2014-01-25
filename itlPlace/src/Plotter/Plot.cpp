#include "HPlotter.h"
#include "HDesign.h"
#include "AdaptiveRoute.h"
#include "VanGinnekenTreeNode.h"

#include "Utils.h"
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

    double startX = 10;
    double startY = 10;
    double finishX, finishY;

    InitFont();

    HPinWrapper pin1 = m_hd[m_hd.Get<HTimingPoint::Pin, HPin>(i.TimingPoint())];
    startX = pin1.X();
    startY = pin1.Y();
    DrawCircle(startX, startY, 3, Color_Peru, false, 3);
    int index = 0;
    for(; i.MoveNext(); index++)
    {
        HPinWrapper pin2 = m_hd[m_hd.Get<HTimingPoint::Pin, HPin>(i.TimingPoint())]; 

        startX  = pin1.X();
        startY  = pin1.Y();
        finishX = pin2.X();
        finishY = pin2.Y();

        if (i.SignalDirection() == SignalDirection_Fall)
            DrawLine(startX, startY, finishX, finishY, Color_Black, false,
                     m_hd.cfg.ValueOf("CriticalPaths.thicknessLines", 1));
        else
            DrawLine(startX, startY, finishX, finishY, Color_Red, false,
                     m_hd.cfg.ValueOf("CriticalPaths.thicknessLines", 1));
        if (index <=1 )
            DrawCircle(finishX, finishY, 2, Color_Plum, false, 2);
        else
            DrawCircle(finishX, finishY, 2, Color_Yellow, false, 2);
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

    DrawCircle(srcPoint.X(), srcPoint.Y(), 4, color, false);

    TemplateTypes<HSteinerPoint>::stack points;
    points.push(srcPoint);
    while (!points.empty())
    {
        srcPoint = points.top();
        points.pop();

        if (srcPoint.HasLeft())
        {
            nextPoint = srcPoint.Left();
            DrawLine(srcPoint.X(), srcPoint.Y(), nextPoint.X(), nextPoint.Y(), color, false);
            points.push(nextPoint);

            if (srcPoint.HasRight())
            {
                nextPoint = srcPoint.Right();
                DrawLine(srcPoint.X(), srcPoint.Y(), nextPoint.X(), nextPoint.Y(), color, false);
                points.push(nextPoint);
            }
        }
        else
        {
            DrawCircle(srcPoint.X(), srcPoint.Y(), 1, color, false);
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

    DrawCircle(srcPoint.X(), srcPoint.Y(), 4, color, false);
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
            DrawLine(srcPoint.X(), srcPoint.Y(), nextPoint.X(), nextPoint.Y(), color, false);
            points.push(nextPoint);

            if (srcPoint.HasRight())
            {
                nextPoint = srcPoint.Right();
                DrawLine(srcPoint.X(), srcPoint.Y(), nextPoint.X(), nextPoint.Y(), color, false);
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
                DrawCircle(srcPoint.X(), srcPoint.Y(), 4, color, false);
                bool f = false;
                if (srcPoint.HasLeft())
                {
                  f = true;
                    nextPoint = srcPoint.Left();
                    DrawLine(srcPoint.X(), srcPoint.Y(), nextPoint.X(), nextPoint.Y(), color, false);
                    points.push(nextPoint);

                    if (srcPoint.HasRight())
                    {
                        nextPoint = srcPoint.Right();
                        DrawLine(srcPoint.X(), srcPoint.Y(), nextPoint.X(), nextPoint.Y(), color, false);
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
            DrawCircle(srcPoint.X(), srcPoint.Y(), 1, color, false);
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

    DrawCircle(srcPoint->x, srcPoint->y, 4, LineColor, false);

    TemplateTypes<VanGinnekenTreeNode*>::stack points;
    points.push(srcPoint);
    while (!points.empty())
    {
        srcPoint = points.top();
        points.pop();

        if (srcPoint->HasLeft())
        {
            nextPoint = srcPoint->GetLeft();
            DrawLine(srcPoint->x, srcPoint->y, nextPoint->x, nextPoint->y, LineColor, false);
            DrawCircle(srcPoint->x, srcPoint->y, 1, VGNodeColor, false);
            //m_hd.Plotter->DrawRectangle(nextPoint->x, nextPoint->y, nextPoint->x + DesignX2ImageX(5), nextPoint->y + DesignY2ImageY(5),  Color(color + 1));
            points.push(nextPoint);

            if (srcPoint->HasRight())
            {
                nextPoint = srcPoint->GetRight();
                DrawLine(srcPoint->x, srcPoint->y, nextPoint->x, nextPoint->y, LineColor, false);
                DrawCircle(srcPoint->x, srcPoint->y, 1, VGNodeColor, false);
                //m_hd.Plotter->DrawRectangle(nextPoint->x, nextPoint->y, nextPoint->x + DesignX2ImageX(5), nextPoint->y + DesignY2ImageY(5),  Color(color + 1));
                points.push(nextPoint);
            }
        }
        else
        {
            DrawCircle(srcPoint->x, srcPoint->y, 1, LineColor, false);
        }
    }
}

void HPlotter::PlotNet(HNet net, Color color)
{
    double startX, startY;
    double finishX, finishY;

    HPinWrapper pin1 = m_hd[m_hd[net].Source()];
    startX = pin1.X();
    startY = pin1.Y();
    DrawCircle(startX, startY, 1, color, false, 3);
    ///cvCircle(IMG, start, 1, color, 3);

    for (HNet::SinksEnumeratorW sew = m_hd[net].GetSinksEnumeratorW(); sew.MoveNext(); )
    {
        startX  = pin1.X();
        startY  = pin1.Y();
        finishX = sew.X();
        finishY = sew.Y();
        DrawCircle(finishX, finishY, 1, Color_Yellow, false, 2);
        DrawLine(startX, startY, finishX, finishY, Color_Red, false, 1, 1);
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
    if (!IsEnabled())
        return;

    for (int i = 0; i < nClusters; i++)
    {
        double startX  = coordinates[2 * i + 0];
        double startY  = coordinates[2 * i + 1];
        double finishX = coordinates[2 * i + 0] - scaling * gradients[2 * i + 0];
        double finishY = coordinates[2 * i + 1] - scaling * gradients[2 * i + 1];

        DrawLine(startX, startY, finishX, finishY, color, false);
        DrawCircle(finishX, finishY, 1, color, false, 1);
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

    //TODO: find proper place
    InitializeHistogramWindow();

    double startX, startY;
    double finishX, finishY;

    double meanK = 0.0;

    for (int i = 0; i < nNets; i++)
    {
        meanK += x[2 * nClusters + i];
        //TODO: take this coordinates into account
        startX  = (double)i / (double)nNets;
        startY  = 0.0;
        finishX = startX;
        finishY = x[2 * nClusters + i] / maxKi;
        DrawKiLine(startX, startY, finishX, finishY, color, false);
    }

    //plot one line
    startX  = 0.0;
    finishX = 1.0;
    startY  = 1.0 / maxKi;
    finishY = 1.0 / maxKi;
    DrawKiLine(startX, startY, finishX, finishY, Color_Black, false);

    //plot mean ki
    meanK /= nNets;
    startX  = 0.0;
    finishX = 1.0;
    startY  = meanK / maxKi;
    finishY = meanK / maxKi;
    DrawKiLine(startX, startY, finishX, finishY, Color_Black, false);
}

void HPlotter::PlotPlacement()
{
    if (!IsEnabled())
        return;

    for (HCells::PlaceableCellsEnumeratorW cell = m_hd.Cells.GetPlaceableCellsEnumeratorW(); cell.MoveNext(); )
    {
        PlotCell(cell, _GetCellColor(cell));
    }

    Color terminalColor = Color_Red;
    Color terminalBackColor = Color_Desktop;

    double startX, startY;
    double finishX, finishY;

    for (HCells::TerminalsEnumeratorW terminal = m_hd.Cells.GetTerminalsEnumeratorW(); terminal.MoveNext(); )
    {
        //draw border of cells
        startX  = terminal.X();
        startY  = terminal.Y();
        finishX = terminal.X() + terminal.Width();
        finishY = terminal.Y() + terminal.Height();

        if ((startX < finishX) && (startY < finishY))
        {
            DrawFilledRectangleWithBorder(startX, startY, finishX, finishY, terminalColor,
                                          terminalBackColor, false);
        }
        else
        {
            //Filled circle
            DrawCircle(startX, startY, 2, terminalBackColor, false, -1);
        }
    }

    //draw primary pins
    Color PIColor = Color_Green;
    Color POColor = Color_Red;

    double centerX, centerY;

    for (HPins::PrimariesEnumeratorW pin = m_hd.Pins.GetEnumeratorW(); pin.MoveNext(); )
    {
        if (!::IsNull(pin.Net()))
        {
            centerX = pin.X();
            centerY = pin.Y();

            if (pin.IsPrimaryInput())
                DrawCircle(centerX, centerY, 3, PIColor, false, -1);

            else
                DrawCircle(centerX, centerY, 3, POColor, false, -1);
        }
    }

    //draw placement region
    Color plBorderColor = Color_Red;

    startX  = m_hd.Circuit.PlacementMinX();
    startY  = m_hd.Circuit.PlacementMinY();
    finishX = m_hd.Circuit.PlacementMinX();
    finishY = m_hd.Circuit.PlacementMaxY();
    DrawLine(startX, startY, finishX, finishY, plBorderColor, false);

    startX  = finishX;
    startY  = finishY;
    finishX = m_hd.Circuit.PlacementMaxX();
    finishY = m_hd.Circuit.PlacementMaxY();
    DrawLine(startX, startY, finishX, finishY, plBorderColor, false);

    startX  = finishX;
    startY  = finishY;
    finishX = m_hd.Circuit.PlacementMaxX();
    finishY = m_hd.Circuit.PlacementMinY();
    DrawLine(startX, startY, finishX, finishY, plBorderColor, false);

    startX  = finishX;
    startY  = finishY;
    finishX = m_hd.Circuit.PlacementMinX();
    finishY = m_hd.Circuit.PlacementMinY();
    DrawLine(startX, startY, finishX, finishY, plBorderColor, false);

    //draw placement center
    centerX = (m_hd.Circuit.PlacementMinX() + m_hd.Circuit.PlacementMaxX()) / 2.0;
    centerY = (m_hd.Circuit.PlacementMinY() + m_hd.Circuit.PlacementMaxY()) / 2.0;
    DrawCircle(centerX, centerY, 4, Color_Black, false);

    DrawTextLine();
}

void HPlotter::PlotPath(HDesign& design, HCriticalPath path, int pathNumber)
{
    ASSERT(design.CanDoTiming());
    char fileName[32];
    char signature[256];
    sprintf(fileName, "critical_path_%d", pathNumber);
    sprintf(signature, "critical path #%d %f", pathNumber, design.GetDouble<HCriticalPath::Criticality>(path));

    design.Plotter->Clear();
    design.Plotter->PlotPlacement();
    design.Plotter->PlotCriticalPath(path);
    design.Plotter->DrawText(signature);
    design.Plotter->Refresh("CriticalPaths.plotWait");
    design.Plotter->SaveImage(fileName,
    design.cfg.ValueOf("plotter.pixDirectory", ".\\") + design.Circuit.Name() + "_critical_path\\");
}

void HPlotter::PlotBinGrid(int nBinRows, int nBinCols)
{
    if (!IsEnabled())
        return;

    double startX, startY;
    double finishX, finishY;

    Color color = Color_GrayText;
    startY      = m_hd.Circuit.PlacementMinY();
    finishY     = m_hd.Circuit.PlacementMaxY();

    //vertical lines
    for (int i = 1; i < nBinCols; i++)
    {
        startX = finishX = m_hd.Circuit.PlacementMinX() + i * m_hd.Circuit.PlacementWidth() / nBinCols;
        DrawLine(startX, startY, finishX, finishY, color, false);
    }

    startX  = m_hd.Circuit.PlacementMinX();
    finishX = m_hd.Circuit.PlacementMaxX();

    //horizontal lines
    for (int i = 1; i < nBinRows; i++)
    {
        startY = finishY = m_hd.Circuit.PlacementMinY() + i * m_hd.Circuit.PlacementHeight() / nBinRows;
        DrawLine(startX, startY, finishX, finishY, color, false);
    }
}

void HPlotter::PlotFillBinGrid(AppCtx* context)
{
    if (!IsEnabled())
        return;

    double startX, startY;
    double finishX, finishY;

    for (int i = 0; i < context->sprData.binGrid.nBinCols; i++)
        for (int j = 0; j < context->sprData.binGrid.nBinRows; j++)
        {
            startX  = context->sprData.binGrid.bins[i][j].xCoord - context->sprData.binGrid.binWidth / 2.0;
            startY  = context->sprData.binGrid.bins[i][j].yCoord - context->sprData.binGrid.binHeight / 2.0;
            finishX = context->sprData.binGrid.bins[i][j].xCoord + context->sprData.binGrid.binWidth / 2.0;
            finishY = context->sprData.binGrid.bins[i][j].yCoord + context->sprData.binGrid.binHeight / 2.0;
            DrawRectangle(startX, startY, finishX, finishY, Color_LightPink, false);

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
