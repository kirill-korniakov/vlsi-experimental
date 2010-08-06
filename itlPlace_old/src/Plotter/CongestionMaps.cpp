#include "HPlotter.h"
#include "TileGrid.h"

#include "conio.h"

void HPlotter::PlotCongestionMaps(HDPGrid& DPGrid)
{
    int nHorTiles  = DPGrid.NumRows() / 2;
    int nVertTiles = nHorTiles;
    int nMaxLines  = DPGrid.Design().cfg.ValueOf("CongestionMap.nMaxLines", 10);
    int nMaxPins   = DPGrid.Design().cfg.ValueOf("CongestionMap.nMaxPins", 10);
    int nMaxCLines = DPGrid.Design().cfg.ValueOf("CongestionMap.nMaxCLines", 4);

    TileGrid tileGrid(nHorTiles, nVertTiles, DPGrid);   

    DrawCongestionMap(DPGrid.Design(), tileGrid, nMaxLines);
    ALERT("press any key to continue...");
    _getch();

    DrawPinDensity(DPGrid.Design(), tileGrid, nMaxPins);
    ALERT("press any key to continue...");
    _getch();

    DrawCriticalCongestionMap(DPGrid.Design(), tileGrid, nMaxCLines);
    ALERT("press any key to continue...");
    _getch();
}

void HPlotter::DrawCongestionMap(HDesign& hd, TileGrid& tg, int nMaxLines)
{
    tg.CalcLinesInTiles(hd);

    for (int i = 0; i < tg.nHorTiles; i++)
    {
        for (int j = 0; j < tg.nVertTiles; j++)
        {            
            double x1 = tg.tiles[i][j].GetX();
            double y1 = tg.tiles[i][j].GetY();
            double x2 = x1 + tg.tileWidth;
            double y2 = y1 + tg.tileHeight;
            hd.Plotter.DrawTileWires(x1, y1, x2, y2, tg.tiles[i][j].GetNWires(), nMaxLines);
        }
    }
}

void HPlotter::DrawPinDensity(HDesign& hd, TileGrid& tg, int nMaxPins)
{
    //calculate number of pins in each tile
    for (HNets::ActiveNetsEnumeratorW curNet = hd.Nets.GetActiveNetsEnumeratorW(); curNet.MoveNext();)
    {
        for (HNetWrapper::PinsEnumeratorW currPin = curNet.GetPinsEnumeratorW(); currPin.MoveNext();)
        {
            TileIndexes currTileIdxs = tg.FindTileByXY(currPin.X(), currPin.Y());
            tg.tiles[currTileIdxs.horInd][currTileIdxs.vertInd].IncNPins();
        }
    }

    //draw pin density
    for (int i = 0; i < tg.nHorTiles; i++)
    {
        for (int j = 0; j < tg.nVertTiles; j++)
        {
            double x1 = tg.tiles[i][j].GetX();
            double y1 = tg.tiles[i][j].GetY();
            double x2 = x1 + tg.tileWidth;
            double y2 = y1 + tg.tileHeight;
            hd.Plotter.DrawTilePins(x1, y1, x2, y2, tg.tiles[i][j].GetNPins(), nMaxPins);
        }
    }
}

void HPlotter::DrawCriticalCongestionMap(HDesign &hd, TileGrid& tg, int nMaxÑLines)
{
    for (HCriticalPaths::EnumeratorW path = hd.CriticalPaths.GetEnumeratorW(); path.MoveNext();)
    {
        HCriticalPath::PointsEnumeratorW currPoint = path.GetEnumeratorW();
        currPoint.MoveNext();
        int nCriticalPoints = 1;
        HPinWrapper currPin = hd[hd.Get<HTimingPoint::Pin, HPin>(currPoint.TimingPoint())];
        double x1 = currPin.X();
        double y1 = currPin.Y();

        while (currPoint.MoveNext())
        {
            nCriticalPoints++;
            HPinWrapper currPin = hd[hd.Get<HTimingPoint::Pin, HPin>(currPoint.TimingPoint())];
            //TileIndexes currTileIdxs = FindTileByXY(currPin.X(), currPin.Y());
            //tiles[currTileIdxs.horInd][currTileIdxs.vertInd].IncNPins();
            HPinWrapper currentPin = hd[hd.Get<HTimingPoint::Pin, HPin>(currPoint.TimingPoint())];
            double x2 = currentPin.X();
            double y2 = currentPin.Y();
            Line newLine(x1, y1, x2, y2);

            if (newLine.GetLineType() > -1)
                tg.criticalLines.push_back(newLine);
            x1 = x2;
            y1 = y2;
            currPoint.MoveNext();
        }

        if (nCriticalPoints <= 1)
        {
            WRITE("only one point in critical path\n");
        }
    }

    //draw
    tg.CalcCriticalWires(hd);

    if (tg.lines.size() != 0)
    {
        int nMaxAllLines  = hd.cfg.ValueOf("CongestionMap.nMaxLines", 10);
        nMaxÑLines = nMaxAllLines * tg.criticalLines.size() / tg.lines.size();
    }

    for (int i = 0; i < tg.nHorTiles; i++)
    {
        for (int j = 0; j < tg.nVertTiles; j++)
        {            
            double x1 = tg.tiles[i][j].GetX();
            double y1 = tg.tiles[i][j].GetY();
            double x2 = x1 + tg.tileWidth;
            double y2 = y1 + tg.tileHeight;
            hd.Plotter.DrawTileWires(x1, y1, x2, y2, tg.tiles[i][j].GetNCriticalWires(),
                nMaxÑLines);
        }
    }
}