#include "HPlotter.h"
#include "PlotterData.h"
#include "HDesign.h"
#include "Auxiliary.h"
#include "AdaptiveRoute.h"
#include "VanGinnekenTreeNode.h"

void HPlotter::PlotCell(HCell cell, Color col)
{
    if (!IsEnabled())
        return;

    CvPoint start, finish;
    CvScalar cellBorderColor = cvScalar(255.0, 0.0, 0.0);

    //draw border of cells
    start.x = DesignX2ImageX(m_hd.GetDouble<HCell::X>(cell));
    start.y = DesignY2ImageY(m_hd.GetDouble<HCell::Y>(cell));
    finish.x = DesignX2ImageX(m_hd.GetDouble<HCell::X>(cell) + m_hd.GetDouble<HCell::Width>(cell));
    finish.y = DesignY2ImageY(m_hd.GetDouble<HCell::Y>(cell) + m_hd.GetDouble<HCell::Height>(cell));

    cvRectangle(IMG, start, finish, cellBorderColor);

    //fill the cell
    start.x++; start.y++;
    finish.x--; finish.y--;
    CvScalar fillColor = GetCvColor(col);
    if (start.x <= finish.x && start.y <= finish.y)
    {
        cvRectangle(IMG, start, finish, fillColor, CV_FILLED);
    }
}

void HPlotter::PlotSites()
{
    if (IsEnabled())
    {
        CvScalar siteColor = cvScalar(0.0, 0.0, 255.0);
        CvPoint start, finish;
        for (HPlacementRows::EnumeratorW row = m_hd.PlacementRows.GetEnumeratorW(); row.MoveNext(); )
        {
            for (int i = 0; i < row.VertSitesCount(); ++i)
                for (int j = 0; j < row.HorizSitesCount(); ++j)
                {
                    start.x = DesignX2ImageX(row.X() + row.SiteWidth() * j);
                    start.y = DesignY2ImageY(row.Y() + row.SiteHeight() * i);
                    finish.x = DesignX2ImageX(row.X() + row.SiteWidth() * (j + 1));
                    finish.y = DesignY2ImageY(row.Y() + row.SiteHeight() * (i + 1));

                    cvRectangle(IMG, start, finish, siteColor, 1);
                }
        }
    }
}

void HPlotter::PlotTileWires(double x1, double y1, double x2, double y2, int nLines, int nMaxLines)
{
    if (!IsEnabled())
        return;

    CvPoint start, finish;
    start.x = DesignX2ImageX(x1);
    start.y = DesignY2ImageY(y1);
    finish.x = DesignX2ImageX(x2);
    finish.y = DesignY2ImageY(y2);
    CvScalar minColor = cvScalar(255.0, 255.0, 255.0);
    CvScalar maxColor = cvScalar(0.0, 0.0, 0.0);
    CvScalar resColor;

    if (nLines >= nMaxLines)
        resColor = maxColor;
    else
        resColor = cvScalar(255 - 255 * nLines / nMaxLines, 255 - 255 * nLines / nMaxLines,
        255 - 255 * nLines / nMaxLines);
    //resColor = cvScalar(255 * nLines / nMaxLines, 255 * nLines / nMaxLines, 255 * nLines / nMaxLines);

    cvRectangle(IMG, start, finish, resColor, CV_FILLED);
    _AutoRefresh();
}

void HPlotter::PlotTilePins(double x1, double y1, double x2, double y2, int nPins, int nMaxPins)
{
    if (!IsEnabled())
        return;

    CvPoint start, finish;
    start.x = DesignX2ImageX(x1);
    start.y = DesignY2ImageY(y1);
    finish.x = DesignX2ImageX(x2);
    finish.y = DesignY2ImageY(y2);
    CvScalar minColor = cvScalar(255.0, 255.0, 255.0);
    CvScalar maxColor = cvScalar(0.0, 0.0, 0.0);
    CvScalar resColor;

    if (nPins >= nMaxPins)
        resColor = maxColor;
    else
        resColor = cvScalar(255 - 255 * nPins / nMaxPins, 255 - 255 * nPins / nMaxPins,
        255 - 255 * nPins / nMaxPins);

    cvRectangle(IMG, start, finish, resColor, CV_FILLED);
    _AutoRefresh();
}

