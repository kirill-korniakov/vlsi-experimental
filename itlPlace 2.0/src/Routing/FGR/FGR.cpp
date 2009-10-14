#include <string>
#include <vector>
#include <map>
#include <limits>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <fstream>

using std::vector;
using std::pair;
using std::endl;
using std::flush;
using std::string;
using std::map;
using std::numeric_limits;
using std::make_pair;
using std::ofstream;

#include "FGR.h"
#include "stdTypes.h"
using namespace fgr;

bool CompareByBox::operator()(const SubNetIdType &a, const SubNetIdType &b) const
{
  double aW = fabs(static_cast<double>(nets[a.first].subnets[a.second].one.x) - 
                   static_cast<double>(nets[a.first].subnets[a.second].two.x));
  double aH = fabs(static_cast<double>(nets[a.first].subnets[a.second].one.y) - 
                   static_cast<double>(nets[a.first].subnets[a.second].two.y));
  pair<double,double> costA(aW+aH,min(aW,aH)/max(aW,aH));

  double bW = fabs(static_cast<double>(nets[b.first].subnets[b.second].one.x) - 
                   static_cast<double>(nets[b.first].subnets[b.second].two.x));
  double bH = fabs(static_cast<double>(nets[b.first].subnets[b.second].one.y) - 
                   static_cast<double>(nets[b.first].subnets[b.second].two.y));
  pair<double,double> costB(bW+bH, min(bW,bH)/max(bW,bH));

  return costA < costB;
}

CostType UnitCost::viaCost(void) const
{
  return viaFactor*edgeBase;
}

CostType UnitCost::operator()(IdType edgeId, IdType netId) const
{
  if(fgr.edges[edgeId].type == VIA)
  {
    return viaCost();
  }
  else
  {
    return edgeBase;
  }
}

CostType DLMCost::viaCost(void) const
{
  return viaFactor*edgeBase;  // 3x as costly as a "regular" segment
}

CostType DLMCost::operator()(IdType edgeId, IdType netId) const
{
  if(fgr.edges[edgeId].type == VIA)
  {
    return viaCost();
  }
  else
  {
    const CapType capacity = fgr.edges[edgeId].capacity;
    const CapType newUsage = fgr.edges[edgeId].usage + max(fgr.minWidths[fgr.edges[edgeId].layer],
                             fgr.nets[netId].minWidth) + fgr.minSpacings[fgr.edges[edgeId].layer];

    if(newUsage > capacity)
    {
      return edgeBase + fgr.edges[edgeId].historyCost*
             min(powMax, pow(powBase, static_cast<CostType>(newUsage - capacity)/static_cast<CostType>(capacity)));
    }
    else
    {
      return edgeBase + fgr.edges[edgeId].historyCost*static_cast<CostType>(newUsage)/static_cast<CostType>(capacity);
    }
  }
}

void FGR::routeFlatSubNets(bool allowOverflow, const CostFunction &f)
{
  ofstream outfile(params.resultsFile.c_str(), std::ios::app);

  if(!outfile.good())
  {
    outfile << "Could not open `" << params.resultsFile << "' for writing." << endl;
  }
  unsigned flatNetsRouted = 0;
  vector<SubNetIdType> order;
  for(unsigned i = 0; i < nets.size(); ++i)
  {
    for(unsigned j = 0; j < nets[i].subnets.size(); ++j)
    {
      order.push_back(make_pair(i,j));
    }
  }

  sort(order.begin(), order.end(), CompareByBox(nets));

  bool bboxConstrain = true;

  for(unsigned i = 0; i < order.size(); ++i)
  {
    if(i % 100 == 0) outfile << i << "\r" << flush;

    const SubNet &snet = nets[order[i].first].subnets[order[i].second];

    if(snet.routed) continue;

    if(snet.one.x == snet.two.x || snet.one.y == snet.two.y)
    {
      routeSubNet(order[i], allowOverflow, bboxConstrain, f);
      if(snet.routed) ++flatNetsRouted;
    }
  }

  outfile << flatNetsRouted << " flat sub nets routed" << endl;
  outfile.close();
}

CostType FGR::routeSubNet(const SubNetIdType &subNetId, bool allowOverflow, bool bboxConstrain, const CostFunction &f)
{
  vector<IdType> path;
  
  SubNet &snet = nets[subNetId.first].subnets[subNetId.second];

  CostType totalCost;

  if(bboxConstrain)
  {
    const Point botleft(min(snet.one.x, snet.two.x), min(snet.one.y, snet.two.y), 0);
    const Point topright(max(snet.one.x, snet.two.x), max(snet.one.y, snet.two.y), 0);

    const bool doNotAllowOverflow = false;
    totalCost = routeMaze(subNetId, doNotAllowOverflow, botleft, topright, f, path);

    if(path.size() == 0)
    {
      // the initial attempt within the bbox failed, try again
      totalCost = routeMaze(subNetId, allowOverflow, Point(0,0,0), Point(xTiles,yTiles,0), f, path);
    }
  }
  else
  {
    totalCost = routeMaze(subNetId, allowOverflow, Point(0,0,0), Point(xTiles,yTiles,0), f, path);
  }

  for(unsigned i = 0; i < path.size(); ++i)
  {
    addSegment(subNetId, path[i]);
  }

  snet.routed = (path.size() > 0);

  return totalCost;
}

void FGR::ripUpSubNet(const SubNetIdType &subNetId)
{
  SubNet &snet = nets[subNetId.first].subnets[subNetId.second];  
  for(unsigned i = 0; i < snet.segments.size(); ++i)
  {
    ripUpSegment(subNetId, snet.segments[i]);
  }
  snet.segments.clear();
  snet.routed = false;
}

