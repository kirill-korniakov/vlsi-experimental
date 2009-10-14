#ifndef _LOW_CRITICAL_PATH_FINDING_
#define _LOW_CRITICAL_PATH_FINDING_

#include <vector>

#include "stdTypes.h"

struct LCriticalPathsFinding
{
  typedef std::pair<IDType,bool> LCriticalPathPoint;
	typedef std::vector<LCriticalPathPoint> LCriticalPath;

  int tpCriticalPaths_Count;
  int tpCriticalPaths_ArrivalCount;
  int tpCriticalPaths_RequiredCount;
  //tpCriticalPaths_Count==tpCriticalPaths_ArrivalCount+tpCriticalPaths_RequiredCount
  LCriticalPath* tpCriticalPaths_TimingPointsIDs;
  double* tpCriticalPaths_Criticality;
  bool* tpCriticalPaths_IsArrivalTimeCriticality;

  LCriticalPathsFinding()
    :tpCriticalPaths_TimingPointsIDs(0),
    tpCriticalPaths_ArrivalCount(0),
    tpCriticalPaths_RequiredCount(0),
    tpCriticalPaths_Criticality(0),
	  tpCriticalPaths_IsArrivalTimeCriticality(0)
  {}
};

#endif //_LOW_CRITICAL_PATH_FINDING_