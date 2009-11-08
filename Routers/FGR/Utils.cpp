#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <limits>
#include <cmath>
#include <cassert>
//#include <sys/resource.h>
#include <stdlib.h>
#include <algorithm>
#include <math.h>
#include <time.h>

using std::vector;
using std::pair;
using std::cout;
using std::endl;
using std::flush;
using std::string;
using std::map;
using std::numeric_limits;
using std::make_pair;
using std::min;
using std::max;
using std::swap;
using std::ofstream;

#include "FGR.h"

double cpuTime(void)
{
  /*struct rusage cputime;
  getrusage(RUSAGE_SELF, &cputime);

  return static_cast<double>(cputime.ru_utime.tv_sec) +
        (1.e-6)*static_cast<double>(cputime.ru_utime.tv_usec) +
        static_cast<double>(cputime.ru_stime.tv_sec) +
        (1.e-6)*static_cast<double>(cputime.ru_stime.tv_usec);*/
  return clock() / CLOCKS_PER_SEC;
}

IdType FGR::xyzToTileId(CoordType x, CoordType y, CoordType z)
{
  assert(x < xTiles);
  assert(y < yTiles);
  assert(z < numLayers);
  return z*yTiles*xTiles + y*xTiles + x;
}

Point FGR::tileIdToXYZ(IdType id)
{
  assert(id < numLayers*xTiles*yTiles);
  return Point(id % xTiles, (id / xTiles) % yTiles, id / (xTiles*yTiles));
}

void FGR::printStatistics(bool checkRouted, bool final)
{
  CapType maxOverfill = 0;
  for(unsigned i = 0; i < edges.size(); ++i)
  {
    if(edges[i].usage > edges[i].capacity)
    {
      maxOverfill = max(maxOverfill, static_cast<CapType>(edges[i].usage - edges[i].capacity));
    }
  }

  unsigned netsRouted = 0, routedLen = 0, numVias = 0;
  for(unsigned i = 0; i < nets.size(); ++i)
  {
    if(nets[i].pins.size() <= 1) continue;

    bool netRouted = true;
    for(unsigned j = 0; j < nets[i].subnets.size(); ++j)
    {
      if(!nets[i].subnets[j].routed)
      {
        netRouted = false;
        break;
      }
    }

    if(!checkRouted || netRouted)
    {
      ++netsRouted;

      numVias += nets[i].numVias;

      routedLen += nets[i].numSegments;
    }
  }

  ofstream outfile(params.resultsFile.c_str());

  if(!outfile.good())
  {
    outfile << "Could not open `" << params.resultsFile << "' for writing." << endl;
  }
  
  if (final) outfile << "final ";
  outfile << "nets routed " << netsRouted << " ("
    << 100.*static_cast<double>(netsRouted)/static_cast<double>(routableNets) << "%)" << endl;
  
  if(final) outfile << "final ";
  
  outfile << "total length for routed nets " << routedLen << endl;
  
  if(final) outfile << "final ";
  
  outfile << "total number of vias " << numVias << endl;
  
  if(final) outfile << "final ";
  
  outfile << "total wirelength " << routedLen + viaFactor*numVias << endl;
  
  if(final) outfile << "final ";
  
  outfile << "number of edges overfull is " << overfullEdges << " ("
    << 100.*static_cast<double>(overfullEdges)/static_cast<double>(nonViaEdges) << "%)" << endl;
  
  if(final) outfile << "final ";
  
  outfile << "max overfill is " << maxOverfill << endl;
  
  if(final) outfile << "final ";
  
  outfile << "total overfill is " << totalOverflow << endl;
  
  if(final) outfile << "final ";
  
  outfile << "avg overfill is " << totalOverflow/static_cast<double>(nonViaEdges) << endl;
  
  if(final) outfile << "final ";
  
  outfile << "CPU time: " << cpuTime() << " seconds" << endl;  
}