void FGR::routeSubNets(bool allowOverflow, const CostFunction &f)
{
  ofstream outfile(params.resultsFile.c_str(), std::ios::app);

  if(!outfile.good())
  {
    outfile << "Could not open `" << params.resultsFile << "' for writing." << endl;
  }
  vector<SubNetIdType> order;
  for(unsigned i = 0; i < nets.size(); ++i)
  {
    for(unsigned j = 0; j < nets[i].subnets.size(); ++j)
    {
      order.push_back(make_pair(i,j));
    }
  }

  sort(order.begin(), order.end(), CompareByBox(nets));

  bool bboxConstrain = true;

  for(unsigned i = 0; i < order.size(); ++i)
  {
    if(i % 100 == 0) outfile << i << "\r" << flush;

    if(nets[order[i].first].subnets[order[i].second].routed) continue;

    routeSubNet(order[i], allowOverflow, bboxConstrain, f);
  }
  outfile.close();
}

CostType FGR::routeMaze(const SubNetIdType &subNetId, bool allowOverflow,
                        const Point &botleft, const Point &topright,
                        const CostFunction &f, vector<IdType> &path)
{
  const IdType netId = subNetId.first;
  const CapType netMinWidth = nets[netId].minWidth;

  SubNet &snet = nets[subNetId.first].subnets[subNetId.second];
  const IdType startTile  = xyzToTileId(snet.one.x, snet.one.y, snet.one.z);
  const IdType targetTile = xyzToTileId(snet.two.x, snet.two.y, snet.two.z);

  pQ.updateTile(startTile, 0., 0., numeric_limits<IdType>::max());

  const LBFunction &lb = (nets[netId].hashedSegments.size() == 0 ? static_cast<LBFunction&>(em) : 
                                                                   static_cast<LBFunction&>(lbm));

  do
  {
    const IdType currTile = pQ.getBestTile();
    const CostType cost = pQ.getData(currTile).actualCost;
    pQ.removeBestTile();

    const Point tile = tileIdToXYZ(currTile);

    if(currTile == targetTile)
    {
      break;
    }

    const CapType addedUsage = max(netMinWidth, minWidths[tile.z]) + minSpacings[tile.z];

    if(tiles[tile.z][tile.y][tile.x].incX != numeric_limits<IdType>::max())
    {
      const IdType nextTile = xyzToTileId(tile.x+1,tile.y,tile.z);
      if(tile.x < topright.x)
      {
        if(!pQ.visited(nextTile) || pQ.getData(nextTile).heapLoc != numeric_limits<IdType>::max())
        {
          const IdType edgeId = tiles[tile.z][tile.y][tile.x].incX;
          const CostType distToEnd = lb(snet.two, Point(tile.x+1,tile.y,tile.z));
          if(!nets[netId].hashedSegments.contains(edgeId))
          {
            const CapType newUsage = edges[edgeId].usage + addedUsage;

            if(allowOverflow || newUsage <= edges[edgeId].capacity)
            {
              const CostType newCost = cost + f(edgeId, netId);
              pQ.updateTile(nextTile, newCost + distToEnd, newCost, currTile);
            }
          }
          else
          {
            const CostType newCost = cost + epsilon;
            pQ.updateTile(nextTile, newCost + distToEnd, newCost, currTile);
          }
        }
      }
    }

    if(tiles[tile.z][tile.y][tile.x].decX != numeric_limits<IdType>::max())
    {
      const IdType nextTile = xyzToTileId(tile.x-1,tile.y,tile.z);
      if(tile.x > botleft.x)
      {
        if(!pQ.visited(nextTile) || pQ.getData(nextTile).heapLoc != numeric_limits<IdType>::max())
        {
          const IdType edgeId = tiles[tile.z][tile.y][tile.x].decX;
          const CostType distToEnd = lb(snet.two, Point(tile.x-1,tile.y,tile.z));
          if(!nets[netId].hashedSegments.contains(edgeId))
          {
            const CapType newUsage = edges[edgeId].usage + addedUsage;

            if(allowOverflow || newUsage <= edges[edgeId].capacity)
            {
              const CostType newCost = cost + f(edgeId, netId);
              pQ.updateTile(nextTile, newCost + distToEnd, newCost, currTile);
            }
          }
          else
          {
            const CostType newCost = cost + epsilon;
            pQ.updateTile(nextTile, newCost + distToEnd, newCost, currTile);
          }
        }
      }
    }

    if(tiles[tile.z][tile.y][tile.x].incY != numeric_limits<IdType>::max())
    {
      const IdType nextTile = xyzToTileId(tile.x,tile.y+1,tile.z);
      if(tile.y < topright.y)
      {
        if(!pQ.visited(nextTile) || pQ.getData(nextTile).heapLoc != numeric_limits<IdType>::max())
        {
          const IdType edgeId = tiles[tile.z][tile.y][tile.x].incY;
          const CostType distToEnd = lb(snet.two, Point(tile.x,tile.y+1,tile.z));
          if(!nets[netId].hashedSegments.contains(edgeId))
          {
            const CapType newUsage = edges[edgeId].usage + addedUsage;

            if(allowOverflow || newUsage <= edges[edgeId].capacity)
            {
              const CostType newCost = cost + f(edgeId, netId);
              pQ.updateTile(nextTile, newCost + distToEnd, newCost, currTile);
            }
          }
          else
          {
            const CostType newCost = cost + epsilon;
            pQ.updateTile(nextTile, newCost + distToEnd, newCost, currTile);
          }
        }
      }
    }

    if(tiles[tile.z][tile.y][tile.x].decY != numeric_limits<IdType>::max())
    {
      const IdType nextTile = xyzToTileId(tile.x,tile.y-1,tile.z);
      if(tile.y > botleft.y)
      {
        if(!pQ.visited(nextTile) || pQ.getData(nextTile).heapLoc != numeric_limits<IdType>::max())
        {
          const IdType edgeId = tiles[tile.z][tile.y][tile.x].decY;
          const CostType distToEnd = lb(snet.two, Point(tile.x,tile.y-1,tile.z));
          if(!nets[netId].hashedSegments.contains(edgeId))
          {
            const CapType newUsage = edges[edgeId].usage + addedUsage;

            if(allowOverflow || newUsage <= edges[edgeId].capacity)
            {
              const CostType newCost = cost + f(edgeId, netId);
              pQ.updateTile(nextTile, newCost + distToEnd, newCost, currTile);
            }
          }
          else
          {
            const CostType newCost = cost + epsilon;
            pQ.updateTile(nextTile, newCost + distToEnd, newCost, currTile);
          }
        }
      }
    }

    if(tiles[tile.z][tile.y][tile.x].incZ != numeric_limits<IdType>::max())
    {
      const IdType nextTile = xyzToTileId(tile.x,tile.y,tile.z+1);
      if(!pQ.visited(nextTile) || pQ.getData(nextTile).heapLoc != numeric_limits<IdType>::max())
      {
        const IdType edgeId = tiles[tile.z][tile.y][tile.x].incZ;
        const CostType distToEnd = lb(snet.two, Point(tile.x,tile.y,tile.z+1));
        if(!nets[netId].hashedSegments.contains(edgeId))
        {
          const CostType newCost = cost + f(edgeId, netId);
          pQ.updateTile(nextTile, newCost + distToEnd, newCost, currTile);
        }
        else
        {
          const CostType newCost = cost + epsilon;
          pQ.updateTile(nextTile, newCost + distToEnd, newCost, currTile);
        }
      }
    }

    if(tiles[tile.z][tile.y][tile.x].decZ != numeric_limits<IdType>::max())
    {
      const IdType nextTile = xyzToTileId(tile.x,tile.y,tile.z-1);
      if(!pQ.visited(nextTile) || pQ.getData(nextTile).heapLoc != numeric_limits<IdType>::max())
      {
        const IdType edgeId = tiles[tile.z][tile.y][tile.x].decZ;
        const CostType distToEnd = lb(snet.two, Point(tile.x,tile.y,tile.z-1));
        if(!nets[netId].hashedSegments.contains(edgeId))
        {
          const CostType newCost = cost + f(edgeId, netId);
          pQ.updateTile(nextTile, newCost + distToEnd, newCost, currTile);
        }
        else
        {
          const CostType newCost = cost + epsilon;
          pQ.updateTile(nextTile, newCost + distToEnd, newCost, currTile);
        }
      }
    }

  }
  while(!pQ.empty());

  // now build up the path, if we found one
  if(pQ.visited(targetTile))
  {
    IdType currTile = targetTile;
    IdType parent = pQ.getData(currTile).parent;

    do
    {
      Point curr = tileIdToXYZ(currTile);
      Point par  = tileIdToXYZ(parent);

      if(curr.x + 1 == par.x)
      {
        path.push_back(tiles[curr.z][curr.y][curr.x].incX);
      }
      else if(curr.x == par.x + 1)
      {
        path.push_back(tiles[curr.z][curr.y][curr.x].decX);
      }
      else if(curr.y + 1 == par.y)
      {
        path.push_back(tiles[curr.z][curr.y][curr.x].incY);
      }
      else if(curr.y == par.y + 1)
      {
        path.push_back(tiles[curr.z][curr.y][curr.x].decY);
      }
      else if(curr.z + 1 == par.z)
      {
        path.push_back(tiles[curr.z][curr.y][curr.x].incZ);
      }
      else if(curr.z == par.z + 1)
      {
        path.push_back(tiles[curr.z][curr.y][curr.x].decZ);
      }
      else
      {
        assert(0);
      }

      currTile = parent;
      parent = pQ.getData(currTile).parent;
    }
    while(parent != numeric_limits<IdType>::max());

    assert(currTile == startTile);
  }

  const CostType finalCost = pQ.visited(targetTile) ? pQ.getData(targetTile).actualCost : numeric_limits<CostType>::max();

  // clean up
  pQ.clear();

  return finalCost;
}

