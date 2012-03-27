#include "OpenCVPlotter.h"
#include "PlotterData.h"
#include "Utils.h"


OpenCVPlotter::OpenCVPlotter(HDesign& design): HPlotter(design)
{
  m_data = new PlotterData();
  IMG = 0;
  m_Data->vw = NULL;
  m_isDestroyed = true;
  m_isHistogramDestroyed = true;
}

OpenCVPlotter::~OpenCVPlotter()
{
  Destroy();
  DestroyHistogramWindow();
  delete m_Data;
}

void OpenCVPlotter::Initialize()
{
  if (_IsEnabled())
  {
    m_isDestroyed = false;

    m_Data->drawingsNum = 0;
    m_Data->autoRefreshStep
      = m_Data->autoRefreshStepDefault
      = m_hd.cfg.ValueOf("plotter.autoRefreshStep", 200);
    m_Data->borderSpace = m_hd.cfg.ValueOf("plotter.borderSpace", 5);

    //create window
    m_Data->windowName = m_hd.cfg.ValueOf("plotter.windowName", "itlPlace");
    cvNamedWindow(m_Data->windowName.c_str());

    //create image
    CvSize imgSize;
    m_Data->textSpaceHeight = m_hd.cfg.ValueOf("plotter.textSpaceHeight", 200);
    imgSize.height = m_hd.cfg.ValueOf("plotter.windowHeight", 700);
    int userHeight = imgSize.height - m_Data->textSpaceHeight;
    imgSize.width = (int)(userHeight * m_hd.Circuit.Width() / m_hd.Circuit.Height());
    IMG = cvCreateImage(imgSize, IPL_DEPTH_8U, 3);
    IMG->origin = 1;

    //mark text field
    CvPoint startTextLine, finishTextLine;
    startTextLine.x = 0;
    startTextLine.y = m_Data->textSpaceHeight;
    finishTextLine.x = imgSize.width;
    finishTextLine.y = m_Data->textSpaceHeight;
    cvLine(IMG, startTextLine, finishTextLine, GetCvColor(Color_Black), 1);

    //calculate coordinate mapping
    m_Data->x_ratio = (imgSize.width - m_Data->borderSpace * 2) / m_hd.Circuit.Width();
    m_Data->y_ratio = (userHeight - m_Data->borderSpace * 2) / m_hd.Circuit.Height();

    m_Data->x_offset = -m_hd.Circuit.MinX();
    m_Data->y_offset = -m_hd.Circuit.MinY() + m_Data->textSpaceHeight / m_Data->y_ratio ;

    //start video writing
    if (m_hd.cfg.ValueOf("plotter.createVideo", true) == true)
      StartVideoWriting("", ".\\video\\");
  }
}

void OpenCVPlotter::Destroy()
{
  if (!m_isDestroyed)
  {
    cvReleaseImage(&IMG);
    cvDestroyWindow(m_Data->windowName.c_str());

    StopVideoWriting();

    m_isDestroyed = true;
  }
}

bool OpenCVPlotter::IsEnabled()
{
  return !m_isDestroyed && _IsEnabled();
}

void OpenCVPlotter::Clear()
{
  if (IsEnabled())
  {
    cvSet(IMG, cvScalar(255.0, 255.0, 255.0));
  }
}

void OpenCVPlotter::Refresh(WaitTime waitTime)
{
  if (IsEnabled())
  {
    cvShowImage(m_Data->windowName.c_str(), IMG);

    cvWaitKey(waitTime);

    if ((m_hd.cfg.ValueOf("plotter.createVideo", true) == true))
      WriteCurrentFrame();

    if ((m_hd.cfg.ValueOf("plotter.saveImages", true) == true))
      SaveImage("", m_hd.cfg.ValueOf("plotter.pixDirectory"));
  }
}

void OpenCVPlotter::SetAutoRefreshMinFrequency(int freq)
{
  if (freq > m_Data->autoRefreshStepDefault)
    m_Data->autoRefreshStep = freq;
  m_Data->drawingsNum = 0;
}

void OpenCVPlotter::ResetAutoRefreshFrequency()
{
  m_Data->autoRefreshStep = m_Data->autoRefreshStepDefault;
  Refresh();
}

void OpenCVPlotter::_AutoRefresh()
{
  if (!IsEnabled())
    return;

  if (m_Data->drawingsNum++ % m_Data->autoRefreshStep == 0)
  {
    Refresh(NO_WAIT);
  }
}