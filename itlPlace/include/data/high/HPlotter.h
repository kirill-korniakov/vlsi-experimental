#pragma once

#include "HBaseDesignItem.h"
#include "HCell.h"
#include "Color.h"
#include "HCriticalPath.h"
#include "HNet.h"
#include "HSteinerPoint.h"

class VanGinnekenTreeNode;

struct AppCtx;
class HDPGrid;
class TileGrid;

class HPlotter
{
public:
  HPlotter(HDesign& design);
  ~HPlotter();

  enum WaitTime
  { 
    NO_WAIT = 1,
    WAIT_1_SECOND = 1000,
    WAIT_3_SECONDS = 3000,
    WAIT_5_SECONDS = 5000,
    WAIT_10_SECONDS = 10000
  };

  void Initialize();
  bool IsEnabled();
  void Destroy();

  void Clear();
  void Refresh(WaitTime waitTime = NO_WAIT);
  void Refresh(const string& cfgPath);
  void SetAutoRefreshMinFrequency(int freq);
  void ResetAutoRefreshFrequency();

  void InitializeHistogramWindow();
  void DestroyHistogramWindow();
  void ClearHistogram();
  void RefreshHistogram(WaitTime waitTime = NO_WAIT);

  void SaveImage(string fileName = "", string dirName = ""); 
  void SaveMilestoneImage(string fileSuffix, bool addToHtmlLog = true);

  void StartVideoWriting(string fileName = "", string dirName = ""); 
  void WriteCurrentFrame();
  void StopVideoWriting();

  //Show methods
  void ShowPlacement(WaitTime waitTime = NO_WAIT);
  void AutoShowPlacement(WaitTime waitTime = NO_WAIT);
  void ShowGlobalPlacement(bool plotWires, int nBinRows, int nBinCols, WaitTime waitTime = NO_WAIT);
  void VisualizeState(int nClusters, int nBinRows, int nBinCols, int nNets,
                      double* x, double* gLSE, double* gSOD, double* gLR, double* gQS, double* g);
  void ShowLegalizationState(WaitTime waitTime, bool drawSites);
  void ShowSteinerForest(Color color, WaitTime waitTime = NO_WAIT);
  void ShowNetSteinerTree(HNet net, Color color, bool isShowPlacement = false, WaitTime waitTime = NO_WAIT, double textSize = -1);
  void ShowCriticalPathSteinerTree(HCriticalPath net, Color color, bool isShowPlacement = false, WaitTime waitTime = NO_WAIT, double textSize = -1);
  void ShowVGTree(HNet net, VanGinnekenTreeNode* tree, Color LineColor, bool isShowPlacement = false, WaitTime waitTime = NO_WAIT, Color VGNodeColor = Color_Red, double textSize = -1);
  void ShowNet(HNet net, bool isShowPlacement = false, WaitTime waitTime = NO_WAIT, Color color = Color_Peru, double textSize = -1);
  
  //Plot methods
  void PlotPlacement();
  static void PlotPath(HDesign& design, HCriticalPath path, int pathNumber);
  void PlotBinGrid(int nBinRows, int nBinCols);
  void PlotFillBinGrid(AppCtx* context);
  void PlotAntiGradients(int nClusters, double* coordinates, double* gradients, double scaling, Color col);
  void PlotCell(HCell cell, Color col);
  void PlotSites();
  void PlotCriticalPath(HCriticalPath);
  void PlotNetSteinerTree(HNet net, Color color);
  void PlotCriticalPathSteinerTree(HCriticalPath path, Color color);
  void PlotVGTree(VanGinnekenTreeNode* tree, Color LineColor, Color VGNodeColor = Color_Red);
  void PlotNet(HNet net, Color color = Color_Peru);
  void PlotFullWires();
  void Plot2PinNets();
  void PlotMostCriticalPaths(HDesign& design, int n = -1);

  void PlotKi(int nClusters, int nNets, double* x, Color color);

  void PlotMu(int tpIdx, int nTP, double mu, double scaling, Color color);
  void PlotMu(double mu, int x, double scaling, Color color);
  void PlotMuLevel(double level, double scaling, Color color = Color_Black);

  //Draw methods
  void DrawRectangle(double x1, double y1, double x2, double y2, Color col, bool doRefresh = true);
  void DrawFilledRectangle(double x, double y, double width, double height, Color col, bool doRefresh = true);
  void DrawFilledRectangleWithBorder(double x1, double y1, double x2, double y2, Color borderColor,
                                     Color fillColor, bool doRefresh = true);
  void DrawText(string text, double textSize = -1);
  void DrawTextInPoint(string text, double x, double y, double textSize = -1);
  void DrawLine(double x1, double y1, double x2, double y2, Color col, bool doRefresh = true);
  void DrawCircle(double x, double y, int radius, Color col, int thickness, bool doRefresh = true);
  void DrawCircle(double x, double y, int radius, Color col, bool doRefresh = true);
  void DrawBar(double x1, double y1, double x2, double y2, Color col, bool doRefresh = true);

  void DrawTextLine();

protected:
  void* m_data;
  HDesign& m_hd;
  bool m_isDestroyed;
  bool m_isHistogramDestroyed;

private:
  bool _IsEnabled();
  void _AutoRefresh();
  bool CantPlotHistogram();

  Color _GetCellColor(HCell cell);
};
