#ifndef __ANALYTICAL_PLACEMENT_REPORTING_H__
#define __ANALYTICAL_PLACEMENT_REPORTING_H__

#include "HDesign.h"
#include "Clustering.h"
#include "OptimizationContext.h"

#include "tao.h"

namespace AnalyticalGlobalPlacement
{
    void ReportTimes();
    void ReportIterationInfo(ClusteringInformation& ci, AppCtx& user);
    void ReportBinGridInfo(AppCtx& context);
    void ReportPreIterationInfo(HDesign &hd, AppCtx &context, int metaIteration, int iteration);
    void ReportPostIterationInfo(HDesign &hd, AppCtx &context, int metaIteration, int iteration );
    int  ReportTerminationReason(TAO_SOLVER tao, int& innerTAOIterations);
    void ReportClusterCoordinates(AppCtx* context);
}

#endif