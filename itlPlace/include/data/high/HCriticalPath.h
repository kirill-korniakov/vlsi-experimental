#ifndef __HIGH_CRITICAL_PATH__
#define __HIGH_CRITICAL_PATH__

#include "HBaseDesignItem.h"
#include "HCriticalPathPoint.h"

BEGINITEM(HCriticalPath, HCriticalPathWrapper)

enum Properties
{
  Criticality,
  PointsCount,
  Points,
  StartPoint,
  EndPoint,
  ExtractionType
};

typedef HEnumerator<HCriticalPathPoint> PointsEnumerator;
typedef HEnumeratorW<HCriticalPathPointWrapper, HCriticalPathPoints> PointsEnumeratorW;

ENDITEM(HCriticalPath)

BEGINHCOLLECTION(HCriticalPaths, HCriticalPath)

  //paths count
  int Count() const {return m_ld->Timing.ncpEnd - 1;}

  ///Enumerator
  typedef HConstEnumerator<HCriticalPath,1> Enumerator;
  typedef HEnumeratorW<HCriticalPathWrapper, HCriticalPaths> EnumeratorW;
  Enumerator GetEnumerator() const {return Enumerator(m_ld->Timing.ncpEnd);}
  EnumeratorW GetEnumeratorW();

  ///Events
  Event<2, int, int> OnGrow; ///int nPathsLimit_before, int nPathsLimit_after
  Event<1, int> OnInitialize; ///int nPathsLimit
  Event<1, HCriticalPath> OnPathAllocation;

  ///Operations
  void Initialize(int pathsLimit);
  void Grow(int pathsLimit);
  HCriticalPath AllocatePath();

  //Getters & Setters
  GETTERS_SETTERS_DEFINITION()

  PROPERTYA(PathExtractionType, HCriticalPath::ExtractionType, m_ld->Timing.cpExtractionType);

  //getters specializations
  GETTER(double, HCriticalPath::Criticality); //implemented in HExternalMethods.h

  GETTER(int, HCriticalPath::PointsCount)
  { return m_ld->Timing.cpAfterLastPointIdx[ARGID] 
    - m_ld->Timing.cpFirstPointIdx[ARGID]; }

  GETTER(HCriticalPath::PointsEnumerator, HCriticalPath::Points)
  { return HCriticalPath::PointsEnumerator(m_ld->Timing.cpFirstPointIdx[ARGID],
      m_ld->Timing.cpAfterLastPointIdx[ARGID]); }

  GETTER(HCriticalPath::PointsEnumeratorW, HCriticalPath::Points); //implemented in HExternalMethods.h

  GETTERA2(HCriticalPathPoint, HCriticalPath::StartPoint, m_ld->Timing.cpFirstPointIdx);

  GETTER(HCriticalPathPoint, HCriticalPath::EndPoint)
  { return ::__ConstructPtr<HCriticalPathPoint>(m_ld->Timing.cpAfterLastPointIdx[ARGID] - 1); }

ENDHCOLLECTION(HCriticalPaths)

BEGINWRAPPER(HCriticalPathWrapper, HCriticalPaths)

  PROPERTYWD(PathExtractionType, ExtractionType);
  //Getters
  GETTERWD(int, PointsCount)
  GETTERWD(double, Criticality)
  GETTERWD(HCriticalPathPoint, EndPoint)
  GETTERWD(HCriticalPathPoint, StartPoint);
  GETTERW(HCriticalPath::PointsEnumerator, Points, GetEnumerator)
  GETTERW(HCriticalPath::PointsEnumeratorW, Points, GetEnumeratorW)

ENDWRAPPER(HCriticalPathWrapper)

inline HCriticalPaths::EnumeratorW HCriticalPaths::GetEnumeratorW()
  { return EnumeratorW(this, 1, m_ld->Timing.ncpEnd); }

#endif //_HIGH_CRITICAL_PATH_FINDING_