void FGR::printStatisticsLight(void)
{
  ofstream outfile(params.resultsFile.c_str());

  if(!outfile.good())
  {
    outfile << "Could not open `" << params.resultsFile << "' for writing." << endl;
  }

  outfile << "total length for routed nets " << totalSegments << endl;
  if(numLayers > 1)
  {
    outfile << "total number of vias " << totalVias << endl;
    outfile << "total wirelength " << totalSegments + viaFactor*totalVias << endl;
  }
  outfile << "number of edges overfull is " << overfullEdges << " ("
       << 100.*static_cast<double>(overfullEdges)/static_cast<double>(nonViaEdges) << "%)" << endl;
  outfile << "total overfill is " << totalOverflow << endl;
  outfile << "avg overfill is " << totalOverflow/static_cast<double>(nonViaEdges) << endl;
  outfile << "CPU time: " << cpuTime() << " seconds" << endl;  
}

void BitBoard::setBit(IdType id)
{
  assert(id < bits.size());
  if(!bits[id])
  {
    bits[id] = true;
    setBits.push_back(id);
  }
}

bool BitBoard::isBitSet(IdType id) const
{
  assert(id < bits.size());
  return bits[id];
}

void BitBoard::clear(void)
{
  for(unsigned i = 0; i < setBits.size(); ++i)
  {
    bits[setBits[i]] = false;
  }
  setBits.clear();
}

void BitBoard::resize(IdType size)
{
  IdType oldSize = bits.size();

  clear();
  bits.resize(size);
  if(oldSize < size)
  {
    for(IdType i = oldSize; i < size; ++i)
    {
      bits[i] = false;
    }
  }
}

void FGR::buildGrid(bool skipEmpty)
{
  tiles.resize(2);

  if(params.layerAssign)
  {
    for(unsigned k = 0; k < 2; ++k)
    {
      tiles[k].resize(yTiles);
      for(unsigned j = 0; j < yTiles; ++j)
      {
        tiles[k][j].resize(xTiles);
      }
    }
  }

  fulltiles.resize(numLayers);
  for(unsigned k = 0; k < numLayers; ++k)
  {
    fulltiles[k].resize(yTiles);
    for(unsigned j = 0; j < yTiles; ++j)
    {
      fulltiles[k][j].resize(xTiles);
    }
  }

  pQ.resize(numLayers*xTiles*yTiles);

  Edge newEdge;
  newEdge.usage = 0;

  if(params.layerAssign)
  {
    edges.reserve((xTiles-1)*yTiles + xTiles*(yTiles-1) + xTiles*yTiles);

    // add horiz edges to 2d problem
    for(unsigned j = 0; j < yTiles; ++j)
    {
      for(unsigned i = 0; i < xTiles-1; ++i)
      {
        newEdge.tile1 = xyzToTileId(i,j,0);
        newEdge.tile2 = xyzToTileId(i+1,j,0);
        newEdge.capacity = 0;
        newEdge.type = HORIZ;
        newEdge.layer = 0;

        tiles[0][j][i].incX = edges.size();
        tiles[0][j][i+1].decX = edges.size();
        edges.push_back(newEdge);
      }
    }

    // add vert edges to 2d problem
    for(unsigned i = 0; i < xTiles; ++i)
    {
      for(unsigned j = 0; j < yTiles-1; ++j)
      {
        newEdge.tile1 = xyzToTileId(i,j,1);
        newEdge.tile2 = xyzToTileId(i,j+1,1);
        newEdge.capacity = 0;
        newEdge.type = VERT;
        newEdge.layer = 1;

        tiles[1][j][i].incY = edges.size();
        tiles[1][j+1][i].decY = edges.size();
        edges.push_back(newEdge);
      }
    }

    nonViaEdges = edges.size();

    // add vias to 2d
    for(unsigned i = 0; i < xTiles; ++i)
    {
      for(unsigned j = 0; j < yTiles; ++j)
      {
        newEdge.tile1 = xyzToTileId(i,j,0);
        newEdge.tile2 = xyzToTileId(i,j,1);
        newEdge.capacity = numeric_limits<CapType>::max();
        newEdge.type = VIA;

        tiles[0][j][i].incZ = edges.size();
        tiles[1][j][i].decZ = edges.size();
        edges.push_back(newEdge);
      }
    }
  }

  fulledges.reserve(numLayers*(xTiles-1)*yTiles + numLayers*xTiles*(yTiles-1) + (numLayers-1)*xTiles*yTiles);

  // add horiz edges to full problem
  for(unsigned k = 0; k < numLayers; ++k)
  {
    if(skipEmpty && horizCaps[k] == 0.) continue;

    for(unsigned j = 0; j < yTiles; ++j)
    {
      for(unsigned i = 0; i < xTiles-1; ++i)
      {
        newEdge.tile1 = xyzToTileId(i,j,k);
        newEdge.tile2 = xyzToTileId(i+1,j,k);
        newEdge.capacity = horizCaps[k];
        newEdge.type = HORIZ;
        newEdge.layer = k;

        fulltiles[k][j][i].incX = fulledges.size();
        fulltiles[k][j][i+1].decX = fulledges.size();
        fulledges.push_back(newEdge);
      }
    }
  }

  // add vert edges to full problem
  for(unsigned k = 0; k < numLayers; ++k)
  {
    if(skipEmpty && vertCaps[k] == 0.) continue;

    for(unsigned i = 0; i < xTiles; ++i)
    {
      for(unsigned j = 0; j < yTiles-1; ++j)
      {
        newEdge.tile1 = xyzToTileId(i,j,k);
        newEdge.tile2 = xyzToTileId(i,j+1,k);
        newEdge.capacity = vertCaps[k];
        newEdge.type = VERT;
        newEdge.layer = k;

        fulltiles[k][j][i].incY = fulledges.size();
        fulltiles[k][j+1][i].decY = fulledges.size();
        fulledges.push_back(newEdge);
      }
    }
  }

  nonViaFullEdges = fulledges.size();

  // add vias to full problem
  for(unsigned k = 0; k < numLayers-1; ++k)
  {
    for(unsigned i = 0; i < xTiles; ++i)
    {
      for(unsigned j = 0; j < yTiles; ++j)
      {
        newEdge.tile1 = xyzToTileId(i,j,k);
        newEdge.tile2 = xyzToTileId(i,j,k+1);
        newEdge.capacity = numeric_limits<CapType>::max();
        newEdge.type = VIA;

        fulltiles[k][j][i].incZ = fulledges.size();
        fulltiles[k+1][j][i].decZ = fulledges.size();
        fulledges.push_back(newEdge);
      }
    }
  }
}

