#ifndef __NetListBuffering_H__
#define __NetListBuffering_H__

#include "NetBufferingAlgorithm.h"
#include "CriticalPathBuffering.h"

class BufferingAllCriticalPath : public NetBufferingAlgorithm
{
public:
    BufferingAllCriticalPath(HDesign& hd): NetBufferingAlgorithm(hd)
    {
    };
    int BufferingCriticalPaths();

protected:
    bool IsAppropriateNumberOfPins(VGAlgorithmData* data, HNet interconnection);
    bool IsAppropriateNumberOfPins(VGAlgorithmData* data, HCriticalPath interconnection);
    bool IsAppropriateNumberOfPins(VGAlgorithmData* data, CriticalPathBuffering& interconnection);
};

class PathBasedBuffering: public BufferingAllCriticalPath
{
public:
    PathBasedBuffering(HDesign& hd):BufferingAllCriticalPath(hd)
    {
    }
    VGVariantsListElement BufferingCriticalPath(HCriticalPath criticalPath, bool isRealBuffering = true);
    VGVariantsListElement BufferingCriticalPath(CriticalPathBuffering& criticalPath, bool isRealBuffering = true);
    int BufferingNetlist();
    int BufferingNetlist2();

protected:
    bool IsLimitationCountCriticalPathExecute(int totalIndex);
};

class RemoveBuffer
{
public:
    VGAlgorithmData* data;
    RemoveBuffer(VGAlgorithmData* oldData);
    RemoveBuffer(HDesign& hd);
    ~RemoveBuffer();
    void RemoveNewBuffering();
};

#endif