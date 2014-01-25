#include "HPlotter.h"
#include "PlotterData.h"
#include "HDesign.h"
#include "OptimizationContext.h"

#define MAX_COLOR 255

Color GetBinColor(double cellAreaInBin, double avgCellAndBufferArea);
double CalculateCellAreaInBin(int binRow, int binCol, HCellWrapper cell, BinGrid binGrid, HDesign* hd);
void FullBinOccupancyMap(bool plotCells, bool plotBuffers, std::vector<double>& map, BinGrid binGrid, HDesign* hd);

void HPlotter::PlotBinOccupancyMap(string fileSuffix) 
{
    bool plotCells = m_hd.cfg.ValueOf("LSE.GlobalPlacement.Plotting.BinOccupancyMap.plotCells", false);
    bool plotBuffers = m_hd.cfg.ValueOf("LSE.GlobalPlacement.Plotting.BinOccupancyMap.plotBuffers", false);

    int nBinRows = binGridForPlotting.nBinRows;
    int nBinCols = binGridForPlotting.nBinCols;
    int nBins = binGridForPlotting.nBins;
        
    std::vector<double> map(nBinRows*nBinCols);
   
    FullBinOccupancyMap(plotCells, plotBuffers, map, binGridForPlotting, &m_hd);

    double totalCellAndBufferArea = 0.0;
    for(HCells::PlaceableCellsEnumeratorW cell = m_hd.Cells.GetPlaceableCellsEnumeratorW(); cell.MoveNext();)
    {
        totalCellAndBufferArea += cell.Height()*cell.Width();
    }
    
    double minX = m_hd.Circuit.PlacementMinX();
    double minY = m_hd.Circuit.PlacementMinY();

    double avgCellAndBufferArea = totalCellAndBufferArea/nBins;
    
    Clear();
    for (int i = 0; i < nBinRows; ++i) 
    {
        for (int j = 0; j < nBinCols; ++j) 
        {
            double binX1 = binGridForPlotting.bins[i][j].xCoord - binGridForPlotting.binWidth /2.0 + minX;
            double binY1 = binGridForPlotting.bins[i][j].yCoord - binGridForPlotting.binHeight/2.0 + minY;
            double binX2 = binX1 + binGridForPlotting.binWidth;
            double binY2 = binY1 + binGridForPlotting.binHeight;
            double cellAreaInBin = map[i*nBinCols + j];
            Color binColor = GetBinColor(cellAreaInBin, avgCellAndBufferArea);
            Color borderColor = Color_GrayText;
            
            DrawFilledRectangleWithBorder(binX1, binY1, binX2, binY2, borderColor, binColor, false);
        }
    }
    SaveMilestoneImage(fileSuffix);
}

void FullBinOccupancyMap(bool plotCells, bool plotBuffers, std::vector<double>& map, BinGrid binGrid, HDesign* hd)
{
    int nBinRows = binGrid.nBinRows;
    int nBinCols = binGrid.nBinCols;

    if(plotCells && plotBuffers)
    {
        for(HCells::PlaceableCellsEnumeratorW cell = hd->Cells.GetPlaceableCellsEnumeratorW(); cell.MoveNext();)
            for (int i = 0; i < nBinRows; ++i)
                for (int j = 0; j < nBinCols; ++j)
                    map[i*nBinCols + j] += CalculateCellAreaInBin(i, j, cell, binGrid, hd);
    }
    else if(plotBuffers) 
    {
        for(HCells::PlaceableCellsEnumeratorW cell = hd->Cells.GetPlaceableCellsEnumeratorW(); cell.MoveNext();) 
        {
            bool isBuffer = (MacroType_BUF == (hd->Get<HMacroType::Type, MacroType>(hd->Get<HCell::MacroType, HMacroType>(cell))));
            if(isBuffer) 
            {         
                for (int i = 0; i < nBinRows; ++i)
                    for (int j = 0; j < nBinCols; ++j)
                        map[i*nBinCols + j] += CalculateCellAreaInBin(i, j, cell, binGrid, hd);
            }
        }
    }
    else if(plotCells) 
    {
        for(HCells::PlaceableCellsEnumeratorW cell = hd->Cells.GetPlaceableCellsEnumeratorW(); cell.MoveNext();) 
        {
            bool isBuffer = (MacroType_BUF == (hd->Get<HMacroType::Type, MacroType>(hd->Get<HCell::MacroType, HMacroType>(cell))));
            if(!isBuffer) 
            {               
                for (int i = 0; i < nBinRows; ++i)
                    for (int j = 0; j < nBinCols; ++j)
                        map[i*nBinCols + j] += CalculateCellAreaInBin(i, j, cell, binGrid, hd);
            }
        }
    }
}

Color GetBinColor(double cellAreaInBin, double avgCellAndBufferArea)//, int nColors)
{
     Color binColor = Color_White;

     if (cellAreaInBin == 0)
     {
         binColor = Color_Black;//RGBToColor(0, 0, 0);
     }
     else if(cellAreaInBin >= 2*avgCellAndBufferArea)
     {
         binColor = Color_White;//RGBToColor(MAX_COLOR, MAX_COLOR, MAX_COLOR);
     }
     else
     { 
         int nColorsInLevel = (MAX_COLOR - 1) / 2;
         double a = avgCellAndBufferArea/nColorsInLevel;

         for (int i = 1; i < MAX_COLOR; ++i)
         {   
             if((i-1)*a < cellAreaInBin && cellAreaInBin <= i*a)
             { 
                 binColor = RGBToColor(i, i, i);
                 break;
             }
         }
     }
       

     return binColor;
}

double CalculateCellAreaInBin(int binRow, int binCol, HCellWrapper cell, BinGrid binGrid, HDesign* hd) 
{
    double binX1 = binGrid.bins[binRow][binCol].xCoord - binGrid.binWidth /2.0;
    double binY1 = binGrid.bins[binRow][binCol].yCoord - binGrid.binHeight/2.0;
    double binX2 = binX1 + binGrid.binWidth;
    double binY2 = binY1 + binGrid.binHeight;

    double x1 = cell.X() - hd->Circuit.PlacementMinX();
    double y1 = cell.Y() - hd->Circuit.PlacementMinY();
    double x2 = x1 + cell.Width();
    double y2 = y1 + cell.Height();

    double height = 0.0;
    double width = 0.0;

    if(x1 <= binX1)
        if(x2 >= binX2)
            width = binGrid.binWidth;
        else if (x2 <= binX1)
            width = 0.0;
        else width = x2 - binX1;
    else if (x1 < binX2)
        if(x2 <= binX2)
            width = x2 - x1;
        else width = binX2 - x1;
    else width = 0.0;

    if(width) {
        if(y1 <= binY1)
            if(y2 >= binY2)
                height = binGrid.binHeight;
            else if (y2 <= binY1)
                height = 0.0;
            else height = y2 - binY1;
        else if (y1 < binY2)
            if(y2 <= binY2)
                height = y2 - y1;
            else height = binY2 - y1;
        else height = 0;
    }

    return (height*width);
}