void FGR::doRRR(void)
{
  ofstream outfile(params.resultsFile.c_str(), std::ios::app);

  if(!outfile.good())
  {
    outfile << "Could not open `" << params.resultsFile << "' for writing." << endl;
  }

  if(params.maxRipIter == 0) return;

  vector<SubNetIdType> subNetsToRip;

  unsigned iterations = 0;

  DLMCost dlm(*this);

  outfile << endl;

  if(params.maxRipIter != numeric_limits<unsigned>::max())
  {
    outfile << "Performing at most " << params.maxRipIter << " rip-up and re-route iteration(s)" << endl;
  }

  double startCPU = cpuTime();

  const bool bboxConstrain = true;
  const bool allowOverflow = true;
  const bool donotallowOverflow = false;

  unsigned prevSegs = numeric_limits<unsigned>::max(), prevVias = numeric_limits<unsigned>::max(), prevOverfullEdges = numeric_limits<unsigned>::max();
  double prevOverflow = numeric_limits<double>::max();

  while(true)
  {
    // figure our which nets to rip-up and
    // update the costs of routing segments

    for(unsigned i = 0; i < edges.size(); ++i)
    {
      if(edges[i].usage > edges[i].capacity)
      {
        subNetsToRip.insert(subNetsToRip.end(), edges[i].subnets.begin(), edges[i].subnets.end());

        edges[i].historyCost += historyIncrement;
      }
    }

    for(unsigned i = 0; i < nets.size(); ++i)
    {
      for(unsigned j = 0; j < nets[i].subnets.size(); ++j)
      {
        if(!nets[i].subnets[j].routed)
        {
          subNetsToRip.push_back(make_pair(i,j));
        }
      }
    }

    if(subNetsToRip.size() == 0)
    {
      outfile << "No more subnets to rip up, quitting" << endl;
      break;
    }

    sort(subNetsToRip.begin(), subNetsToRip.end());
    subNetsToRip.erase(unique(subNetsToRip.begin(), subNetsToRip.end()), subNetsToRip.end());  

    // did we get stuck with the exact same solution?
    bool rebuild = totalSegments == prevSegs && totalVias == prevVias && 
                   prevOverfullEdges == overfullEdges && prevOverflow == totalOverflow;

    prevSegs = totalSegments;
    prevVias = totalVias;
    prevOverfullEdges = overfullEdges;
    prevOverflow = totalOverflow;

    if(rebuild)
    {
      vector<IdType> netsToRebuild;
      for(unsigned i = 0; i < subNetsToRip.size(); ++i)
      {
        netsToRebuild.push_back(subNetsToRip[i].first);
      }
      
      sort(netsToRebuild.begin(), netsToRebuild.end());
      netsToRebuild.erase(unique(netsToRebuild.begin(), netsToRebuild.end()), netsToRebuild.end());

      outfile << endl << "rebuilding " << netsToRebuild.size() << " nets" << endl;

      for(unsigned i = 0; i < netsToRebuild.size(); ++i)
      {
        if(i % 100 == 0) outfile << i << "\r" << flush;

        redoTree(netsToRebuild[i], allowOverflow, bboxConstrain, dlm);
      }
    }
    else
    {
      sort(subNetsToRip.begin(), subNetsToRip.end(), CompareByBox(nets));

      outfile << endl << "number of sub nets that need to be ripped up: " << subNetsToRip.size() << endl;

      for(unsigned i = 0; i < subNetsToRip.size(); ++i)
      {
        if(i % 100 == 0) outfile << i << "\r" << flush;

        const SubNetIdType &subNetId = subNetsToRip[i];

        unsigned overfullEdgesBefore = overfullEdges;

        ripUpSubNet(subNetId);

        if(totalOverflow <= 50. && 2.*static_cast<double>(overfullEdges) >= totalOverflow && overfullEdgesBefore != overfullEdges)
        {
          // try to route without overflow
          routeSubNet(subNetId, donotallowOverflow, bboxConstrain, dlm);
          if(!nets[subNetId.first].subnets[subNetId.second].routed)
          {
            // failing that, allow overflow
            routeSubNet(subNetId, allowOverflow, bboxConstrain, dlm);
          }
        }
        else
        {
          routeSubNet(subNetId, allowOverflow, bboxConstrain, dlm);
        }

        // end as soon as possible
        if(overfullEdges == 0) break;
      }
    }

    subNetsToRip.clear();
    ++iterations;
    outfile << "after rip-up iteration " << iterations << endl;
    printStatisticsLight();
    double cpuTimeUsed = cpuTime();

    if(iterations >= params.maxRipIter)
    {
      outfile << "Iterations exceeded, quitting" << endl;
      break;
    }
    if(cpuTimeUsed > startCPU + params.timeOut)
    {
      outfile << "Timeout exceeeded, quitting" << endl;
      break;
    }
  }
  outfile.close();
}