void HPlotter::PlotCriticalPath(HCriticalPath aPath)
{
    if (IsEnabled())
    {
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
}

void HPlotter::PlotNetSteinerTree(HNet net, Color color)
{
    if (!IsEnabled()) return;

    AdaptiveRoute(m_hd, net);

    HNetWrapper netw = m_hd[net];
    HPinWrapper src =  m_hd[netw.Source()];
    HSteinerPointWrapper srcPoint = m_hd[m_hd.SteinerPoints[src]];
    HSteinerPointWrapper nextPoint = srcPoint;

    m_hd.Plotter.DrawCircle2(srcPoint.X(), srcPoint.Y(), 4, color);

    TemplateTypes<HSteinerPoint>::stack points;
    points.push(srcPoint);
    while (!points.empty())
    {
        srcPoint = points.top();
        points.pop();

        if (srcPoint.HasLeft())
        {
            nextPoint = srcPoint.Left();
            m_hd.Plotter.DrawLine2(srcPoint.X(), srcPoint.Y(), nextPoint.X(), nextPoint.Y(), color);
            points.push(nextPoint);

            if (srcPoint.HasRight())
            {
                nextPoint = srcPoint.Right();
                m_hd.Plotter.DrawLine2(srcPoint.X(), srcPoint.Y(), nextPoint.X(), nextPoint.Y(), color);
                points.push(nextPoint);
            }
        }
        else
        {
            m_hd.Plotter.DrawCircle2(srcPoint.X(), srcPoint.Y(), 1, color);
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

    m_hd.Plotter.DrawCircle2(srcPoint.X(), srcPoint.Y(), 4, color);
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
            m_hd.Plotter.DrawLine2(srcPoint.X(), srcPoint.Y(), nextPoint.X(), nextPoint.Y(), color);
            points.push(nextPoint);

            if (srcPoint.HasRight())
            {
                nextPoint = srcPoint.Right();
                m_hd.Plotter.DrawLine2(srcPoint.X(), srcPoint.Y(), nextPoint.X(), nextPoint.Y(), color);
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
                m_hd.Plotter.DrawCircle2(srcPoint.X(), srcPoint.Y(), 4, color);
                bool f = false;
                if (srcPoint.HasLeft())
                {
                  f = true;
                    nextPoint = srcPoint.Left();
                    m_hd.Plotter.DrawLine2(srcPoint.X(), srcPoint.Y(), nextPoint.X(), nextPoint.Y(), color);
                    points.push(nextPoint);

                    if (srcPoint.HasRight())
                    {
                        nextPoint = srcPoint.Right();
                        m_hd.Plotter.DrawLine2(srcPoint.X(), srcPoint.Y(), nextPoint.X(), nextPoint.Y(), color);
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
            m_hd.Plotter.DrawCircle2(srcPoint.X(), srcPoint.Y(), 1, color);
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

    m_hd.Plotter.DrawCircle2(srcPoint->x, srcPoint->y, 4, LineColor);

    TemplateTypes<VanGinnekenTreeNode*>::stack points;
    points.push(srcPoint);
    while (!points.empty())
    {
        srcPoint = points.top();
        points.pop();

        if (srcPoint->HasLeft())
        {
            nextPoint = srcPoint->GetLeft();
            m_hd.Plotter.DrawLine2(srcPoint->x, srcPoint->y, nextPoint->x, nextPoint->y, LineColor);
            m_hd.Plotter.DrawCircle2(srcPoint->x, srcPoint->y, 1, VGNodeColor);
            //m_hd.Plotter.DrawRectangle(nextPoint->x, nextPoint->y, nextPoint->x + DesignX2ImageX(5), nextPoint->y + DesignY2ImageY(5),  Color(color + 1));
            points.push(nextPoint);

            if (srcPoint->HasRight())
            {
                nextPoint = srcPoint->GetRight();
                m_hd.Plotter.DrawLine2(srcPoint->x, srcPoint->y, nextPoint->x, nextPoint->y, LineColor);
                m_hd.Plotter.DrawCircle2(srcPoint->x, srcPoint->y, 1, VGNodeColor);
                //m_hd.Plotter.DrawRectangle(nextPoint->x, nextPoint->y, nextPoint->x + DesignX2ImageX(5), nextPoint->y + DesignY2ImageY(5),  Color(color + 1));
                points.push(nextPoint);
            }
        }
        else
        {
            m_hd.Plotter.DrawCircle2(srcPoint->x, srcPoint->y, 1, LineColor);
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