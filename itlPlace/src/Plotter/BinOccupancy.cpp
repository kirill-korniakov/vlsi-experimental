#include "HPlotter.h"
#include "HDesign.h"
#include "PlotterData.h"
#include "OptimizationContext.h"

double CalculateCellAreaInBin(int binRow, int binCol, HCellWrapper cell, AppCtx* context) 
{
    BinGrid& binGrid = context->sprData.binGrid;

    double binX1 = binGrid.bins[binRow][binCol].xCoord - binGrid.binWidth /2.0 + context->hd->Circuit.PlacementMinX();
    double binY1 = binGrid.bins[binRow][binCol].yCoord - binGrid.binHeight/2.0 + context->hd->Circuit.PlacementMinY();
    double binX2 = binX1 + binGrid.binWidth;
    double binY2 = binY1 + binGrid.binHeight;

    double x1 = cell.X();
    double y1 = cell.Y();
    double x2 = cell.X() + cell.Width();
    double y2 = cell.Y() + cell.Height();

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

void HPlotter::PlotBinOccupancyMap(AppCtx* context, string fileSuffix) 
{
    bool plotCells = m_hd.cfg.ValueOf("LSE.GlobalPlacement.Plotting.BinOccupancyMap.plotCells", false);
    bool plotBuffers = m_hd.cfg.ValueOf("LSE.GlobalPlacement.Plotting.BinOccupancyMap.plotBuffers", false);

    BinGrid& binGrid = context->sprData.binGrid;
    int nBinRows = binGrid.nBinRows;
    int nBinCols = binGrid.nBinCols;
        
    Clear();

    std::vector<double> map(nBinRows*nBinCols);
     
    if(plotCells && plotBuffers) {
        for(HCells::PlaceableCellsEnumeratorW cell = m_hd.Cells.GetPlaceableCellsEnumeratorW(); cell.MoveNext();) {
            for (int i = 0; i < nBinRows; ++i) {
                for (int j = 0; j < nBinCols; ++j) {
                    map[i*nBinCols + j] += CalculateCellAreaInBin(i, j, cell, context);
                }
            }
        }
    }
    else if(plotBuffers) {
        for(HCells::PlaceableCellsEnumeratorW cell = m_hd.Cells.GetPlaceableCellsEnumeratorW(); cell.MoveNext();) {
            bool isBuffer = (MacroType_BUF == (m_hd.Get<HMacroType::Type, MacroType>(m_hd.Get<HCell::MacroType, HMacroType>(cell))));
            if(isBuffer) {         
                for (int i = 0; i < nBinRows; ++i) {
                    for (int j = 0; j < nBinCols; ++j) {
                        map[i*nBinCols + j] += CalculateCellAreaInBin(i, j, cell, context);
                    }
                }
            }
        }
    }
    else if(plotCells) {
        for(HCells::PlaceableCellsEnumeratorW cell = m_hd.Cells.GetPlaceableCellsEnumeratorW(); cell.MoveNext();) {
            bool isBuffer = (MacroType_BUF == (m_hd.Get<HMacroType::Type, MacroType>(m_hd.Get<HCell::MacroType, HMacroType>(cell))));
            if(!isBuffer) {               
                for (int i = 0; i < nBinRows; ++i) {
                    for (int j = 0; j < nBinCols; ++j) {
                        map[i*nBinCols + j] += CalculateCellAreaInBin(i, j, cell, context);
                    }
                }
            }
        }
    }

    double totalCellAndBufferArea = 0.0; //!=context->sprData.totalCellArea;
    for(HCells::PlaceableCellsEnumeratorW cell = m_hd.Cells.GetPlaceableCellsEnumeratorW(); cell.MoveNext();)
    {
        totalCellAndBufferArea += cell.Height()*cell.Width();
    }
    //ALERT("totalCellAndBufferArea = %f", totalCellAndBufferArea);
    //ALERT("totalCellArea          = %f", context->sprData.totalCellArea);

    double avgCellAndBufferArea = totalCellAndBufferArea/binGrid.nBins;
    for (int i = 0; i < nBinRows; ++i) 
    {
        for (int j = 0; j < nBinCols; ++j) 
        {
            double binX1 = binGrid.bins[i][j].xCoord - binGrid.binWidth /2.0 + m_hd.Circuit.PlacementMinX();
            double binY1 = binGrid.bins[i][j].yCoord - binGrid.binHeight/2.0 + m_hd.Circuit.PlacementMinY();
            double binX2 = binX1 + binGrid.binWidth;
            double binY2 = binY1 + binGrid.binHeight;
            double cellAreaInBin = map[i*nBinCols + j];
            Color color;
            if(cellAreaInBin > 2*avgCellAndBufferArea) color = Color_Black;
            else if(cellAreaInBin > avgCellAndBufferArea) color = Color_Gray;
            else color = Color_White;
            DrawFilledRectangleWithBorder(binX1, binY1, binX2, binY2, Color_GrayText, color, false);
        }
    }

    SaveMilestoneImage(fileSuffix);
}