void FGR::layerAssignment(void)
{
  ofstream outfile(params.resultsFile.c_str(), std::ios::app);;
  
  if(!params.layerAssign)
  {
    outfile << "skipping layer assignment" << endl;
    return;
  }

  // clear the segment assignments from each net
  totalSegments = 0;
  totalVias = 0;
  for(unsigned i = 0; i < nets.size(); ++i)
  {
    nets[i].hashedSegments.clear();
    nets[i].numSegments = 0;
    nets[i].numVias = 0;
  }

  vector<SubNetIdType> order;
  for(unsigned i = 0; i < nets.size(); ++i)
  {
    for(unsigned j = 0; j < nets[i].subnets.size(); ++j)
    {
      order.push_back(make_pair(i,j));
    }
  }
  sort(order.begin(), order.end(), CompareByBox(nets));

  for(unsigned i = 0; i < order.size(); ++i)
  {
    if(i % 100 == 0) outfile << i << "\r" << flush;

    SubNet &snet = nets[order[i].first].subnets[order[i].second];
    const unsigned netId = order[i].first;

    Point prevTile = snet.one;
    Point currTile = prevTile;
    Point endTile = snet.two;
    Point fullPrevTile = prevTile;
    Point fullCurrTile = prevTile;

    vector<IdType> assignedSegments;

    do
    {
      // search for the next tile
      if(prevTile.x != currTile.x + 1 &&
         tiles[currTile.z][currTile.y][currTile.x].incX != numeric_limits<IdType>::max() &&
         binary_search(snet.segments.begin(), snet.segments.end(), tiles[currTile.z][currTile.y][currTile.x].incX))
      {
        prevTile = currTile;
        currTile.x += 1;
      }
      else if(prevTile.x + 1 != currTile.x &&
              tiles[currTile.z][currTile.y][currTile.x].decX != numeric_limits<IdType>::max() &&
              binary_search(snet.segments.begin(), snet.segments.end(), tiles[currTile.z][currTile.y][currTile.x].decX))
      {
        prevTile = currTile;
        currTile.x -= 1;
      }
      else if(prevTile.y != currTile.y + 1 &&
              tiles[currTile.z][currTile.y][currTile.x].incY != numeric_limits<IdType>::max() &&
              binary_search(snet.segments.begin(), snet.segments.end(), tiles[currTile.z][currTile.y][currTile.x].incY))
      {
        prevTile = currTile;
        currTile.y += 1;
      }
      else if(prevTile.y + 1 != currTile.y &&
              tiles[currTile.z][currTile.y][currTile.x].decY != numeric_limits<IdType>::max() &&
              binary_search(snet.segments.begin(), snet.segments.end(), tiles[currTile.z][currTile.y][currTile.x].decY))
      {
        prevTile = currTile;
        currTile.y -= 1;
      }
      else if(prevTile.z != currTile.z + 1 &&
              tiles[currTile.z][currTile.y][currTile.x].incZ != numeric_limits<IdType>::max() &&
              binary_search(snet.segments.begin(), snet.segments.end(), tiles[currTile.z][currTile.y][currTile.x].incZ))
      {
        prevTile = currTile;
        currTile.z += 1;
      }
      else if(prevTile.z + 1 != currTile.z &&
              tiles[currTile.z][currTile.y][currTile.x].decZ != numeric_limits<IdType>::max() &&
              binary_search(snet.segments.begin(), snet.segments.end(), tiles[currTile.z][currTile.y][currTile.x].decZ))
      {
        prevTile = currTile;
        currTile.z -= 1;
      }
      else
      {
        assert(0); // something went wrong
      }

      // connect these tiles in the 3d version
      if(prevTile.x != currTile.x)
      {
        fullPrevTile = fullCurrTile;

        if(prevTile.x + 1 == currTile.x)
        {
          // figure out the layer for the next full tile
          CoordType bestLayer = numeric_limits<CoordType>::max();
          unsigned bestVias = numeric_limits<unsigned>::max();
          double bestOverflow = numeric_limits<double>::max();
          for(CoordType j = 0; j < numLayers; ++j)
          {
            if(fulltiles[j][prevTile.y][prevTile.x].incX != numeric_limits<IdType>::max())
            {
              if(!nets[netId].hashedSegments.contains(fulltiles[j][prevTile.y][prevTile.x].incX))
              {
                double addedUsage = max(nets[netId].minWidth, minWidths[j]) + minSpacings[j];
                double overflow = max(0., addedUsage + fulledges[fulltiles[j][prevTile.y][prevTile.x].incX].usage -
                                          fulledges[fulltiles[j][prevTile.y][prevTile.x].incX].capacity);
                unsigned neededVias = 0;
                for(CoordType k = min(j, fullPrevTile.z); k < max(j, fullPrevTile.z); ++k)
                {
                  if(!nets[netId].hashedSegments.contains(fulltiles[k][prevTile.y][prevTile.x].incZ))
                  {
                    ++neededVias;
                  }
                }

                if(overflow < bestOverflow || (overflow == bestOverflow && neededVias < bestVias))
                {
                  bestLayer = j;
                  bestVias = neededVias;
                  bestOverflow = overflow;
                }
              }
              else
              {
                double overflow = -1.;
                unsigned neededVias = 0;
                for(CoordType k = min(j, fullPrevTile.z); k < max(j, fullPrevTile.z); ++k)
                {
                  if(!nets[netId].hashedSegments.contains(fulltiles[k][prevTile.y][prevTile.x].incZ))
                  {
                    ++neededVias;
                  }
                }

                if(overflow < bestOverflow || (overflow == bestOverflow && neededVias < bestVias))
                {
                  bestLayer = j;
                  bestVias = neededVias;
                  bestOverflow = overflow;
                }
              }
            }
          }
          assert(bestLayer != numeric_limits<CoordType>::max());
          assert(prevTile.x == fullPrevTile.x && prevTile.y == fullPrevTile.y);

          // add the stuff in
          for(CoordType j = min(fullPrevTile.z, bestLayer); j < max(fullPrevTile.z, bestLayer); ++j)
          {
            assignedSegments.push_back(fulltiles[j][prevTile.y][prevTile.x].incZ);
          }
          assignedSegments.push_back(fulltiles[bestLayer][prevTile.y][prevTile.x].incX);
          fullCurrTile = currTile;
          fullCurrTile.z = bestLayer;
        }
        else
        {
          // figure out the layer for the next full tile
          CoordType bestLayer = numeric_limits<CoordType>::max();
          unsigned bestVias = numeric_limits<unsigned>::max();
          double bestOverflow = numeric_limits<double>::max();
          for(CoordType j = 0; j < numLayers; ++j)
          {
            if(fulltiles[j][prevTile.y][prevTile.x].decX != numeric_limits<IdType>::max())
            {
              if(!nets[netId].hashedSegments.contains(fulltiles[j][prevTile.y][prevTile.x].decX))
              {
                double addedUsage = max(nets[netId].minWidth, minWidths[j]) + minSpacings[j];
                double overflow = max(0., addedUsage + fulledges[fulltiles[j][prevTile.y][prevTile.x].decX].usage -
                                          fulledges[fulltiles[j][prevTile.y][prevTile.x].decX].capacity);
                unsigned neededVias = 0;
                for(CoordType k = min(j, fullPrevTile.z); k < max(j, fullPrevTile.z); ++k)
                {
                  if(!nets[netId].hashedSegments.contains(fulltiles[k][prevTile.y][prevTile.x].incZ))
                  {
                    ++neededVias;
                  }
                }

                if(overflow < bestOverflow || (overflow == bestOverflow && neededVias < bestVias))
                {
                  bestLayer = j;
                  bestVias = neededVias;
                  bestOverflow = overflow;
                }
              }
              else
              {
                double overflow = -1.;
                unsigned neededVias = 0;
                for(CoordType k = min(j, fullPrevTile.z); k < max(j, fullPrevTile.z); ++k)
                {
                  if(!nets[netId].hashedSegments.contains(fulltiles[k][prevTile.y][prevTile.x].incZ))
                  {
                    ++neededVias;
                  }
                }

                if(overflow < bestOverflow || (overflow == bestOverflow && neededVias < bestVias))
                {
                  bestLayer = j;
                  bestVias = neededVias;
                  bestOverflow = overflow;
                }
              }
            }
          }
          assert(bestLayer != numeric_limits<CoordType>::max());
          assert(prevTile.x == fullPrevTile.x && prevTile.y == fullPrevTile.y);

          // add the stuff in
          for(CoordType j = min(fullPrevTile.z, bestLayer); j < max(fullPrevTile.z, bestLayer); ++j)
          {
            assignedSegments.push_back(fulltiles[j][prevTile.y][prevTile.x].incZ);
          }
          assignedSegments.push_back(fulltiles[bestLayer][prevTile.y][prevTile.x].decX);
          fullCurrTile = currTile;
          fullCurrTile.z = bestLayer;
        }
      }
      else if(prevTile.y != currTile.y)
      {
        fullPrevTile = fullCurrTile;
        // figure out next full tile

        if(prevTile.y + 1 == currTile.y)
        {
          // figure out the layer for the next full tile
          CoordType bestLayer = numeric_limits<CoordType>::max();
          unsigned bestVias = numeric_limits<unsigned>::max();
          double bestOverflow = numeric_limits<double>::max();
          for(CoordType j = 0; j < numLayers; ++j)
          {
            if(fulltiles[j][prevTile.y][prevTile.x].incY != numeric_limits<IdType>::max())
            {
              if(!nets[netId].hashedSegments.contains(fulltiles[j][prevTile.y][prevTile.x].incY))
              {
                double addedUsage = max(nets[netId].minWidth, minWidths[j]) + minSpacings[j];
                double overflow = max(0., addedUsage + fulledges[fulltiles[j][prevTile.y][prevTile.x].incY].usage -
                                          fulledges[fulltiles[j][prevTile.y][prevTile.x].incY].capacity);
                unsigned neededVias = 0;
                for(CoordType k = min(j, fullPrevTile.z); k < max(j, fullPrevTile.z); ++k)
                {
                  if(!nets[netId].hashedSegments.contains(fulltiles[k][prevTile.y][prevTile.x].incZ))
                  {
                    ++neededVias;
                  }
                }

                if(overflow < bestOverflow || (overflow == bestOverflow && neededVias < bestVias))
                {
                  bestLayer = j;
                  bestVias = neededVias;
                  bestOverflow = overflow;
                }
              }
              else
              {
                double overflow = -1.;
                unsigned neededVias = 0;
                for(CoordType k = min(j, fullPrevTile.z); k < max(j, fullPrevTile.z); ++k)
                {
                  if(!nets[netId].hashedSegments.contains(fulltiles[k][prevTile.y][prevTile.x].incZ))
                  {
                    ++neededVias;
                  }
                }

                if(overflow < bestOverflow || (overflow == bestOverflow && neededVias < bestVias))
                {
                  bestLayer = j;
                  bestVias = neededVias;
                  bestOverflow = overflow;
                }
              }
            }
          }
          assert(bestLayer != numeric_limits<CoordType>::max());
          assert(prevTile.x == fullPrevTile.x && prevTile.y == fullPrevTile.y);

          // add the stuff in
          for(CoordType j = min(fullPrevTile.z, bestLayer); j < max(fullPrevTile.z, bestLayer); ++j)
          {
            assignedSegments.push_back(fulltiles[j][prevTile.y][prevTile.x].incZ);
          }
          assignedSegments.push_back(fulltiles[bestLayer][prevTile.y][prevTile.x].incY);
          fullCurrTile = currTile;
          fullCurrTile.z = bestLayer;
        }
        else
        {
          // figure out the layer for the next full tile
          CoordType bestLayer = numeric_limits<CoordType>::max();
          unsigned bestVias = numeric_limits<unsigned>::max();
          double bestOverflow = numeric_limits<double>::max();
          for(CoordType j = 0; j < numLayers; ++j)
          {
            if(fulltiles[j][prevTile.y][prevTile.x].decY != numeric_limits<IdType>::max())
            {
              if(!nets[netId].hashedSegments.contains(fulltiles[j][prevTile.y][prevTile.x].decY))
              {
                double addedUsage = max(nets[netId].minWidth, minWidths[j]) + minSpacings[j];
                double overflow = max(0., addedUsage + fulledges[fulltiles[j][prevTile.y][prevTile.x].decY].usage -
                                          fulledges[fulltiles[j][prevTile.y][prevTile.x].decY].capacity);
                unsigned neededVias = 0;
                for(CoordType k = min(j, fullPrevTile.z); k < max(j, fullPrevTile.z); ++k)
                {
                  if(!nets[netId].hashedSegments.contains(fulltiles[k][prevTile.y][prevTile.x].incZ))
                  {
                    ++neededVias;
                  }
                }

                if(overflow < bestOverflow || (overflow == bestOverflow && neededVias < bestVias))
                {
                  bestLayer = j;
                  bestVias = neededVias;
                  bestOverflow = overflow;
                }
              }
              else
              {
                double overflow = -1.;
                unsigned neededVias = 0;
                for(CoordType k = min(j, fullPrevTile.z); k < max(j, fullPrevTile.z); ++k)
                {
                  if(!nets[netId].hashedSegments.contains(fulltiles[k][prevTile.y][prevTile.x].incZ))
                  {
                    ++neededVias;
                  }
                }

                if(overflow < bestOverflow || (overflow == bestOverflow && neededVias < bestVias))
                {
                  bestLayer = j;
                  bestVias = neededVias;
                  bestOverflow = overflow;
                }
              }
            }
          }
          assert(bestLayer != numeric_limits<CoordType>::max());
          assert(prevTile.x == fullPrevTile.x && prevTile.y == fullPrevTile.y);

          // add the stuff in
          for(CoordType j = min(fullPrevTile.z, bestLayer); j < max(fullPrevTile.z, bestLayer); ++j)
          {
            assignedSegments.push_back(fulltiles[j][prevTile.y][prevTile.x].incZ);
          }
          assignedSegments.push_back(fulltiles[bestLayer][prevTile.y][prevTile.x].decY);
          fullCurrTile = currTile;
          fullCurrTile.z = bestLayer;
        }
      }
    }
    while(currTile != endTile);

    // connect to the last tile
    for(CoordType j = min(fullCurrTile.z, endTile.z); j < max(fullCurrTile.z, endTile.z); ++j)
    {
      assignedSegments.push_back(fulltiles[j][fullCurrTile.y][fullCurrTile.x].incZ);
    }

    // the subnet is now connected in the 3d world
    sort(assignedSegments.begin(), assignedSegments.end());
    assignedSegments.erase(unique(assignedSegments.begin(), assignedSegments.end()), assignedSegments.end());

    // now update proper data structures
    snet.segments = assignedSegments;
    for(unsigned j = 0; j < snet.segments.size(); ++j)
    {
      const IdType edgeId = snet.segments[j];

      const CapType addedUsage = fulledges[edgeId].type == VIA ? 1 : max(nets[netId].minWidth, minWidths[fulledges[edgeId].layer]) +
                                                                     minSpacings[fulledges[edgeId].layer];

      if(!nets[netId].hashedSegments.contains(edgeId))
      {
        fulledges[edgeId].usage += addedUsage;
        if(fulledges[edgeId].type == VIA)
        {
          ++nets[netId].numVias;
          ++totalVias;
        }
        else
        {
          ++nets[netId].numSegments;
          ++totalSegments;
        }        
      }
      nets[netId].hashedSegments.insert(edgeId);

      vector<SubNetIdType>::iterator pos = lower_bound(fulledges[edgeId].subnets.begin(), fulledges[edgeId].subnets.end(), order[i]);
      assert(pos == fulledges[edgeId].subnets.end() || *pos != order[i]);
      fulledges[edgeId].subnets.insert(pos, order[i]);
    }
  }

  // update some counters
  totalOverflow = 0;
  overfullEdges = 0;
  nonViaEdges = nonViaFullEdges;
  nonViaFullEdges = 0;

  for(unsigned i = 0; i < fulledges.size(); ++i)
  {
    if(fulledges[i].usage > fulledges[i].capacity)
    {
      ++overfullEdges;
      totalOverflow += (fulledges[i].usage - fulledges[i].capacity);
    }
  }

  // swap out the edges and tiles
  tiles.clear(); tiles.swap(fulltiles);
  edges.clear(); edges.swap(fulledges);
  outfile.close();
}

