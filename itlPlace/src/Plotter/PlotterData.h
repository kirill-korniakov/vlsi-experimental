#ifndef __PLOTTER_DATA__
#define __PLOTTER_DATA__

#include "OpenCV/cv.h"
#include "OpenCV/highgui.h"

struct PlotterData
{
    IplImage* img;
    string windowName;
    CvVideoWriter* vw;

    IplImage* histogramImg;
    string histogramWindowName;

    int autoRefreshStepDefault;
    int autoRefreshStep;
    int drawingsNum;

    int borderSpace;
    double x_offset;
    double y_offset;
    double x_ratio;
    double y_ratio;
    int textSpaceHeight;
};

static inline int _DesignX2ImageX(double x, PlotterData* data)
{
    return data->borderSpace + (int)((x + data->x_offset) * data->x_ratio);
}

static inline int _DesignY2ImageY(double y, PlotterData* data)
{
    return data->borderSpace + (int)((y + data->y_offset) * data->y_ratio);
}

static inline int _NormalX2ImageX(double x, int width)
{
    return (int)(x * width);
}

static inline int _NormalY2ImageY(double y, int shiftY, int height)
{
    return shiftY + (int)(y * height);
}

#define m_Data ((PlotterData*)m_data)
#define IMG m_Data->img
#define DesignX2ImageX(x) _DesignX2ImageX((x), m_Data)
#define DesignY2ImageY(y) _DesignY2ImageY((y), m_Data)
#define NormalX2ImageX(x) _NormalX2ImageX((x), m_Data->img->width)
#define NormalY2ImageY(y) _NormalY2ImageY((y), data->textSpaceHeight, m_Data->img->height - m_Data->textSpaceHeight)
#define HNormalX2ImageX(x) _NormalX2ImageX((x), m_Data->histogramImg->width)
#define HNormalY2ImageY(y) _NormalY2ImageY((y), 0, m_Data->histogramImg->height)

static inline CvScalar GetCvColor(Color col)
{
    CvScalar res;
    ::ToRGB(col, res.val[2], res.val[1], res.val[0]);
    res.val[3] = 0.0;
    return res;
}

#endif