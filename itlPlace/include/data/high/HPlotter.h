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

class HPlotter
{
public:
  HPlotter::HPlotter(HDesign& design):m_hd(design) {}
  virtual ~HPlotter() {}
  virtual void Initialize() = 0;

  enum WaitTime
  { 
    NO_WAIT = 1,
    WAIT_1_SECOND = 1000,
    WAIT_3_SECONDS = 3000,
    WAIT_5_SECONDS = 5000,
    WAIT_10_SECONDS = 10000
  };

  virtual bool IsEnabled() = 0;
  virtual void Clear() = 0;

  void Refresh(const string& cfgPath);
  virtual void Refresh(WaitTime waitTime = NO_WAIT) = 0;
  virtual void SetAutoRefreshMinFrequency(int freq) = 0;
  virtual void ResetAutoRefreshFrequency() = 0;

  virtual void InitializeHistogramWindow() = 0;
  ///void DestroyHistogramWindow();
  virtual void ClearHistogram() = 0;
  virtual void RefreshHistogram(WaitTime waitTime = NO_WAIT) = 0;

  virtual void SaveImage(string fileName = "", string dirName = "") = 0;
  virtual void SaveMilestoneImage(string fileSuffix, bool addToHtmlLog = true) = 0;

  virtual void StartVideoWriting(string fileName = "", string dirName = "") = 0;
  virtual void WriteCurrentFrame() = 0;
  virtual void StopVideoWriting() = 0;

  //Show methods
  void ShowPlacement(WaitTime waitTime = NO_WAIT);
  virtual void AutoShowPlacement(WaitTime waitTime = NO_WAIT) = 0;
  void ShowGlobalPlacement(bool plotWires, int nBinRows, int nBinCols, WaitTime waitTime = NO_WAIT);
  void VisualizeState(int nClusters, int nBinRows, int nBinCols, int nNets,
                      double* x, double* gLSE, double* gSOD, double* gLR, double* gQS, double* g);
  void ShowLegalizationState(WaitTime waitTime, bool drawSites);
  void ShowSteinerForest(Color color, WaitTime waitTime = NO_WAIT);
  void ShowNetSteinerTree(HNet net, Color color, bool isShowPlacement = false,
                          WaitTime waitTime = NO_WAIT, double textSize = -1);

  void ShowCriticalPathSteinerTree(HCriticalPath net, Color color, bool isShowPlacement = false,
                                   WaitTime waitTime = NO_WAIT, double textSize = -1);

  void ShowVGTree(HNet net, VanGinnekenTreeNode* tree, Color LineColor, bool isShowPlacement = false,
                  WaitTime waitTime = NO_WAIT, Color VGNodeColor = Color_Red, double textSize = -1);

  void ShowNet(HNet net, bool isShowPlacement = false, WaitTime waitTime = NO_WAIT,
               Color color = Color_Peru, double textSize = -1);
  
  //Plot methods
  void PlotPlacement();
  static void PlotPath(HDesign& design, HCriticalPath path, int pathNumber);
  void PlotBinGrid(int nBinRows, int nBinCols);
  void PlotFillBinGrid(AppCtx* context);
  void PlotAntiGradients(int nClusters, double* coordinates, double* gradients, double scaling,
                         Color col);

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
  virtual void PlotMuLevel(double level, double scaling, Color color = Color_Black) = 0;

  //Draw methods
  virtual void DrawRectangle(double x1, double y1, double x2, double y2, Color col,
                             bool doRefresh = true) = 0;

  virtual void DrawFilledRectangle(double x, double y, double width, double height, Color col,
                                   bool doRefresh = true) = 0;

  virtual void DrawFilledRectangleWithBorder(double x1, double y1, double x2, double y2,
                                        Color borderColor, Color fillColor, bool doRefresh = true) = 0;

  virtual void DrawText(string text, double textSize = -1) = 0;
  virtual void DrawTextInPoint(string text, double x, double y, double textSize = -1) = 0;
  virtual void DrawLine(double x1, double y1, double x2, double y2, Color col, bool doRefresh = true,
                        int thickness = 1, int lineType = 8) = 0;

  virtual void DrawKiLine(double x1, double y1, double x2, double y2, Color col,
                          bool doRefresh = true) = 0;

  virtual void DrawCircle(double x, double y, int radius, Color col, bool doRefresh = true,
                          int thickness = 1) = 0;

  virtual void DrawBar(double x1, double y1, double x2, double y2, Color col,
                       bool doRefresh = true) = 0;

  virtual void InitFont() = 0;
  virtual void DrawTextLine() = 0;

protected:
  HDesign& m_hd;

protected:
  bool _IsEnabled();
  virtual void _AutoRefresh() = 0;
  virtual bool CantPlotHistogram() = 0;

  Color _GetCellColor(HCell cell);
};