void FGR::greedyImprovement(void)
{
  ofstream outfile(params.resultsFile.c_str(), std::ios::app);
  if(totalOverflow > 0)
  {
    outfile << "skipping greedy improvement due to illegal solution" << endl;
    return;
  }

  UnitCost uc(*this);

  const bool donotallowOverflow = false;
  const bool noBBoxConstrain = false;

  vector<SubNetIdType> order;
  for(unsigned i = 0; i < nets.size(); ++i)
  {
    for(unsigned j = 0; j < nets[i].subnets.size(); ++j)
    {
      order.push_back(make_pair(i,j));
    }
  }
  sort(order.begin(), order.end(), CompareByBox(nets));

  outfile << "performing " << params.maxGreedyIter << " greedy improvement iteration(s)" << endl;

  for(unsigned iterations = 1; iterations <= params.maxGreedyIter; ++iterations)
  {
    outfile << endl << "examining " << order.size() << " subnets " << endl;
    for(unsigned i = 0; i < order.size(); ++i)
    {
      if(i % 100 == 0) outfile << i << "\r" << flush;

      ripUpSubNet(order[i]);

      routeSubNet(order[i], donotallowOverflow, noBBoxConstrain, uc);
    }
    outfile << "after greedy improvement iteration " << iterations << endl;
    printStatisticsLight();
  }
  outfile.close();
}

