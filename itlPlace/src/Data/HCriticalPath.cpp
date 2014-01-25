#include "HDesign.h"

COLLECTIONCONSTRUCTOR(HCriticalPaths)

PROPERTYA(HCriticalPaths, PathExtractionType, HCriticalPath::ExtractionType, m_ld->Timing.cpExtractionType);

//getters specializations
GETTER(HCriticalPaths, double, HCriticalPath::Criticality); //implemented in HExternalMethods.h

GETTER(HCriticalPaths, int, HCriticalPath::PointsCount)
{ return m_ld->Timing.cpAfterLastPointIdx[ARGID]
- m_ld->Timing.cpFirstPointIdx[ARGID]; }

GETTER(HCriticalPaths, HCriticalPath::PointsEnumerator, HCriticalPath::Points)
{ return HCriticalPath::PointsEnumerator(m_ld->Timing.cpFirstPointIdx[ARGID],
  m_ld->Timing.cpAfterLastPointIdx[ARGID]); }

GETTER(HCriticalPaths, HCriticalPath::PointsEnumeratorW, HCriticalPath::Points); //implemented in HExternalMethods.h

GETTERA2(HCriticalPaths, HCriticalPathPoint, HCriticalPath::StartPoint, m_ld->Timing.cpFirstPointIdx);

GETTER(HCriticalPaths, HCriticalPathPoint, HCriticalPath::EndPoint)
{ return ::__ConstructPtr<HCriticalPathPoint>(m_ld->Timing.cpAfterLastPointIdx[ARGID] - 1); }

#define CP_POINTS_ALLOC_SIZE 100
#define CP_PATHS_ALLOC_SIZE 10

void HCriticalPaths::Initialize(int pathsLimit)
{
  m_initialized = true;
  if(m_ld->Timing.ncpLimit < pathsLimit)
    Grow(pathsLimit);
  m_ld->Timing.cpFirstPointIdx[0] = 0;
  m_ld->Timing.cpAfterLastPointIdx[0] = 0;
  m_ld->Timing.cpExtractionType[0] = PathExtractionType_Arrival;

  m_ld->Timing.ncpEnd = 1;
  OnInitialize(pathsLimit);
}

void HCriticalPaths::Grow(int pathsLimit)
{
  int old_size = m_ld->Timing.ncpLimit;
  ::Grow(&m_ld->Timing.cpFirstPointIdx, old_size, pathsLimit);
  ::Grow(&m_ld->Timing.cpAfterLastPointIdx, old_size, pathsLimit);
  ::Grow(&m_ld->Timing.cpExtractionType, old_size, pathsLimit);
  m_ld->Timing.ncpLimit = pathsLimit;

  OnGrow(old_size, pathsLimit);
}

HCriticalPath HCriticalPaths::AllocatePath()
{
  while (m_ld->Timing.ncpEnd >= m_ld->Timing.ncpLimit)
    Grow(GetNewSize(m_hd->cfg, m_ld->Timing.ncpLimit, "data.criticalPathsLimitGrow", 1, 0, 1.25));

  HCriticalPath res = ::__ConstructPtr<HCriticalPath>(m_ld->Timing.ncpEnd);
  m_ld->Timing.ncpEnd++;

  OnPathAllocation(res);

  return res;
}
