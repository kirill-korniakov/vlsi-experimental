#ifndef __MU_REPORTER__
#define __MU_REPORTER__

#include "HDesign.h"

class TimingPointMus;

class MuReporter
{
public:
    MuReporter(HDesign& design);
    void Report(HDesign& design, TimingPointMus* mus, string msg = "");

    string GetCellPinName(HDesign& design, HTimingPointWrapper pt);

private:
    TimingPointMus* mus;
    double scaling;
    int waitTime;

    int plotX;

    void ReportMus(HDesign& design);
    void PlotMusInTopologicalOrder(HDesign& design);
    void PlotPathMus(HDesign& design, HCriticalPath path, int pathIdx);
    void PlotMusInCriticalPathOrder(HDesign& design);
};

#endif