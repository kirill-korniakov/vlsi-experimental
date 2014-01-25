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

ENDHCOLLECTION(HCriticalPaths)

PROPERTYADECL(HCriticalPaths, PathExtractionType, HCriticalPath::ExtractionType, m_ld->Timing.cpExtractionType)

//getters specializations
GETTER(HCriticalPaths, double, HCriticalPath::Criticality); //implemented in HExternalMethods.h
GETTER(HCriticalPaths, int, HCriticalPath::PointsCount);
GETTER(HCriticalPaths, HCriticalPath::PointsEnumerator, HCriticalPath::Points);
GETTER(HCriticalPaths, HCriticalPath::PointsEnumeratorW, HCriticalPath::Points); //implemented in HExternalMethods.h
GETTER(HCriticalPaths, HCriticalPathPoint, HCriticalPath::EndPoint);
GETTERA2DECL(HCriticalPaths, HCriticalPathPoint, HCriticalPath::StartPoint, m_ld->Timing.cpFirstPointIdx);

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
