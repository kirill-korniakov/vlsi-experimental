#include "Reporting.h"
#include "PathFinder.h"

void ReportPath(HDesign& design, PathType& path)
{
    for (int i = 0; i < path.size(); i++) 
    {
        Edge edge = path[i];
        string src = GetCellPinName(design, edge.source);
        string snk = GetCellPinName(design, edge.sink);
        string net = design.GetString<HNet::Name>(edge.net);
        WRITE("%s - %s - %s;", src.c_str(), net.c_str(), snk.c_str());
    }
    WRITELINE("");
}

bool IsStartEqualEnd(HDesign& design, PathType& path)
{
    HCell end = design.Get<HPin::Cell, HCell>(path[path.size()-1].sink);
    HCell start = design.Get<HPin::Cell, HCell>(path[0].source);

    return (start == end);
}

bool IsDeadPath(HDesign& design, PathType& path)
{
    HPin sink = path[path.size()-1].sink;
    HTimingPointWrapper tp = design[design.TimingPoints[sink]];

    if (IsStartEqualEnd(design, path))
    {
        LOGCRITICAL("Cycle found!");
    }

    if (tp.IsTimingEndPoint())
        return true;
    else
        return false;
}

void PruneDeadPaths(HDesign& design, PathesStorageType& storage)
{
    for (size_t i = 0; i < storage.size(); i++) 
    {
        if (IsDeadPath(design, storage[i]))
        {
            //ReportPath(design, storage[i]);

            if (i == storage.size() - 1)
                storage.pop_back();
            else
            {
                storage[i] = storage[storage.size() - 1];
                storage.pop_back();
                i--;
            }
        }
    }
}

void BranchAtTheCell(HDesign& design, PathesStorageType& storage, PathType& path, HCell cell)
{
    for (HCell::PinsEnumeratorW pin = design[cell].GetPinsEnumeratorW(); pin.MoveNext(); )
        if (pin.Direction() == PinDirection_OUTPUT && !::IsNull(pin.Net()))
        {
            Edge edge;
            edge.source = pin;
            edge.net = pin.Net();
            for (HNet::SinksEnumeratorW sink = design.Get<HNet::Sinks, HNet::SinksEnumeratorW>(pin.Net()); sink.MoveNext(); )
            {
                edge.sink = sink;
                PathType addPath = path;
                addPath.push_back(edge);
                storage.push_back(addPath);
            }
        }
}

PathesStorageType Branching(HDesign& design, PathesStorageType& storage) 
{
    PathesStorageType newStorage;

    for (size_t i = 0; i < storage.size(); i++) 
    {
        Edge lastEdge = storage[i].back();
        HCell end = design.Get<HPin::Cell, HCell>(lastEdge.sink);
        PathType path = storage[i];

        BranchAtTheCell(design, newStorage, path, end);
    }

    return newStorage;
}

int GetPathIdx(HDesign& design, PathesStorageType& storage, HCell targetCell)
{
    for (size_t i = 0; i < storage.size(); i++) 
    {
        Edge lastEdge = storage[i].back();
        HCell end = design.Get<HPin::Cell, HCell>(lastEdge.sink);
        if (targetCell == end)
            return i;
    }
    return -1;
}

PathType FindPath(HDesign& design, HCell startCell, HCell targetCell)
{
    PathesStorageType allPathes;
    PathType path;

    BranchAtTheCell(design, allPathes, path, startCell);
    PruneDeadPaths(design, allPathes);

    while (!allPathes.empty())
    {
        int pathIdx = GetPathIdx(design, allPathes, targetCell);
        if (pathIdx != -1)
            return allPathes[pathIdx];

        allPathes = Branching(design, allPathes);
        PruneDeadPaths(design, allPathes);
    }

    PathType emptyPath;
    return emptyPath;
}

CellVector GetCellsFromPathes(HDesign& design, PathesStorageType& storage)
{
    CellVector pathCells;

    for (size_t i = 0; i < storage.size(); i++) 
    {
        for (size_t j = 0; j < storage[i].size(); j++)
        {
            HCell sinkCell = design.Get<HPin::Cell, HCell>(storage[i][j].sink);
            pathCells.push_back(sinkCell);
        }
    }
    CellVector::iterator new_end = unique(pathCells.begin(), pathCells.end());
    pathCells.erase(new_end, pathCells.end());

    return pathCells;
}

CellVector InsertUnique(CellVector& dest, CellVector& src)
{
    for (int i = 0; i < src.size(); i++)
    {
        CellVector::iterator iter = find(dest.begin(), dest.end(), src[i]);
        if (iter == dest.end())
            dest.push_back(src[i]);
    }
    //dest.insert(dest.begin(), src.begin(), src.end());

    //CellVector::iterator new_end = unique(dest.begin(), dest.end());
    //dest.erase(new_end, dest.end());
    
    return dest;
}

void RemoveDuplicates(CellVector& dest, CellVector& src)
{
    for (int i = 0; i < src.size(); i++)
    {
        CellVector::iterator iter = find(dest.begin(), dest.end(), src[i]);
        if (iter != dest.end())
            dest.erase(iter);
    }
}

CellVector GetReachable(HDesign& design, HCell startCell)
{
    PathesStorageType allPathes;
    PathType path;
    CellVector reachableCells;
    CellVector pathCells;

    BranchAtTheCell(design, allPathes, path, startCell);
    pathCells = GetCellsFromPathes(design, allPathes);
    reachableCells = InsertUnique(reachableCells, pathCells);
    PruneDeadPaths(design, allPathes);

    while (!allPathes.empty())
    {
        allPathes = Branching(design, allPathes);
        pathCells = GetCellsFromPathes(design, allPathes);
        reachableCells = InsertUnique(reachableCells, pathCells);
        PruneDeadPaths(design, allPathes);
    }

    return reachableCells;
}
