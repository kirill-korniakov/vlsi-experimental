#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <limits>
#include <limits.h>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <math.h>

using std::ifstream;
using std::ofstream;
using std::vector;
using std::pair;
using std::endl;
using std::flush;
using std::string;
using std::map;
using std::numeric_limits;
using std::make_pair;

#include "FGR.h"
#include "stdTypes.h"
using namespace fgr;

void expect(string s1, string s2)
{
  if(s1 != s2)
  {
//    outfile << "Parsing error. Expected `" << s2
//         << "' but got `" << s1 << "' instead" << endl;
    exit(0);
  }
}

double scoreTree(const vector<pair<Point,Point> > &tree)
{
  double score = 0.;

  for(unsigned i = 0; i < tree.size(); ++i)
  {
    score += static_cast<double>(max(tree[i].first.x, tree[i].second.x) - min(tree[i].first.x, tree[i].second.x));
    score += static_cast<double>(max(tree[i].first.y, tree[i].second.y) - min(tree[i].first.y, tree[i].second.y));
  }

  return score;
}

void FGR::parseInput(const char *filename)
{
  if(params.labyrinth)
  {
    parseInputLab(filename);
    return;
  }

  ifstream infile(filename);
  ofstream outfile(params.resultsFile.c_str(), std::ios::app);

  if(!infile.good())
  {
    outfile << "Could not open `" << filename << "' for reading" << endl;
    exit(0);
  }
  else
  {
    outfile << "Reading from `" << filename << "' ..." << endl;
  }

  string tmp;

  infile >> tmp; expect(tmp, string("grid"));

  infile >> xTiles >> yTiles >> numLayers;

  if(numLayers <= 2) params.layerAssign = false;

  outfile << "grid size " << xTiles << "x" << yTiles << endl;

  infile >> tmp; expect(tmp, string("vertical"));
  infile >> tmp; expect(tmp, string("capacity"));

  for(unsigned i = 0; i < numLayers; ++i)
  {
    unsigned cap;
    infile >> cap;
    vertCaps.push_back(cap);
  }

  infile >> tmp; expect(tmp, string("horizontal"));
  infile >> tmp; expect(tmp, string("capacity"));

  for(unsigned i = 0; i < numLayers; ++i)
  {
    unsigned cap;
    infile >> cap;
    horizCaps.push_back(cap);
  }

  infile >> tmp; expect(tmp, string("minimum"));
  infile >> tmp; expect(tmp, string("width"));

  for(unsigned i = 0; i < numLayers; ++i)
  {
    unsigned minwidth;
    infile >> minwidth;
    minWidths.push_back(minwidth);
  }

  infile >> tmp; expect(tmp, string("minimum"));
  infile >> tmp; expect(tmp, string("spacing"));

  for(unsigned i = 0; i < numLayers; ++i)
  {
    unsigned minspacing;
    infile >> minspacing;
    minSpacings.push_back(minspacing);
  }

  infile >> tmp; expect(tmp, string("via"));
  infile >> tmp; expect(tmp, string("spacing"));

  for(unsigned i = 0; i < numLayers; ++i)
  {
    unsigned viaspacing;
    infile >> viaspacing;
    viaSpacings.push_back(viaspacing);
  }

  infile >> minX >> minY >> tileWidth >> tileHeight;

  ///halfWidth = static_cast<LenType>(trunc(0.5*tileWidth));
  halfWidth = static_cast<LenType>(0.5*tileWidth);
  ///halfHeight = static_cast<LenType>(trunc(0.5*tileHeight));
  halfHeight = static_cast<LenType>(0.5*tileHeight);

  infile >> tmp; expect(tmp, string("num"));
  infile >> tmp; expect(tmp, string("net"));

  unsigned numNets;

  infile >> numNets;

  outfile << "total nets " << numNets << endl;

  unsigned netsTooBig = 0;
  nets.reserve(numNets);

  for(unsigned i = 0; i < numNets; ++i)
  {
    Net temp;
    unsigned numPins;
    string name;
    IdType id;

    infile >> name >> id >> numPins >> temp.minWidth;

    for(unsigned j = 0; j < numPins; ++j)
    {
      // translate absolute coords to grid coords
      double pinX, pinY;
      unsigned layer;

      infile >> pinX >> pinY >> layer;

      Point newPin;
      newPin.x = static_cast<unsigned>(floor((pinX - minX)/tileWidth));
      newPin.y = static_cast<unsigned>(floor((pinY - minY)/tileHeight));
      newPin.z = layer-1;

      if(params.layerAssign && newPin.z >= 2)
      {
        outfile << "Warning: layer assignment is incompatible with "
             << "pins not being on layers 1 or 2, capping the layer to 2" << endl;
        newPin.z = 1;
      }

      temp.pins.push_back(newPin);
    }

    sort(temp.pins.begin(), temp.pins.end());
    temp.pins.erase(unique(temp.pins.begin(), temp.pins.end()), temp.pins.end());

    if(numPins <= 1000 && temp.pins.size() > 1)
    {
      ++routableNets;

      // build the MST tree
      vector<pair<Point,Point> > mstTree;
      vector<unsigned> distanceToTree(temp.pins.size(), UINT_MAX);
      vector<bool> inTree(temp.pins.size(), false);
      vector<unsigned> parent(temp.pins.size(), UINT_MAX);

      unsigned currentPin = 0, nextPin = UINT_MAX;
      for(unsigned j = 0; j < temp.pins.size()-1; ++j)
      {
        inTree[currentPin] = true;
        unsigned bestDistance = UINT_MAX;
        for(unsigned k = 0; k < temp.pins.size(); ++k)
        {
          if(inTree[k]) continue;

          unsigned thisDistance = max(temp.pins[k].x, temp.pins[currentPin].x)
                                - min(temp.pins[k].x, temp.pins[currentPin].x)
                                + max(temp.pins[k].y, temp.pins[currentPin].y)
                                - min(temp.pins[k].y, temp.pins[currentPin].y);
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
        mstTree.push_back(make_pair(temp.pins[nextPin], temp.pins[parent[nextPin]]));
        currentPin = nextPin;
      }

      // add in the MST tree
      for(unsigned j = 0; j < mstTree.size(); ++j)
      {
        SubNet stemp;
        stemp.one = mstTree[j].first;
        stemp.two = mstTree[j].second;
        temp.subnets.push_back(stemp);
      }

      netNameToIdx[name] = nets.size();
      nets.push_back(temp);
      netNames.push_back(name);
      netIds.push_back(id);
    }
    else
    {
      if(numPins > 1000) ++netsTooBig;
      netNameToIdx[name] = numeric_limits<IdType>::max();
    }
  }

  outfile << "number of nets too big: " << netsTooBig << endl;

  // build the routing grid graph
  bool skipEmpty = true;
  buildGrid(skipEmpty);

  unsigned capacityChanges;
  infile >> capacityChanges;

  for(unsigned i = 0; i < capacityChanges; ++i)
  {
    unsigned gridCol1, gridRow1, layer1, gridCol2, gridRow2, layer2, newCap;

    infile >> gridCol1 >> gridRow1 >> layer1
           >> gridCol2 >> gridRow2 >> layer2
           >> newCap;

    assert(layer1 == layer2);
    if(gridCol1 == gridCol2)
    {
      // vert segment
      assert(gridRow1 == gridRow2+1 || gridRow1+1 == gridRow2);
      if(fulltiles[layer1-1][min(gridRow1,gridRow2)][gridCol1].incY == numeric_limits<IdType>::max())
      {
        if(newCap != 0.)
        {
          outfile << "Increasing capacity on a previously empty edge." << endl;
          exit(0);
        }
      }
      else
      {
        fulledges[fulltiles[layer1-1][min(gridRow1,gridRow2)][gridCol1].incY].capacity = newCap;
        if(newCap == 0.)
        {
          fulltiles[layer1-1][min(gridRow1,gridRow2)][gridCol1].incY = numeric_limits<IdType>::max();
          fulltiles[layer1-1][max(gridRow1,gridRow2)][gridCol1].decY = numeric_limits<IdType>::max();
        }
      }
    }
    else if(gridRow1 == gridRow2)
    {
      // horiz segment
      assert(gridCol1 == gridCol2+1 || gridCol1+1 == gridCol2);
      if(fulltiles[layer1-1][gridRow1][min(gridCol1,gridCol2)].incX == numeric_limits<IdType>::max())
      {
        if(newCap != 0.)
        {
          outfile << "Increasing capacity on a previously empty edge." << endl;
          exit(0);
        }
      }
      else
      {
        fulledges[fulltiles[layer1-1][gridRow1][min(gridCol1,gridCol2)].incX].capacity = newCap;
        if(newCap == 0.)
        {
          fulltiles[layer1-1][gridRow1][min(gridCol1,gridCol2)].incX = numeric_limits<IdType>::max();
          fulltiles[layer1-1][gridRow1][max(gridCol1,gridCol2)].decX = numeric_limits<IdType>::max();
        }
      }
    }
    else
    {
      outfile << "Bad capacity change" << endl;
      exit(0);
    }
  }

  // assign proper edge capacities to 2d problem
  if(params.layerAssign)
  {
    for(unsigned i = 0; i < fulledges.size(); ++i)
    {
      Point starttile = tileIdToXYZ(fulledges[i].tile1);
      if(fulledges[i].type == HORIZ)
      {
        unsigned edgeId = tiles[0][starttile.y][starttile.x].incX;
        edges[edgeId].capacity += fulledges[i].capacity;
      }
      else if(fulledges[i].type == VERT)
      {
        unsigned edgeId = tiles[1][starttile.y][starttile.x].incY;
        edges[edgeId].capacity += fulledges[i].capacity;
      }
    }

    // remove 0 cap edges
    for(unsigned i = 0; i < edges.size(); ++i)
    {
      if(edges[i].capacity == 0.)
      {
        if(edges[i].type == HORIZ)
        {
          Point tile1 = tileIdToXYZ(edges[i].tile1);
          tiles[tile1.z][tile1.y][tile1.x].incX = numeric_limits<IdType>::max();
          tiles[tile1.z][tile1.y][tile1.x+1].decX = numeric_limits<IdType>::max();
        }
        else if(edges[i].type == VERT)
        {
          Point tile1 = tileIdToXYZ(edges[i].tile1);
          tiles[tile1.z][tile1.y][tile1.x].incY = numeric_limits<IdType>::max();
          tiles[tile1.z][tile1.y+1][tile1.x].decX = numeric_limits<IdType>::max();
        }
      }
    }
  }
  else
  {
    tiles.clear(); tiles.swap(fulltiles);
    edges.clear(); edges.swap(fulledges);
    nonViaEdges = nonViaFullEdges;
  }

  infile.close();
  outfile.close();
}

void FGR::parseInputAsLab(HDPGrid& grid)
{
  ofstream outfile(params.resultsFile.c_str(), std::ios::app);
  xTiles = grid.Design().cfg.ValueOf("FGRRouting.nHorTiles", 0);
  yTiles = grid.Design().cfg.ValueOf("FGRRouting.nVertTiles", 0);

  if (xTiles * yTiles == 0)
  {
    xTiles = grid.NumRows() / 2;
    yTiles = grid.NumRows() / 2;
  }

  numLayers = 1;
  params.layerAssign = false;

  outfile << "grid size " << xTiles << "x" << yTiles << endl;

  unsigned vcap, hcap;

  if (grid.Design().cfg.ValueOf("FGRRouting.calculateCapacity", false))
  {
    double rowHeight          = grid.SiteHeight();
    double Pitch              = 0;
    unsigned powerTracsPerRow = grid.Design().cfg.ValueOf(
                                "FGRRouting.powerTracsPerRow", 2);

    for (HRoutingLayers::EnumeratorW layer =
         grid.Design().RoutingLayers.GetEnumeratorW(); layer.MoveNext(); )
    {
      if (layer.Name() == "Metal2" || layer.Name() == "metal2")
      {
        Pitch = layer.Pitch();
        break;
      }
    }

    if (Pitch == 0)
    {
      LOGCRITICAL("Pitch of layer2 was not found");
    }

    vcap = (unsigned)((1 - powerTracsPerRow * Pitch / rowHeight) * 2 * rowHeight / Pitch);
    hcap = vcap;
  }

  else
  {
    vcap = grid.Design().cfg.ValueOf("FGRRouting.verticalCapacity", 20);
    hcap = grid.Design().cfg.ValueOf("FGRRouting.horizontalCapacity", 20);
  }

  vertCaps.push_back(vcap);
  horizCaps.push_back(hcap);
  outfile << "vertical capacity   " << vcap << "\nhorizontal capacity " <<hcap<< endl;

  minWidths.push_back(0);
  minSpacings.push_back(0);
  viaSpacings.push_back(0);

  minX = 0;
  minY = 0;
  tileWidth = 1;
  tileHeight = 1;
  halfWidth = 0;
  halfHeight = 0;

  //double xMin    = grid.ColumnX(grid.NumCols());
  //double yMin    = grid.RowY(grid.NumRows());
	double xMin    = grid.ColumnX(grid.NumCols()-1) + grid.SiteWidth();
	double yMin    = grid.RowY(grid.NumRows()-1) + grid.SiteHeight();
	
	double xMax    = grid.ColumnX(0);
  double yMax    = grid.RowY(0);
  unsigned nNets = 0;

  for (HNets::ActiveNetsEnumeratorW curNet = grid.Design().Nets.GetActiveNetsEnumeratorW(); curNet.MoveNext();)
  {
    for (HNetWrapper::PinsEnumeratorW currPin = curNet.GetPinsEnumeratorW(); currPin.MoveNext();)
    {
      if (xMin > currPin.X())
        xMin = currPin.X();

      if (xMax < currPin.X())
        xMax = currPin.X();

      if (yMin > currPin.Y())
        yMin = currPin.Y();

      if (yMax < currPin.Y())
        yMax = currPin.Y();
    }
    nNets++;
  }

  xMax += grid.SiteWidth();  //in order to place pins with xNax (or yMax)
  yMax += grid.SiteHeight(); //to the last tiles
  outfile << "total nets " << nNets << endl;

  unsigned netsTooBig = 0;
  nets.reserve(nNets);
  int netId = 0;
  double c_tileWidth  = (xMax - xMin) / xTiles;
  double c_tileHeight = (yMax - yMin) / yTiles;

  //for(unsigned i = 0; i < nNets; ++i)
  for (HNets::ActiveNetsEnumeratorW nIter = grid.Design().Nets.GetActiveNetsEnumeratorW(); nIter.MoveNext(); )
  {
    Net temp;
    unsigned numPins;
    string name;
    IdType id;

    name = nIter.Name().c_str();
    //name = name + "_" + netId;
    id = netId;
    netId++;
    numPins = nIter.PinsCount();
    temp.minWidth = 1;

    //for(unsigned j = 0; j < numPins; ++j)
    for (HNetWrapper::PinsEnumeratorW currPin = nIter.GetPinsEnumeratorW(); currPin.MoveNext();)
    {
      // translate absolute coords to grid coords
      double pinX = (currPin.X() - xMin) / c_tileWidth;
      double pinY = (currPin.Y() - yMin) / c_tileHeight;

      Point newPin;
      newPin.x = static_cast<unsigned>(floor((pinX - minX) / tileWidth));
      newPin.y = static_cast<unsigned>(floor((pinY - minY) / tileHeight));
      newPin.z = 0;
      temp.pins.push_back(newPin);
    }

    sort(temp.pins.begin(), temp.pins.end());
    temp.pins.erase(unique(temp.pins.begin(), temp.pins.end()), temp.pins.end());

    if(numPins <= 1000 && temp.pins.size() > 1)
    {
      ++routableNets;

      // build the MST tree
      vector<pair<Point,Point> > mstTree;
      vector<unsigned> distanceToTree(temp.pins.size(), UINT_MAX);
      vector<bool> inTree(temp.pins.size(), false);
      vector<unsigned> parent(temp.pins.size(), UINT_MAX);

      unsigned currentPin = 0, nextPin = UINT_MAX;
      for(unsigned j = 0; j < temp.pins.size()-1; ++j)
      {
        inTree[currentPin] = true;
        unsigned bestDistance = UINT_MAX;
        for(unsigned k = 0; k < temp.pins.size(); ++k)
        {
          if(inTree[k]) continue;

          unsigned thisDistance = max(temp.pins[k].x, temp.pins[currentPin].x)
            - min(temp.pins[k].x, temp.pins[currentPin].x)
            + max(temp.pins[k].y, temp.pins[currentPin].y)
            - min(temp.pins[k].y, temp.pins[currentPin].y);
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
        mstTree.push_back(make_pair(temp.pins[nextPin], temp.pins[parent[nextPin]]));
        currentPin = nextPin;
      }

      // add in the MST tree
      for(unsigned j = 0; j < mstTree.size(); ++j)
      {
        SubNet stemp;
        stemp.one = mstTree[j].first;
        stemp.two = mstTree[j].second;
        temp.subnets.push_back(stemp);
      }

      netNameToIdx[name] = nets.size();
      nets.push_back(temp);
      netNames.push_back(name);
      netIds.push_back(id);
    }
    else
    {
      if(numPins > 1000)
      {
        ++netsTooBig;
        netNameToIdx[name] = numeric_limits<IdType>::max();
      }
      else
      {
        netNameToIdx[name] = nets.size();
        nets.push_back(temp);
        netNames.push_back(name);
        netIds.push_back(id);
      }
    }
  }

  outfile << "number of nets too big: " << netsTooBig << endl;

  // build the routing grid graph
  bool skipEmpty = true;
  buildGrid(skipEmpty);

  tiles.clear(); tiles.swap(fulltiles);
  edges.clear(); edges.swap(fulledges);
  nonViaEdges = nonViaFullEdges;
  outfile.close();
}

void FGR::parseInputLab(const char *filename)
{
  ifstream infile(filename);
  ofstream outfile(params.resultsFile.c_str(), std::ios::app);

  if(!infile.good())
  {
    outfile << "Could not open `" << filename << "' for reading" << endl;
    exit(0);
  }
  else
  {
    outfile << "Reading from `" << filename << "' ..." << endl;
  }

  string tmp;

  infile >> tmp; expect(tmp, string("grid"));

  infile >> xTiles >> yTiles;
  numLayers = 1;
  params.layerAssign = false;

  outfile << "grid size " << xTiles << "x" << yTiles << endl;

  infile >> tmp; expect(tmp, string("vertical"));
  infile >> tmp; expect(tmp, string("capacity"));

  for(unsigned i = 0; i < numLayers; ++i)
  {
    unsigned cap;
    infile >> cap;
    vertCaps.push_back(cap);
  }

  infile >> tmp; expect(tmp, string("horizontal"));
  infile >> tmp; expect(tmp, string("capacity"));

  for(unsigned i = 0; i < numLayers; ++i)
  {
    unsigned cap;
    infile >> cap;
    horizCaps.push_back(cap);
  }

  minWidths.push_back(0);
  minSpacings.push_back(0);
  viaSpacings.push_back(0);

  minX = 0;
  minY = 0;
  tileWidth = 1;
  tileHeight = 1;
  halfWidth = 0;
  halfHeight = 0;

  infile >> tmp; expect(tmp, string("num"));
  infile >> tmp; expect(tmp, string("net"));

  unsigned numNets;

  infile >> numNets;

  outfile << "total nets " << numNets << endl;

  unsigned netsTooBig = 0;
  nets.reserve(numNets);

  for(unsigned i = 0; i < numNets; ++i)
  {
    Net temp;
    unsigned numPins;
    string name;
    IdType id;

    infile >> name >> id >> numPins;
    temp.minWidth = 1;

    for(unsigned j = 0; j < numPins; ++j)
    {
      // translate absolute coords to grid coords
      double pinX, pinY;

      infile >> pinX >> pinY;

      Point newPin;
      newPin.x = static_cast<unsigned>(floor((pinX - minX)/tileWidth));
      newPin.y = static_cast<unsigned>(floor((pinY - minY)/tileHeight));
      newPin.z = 0;
      temp.pins.push_back(newPin);
    }

    sort(temp.pins.begin(), temp.pins.end());
    temp.pins.erase(unique(temp.pins.begin(), temp.pins.end()), temp.pins.end());

    if(numPins <= 1000 && temp.pins.size() > 1)
    {
      ++routableNets;

      // build the MST tree
      vector<pair<Point,Point> > mstTree;
      vector<unsigned> distanceToTree(temp.pins.size(), UINT_MAX);
      vector<bool> inTree(temp.pins.size(), false);
      vector<unsigned> parent(temp.pins.size(), UINT_MAX);

      unsigned currentPin = 0, nextPin = UINT_MAX;
      for(unsigned j = 0; j < temp.pins.size()-1; ++j)
      {
        inTree[currentPin] = true;
        unsigned bestDistance = UINT_MAX;
        for(unsigned k = 0; k < temp.pins.size(); ++k)
        {
          if(inTree[k]) continue;

          unsigned thisDistance = max(temp.pins[k].x, temp.pins[currentPin].x)
                                - min(temp.pins[k].x, temp.pins[currentPin].x)
                                + max(temp.pins[k].y, temp.pins[currentPin].y)
                                - min(temp.pins[k].y, temp.pins[currentPin].y);
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
        mstTree.push_back(make_pair(temp.pins[nextPin], temp.pins[parent[nextPin]]));
        currentPin = nextPin;
      }

      // add in the MST tree
      for(unsigned j = 0; j < mstTree.size(); ++j)
      {
        SubNet stemp;
        stemp.one = mstTree[j].first;
        stemp.two = mstTree[j].second;
        temp.subnets.push_back(stemp);
      }

      netNameToIdx[name] = nets.size();
      nets.push_back(temp);
      netNames.push_back(name);
      netIds.push_back(id);
    }
    else
    {
      if(numPins > 1000)
      {
        ++netsTooBig;
        netNameToIdx[name] = numeric_limits<IdType>::max();
      }
      else
      {
        netNameToIdx[name] = nets.size();
        nets.push_back(temp);
        netNames.push_back(name);
        netIds.push_back(id);
      }
    }
  }

  outfile << "number of nets too big: " << netsTooBig << endl;

  // build the routing grid graph
  bool skipEmpty = true;
  buildGrid(skipEmpty);

  tiles.clear(); tiles.swap(fulltiles);
  edges.clear(); edges.swap(fulledges);
  nonViaEdges = nonViaFullEdges;

  infile.close();
  outfile.close();
}

void FGR::parseInputMapper(const char *filename)
{
  ifstream infile(filename);
  ofstream outfile(params.resultsFile.c_str(), std::ios::app);

  if(!infile.good())
  {
    outfile << "Could not open `" << filename << "' for reading" << endl;
    exit(0);
  }
  else
  {
    outfile << "Reading from `" << filename << "' ..." << endl;
  }

  string tmp;

  infile >> tmp; expect(tmp, string("grid"));

  infile >> xTiles >> yTiles >> numLayers;

  if(numLayers <= 2) params.layerAssign = false;

  outfile << "grid size " << xTiles << "x" << yTiles << endl;

  infile >> tmp; expect(tmp, string("vertical"));
  infile >> tmp; expect(tmp, string("capacity"));

  for(unsigned i = 0; i < numLayers; ++i)
  {
    unsigned cap;
    infile >> cap;
    vertCaps.push_back(cap);
  }

  infile >> tmp; expect(tmp, string("horizontal"));
  infile >> tmp; expect(tmp, string("capacity"));

  for(unsigned i = 0; i < numLayers; ++i)
  {
    unsigned cap;
    infile >> cap;
    horizCaps.push_back(cap);
  }

  infile >> tmp; expect(tmp, string("minimum"));
  infile >> tmp; expect(tmp, string("width"));

  for(unsigned i = 0; i < numLayers; ++i)
  {
    unsigned minwidth;
    infile >> minwidth;
    minWidths.push_back(minwidth);
  }

  infile >> tmp; expect(tmp, string("minimum"));
  infile >> tmp; expect(tmp, string("spacing"));

  for(unsigned i = 0; i < numLayers; ++i)
  {
    unsigned minspacing;
    infile >> minspacing;
    minSpacings.push_back(minspacing);
  }

  infile >> tmp; expect(tmp, string("via"));
  infile >> tmp; expect(tmp, string("spacing"));

  for(unsigned i = 0; i < numLayers; ++i)
  {
    unsigned viaspacing;
    infile >> viaspacing;
    viaSpacings.push_back(viaspacing);
  }

  infile >> minX >> minY >> tileWidth >> tileHeight;

  ///halfWidth = static_cast<LenType>(trunc(0.5*tileWidth));
  halfWidth = static_cast<LenType>(0.5*tileWidth);
  ///halfHeight = static_cast<LenType>(trunc(0.5*tileHeight));
  halfHeight = static_cast<LenType>(0.5*tileHeight);

  infile >> tmp; expect(tmp, string("num"));
  infile >> tmp; expect(tmp, string("net"));

  unsigned numNets;

  infile >> numNets;

  outfile << "total nets " << numNets << endl;

  nets.reserve(numNets);

  for(unsigned i = 0; i < numNets; ++i)
  {
    Net temp;
    unsigned numPins;
    string name;
    IdType id;

    infile >> name >> id >> numPins >> temp.minWidth;

    for(unsigned j = 0; j < numPins; ++j)
    {
      // translate absolute coords to grid coords
      double pinX, pinY;
      unsigned layer;

      infile >> pinX >> pinY >> layer;

      Point newPin;
      newPin.x = static_cast<unsigned>(floor((pinX - minX)/tileWidth));
      newPin.y = static_cast<unsigned>(floor((pinY - minY)/tileHeight));
      newPin.z = layer-1;
      temp.pins.push_back(newPin);
    }

    sort(temp.pins.begin(), temp.pins.end());
    temp.pins.erase(unique(temp.pins.begin(), temp.pins.end()), temp.pins.end());

    if(numPins <= 1000 && temp.pins.size() > 1)
    {
      ++routableNets;

      SubNet sTemp;
      temp.subnets.push_back(sTemp);

      netNameToIdx[name] = nets.size();
      nets.push_back(temp);
      netNames.push_back(name);
      netIds.push_back(id);
    }
    else
    {
      netNameToIdx[name] = numeric_limits<IdType>::max();
    }
  }

  // build the routing grid graph
  bool skipEmpty = false;
  buildGrid(skipEmpty);

  unsigned capacityChanges;
  infile >> capacityChanges;

  for(unsigned i = 0; i < capacityChanges; ++i)
  {
    unsigned gridCol1, gridRow1, layer1, gridCol2, gridRow2, layer2, newCap;

    infile >> gridCol1 >> gridRow1 >> layer1
           >> gridCol2 >> gridRow2 >> layer2
           >> newCap;

    assert(layer1 == layer2);
    if(gridCol1 == gridCol2)
    {
      // vert segment
      assert(gridRow1 == gridRow2+1 || gridRow1+1 == gridRow2);
      fulledges[fulltiles[layer1-1][min(gridRow1,gridRow2)][gridCol1].incY].capacity = newCap;
    }
    else if(gridRow1 == gridRow2)
    {
      // horiz segment
      assert(gridCol1 == gridCol2+1 || gridCol1+1 == gridCol2);
      fulledges[fulltiles[layer1-1][gridRow1][min(gridCol1,gridCol2)].incX].capacity = newCap;
    }
    else
    {
      assert(0);
    }
  }

  tiles.clear(); tiles.swap(fulltiles);
  edges.clear(); edges.swap(fulledges);
  nonViaEdges = nonViaFullEdges;

  infile.close();
  outfile.close();
}

void FGR::parseSolution(const char* filename)
{
  ifstream infile(filename);
  ofstream outfile(params.resultsFile.c_str(), std::ios::app);

  if(!infile.good())
  {
    outfile << "Could not open `" << filename << "' for reading" << endl;
    exit(0);
  }
  else
  {
    outfile << "Reading from `" << filename << "' ..." << endl;
  }

  string netname;

  infile >> netname;

  while(infile.good())
  {
    if(netNameToIdx.find(netname) == netNameToIdx.end())
    {
      outfile << "unknown net with name `" << netname << "'" << endl;
      exit(0);
    }

    unsigned netId = netNameToIdx[netname];

    char junk;
    string restofline;
    getline(infile, restofline);

    while(infile.peek() == '(')
    {
      unsigned x1, y1, z1, x2, y2, z2;
      infile >> junk >> x1 >> junk >> y1 >> junk >> z1
             >> junk >> junk >> junk
             >> x2 >> junk >> y2 >> junk >> z2;
      getline(infile, restofline);

      unsigned tilex1, tiley1, tilez1, tilex2, tiley2, tilez2;

      tilex1 = static_cast<unsigned>(floor((double)(x1 - minX)/tileWidth));
      tilex2 = static_cast<unsigned>(floor((double)(x2 - minX)/tileWidth));
      tiley1 = static_cast<unsigned>(floor((double)(y1 - minY)/tileHeight));
      tiley2 = static_cast<unsigned>(floor((double)(y2 - minY)/tileHeight));
      tilez1 = z1 - 1;
      tilez2 = z2 - 1;

      if(tilex1 >= xTiles)
      {
        outfile << "illegal x position " << x1 << endl;
        exit(0);
      }
      if(tiley1 >= yTiles)
      {
        outfile << "illegal y position " << y1 << endl;
        exit(0);
      }
      if(tilez1 >= numLayers)
      {
        outfile << "illegal layer " << z1 << endl;
        exit(0);
      }

      if(tilex2 >= xTiles)
      {
        outfile << "illegal x position " << x2 << endl;
        exit(0);
      }
      if(tiley2 >= yTiles)
      {
        outfile << "illegal y position " << y2 << endl;
        exit(0);
      }
      if(tilez2 >= numLayers)
      {
        outfile << "illegal layer " << z2 << endl;
        exit(0);
      }

      x1 = tilex1;
      y1 = tiley1;
      z1 = tilez1;
      x2 = tilex2;
      y2 = tiley2;
      z2 = tilez2;

      if(x1 != x2)
      {
        for(unsigned j = min(x1,x2); j < max(x1,x2); ++j)
        {
          IdType edgeId = tiles[z1][y1][j].incX;
          assert(edgeId != numeric_limits<IdType>::max());
          addSegment(make_pair(netId,0), edgeId);
        }
      }
      else if(y1 != y2)
      {
        for(unsigned j = min(y1,y2); j < max(y1,y2); ++j)
        {
          IdType edgeId = tiles[z1][j][x1].incY;
          assert(edgeId != numeric_limits<IdType>::max());
          addSegment(make_pair(netId,0), edgeId);
        }
      }
      else
      {
        for(unsigned j = min(z1,z2); j < max(z1,z2); ++j)
        {
          IdType edgeId = tiles[j][y1][x1].incZ;
          assert(edgeId != numeric_limits<IdType>::max());
          addSegment(make_pair(netId,0), edgeId);
        }
      }
    }

    getline(infile, restofline); // the trailing !

    // read in the next one
    infile >> netname;
  }
  outfile.close();
}

void FGR::writeRoutes(const string &filename)
{
  ofstream outfile(filename.c_str());

  if(!outfile.good())
  {
    outfile << "Could not open `" << filename << "' for writing." << endl;
    return;
  }

  outfile << "Writing `" << filename << "' ..." << flush;

  for(unsigned i = 0; i < nets.size(); ++i)
  {
    bool netRouted = true;
    for(unsigned j = 0; j < nets[i].subnets.size(); ++j)
    {
      if(!nets[i].subnets[j].routed)
      {
        netRouted = false;
        break;
      }
    }

    if(params.labyrinth && nets[i].pins.size() == 1)
    {
      outfile << netNames[i] << " " << netIds[i] << " 1" << endl << "("
              << minX + tileWidth*static_cast<double>(nets[i].pins[0].x) + halfWidth << ","
              << minY + tileHeight*static_cast<double>(nets[i].pins[0].y) + halfHeight << ")-("
              << minX + tileWidth*static_cast<double>(nets[i].pins[0].x) + halfWidth << ","
              << minY + tileHeight*static_cast<double>(nets[i].pins[0].y) + halfHeight << ")"
              << endl << "!" << endl;
    }
    else if(netRouted)
    {
      vector<unsigned> usedEdges;
      usedEdges.reserve(nets[i].hashedSegments.size());
      for(SegmentHash::iterator it = nets[i].hashedSegments.begin(); it != nets[i].hashedSegments.end(); ++it)
      {
        usedEdges.push_back(it->first);
      }

      sort(usedEdges.begin(), usedEdges.end());

      vector<pair<Point, Point> > segments;

      // add routed segments
      assert(usedEdges.size() > 0);
      Point startPt = tileIdToXYZ(edges[usedEdges[0]].tile1),
            endPt = tileIdToXYZ(edges[usedEdges[0]].tile2);
      for(unsigned j = 1; j < usedEdges.size(); ++j)
      {
        unsigned prevEdge = usedEdges[j-1];
        unsigned currEdge = usedEdges[j];

        if(edges[prevEdge].type != edges[currEdge].type ||
           edges[prevEdge].tile2 != edges[currEdge].tile1)
        {
          segments.push_back(make_pair(startPt, endPt));
          startPt = tileIdToXYZ(edges[currEdge].tile1);
          endPt   = tileIdToXYZ(edges[currEdge].tile2);
        }
        else
        {
          endPt = tileIdToXYZ(edges[currEdge].tile2);
        }
      }
      segments.push_back(make_pair(startPt, endPt));

      outfile << netNames[i] << " " << netIds[i] << " " << segments.size() << endl;
      for(unsigned j = 0; j < segments.size(); ++j)
      {
        outfile << "(" << minX + tileWidth*static_cast<double>(segments[j].first.x) + halfWidth << ","
                       << minY + tileHeight*static_cast<double>(segments[j].first.y) + halfHeight;
        if(!params.labyrinth) outfile << "," << segments[j].first.z+1;
        outfile << ")-(" << minX + tileWidth*static_cast<double>(segments[j].second.x) + halfWidth << ","
                         << minY + tileHeight*static_cast<double>(segments[j].second.y) + halfHeight;
        if(!params.labyrinth) outfile << "," << segments[j].second.z+1;
        outfile << ")" << endl;
      }
      outfile << "!" << endl;
    }
  }

  outfile << " done" << endl;
  outfile.close();
}

const char* coolMap[] = {
/* columns rows colors chars-per-pixel */
"a c #ffffff", "b c #0000af", "c c #0000bf", "d c #0000cf", "e c #0000ef", 
"f c #0000ff", "g c #002bff", "h c #004bff", "i c #006bff", "j c #007bff", 
"k c #008bff", "l c #009bff", "m c #00abff", "n c #00bbff", "o c #00dbff", 
"p c #00ebff", "q c #00fbff", "r c #00fcff", "s c #00fffa", "t c #00fff5",
"u c #00fff1", "v c #00fff3", "w c #00ffe3", "x c #00ffd3", "y c #00ffc3", 
"z c #00ffb3", "A c #00ffa3", "B c #00ff93", "C c #00ff73", "D c #00ff63", 
"E c #00ff43", "F c #00ff2e", "G c #00ff13", "H c #00ff0c", "I c #0dff00", 
"J c #3aff00", "K c #4dff00", "L c #7dff00", "M c #adff00", "N c #bdff00",
"O c #cdff00", "P c #ddff00", "Q c #edff00", "R c #feff00", "S c #fdff00", 
"T c #fff100", "U c #fff200", "V c #ffe100", "W c #ffd100", "X c #ffcd00",
"Y c #ffb600", "Z c #ffb100", "0 c #ffa100", "1 c #ff8100", "2 c #ff7100",
"3 c #ff001e", "4 c #ff002e", "5 c #df0000", "6 c #cf0000", "7 c #bf0000",
"8 c #af0000", "9 c #9f0000", ", c #8f0000", ". c #7f0000"	
};

const char* Colors[] = {
"a", "b", "c", "d", "e", "f", "g", "h",
"i", "j", "k", "l", "m", "n", "o", "p",
"q", "r", "s", "t", "u", "v", "w", "x",
"y", "z", "A", "B", "C", "D", "E", "F",
"G", "H", "I", "J", "K", "L", "M", "N",
"O", "P", "Q", "R", "S", "T", "U", "V",
"W", "X", "Y", "Z", "0", "1", "2", "3",
"4", "5", "6", "7", "8", "9", ",", "."};

void FGR::plotXPM(const string &filename, unsigned nPix)
{
  const unsigned numColors   = 64;
  const unsigned nGoodColors = 55; //colors used to draw not overflowed tiles
  unsigned multiplier = (unsigned)floor((double)nPix / xTiles); //num of pixels in tile's edge 

  string fullfilename = filename + string(".xpm");

  ofstream xpmFile(fullfilename.c_str());

  xpmFile<<"/* XPM */"<<endl;
  xpmFile<<"static char *congestion[] = {"<<endl;
  xpmFile<<"/* columns rows colors chars-per-pixel */"<<endl;
  xpmFile<<"\""<<xTiles * multiplier<<" "<<yTiles * multiplier<<" "
         <<numColors<<" 1\","<<endl;

  for(unsigned i = 0; i < numColors; ++i)
    xpmFile<<"\""<<coolMap[i]<<"\","<<endl;

  xpmFile<<"/* pixels */"<<endl;

  vector< vector< double > > image;
  double maxRatio = 1.;

  //calculate usage of all edges
  for(unsigned rj = 1; rj <= yTiles; ++rj)
  {
    unsigned j = yTiles - rj;
    vector<double> horizLine;

    for(unsigned i = 0; i < xTiles; ++i)
    {
      double x1Usage = 0., x1Cap = 0., y1Usage = 0., y1Cap = 0.;
      double x2Usage = 0., x2Cap = 0., y2Usage = 0., y2Cap = 0.;

      for(unsigned k = 0; k < tiles.size(); ++k)
      {
        unsigned x1EdgeId = tiles[k][j][i].incX; //right edge of this tile
        unsigned y1EdgeId = tiles[k][j][i].incY; //top edge
        unsigned x2EdgeId = tiles[k][j][i].decX; //left edge
        unsigned y2EdgeId = tiles[k][j][i].decY; //down edge

        if(x1EdgeId != UINT_MAX)
        {
          x1Usage += edges[x1EdgeId].usage; //num of wires croosing this edge
          x1Cap += edges[x1EdgeId].capacity;
        }
        if(y1EdgeId != UINT_MAX)
        {
          y1Usage += edges[y1EdgeId].usage;
          y1Cap += edges[y1EdgeId].capacity;
        }
        if(x2EdgeId != UINT_MAX)
        {
          x2Usage += edges[x2EdgeId].usage;
          x2Cap += edges[x2EdgeId].capacity;
        }
        if(y2EdgeId != UINT_MAX)
        {
          y2Usage += edges[y2EdgeId].usage;
          y2Cap += edges[y2EdgeId].capacity;
        }
      } //for(unsigned k = 0; k < tiles.size(); ++k)

      double x1Ratio = 0, y1Ratio = 0, x2Ratio = 0, y2Ratio = 0;
      
      if (x1Cap > 0)
        x1Ratio = static_cast<double>(x1Usage)/static_cast<double>(x1Cap);
        
      if (y1Cap > 0)
        y1Ratio = static_cast<double>(y1Usage)/static_cast<double>(y1Cap);
        
      if (x2Cap > 0)  
        x2Ratio = static_cast<double>(x2Usage)/static_cast<double>(x2Cap);
        
      if (y2Cap > 0)
        y2Ratio = static_cast<double>(y2Usage)/static_cast<double>(y2Cap);

      double totalRatio = max(max(x1Ratio, y1Ratio), max(x2Ratio, y2Ratio));
      maxRatio = max(maxRatio, totalRatio);

      for (unsigned i1 = 0; i1 < multiplier; i1++)
        horizLine.push_back(totalRatio);
    } //for(unsigned i = 0; i < xTiles; ++i)

    for (unsigned i1 = 0; i1 < multiplier; i1++)
      image.push_back(horizLine);
  } //for(unsigned rj = 1; rj <= yTiles; ++rj)

  //draw tiles
  for(unsigned j = 0; j < yTiles * multiplier; ++j)
  {
    xpmFile<<"\"";

    for(unsigned i = 0; i < xTiles * multiplier; ++i)
    {
      unsigned index = 0;

      if (image[j][i] <= 1) //tile was not overflowed
        index = static_cast<unsigned>(floor((image[j][i]) //use
                * static_cast<double>(nGoodColors-1)));   //"good" colors

      else //tile was overflowed
      	index = static_cast<unsigned>(floor((image[j][i]/maxRatio) //use "bad" colors
                * static_cast<double>(numColors - nGoodColors - 1))) + nGoodColors;

      xpmFile<<Colors[index];
    }

    if(j == yTiles * multiplier - 1) //last line in the file
    {
      xpmFile<<"\""<<endl;
    }
    else
    {
      xpmFile<<"\","<<endl;
    }
  }

  xpmFile<<");"<<endl;
  xpmFile.close();
}

void FGR::plotEdgesXPM(const string &filename, unsigned nPix)
{
  const unsigned numColors   = 64;
  const unsigned nGoodColors = 55;
  unsigned multiplier = (unsigned)floor((double)nPix / xTiles); 

  string fullfilename = filename + string("edges.xpm");

  ofstream xpmFile(fullfilename.c_str());

  xpmFile<<"/* XPM */"<<endl;
  xpmFile<<"static char *congestion[] = {"<<endl;
  xpmFile<<"/* columns rows colors chars-per-pixel */"<<endl;
  xpmFile<<"\""<<xTiles * multiplier<<" "<<yTiles * multiplier<<" "
         <<numColors<<" 1\","<<endl;

  for(unsigned i=0; i<numColors; ++i)
    xpmFile<<"\""<<coolMap[i]<<"\","<<endl;

  xpmFile<<"/* pixels */"<<endl;

  vector< vector< double > > image;
  double maxRatio = 1.;

  //calculate usage of edges
  for(unsigned rj = 1; rj <= yTiles; ++rj)
  {
    unsigned j = yTiles - rj;
    vector<double> horizLine;
    vector<double> vertEdgeLine;

    for(unsigned i = 0; i < xTiles; ++i)
    {
      double x1Usage = 0., x1Cap = 0., y1Usage = 0., y1Cap = 0.;
      for(unsigned k = 0; k < tiles.size(); ++k)
      {
        unsigned x1EdgeId = tiles[k][j][i].incX; //right edge of this tile
        unsigned y1EdgeId = tiles[k][j][i].decY; //down edge

        if(x1EdgeId != UINT_MAX)
        {
          x1Usage += edges[x1EdgeId].usage;
          x1Cap += edges[x1EdgeId].capacity;
        }
        if(y1EdgeId != UINT_MAX)
        {
          y1Usage += edges[y1EdgeId].usage;
          y1Cap += edges[y1EdgeId].capacity;
        }
      }

      double x1Ratio = 0, y1Ratio = 0;
      
      if (x1Cap > 0)
        x1Ratio = static_cast<double>(x1Usage)/static_cast<double>(x1Cap);
        
      if (y1Cap > 0)
        y1Ratio = static_cast<double>(y1Usage)/static_cast<double>(y1Cap);

      double totalRatio = y1Ratio;
      maxRatio = max(maxRatio, max(totalRatio, x1Ratio));

      for (unsigned i1 = 0; i1 < multiplier; i1++)  //put down edge of a tile
        horizLine.push_back(totalRatio);
      
      //----remember right edge--------------------
      unsigned j1 = 0;

      while (j1 < multiplier / 4)
      {
        vertEdgeLine.push_back(x1Ratio); //add pixels for right edge;
        j1++;
      }

      while (j1 < multiplier)
      {
        vertEdgeLine.push_back(0);       //add pixels for space
        j1++;
      }
      //--------------------------------------
    } //for(unsigned i = 0; i < xTiles; ++i)

    //---add right edge-----------------------
    unsigned j2 = 0;

    while (j2 < multiplier / 4)
    {
      image.push_back(horizLine);
      j2++;
    }

    while (j2 < multiplier)
    {
      image.push_back(vertEdgeLine);
      j2++;
    }
    //----------------------------------------
  } //for(unsigned rj = 1; rj <= yTiles; ++rj)
  
  //draw edges
  for(unsigned j = 0; j < yTiles * multiplier; ++j)
  {
    xpmFile<<"\"";

    for(unsigned i = 0; i < xTiles * multiplier; ++i)
    {
      unsigned index = 0;

      if (image[j][i] <= 1) //edge is not overflowed
        index = static_cast<unsigned>(floor((image[j][i]) //use "good"
                * static_cast<double>(nGoodColors-1)));   //colors

      else //edge is overflowed
      	index = static_cast<unsigned>(floor((image[j][i]/maxRatio) //use "bad" colors
                * static_cast<double>(numColors - nGoodColors - 1))) + nGoodColors;

      xpmFile<<Colors[index];
    }
    if(j == yTiles * multiplier - 1)
    {
      xpmFile<<"\""<<endl;
    }
    else
    {
      xpmFile<<"\","<<endl;
    }
  }
  xpmFile<<");"<<endl;
  xpmFile.close();
}

void FGR::plotColorsXPM(const string &filename, unsigned nVertPix)
{
  const unsigned numColors   = 64;
  const unsigned nGoodColors = 55;
  const unsigned nHorPix     = 1000;
  unsigned multiplier = (unsigned) floor((double)nVertPix / numColors);

  string fullfilename = filename + string(".xpm");

  ofstream xpmFile(fullfilename.c_str());

  xpmFile<<"/* XPM */"<<endl;
  xpmFile<<"static char *congestion[] = {"<<endl;
  xpmFile<<"/* columns rows colors chars-per-pixel */"<<endl;
  xpmFile<<"\""<<nHorPix<<" "<<numColors * multiplier<<" "
         <<numColors<<" 1\","<<endl;

  for(unsigned i=0; i<numColors; ++i)
    xpmFile<<"\""<<coolMap[i]<<"\","<<endl;

  xpmFile<<"/* pixels */"<<endl;

  vector< vector< double > > image;

  for(unsigned rj = 0; rj < numColors; ++rj) //for all colors
  {
    vector<double> horizLine;
    vector<double> whiteLine;

    for (unsigned i1 = 0; i1 < 200; i1++)
    {
        horizLine.push_back(rj);         //add index of a current color
        whiteLine.push_back(0);
    }
    
    for (unsigned i1 = 200; i1 < nHorPix; i1++)
    {
        horizLine.push_back(0);         //add space
        whiteLine.push_back(0);
    }

    unsigned i2 = 0;
    
    while (i2 < multiplier / 2)
    {
      image.push_back(horizLine);                  //add few lines with this color
      i2++;
    }
    
    while (i2 < multiplier)
    {
      image.push_back(whiteLine);                  //add white lines
      i2++;
    }
  }

  for(unsigned j = 0; j < numColors * multiplier; ++j)
  {
    xpmFile<<"\"";

    for(unsigned i = 0; i < nHorPix; ++i)
    {
      unsigned index = (unsigned) image[j][i];
      xpmFile<<Colors[index];
    }

    if(j == numColors * multiplier - 1)
    {
      xpmFile<<"\""<<endl;
    }
    else
    {
      xpmFile<<"\","<<endl;
    }
  }
  xpmFile<<");"<<endl;
  xpmFile.close();
}