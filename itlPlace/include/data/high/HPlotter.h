#ifndef __HIGH_PLOTTER_H__
#define __HIGH_PLOTTER_H__

#include "HBaseDesignItem.h"
#include "HCell.h"
#include "Color.h"
#include "HCriticalPath.h"
#include "HNet.h"
#include "HSteinerPoint.h"

class HPlotter
{
protected:
  void* m_data;
  HDesign& m_hd;
  bool m_isDestroyed;

public:
  HPlotter(HDesign& design);
  ~HPlotter();

  void Initialize();
  void Destroy();
  bool IsEnabled();

  enum WaitTime
  { 
    NO_WAIT = 1,
    WAIT_1_SECOND = 1000,
    WAIT_3_SECONDS = 3000,
    WAIT_5_SECONDS = 5000,
    WAIT_10_SECONDS = 10000
  };

  void ShowPlacement(WaitTime waitTime = NO_WAIT);
  void AutoShowPlacement(WaitTime waitTime = NO_WAIT);
  void ShowGlobalPlacement(bool plotWires, int nBinRows, int nBinCols, WaitTime waitTime = NO_WAIT);
  void ShowGradients(int nClusters, int nBinRows, int nBinCols, int nNets,
    double* x, double* gLSE, double* gSOD, double* gLR, double* gQS, double* g,
    double scaling, WaitTime waitTime = NO_WAIT);
  void ShowLegalizationState(WaitTime waitTime, bool drawSites);

  void DrawTileWires(double x1, double y1, double x2, double y2, int nLines,
                     int nMaxLines);

  void DrawTilePins(double x1, double y1, double x2, double y2, int nPins,
                    int nMaxPins);
  void PlotPlacement();
  void PlotBinGrid(int nBinRows, int nBinCols);
  void PlotGradients(int nClusters, double* coordinates, double* gradients, double scaling, Color col);
  void PlotCell(HCell cell, Color col);
  void PlotSites();
  void PlotCriticalPath(HCriticalPath, string fileName);
  void PlotNetSteinerTree(HNet net, Color color);
  void PlotText(string text, double textSize = -1);
  void PlotNet(HNetWrapper net);
  void PlotFullWires();
  void Plot2PinNets();
  void PlotKi(int nClusters, int nNets, double* x, Color color);

  void SizeChange(int changeHeight);

  void DrawLine(double x1, double y1, double x2, double y2, Color col);
  void DrawCircle(double x, double y, int radius, Color col);
  void DrawRectangle(double x1, double y1, double x2, double y2, Color col);
  void DrawBar(double x1, double y1, double x2, double y2, Color col);

  void Clear();
  void Refresh(WaitTime waitTime = NO_WAIT);
  void SetAutoRefreshMinFrequency(int freq);
  void ResetAutoRefreshFrequency();

  void SaveImage(string fileName = "", string dirName = ""); 
  void SaveMilestoneImage(string fileSuffix);

  void StartVideoWriting(string fileName = "", string dirName = ""); 
  void WriteCurrentFrame();
  void StopVideoWriting();

private:
  bool _IsEnabled();
  void _AutoRefresh();

  Color _GetCellColor(HCell cell);
};

#endif //__HIGH_PLOTTER_H__

