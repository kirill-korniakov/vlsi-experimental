#ifndef _OPEN_CV_PLOTTER_
#define _OPEN_CV_PLOTTER_

#include "HPlotter.h"

class OpenCVPlotter: public HPlotter
{
public:
  OpenCVPlotter(HDesign& design);
  ~OpenCVPlotter();

  void Initialize();
  bool IsEnabled();
  void Destroy();

  void Clear();
  void Refresh(WaitTime waitTime = NO_WAIT);
  void SetAutoRefreshMinFrequency(int freq);
  void ResetAutoRefreshFrequency();

  //------Draw methods-----
  void DrawBar(double x1, double y1, double x2, double y2, Color col, bool doRefresh = true);
  void DrawCircle(double x, double y, int radius, Color col, bool doRefresh = true, int thickness = 1);
  void DrawRectangle(double x1, double y1, double x2, double y2, Color col, bool doRefresh = true);
  void DrawFilledRectangle(double x, double y, double width, double height, Color col,
                           bool doRefresh = true);

  void DrawFilledRectangleWithBorder(double x1, double y1, double x2, double y2, Color borderColor,
                                     Color fillColor, bool doRefresh = true);

  void DrawLine(double x1, double y1, double x2, double y2, Color col, bool doRefresh = true,
                int thickness = 1, int lineType = 8);

  void DrawKiLine(double x1, double y1, double x2, double y2, Color col, bool doRefresh = true);
  void DrawText(string text, double textSize = -1);
  void DrawTextInPoint(string text, double x, double y, double textSize = -1);
  void InitFont();
  void DrawTextLine();
  //--------------------------

  virtual void AutoShowPlacement(WaitTime waitTime = NO_WAIT);

  void SaveImage(string fileName = "", string dirName = ""); 
  void SaveMilestoneImage(string fileSuffix, bool addToHtmlLog = true);

  void StartVideoWriting(string fileName = "", string dirName = ""); 
  void WriteCurrentFrame();
  void StopVideoWriting();

  void PlotMuLevel(double level, double scaling, Color color = Color_Black);
  void InitializeHistogramWindow();
  void DestroyHistogramWindow();
  void ClearHistogram();
  void RefreshHistogram(WaitTime waitTime = NO_WAIT);

protected:
  void* m_data;
  bool m_isDestroyed;
  bool m_isHistogramDestroyed;

private:
  void _AutoRefresh();
  bool CantPlotHistogram();
};

#endif
