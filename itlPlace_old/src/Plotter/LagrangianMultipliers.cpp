#include "HPlotter.h"
#include "HDesign.h"
#include "PlotterData.h"

bool HPlotter::CantPlotHistogram() 
{
    return !IsEnabled() || m_isHistogramDestroyed;
}

void HPlotter::PlotMu(int tpIdx, int nTP, double mu, double scaling, Color color)
{
    if (CantPlotHistogram()) return;

    int x = HNormalX2ImageX((double)tpIdx/(double)nTP);
    PlotMu(mu, x, scaling, color);
}

void HPlotter::PlotMu(double mu, int x, double scaling, Color color)
{
    if (CantPlotHistogram()) return;

    CvPoint start, finish;
    start.x = x;
    start.y = HNormalY2ImageY(0.0);
    finish.x = start.x;
    finish.y = HNormalY2ImageY(mu * scaling);
    cvDrawLine(m_Data->histogramImg, start, finish, GetCvColor(color));
}

void HPlotter::PlotMuLevel(double level, double scaling, Color color)
{
    if (CantPlotHistogram()) return;

    CvPoint start, finish;
    start.x = HNormalX2ImageX(0.0);
    start.y = HNormalY2ImageY(level * scaling);
    finish.x = m_Data->histogramImg->width;
    finish.y = HNormalY2ImageY(level * scaling);
    cvDrawLine(m_Data->histogramImg, start, finish, GetCvColor(color));

    //print mu level value
    CvFont font;
    cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, 0.5, 0.5);
    char value[256];
    sprintf(value, "%.1f", level);
    cvPutText(m_Data->histogramImg, value, start, &font, GetCvColor(Color_Black));
}

void HPlotter::InitializeHistogramWindow()
{
    if (!_IsEnabled() || !m_isHistogramDestroyed)
        return;

    m_isHistogramDestroyed = false;

    //create window
    m_Data->histogramWindowName = "itlPlace histogram";
    cvNamedWindow(m_Data->histogramWindowName.c_str());

    //create image
    CvSize imgSize;
    imgSize.height = m_hd.cfg.ValueOf("plotter.histogramWindowHeight", 300);
    imgSize.width = m_hd.cfg.ValueOf("plotter.histogramWindowWidth", 1000);
    m_Data->histogramImg = cvCreateImage(imgSize, IPL_DEPTH_8U, 3);
    m_Data->histogramImg->origin = 1;
}

void HPlotter::DestroyHistogramWindow()
{
    if (!m_isHistogramDestroyed)
    {
        cvReleaseImage(&m_Data->histogramImg);
        cvDestroyWindow(m_Data->histogramWindowName.c_str());

        m_isHistogramDestroyed = true;
    }
}

void HPlotter::ClearHistogram()
{
    if (CantPlotHistogram()) return;

    cvSet(m_Data->histogramImg, cvScalar(255.0, 255.0, 255.0));
}

void HPlotter::RefreshHistogram(WaitTime waitTime)
{
    if (CantPlotHistogram()) return;

    cvShowImage(m_Data->histogramWindowName.c_str(), m_Data->histogramImg);
    cvWaitKey(waitTime);
}