#ifndef __CriticalPahtBuffering_H__
#define __CriticalPahtBuffering_H__

#include "HCriticalPathPoint.h"
#include "HTimingPoint.h"


class CriticalPathBuffering
{

public: 
    HDesign* design;
    HCriticalPathPoint* points;
    int indexPoint;
    int pointCount;    
    int indexPaht;
    CriticalPathBuffering* next;
    bool isSplit;
    double criticality;


    CriticalPathBuffering()
    {
    }

    CriticalPathBuffering& operator = (const CriticalPathBuffering& c) 
    {
        if (&c != this)
        {
            design = c.design;
            points = c.points;
            indexPoint = c.indexPoint;
            pointCount = c.pointCount;    
            indexPaht = c.indexPaht;
            next = c.next;
            isSplit = c.isSplit;
            criticality = c.criticality;
        }
        return *this;
    }

    CriticalPathBuffering(int ind, int pc, double cri, HCriticalPath::PointsEnumeratorW& cppw, HDesign* hd)
    {
        design = hd;
        indexPoint = 0;
        pointCount = pc + 1;
        indexPaht = ind;
        points = new HCriticalPathPoint [pointCount];
        isSplit = false;
        criticality = cri;
        next = 0;

        points[0] = cppw;
        for (int i = 1; cppw.MoveNext(); i++)
            points[i] = cppw;
    }

    bool MoveNext()
    {
        indexPoint++;
        return (indexPoint < pointCount);
    }

    HTimingPoint TimingPoint()
    {
        if (indexPoint < pointCount)
            return design->CriticalPathPoints.Get<HCriticalPathPoint::TimingPoint, HTimingPoint>( points[indexPoint]);
        else
            return design->TimingPoints.Null();
    }

    HTimingPoint TimingPointStart()
    {
        return design->CriticalPathPoints.Get<HCriticalPathPoint::TimingPoint, HTimingPoint>( points[0]);
    }

    SignalDirection SignalDirection() 
    {
        if (indexPoint < pointCount)
            return (*design)[points[indexPoint]].SignalDirection();
        else
            return SignalDirection::SignalDirection_Default;
    }

    double Criticality()
    {
        if (!isSplit)
            return criticality;
        else 
            return 0;
    }
};

class CriticalPathBufferingComparator
{
    HDesign& design;
public:
    CriticalPathBufferingComparator(HDesign& hd): design(hd) {}

    bool operator() (CriticalPathBuffering& path1, CriticalPathBuffering& path2) const
    { 
        return path1.Criticality() < path2.Criticality();
    }
};

class CriticalPathBufferingList
{
public:
    HDesign& design;
    CriticalPathBuffering* list;
    std::vector<CriticalPathBuffering> paths;

    CriticalPathBufferingList(HDesign& hd): design(hd)
    {
        //paths = new std::vector<CriticalPathBuffering>(design.CriticalPaths.Count());
        paths.resize(design.CriticalPaths.Count());
        int idx = 1;

        HCriticalPaths::Enumerator i = design.CriticalPaths.GetEnumerator();
        i.MoveNext();
        list = new CriticalPathBuffering(0, (i,design).PointsCount(), design.GetDouble<HCriticalPath::Criticality>(i),(i,design).GetEnumeratorW(), &design);
        paths[0] = *list;
        CriticalPathBuffering* cur = list;
        for(;i.MoveNext() ;)
        {
            cur->next = new CriticalPathBuffering(idx, (i,design).PointsCount(), design.GetDouble<HCriticalPath::Criticality>(i), (i,design).GetEnumeratorW(), &design);
            paths[idx] =  *cur->next;
            cur = cur->next;
            idx++;
        }
        std::sort(paths.begin(), paths.end(), CriticalPathBufferingComparator(design));
    }
};

#endif