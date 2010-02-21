#ifndef __VLSIFLOW_H__
#define __VLSIFLOW_H__

#include "HDesign.h"
#include "TableFormatter.h"

void InitFlowMetricsTable(TableFormatter& fmt, HDesign& design);
void RunFlow(HDesign& hd, TableFormatter& flowMetrics);

#endif //__VLSIFLOW_H__