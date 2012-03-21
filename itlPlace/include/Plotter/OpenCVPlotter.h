#ifndef _OPEN_CV_PLOTTER_
#define _OPEN_CV_PLOTTER_

#include "HPlotter.h"
//#include "PlotterData.h"

class OpenCVPlotter: public HPlotter
{
public:
  OpenCVPlotter(HDesign& design);
  ~OpenCVPlotter() {}
  virtual void AutoShowPlacement(WaitTime waitTime = NO_WAIT);
/*
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

  //Draw methods
  void DrawRectangle(double x1, double y1, double x2, double y2, Color col, bool doRefresh = true);
  void DrawFilledRectangle(double x, double y, double width, double height, Color col, bool doRefresh = true);
  void DrawFilledRectangleWithBorder(double x1, double y1, double x2, double y2, Color borderColor,
                                     Color fillColor, bool doRefresh = true);
  void DrawText(string text, double textSize = -1);
  void DrawTextInPoint(string text, double x, double y, double textSize = -1);
  void DrawLine(double x1, double y1, double x2, double y2, Color col, bool doRefresh = true,
                int thickness = 1, int lineType = 8);
  void DrawKiLine(double x1, double y1, double x2, double y2, Color col, bool doRefresh = true);
  void DrawCircle(double x, double y, int radius, Color col, bool doRefresh = true, int thickness = 1);
  void DrawBar(double x1, double y1, double x2, double y2, Color col, bool doRefresh = true);

  void InitFont();

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

  Color _GetCellColor(HCell cell);*/
};

#endif