IdType PQueue::getBestTile(void) const
{
  assert(!empty());
  return heap.front();
}

void PQueue::removeBestTile(void)
{
  assert(!empty());

  data[heap.back()].heapLoc = 0;
  data[heap.front()].heapLoc = numeric_limits<IdType>::max();
  heap.front() = heap.back();
  heap.pop_back();

  // now heap down
  IdType idx = 0, lc = 1, rc = 2;
  while(lc < heap.size())
  {
    if(rc < heap.size())
    {
      if(data[heap[lc]].totalCost < data[heap[rc]].totalCost)
      {
        if(data[heap[idx]].totalCost > data[heap[lc]].totalCost)
        {
          data[heap[idx]].heapLoc = lc;
          data[heap[lc]].heapLoc = idx;
          swap(heap[idx], heap[lc]);
          idx = lc;
        }
        else
        {
          break;
        }
      }
      else
      {
        if(data[heap[idx]].totalCost > data[heap[rc]].totalCost)
        {
          data[heap[idx]].heapLoc = rc;
          data[heap[rc]].heapLoc = idx;
          swap(heap[idx], heap[rc]);
          idx = rc;
        }
        else
        {
          break;
        }
      }
    }
    else
    {
      if(data[heap[idx]].totalCost > data[heap[lc]].totalCost)
      {
        data[heap[idx]].heapLoc = lc;
        data[heap[lc]].heapLoc = idx;
        swap(heap[idx], heap[lc]);
        idx = lc;
      }
      else
      {
        break;
      }
    }
    lc = 2*idx + 1;
    rc = lc + 1;
  }
}

void PQueue::updateTile(IdType tileId, CostType totalCost, CostType actualCost, IdType parent)
{
  if(dataValid.isBitSet(tileId))
  {
    if(totalCost < data[tileId].totalCost)
    {
      data[tileId].totalCost = totalCost;
      data[tileId].actualCost = actualCost;
      data[tileId].parent = parent;

      // heap up
      IdType idx = data[tileId].heapLoc, parent;
      while(idx > 0)
      {
        parent = (idx-1)/2;
        if(data[heap[idx]].totalCost < data[heap[parent]].totalCost)
        {
          data[heap[idx]].heapLoc = parent;
          data[heap[parent]].heapLoc = idx;
          swap(heap[idx], heap[parent]);
          idx = parent;
        }
        else
        {
          break;
        }
      }
    }
  }
  else
  {
    data[tileId].totalCost = totalCost;
    data[tileId].actualCost = actualCost;
    data[tileId].parent = parent;
    data[tileId].heapLoc = heap.size();
    heap.push_back(tileId);
    dataValid.setBit(tileId);

    // heap up
    IdType idx = data[tileId].heapLoc;
    while(idx > 0)
    {
      parent = (idx-1)/2;
      if(data[heap[idx]].totalCost < data[heap[parent]].totalCost)
      {
        data[heap[idx]].heapLoc = parent;
        data[heap[parent]].heapLoc = idx;
        swap(heap[idx], heap[parent]);
        idx = parent;
      }
      else
      {
        break;
      }
    }
  }
}

