#include "HDesign.h"

struct Edge
{
    HPin source;
    HPin sink;
    HNet net;
};

typedef std::vector<HCell> CellVector;
typedef std::vector<Edge> PathType;
typedef std::vector<std::vector<Edge>> PathesStorageType;

void RemoveDuplicates(CellVector& dest, CellVector& src);

void ReportPath(HDesign& design, PathType& path);
PathType FindPath(HDesign& design, HCell startCell, HCell targetCell);
CellVector GetReachable(HDesign& design, HCell startCell);