void FGR::initialRouting(void)
{
  ofstream outfile(params.resultsFile.c_str(), std::ios::app);
  unsigned totalSubNets = 0;
  for(unsigned i = 0; i < nets.size(); ++i)
  {
    totalSubNets += nets[i].subnets.size();
  }
  outfile << "total sub nets " << totalSubNets << endl;

  DLMCost dlm(*this);

  outfile << "routing flat nets" << endl;
  const bool donotallowOverflow = false;
  routeFlatSubNets(donotallowOverflow, dlm);
  outfile << "CPU time: " << cpuTime() << " seconds " << endl;

  outfile << "routing remaining nets" << endl;
  const bool allowOverflow = true;
  routeSubNets(allowOverflow, dlm);

  printStatistics();
  outfile.close();
}

double FGR::congEstScoreSegment(IdType netId, const Point &a, const Point &b)
{
  double xFirstScore = 0.;

  for(unsigned j = min(a.x, b.x); j < max(a.x, b.x); ++j)
  {
    unsigned ycoord = min(a.y, b.y);
    double usage = 0., cap = 0., hist = 0., typUsage = 0., useful = 0.;
    for(unsigned k = 0; k < tiles.size(); ++k)
    {
      unsigned edgeId = tiles[k][ycoord][j].incX;
      if(edgeId < edges.size())
      {
        usage += edges[edgeId].usage;
        cap += edges[edgeId].capacity;
        if(edges[edgeId].capacity > 0.)
        {
          hist += edges[edgeId].historyCost;
          typUsage += minWidths[k] + minSpacings[k];
          useful += 1.;
        }
      }
    }
    hist /= useful;
    typUsage /= useful;
    usage += typUsage;

    if(usage <= cap)
    {
      xFirstScore += edgeBase + hist*max(1., usage/cap);
    }
    else
    {
      xFirstScore += edgeBase + hist*min(powMax, pow(powBase, (usage - cap)/(cap)));
    }
  }

  for(unsigned j = min(a.y, b.y); j < max(a.y, b.y); ++j)
  {
    unsigned xcoord = max(a.x, b.x);
    double usage = 0., cap = 0., hist = 0., typUsage = 0., useful = 0.;
    for(unsigned k = 0; k < tiles.size(); ++k)
    {
      unsigned edgeId = tiles[k][j][xcoord].incY;
      if(edgeId < edges.size())
      {
        usage += edges[edgeId].usage;
        cap += edges[edgeId].capacity;
        if(edges[edgeId].capacity > 0.)
        {
          hist += edges[edgeId].historyCost;
          typUsage += minWidths[k] + minSpacings[k];
          useful += 1.;
        }
      }
    }
    hist /= useful;
    typUsage /= useful;
    usage += typUsage;

    if(usage <= cap)
    {
      xFirstScore += edgeBase + hist*max(1., usage/cap);
    }
    else
    {
      xFirstScore += edgeBase + hist*min(powMax, pow(powBase, (usage - cap)/(cap)));
    }
  }

  double yFirstScore = 0.;

  for(unsigned j = min(a.y, b.y); j < max(a.y, b.y); ++j)
  {
    unsigned xcoord = min(a.x, b.x);
    double usage = 0., cap = 0., hist = 0., typUsage = 0., useful = 0.;
    for(unsigned k = 0; k < tiles.size(); ++k)
    {
      unsigned edgeId = tiles[k][j][xcoord].incY;
      if(edgeId < edges.size())
      {
        usage += edges[edgeId].usage;
        cap += edges[edgeId].capacity;
        if(edges[edgeId].capacity > 0.)
        {
          hist += edges[edgeId].historyCost;
          typUsage += minWidths[k] + minSpacings[k];
          useful += 1.;
        }
      }
    }
    hist /= useful;
    typUsage /= useful;
    usage += typUsage;

    if(usage <= cap)
    {
      yFirstScore += edgeBase + hist*max(1., usage/cap);
    }
    else
    {
      yFirstScore += edgeBase + hist*min(powMax, pow(powBase, (usage - cap)/(cap)));
    }
  }

  for(unsigned j = min(a.x, b.x); j < max(a.x, b.x); ++j)
  {
    unsigned ycoord = max(a.y, b.y);
    double usage = 0., cap = 0., hist = 0., typUsage = 0., useful = 0.;
    for(unsigned k = 0; k < tiles.size(); ++k)
    {
      unsigned edgeId = tiles[k][ycoord][j].incX;
      if(edgeId < edges.size())
      {
        usage += edges[edgeId].usage;
        cap += edges[edgeId].capacity;
        if(edges[edgeId].capacity > 0.)
        {
          hist += edges[edgeId].historyCost;
          typUsage += minWidths[k] + minSpacings[k];
          useful += 1.;
        }
      }
    }
    hist /= useful;
    typUsage /= useful;
    usage += typUsage;

    if(usage <= cap)
    {
      yFirstScore += edgeBase + hist*max(1., usage/cap);
    }
    else
    {
      yFirstScore += edgeBase + hist*min(powMax, pow(powBase, (usage - cap)/(cap)));
    }
  }

  return min(xFirstScore, yFirstScore);
}