const PQueue::tileData& PQueue::getData(IdType tileId) const
{
  assert(dataValid.isBitSet(tileId));
  return data[tileId];
}

bool PQueue::visited(IdType tileId) const
{
  return dataValid.isBitSet(tileId);
}

void PQueue::clear(void)
{
  dataValid.clear();
  heap.clear();
}

void FGR::addSegment(const SubNetIdType &subNetId, IdType edgeId)
{
  const IdType netId = subNetId.first;
  const CapType addedUsage = edges[edgeId].type == VIA ? 1 : max(nets[netId].minWidth, minWidths[edges[edgeId].layer]) +
                                                             minSpacings[edges[edgeId].layer];

  SubNet &snet = nets[subNetId.first].subnets[subNetId.second];  
  vector<IdType>::iterator pos2 = lower_bound(snet.segments.begin(), snet.segments.end(), edgeId);
  assert(pos2 == snet.segments.end() || *pos2 != edgeId);
  snet.segments.insert(pos2, edgeId);

  vector<SubNetIdType>::iterator pos3 = lower_bound(edges[edgeId].subnets.begin(), edges[edgeId].subnets.end(), subNetId);
  assert(pos3 == edges[edgeId].subnets.end() || *pos3 != subNetId);
  edges[edgeId].subnets.insert(pos3, subNetId);

  if(!nets[netId].hashedSegments.contains(edgeId))
  {
    CapType oldOverflow = edges[edgeId].usage > edges[edgeId].capacity ? edges[edgeId].usage - edges[edgeId].capacity : 0;
    totalOverflow -= oldOverflow;
    edges[edgeId].usage += addedUsage;
    CapType newOverflow = edges[edgeId].usage > edges[edgeId].capacity ? edges[edgeId].usage - edges[edgeId].capacity : 0;
    totalOverflow += newOverflow;
    if(oldOverflow == 0 && newOverflow > 0) { ++overfullEdges; }
    if(edges[edgeId].type == VIA)
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
}

void FGR::ripUpSegment(const SubNetIdType &subNetId, IdType edgeId)
{
  const IdType netId = subNetId.first;
  const CapType addedUsage = edges[edgeId].type == VIA ? 1 : max(nets[netId].minWidth, minWidths[edges[edgeId].layer]) +
                                                             minSpacings[edges[edgeId].layer];

  assert(edges[edgeId].usage >= addedUsage);

  nets[netId].hashedSegments.remove(edgeId);
  if(!nets[netId].hashedSegments.contains(edgeId))
  {
    CapType oldOverflow = edges[edgeId].usage > edges[edgeId].capacity ? edges[edgeId].usage - edges[edgeId].capacity : 0;
    totalOverflow -= oldOverflow;
    edges[edgeId].usage -= addedUsage;
    CapType newOverflow = edges[edgeId].usage > edges[edgeId].capacity ? edges[edgeId].usage - edges[edgeId].capacity : 0;
    totalOverflow += newOverflow;
    if(oldOverflow > 0 && newOverflow == 0) { --overfullEdges; }
    if(edges[edgeId].type == VIA)
    {
      --nets[netId].numVias;
      --totalVias;
    }
    else
    {
      --nets[netId].numSegments;
      --totalSegments;
    }
  }

  vector<SubNetIdType>::iterator pos = lower_bound(edges[edgeId].subnets.begin(), edges[edgeId].subnets.end(), subNetId);
  assert(pos != edges[edgeId].subnets.end() && *pos == subNetId);
  edges[edgeId].subnets.erase(pos);
}

/*inline*/ CostType LBManhattan::operator()(const Point &a, const Point &b) const
{
  return min(epsilon,edgeBase)*(fabs(static_cast<CostType>(a.x) - static_cast<CostType>(b.x)) +
             fabs(static_cast<CostType>(a.y) - static_cast<CostType>(b.y)) +
             fabs(static_cast<CostType>(a.z) - static_cast<CostType>(b.z)));
}


/*inline*/ CostType ExactManhattan::operator()(const Point &a, const Point &b) const
{
  return edgeBase*(fabs(static_cast<CostType>(a.x) - static_cast<CostType>(b.x)) +
                   fabs(static_cast<CostType>(a.y) - static_cast<CostType>(b.y)) +
                   viaFactor*fabs(static_cast<CostType>(a.z) - static_cast<CostType>(b.z)));
}

void FGRParams::usage(const char *exename)
{
/*  ofstream outfile(resultsFile.c_str());
  outfile << "Usage: " << exename << " <filename> [options]" << endl;
  outfile << endl;
  outfile << "Available options:" << endl;
  outfile << "  -labyrinth            Input is in Labyrinth format" << endl;
  outfile << "  -o <filename>         Save routes in <filename>" << endl;
  outfile << "  -maxRipIter <uint>    Maximum rip-up and re-route iterations" << endl;
  outfile << "  -timeOut <double>     Rip-up and re-route timeout (seconds)" << endl;
  outfile << "  -maxGreedyIter <uint> Maximum greedy iterations" << endl;
  outfile << "  -full3d               Do not use layer assignment" << endl;
  outfile << endl;*/
  exit(0);
}

void FGRParams::setDefault(void)
{
  layerAssign = true;
  labyrinth = false;
  maxRipIter = numeric_limits<unsigned>::max();
  maxGreedyIter = 1;
  timeOut = 60.*60.*24.; // 24 hours
  outputFile = "";
}

void FGRParams::print(void) const
{
  ofstream outfile(resultsFile.c_str());
  outfile << endl << "FGR parameters:" << endl;
  outfile << "Labyrinth format:       " << (labyrinth ? "true" : "false") << endl;
  outfile << "Layer assignment:       " << (layerAssign ? "true" : "false") << endl;
  outfile << "Maximum RRR iterations: " << maxRipIter << endl;
  outfile << "Max RRR runtime:        " << timeOut << " seconds" << endl;
  if(!outputFile.empty())
    outfile << "Save routes to file:    `" << outputFile << "'" << endl;
  outfile << endl;
}

FGRParams::FGRParams(int argc, char **argv)
{
  setDefault();
  ofstream outfile(resultsFile.c_str());

  if(argc < 2)
  {
    usage(argv[0]);
  }

  for(int i = 2; i < argc; ++i)
  {
    if(argv[i] == string("-h") || argv[i] == string("-help"))
    {
      usage(argv[0]);
    }
    else if(argv[i] == string("-o"))
    {
      if(i+1 < argc)
      {
        outputFile  = argv[++i];
        resultsFile = outputFile + ".results";
      }
      else
      {
        outfile << "option -o requires an argument" << endl;
        usage(argv[0]);
      }
    }
    else if(argv[i] == string("-full3d"))
    {
      layerAssign = false;
    }
    else if(argv[i] == string("-labyrinth"))
    {
      labyrinth = true;
    }
    else if(argv[i] == string("-maxRipIter"))
    {
      if(i+1 < argc)
      {
        maxRipIter = static_cast<unsigned>(atoi(argv[++i]));
      }
      else
      {
        outfile << "option -maxRipIter requires an argument" << endl;
        usage(argv[0]);
      }
    }
    else if(argv[i] == string("-maxGreedyIter"))
    {
      if(i+1 < argc)
      {
        maxGreedyIter = static_cast<unsigned>(atoi(argv[++i]));
      }
      else
      {
        outfile << "option -maxGreedyIter requires an argument" << endl;
        usage(argv[0]);
      }
    }
    else if(argv[i] == string("-timeOut"))
    {
      if(i+1 < argc)
      {
        timeOut = atof(argv[++i]);
      }
      else
      {
        outfile << "option -timeOut requires an argument" << endl;
        usage(argv[0]);
      }
    }
    else
    {
      outfile << "unknown commandline option" << endl;
      usage(argv[0]);
    }
  }
}
