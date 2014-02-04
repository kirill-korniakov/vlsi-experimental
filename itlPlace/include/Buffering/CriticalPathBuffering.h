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

    KSignalDirection SignalDirection()
    {
        if (indexPoint < pointCount)
            return (*design)[points[indexPoint]].SignalDirection();
        else
            return SignalDirection_Default;
    }

    double Criticality() const
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

    bool operator() (const CriticalPathBuffering& path1, const CriticalPathBuffering& path2) const
    { 
        return path1.Criticality() < path2.Criticality();
    }

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
    std::vector<HNet> nets;
    int idx;
    CriticalPathBuffering* last;

    CriticalPathBufferingList(HDesign& hd): design(hd)
    {
        //paths = new std::vector<CriticalPathBuffering>(design.CriticalPaths.Count());        
        //nets.resize(design.Nets.Count());
        idx = 1;
        last = 0;

        HCriticalPaths::Enumerator i = design.CriticalPaths.GetEnumerator();
        i.MoveNext();
        //CriticalPathBuffering(int ind, int pc, double cri, HCriticalPath::PointsEnumeratorW& cppw, HDesign* hd)
        HCriticalPath::PointsEnumeratorW tmp = (i, design).GetEnumeratorW();
        list = new CriticalPathBuffering(0, (i,design).PointsCount(), design.GetDouble<HCriticalPath::Criticality>(i),
                                         tmp, &design);
        CriticalPathBuffering* cur = list;
        for(;i.MoveNext();)
        {
            HCriticalPath::PointsEnumeratorW tmp2 = (i,design).GetEnumeratorW();
            cur->next = new CriticalPathBuffering(idx, (i,design).PointsCount(), design.GetDouble<HCriticalPath::Criticality>(i),
                                                  tmp2, &design);
            cur = cur->next;
            last = cur;
            idx++;
        }
        drob();
        pp();
        paths.resize(idx);
        SetVector();
        std::sort(paths.begin(), paths.end(), CriticalPathBufferingComparator(design));
    }

    void drob()
    {
        //попарный перебор
        CriticalPathBuffering* cur = list;    
        CriticalPathBuffering* old1 = list; 
        CriticalPathBuffering* old2 = list; 
        while(cur != 0)
        {
            CriticalPathBuffering* cur2 = list;
            while(cur2 != 0)
            {
                if (cur != 0)
                {
                    cur->indexPoint = 0;
                    if (cur2->indexPaht != cur->indexPaht)
                    {
                        for(;(cur->MoveNext() && (!cur->isSplit));)
                            for(;(cur2->MoveNext() && (!cur->isSplit));)
                            {
                                //если неты совпадут
                                if (design.Pins.Get<HPin::Net, HNet>(design.TimingPoints.Get<HTimingPoint::Pin, HPin>(design.CriticalPathPoints.Get<HCriticalPathPoint::TimingPoint, HTimingPoint>(cur->points[cur->indexPoint]))) == 
                                    design.Pins.Get<HPin::Net, HNet>(design.TimingPoints.Get<HTimingPoint::Pin, HPin>(design.CriticalPathPoints.Get<HCriticalPathPoint::TimingPoint, HTimingPoint>(cur2->points[cur2->indexPoint]))))
                                {
                                    std::list<HCriticalPathPoint> l1;
                                    std::list<HCriticalPathPoint> l2;
                                    std::list<HCriticalPathPoint> l;
                                    int oldindex1 = cur->indexPoint;
                                    int oldindex2 = cur2->indexPoint;

                                    if ((cur->pointCount == 0) || (cur2->pointCount == 0))
                                        int ty = 0;

                                    //если совпадают начала путей
                                    if ((oldindex1 == 1) && (oldindex2))
                                    {
                                        oldindex1 = -1;
                                        oldindex2 = -1;
                                        cur->indexPoint = 0; 
                                        cur2->indexPoint = 0;
                                    }
                                    else
                                        int tt = 0;

                                    bool f = false;
                                    int c = 0, c1 = 0, c2 = 0;
                                    //выделяем совпадающие
                                    for (;cur->MoveNext() && cur2->MoveNext();)
                                    {
                                        if (design.Pins.Get<HPin::Net, HNet>(design.TimingPoints.Get<HTimingPoint::Pin, HPin>(design.CriticalPathPoints.Get<HCriticalPathPoint::TimingPoint, HTimingPoint>(cur->points[cur->indexPoint]))) 
                                            == design.Pins.Get<HPin::Net, HNet>(design.TimingPoints.Get<HTimingPoint::Pin, HPin>(design.CriticalPathPoints.Get<HCriticalPathPoint::TimingPoint, HTimingPoint>(cur2->points[cur2->indexPoint]))))
                                        {
                                            l.push_back(cur->points[cur->indexPoint]);
                                            c++;
                                        }
                                        else
                                        {
                                            f = true;
                                            break;
                                        }
                                    }
                                    
                                    if ((c % 2) != 0)
                                        int uyu = 0;
                                    //создаем новый путь
                                    if (c != 0)
                                    {
                                        CriticalPathBuffering* ncpb = new CriticalPathBuffering();
                                        ncpb->design = &design;
                                        ncpb->indexPoint = 0;
                                        ncpb->pointCount = c + 1;
                                        ncpb->indexPaht = idx;
                                        idx++;
                                        ncpb->points = new HCriticalPathPoint [ncpb->pointCount];
                                        ncpb->isSplit = true;
                                        ncpb->criticality = idx;
                                        ncpb->next = 0;
                                        last->next = ncpb;
                                        last = last->next;
                                        ncpb->points[0] = design.CriticalPathPoints.Null();
                                        int j = 1;
                                        for (std::list<HCriticalPathPoint>::iterator i = l.begin(); i != l.end(); i++, j++)
                                            ncpb->points[j] = *i;
                                        
                                    }

                                    //если конци не совпали, выделяем для концов
                                    if (f)
                                    {
                                        nets.push_back(design.Pins.Get<HPin::Net, HNet>(design.TimingPoints.Get<HTimingPoint::Pin, HPin>(design.CriticalPathPoints.Get<HCriticalPathPoint::TimingPoint, HTimingPoint>(cur->points[cur->indexPoint - 1])))); 
                                        
                                        cur->indexPoint++;
                                        cur2->indexPoint++;
                                        for(;cur->MoveNext();)
                                        {
                                            l1.push_back(cur->points[cur->indexPoint]);
                                            c1++;
                                        }
                                        if ((c1 % 2) != 0)
                                            int uyu = 0;
                                        CriticalPathBuffering* ncpb1 = new CriticalPathBuffering();
                                        ncpb1->design = &design;
                                        ncpb1->indexPoint = 0;
                                        ncpb1->pointCount = c1 + 1;
                                        ncpb1->indexPaht = idx;
                                        idx++;
                                        ncpb1->points = new HCriticalPathPoint [ncpb1->pointCount];
                                        ncpb1->isSplit = true;
                                        ncpb1->criticality = idx;
                                        ncpb1->next = 0;
                                        last->next = ncpb1;
                                        last = last->next;
                                        ncpb1->points[0] = design.CriticalPathPoints.Null();
                                        int j1 = 1;
                                        for (std::list<HCriticalPathPoint>::iterator i1 = l1.begin(); i1 != l1.end(); i1++, j1++)
                                            ncpb1->points[j1] = *i1;

                                        for(;cur2->MoveNext();)
                                        {
                                            l2.push_back(cur2->points[cur2->indexPoint]);
                                            c2++;
                                        }
                                        if ((c2 % 2) != 0)
                                            int uyu = 0;
                                        CriticalPathBuffering* ncpb2 = new CriticalPathBuffering();
                                        ncpb2->design = &design;
                                        ncpb2->indexPoint = 0;
                                        ncpb2->pointCount = c2 + 1;
                                        ncpb2->indexPaht = idx;
                                        idx++;
                                        ncpb2->points = new HCriticalPathPoint [ncpb2->pointCount];
                                        ncpb2->isSplit = true;
                                        ncpb2->criticality = idx;
                                        ncpb2->next = 0;
                                        last->next = ncpb2;
                                        last = last->next;
                                        ncpb2->points[0] = design.CriticalPathPoints.Null();
                                        int j2 = 1;
                                        for (std::list<HCriticalPathPoint>::iterator i2 = l2.begin(); i2 != l2.end(); i2++, j2++)
                                            ncpb2->points[j2] = *i2;
                                    }

                                    //обрубаем старые
                                    cur->indexPoint = oldindex1;
                                    cur2->indexPoint = oldindex2;

                                    cur->pointCount = cur->indexPoint + 1;
                                    cur2->pointCount = cur->indexPoint + 1;

                                    if (cur->pointCount == 0)
                                    {
                                        old1->next = cur->next;

                                            cur->isSplit = true;
                                        cur->indexPoint = 0;
                                        cur = cur->next;
                                    }
                                    if (cur2->pointCount == 0)
                                    {
                                        old2->next = cur2->next;

                                            cur2->isSplit = true;
                                        cur2->indexPoint = 0;
                                    }


                                }
                            }
                    }
                }
                old2 = cur2;
                cur2->indexPoint = 0;
                cur2 = cur2->next;

            }
            old1 = cur;
            cur->indexPoint = 0;
            cur = cur->next;
        }
    }

    void pp()
    {
        CriticalPathBuffering* cur = list;
        CriticalPathBuffering* c = 0;
        bool f = false;
        while(cur != 0)
        {
            f = false;
            if (cur->next != 0)
            {
                c = cur->next;
                if (c->pointCount < 3)
                {
                    cur->next = c->next;
                    idx--;
                    f = true;
                }
            }
            if (!f)
                cur = cur->next;
        }
        if (list->pointCount < 3)
        {
            list = list->next;
            idx--;
        }

        int i = 0;
        cur = list;
        
        while(cur != 0)
        {
            cur->indexPaht = i;
            if ((cur->pointCount % 2) == 0)
                int uyu = 0;
            cur = cur->next;
            i++;
        }
        idx = i;

    }

    void SetVector()
    {
        CriticalPathBuffering* cur = list;
        while(cur != 0)
        {
            paths[cur->indexPaht] = *cur ;
            cur = cur->next;
        }
    }
};

#endif