void FGR::redoTree(IdType netId, bool allowOverflow, bool bboxConstrain, const CostFunction &f)
{
  if(nets[netId].subnets.size() <= 1)
  {
    return;
  }

  vector<pair<Point,Point> > origTree, mstTree;
  vector<vector<IdType> > origTreeSegs;

  for(unsigned i = 0; i < nets[netId].subnets.size(); ++i)
  {
    origTree.push_back(make_pair(nets[netId].subnets[i].one, nets[netId].subnets[i].two));
    origTreeSegs.push_back(nets[netId].subnets[i].segments);
    ripUpSubNet(make_pair(netId,i));
  }
  nets[netId].subnets.clear();

  // build a congestion driven MST
  vector<double> distanceToTree(nets[netId].pins.size(), numeric_limits<double>::max());
  vector<bool> inTree(nets[netId].pins.size(), false);
  vector<unsigned> parent(nets[netId].pins.size(), numeric_limits<unsigned>::max());

  unsigned currentPin = 0, nextPin = numeric_limits<unsigned>::max();
  for(unsigned j = 0; j < nets[netId].pins.size()-1; ++j)
  {
    inTree[currentPin] = true;
    double bestDistance = numeric_limits<double>::max();
    for(unsigned k = 0; k < nets[netId].pins.size(); ++k)
    {
      if(inTree[k]) continue;

      double thisDistance = congEstScoreSegment(netId, nets[netId].pins[k], nets[netId].pins[currentPin]);

      if(thisDistance < distanceToTree[k])
      {
        distanceToTree[k] = thisDistance;
        parent[k] = currentPin;
      }
      if(distanceToTree[k] < bestDistance)
      {
        bestDistance = distanceToTree[k];
        nextPin = k;
      }
    }

    mstTree.push_back(make_pair(nets[netId].pins[nextPin], nets[netId].pins[parent[nextPin]]));
    currentPin = nextPin;
  }

  // evaluate the trees we have
  double bestTreeScore = numeric_limits<double>::max();
  vector<pair<Point, Point> > *bestTree = NULL;
  vector<vector<IdType> > bestTreeSegs;

  double origTreeScore = 0.;
  for(unsigned i = 0; i < origTree.size(); ++i)
  {
    SubNet stemp;
    stemp.one = origTree[i].first;
    stemp.two = origTree[i].second;
    nets[netId].subnets.push_back(stemp);
  }

  for(unsigned i = 0; i < origTreeSegs.size(); ++i)
  {
    for(unsigned j = 0; j < origTreeSegs[i].size(); ++j)
    {
      if(!nets[netId].hashedSegments.contains(origTreeSegs[i][j]))
      {
        origTreeScore += f(origTreeSegs[i][j], netId);
      }
      addSegment(make_pair(netId, i), origTreeSegs[i][j]);
    }
  }

  if(origTreeScore < bestTreeScore)
  {
    bestTreeScore = origTreeScore;
    bestTree = &origTree;
    bestTreeSegs.clear();
    for(unsigned i = 0; i < nets[netId].subnets.size(); ++i)
    {
      bestTreeSegs.push_back(nets[netId].subnets[i].segments);
    }
  }

  for(unsigned i = 0; i < nets[netId].subnets.size(); ++i)
  {
    ripUpSubNet(make_pair(netId, i));
  }
  nets[netId].subnets.clear();

  double mstScore = 0.;
  if(!mstTree.empty())
  {
    for(unsigned i = 0; i < mstTree.size(); ++i)
    {
      SubNet stemp;
      stemp.one = mstTree[i].first;
      stemp.two = mstTree[i].second;
      nets[netId].subnets.push_back(stemp);
    }

    for(unsigned i = 0; i < nets[netId].subnets.size(); ++i)
    {
      mstScore += routeSubNet(make_pair(netId, i), allowOverflow, bboxConstrain, f);
    }
    for(SegmentHash::iterator i = nets[netId].hashedSegments.begin();
        i != nets[netId].hashedSegments.end(); ++i)
    {
      if(i->second > 1)
      {
        mstScore -= epsilon*static_cast<double>(i->second - 1);
      }
    }

    if(mstScore < bestTreeScore)
    {
      bestTreeScore = mstScore;
      bestTree = &mstTree;
      bestTreeSegs.clear();
      for(unsigned i = 0; i < nets[netId].subnets.size(); ++i)
      {
        bestTreeSegs.push_back(nets[netId].subnets[i].segments);
      }
    }

    for(unsigned i = 0; i < nets[netId].subnets.size(); ++i)
    {
      ripUpSubNet(make_pair(netId, i));
    }
    nets[netId].subnets.clear();
  }
  else
  {
    mstScore = numeric_limits<double>::max();
  }

  for(unsigned i = 0; i < bestTree->size(); ++i)
  {
    SubNet stemp;
    stemp.one = (*bestTree)[i].first;
    stemp.two = (*bestTree)[i].second;
    nets[netId].subnets.push_back(stemp);
  }

  // route this new tree
  for(unsigned i = 0; i < bestTreeSegs.size(); ++i)
  {
    for(unsigned j = 0; j < bestTreeSegs[i].size(); ++j)
    {
      addSegment(make_pair(netId, i), bestTreeSegs[i][j]);
    }
  }

  for(unsigned i = 0; i < nets[netId].subnets.size(); ++i)
  {
    nets[netId].subnets[i].routed = true;
  }
}
