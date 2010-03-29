#include "CriticalPathUtils.h"

using namespace Utils;



int Utils::IterateMostCriticalPaths(HDesign& hd, int nPaths, CriticalPathHandler action)
{
  if (nPaths == -1 || nPaths > hd.CriticalPaths.Count())
    nPaths = hd.CriticalPaths.Count(); //we will see all critical paths
  else if (nPaths == 0)
    return 0;

  std::vector<HCriticalPath> paths(hd.CriticalPaths.Count());
  int idx = 0;
  for(HCriticalPaths::Enumerator i = hd.CriticalPaths.GetEnumerator(); i.MoveNext();)
    paths[idx++] = i;

  std::sort(paths.begin(), paths.end(), CriticalPathComparator(hd));

  for(int i = 0; i < nPaths; ++i)
    action(hd, paths[i], i + 1);

  return nPaths;
}

int Utils::IterateMostCriticalPaths(HDesign& hd, int nPaths, CriticalPathStopableHandler action)
{
  if (nPaths == -1 || nPaths > hd.CriticalPaths.Count())
    nPaths = hd.CriticalPaths.Count(); //we will see all critical paths
  else if (nPaths == 0)
    return 0;

  std::vector<HCriticalPath> paths(hd.CriticalPaths.Count());
  int idx = 0;
  for(HCriticalPaths::Enumerator i = hd.CriticalPaths.GetEnumerator(); i.MoveNext();)
    paths[idx++] = i;

  std::sort(paths.begin(), paths.end(), CriticalPathComparator(hd));

  for(int i = 0; i < nPaths; ++i)
    if (!action(hd, paths[i], i + 1))
      return i + 1;

  return nPaths;
}
