#include "HPlotter.h"
#include "PlotterData.h"
#include "HDesign.h"
#include "Auxiliary.h"

void HPlotter::StartVideoWriting(string fileName, string dirName)
{
    CvSize movieFrameSize = cvSize(m_Data->img->width, m_Data->img->height);
    string movieFileName;
    if (fileName == "")
    { 
        movieFileName = Aux::CreateCoolFileName(dirName,  m_hd.Circuit.Name() ,".avi");
    }
    else
        movieFileName = Aux::CreateCoolFileName(dirName, fileName, ".avi");

    char codecName[] = "xvid";
    double fps = m_hd.cfg.ValueOf("plotter.fps", 1.0);
    m_Data->vw = cvCreateVideoWriter(movieFileName.c_str(), 
        (CV_FOURCC(codecName[0], codecName[1], codecName[2], codecName[3])),
        fps, movieFrameSize);

    //TODO: check if writer created
}

void HPlotter::WriteCurrentFrame()
{
    //TODO: check if writer created
    cvWriteFrame(m_Data->vw, IMG);
}


void HPlotter::StopVideoWriting()
{
    if (m_Data->vw)
    {
        cvReleaseVideoWriter(&m_Data->vw);
    }
}