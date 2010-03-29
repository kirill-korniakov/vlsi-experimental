#ifndef __CRITICALPATHUTILS_H__
#define __CRITICALPATHUTILS_H__

#include "HDesign.h"

namespace Utils
{
	class CriticalPathComparator
	{
		HDesign& design;
	public:
		CriticalPathComparator(HDesign& hd): design(hd) {}

		bool operator() (HCriticalPath path1, HCriticalPath path2) const
		{ return design.GetDouble<HCriticalPath::Criticality>(path1)
		< design.GetDouble<HCriticalPath::Criticality>(path2);
		}
	};

  typedef fastdelegate::FastDelegate3<HDesign&, HCriticalPath, int> CriticalPathHandler;
  typedef fastdelegate::FastDelegate3<HDesign&, HCriticalPath, int, bool> CriticalPathStopableHandler;

  const int ALL_PATHS = -1;

  int IterateMostCriticalPaths(HDesign& hd, int nPaths, CriticalPathHandler action);
  int IterateMostCriticalPaths(HDesign& hd, int nPaths, CriticalPathStopableHandler action);
}

#endif //__CRITICALPATHUTILS_H__