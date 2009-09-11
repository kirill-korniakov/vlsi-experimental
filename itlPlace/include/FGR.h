#ifndef _FGR_H_
#define _FGR_H_
#include <limits>
#include <time.h>
#include "stdTypes.h"
#include "HDPGrid.h"

//using namespace std;
using std::pair;
using std::vector;
using std::map;

typedef unsigned short CoordType;
typedef unsigned IdType;
typedef pair<unsigned, unsigned> SubNetIdType;
typedef unsigned LenType;
typedef unsigned short CapType;
typedef double CostType;

namespace fgr
{
  class Point
  {
  public:
    CoordType x, y, z;

    Point(): x(0), y(0), z(0) {}
    Point(CoordType X, CoordType Y, CoordType Z): x(X), y(Y), z(Z) {}
    Point(const Point &orig): x(orig.x), y(orig.y), z(orig.z) {}

    const Point &operator=(const Point &assign) { x = assign.x; y = assign.y; z = assign.z; return *this; }
  };

  inline bool operator==(const Point &a, const Point &b)
  {
    return a.x == b.x && a.y == b.y && a.z == b.z;
  }

  inline bool operator!=(const Point &a, const Point &b)
  {
    return a.x != b.x || a.y != b.y || a.z != b.z;
  }

  inline bool operator<(const Point &a, const Point &b)
  {
    return a.x < b.x || (a.x == b.x && a.y < b.y) ||
      (a.x == b.x && a.y == b.y && a.z < b.z);
  }

  template <typename T> class MemEfficientMultiSet
  {
  private:
    typedef pair<T, size_t> elementType;
    vector<elementType> theData;

  public:

    MemEfficientMultiSet(): theData(0) {}
    MemEfficientMultiSet(const MemEfficientMultiSet &orig): theData(orig.theData) {}  

    void clear(void)
    {
      theData.clear();
    }

    void insert(const T &data)
    {
      typename vector<elementType>::iterator pos = lower_bound(theData.begin(), theData.end(), make_pair(data, static_cast<size_t>(0)));
      if(pos != theData.end() && pos->first == data)
      {
        pos->second += 1;
      }
      else
      {
        theData.insert(pos, make_pair(data, 1));
      }
    }

    void remove(const T &data)
    {
      typename vector<elementType>::iterator pos = lower_bound(theData.begin(), theData.end(), make_pair(data, static_cast<size_t>(0)));
      assert(pos != theData.end() && pos->first == data);
      if(pos->second > 1)
      {
        pos->second -= 1;
      }
      else
      {
        theData.erase(pos);
      }
    }

    bool contains(const T &data) const
    {
      typename vector<elementType>::const_iterator pos = lower_bound(theData.begin(), theData.end(), make_pair(data, static_cast<size_t>(0)));
      return pos != theData.end() && pos->first == data;
    }

    size_t getCount(const T &data) const
    {
      typename vector<elementType>::const_iterator pos = lower_bound(theData.begin(), theData.end(), make_pair(data, static_cast<size_t>(0)));
      if(pos != theData.end() && pos->first == data)
      {
        return pos->second;
      }
      else
      {
        return static_cast<size_t>(0);
      }
    }

    size_t size(void) const
    {
      return theData.size();
    }

    typedef typename vector<elementType>::iterator iterator;

    iterator begin(void)
    {
      return theData.begin();
    }

    iterator end(void)
    {
      return theData.end();
    }
  };

  typedef MemEfficientMultiSet<IdType> SegmentHash;

  class SubNet
  {
  public:
    Point one, two;
    bool routed;
    vector<IdType> segments;

    SubNet(): routed(false) {}
    SubNet(const SubNet &orig): one(orig.one), two(orig.two), 
      routed(orig.routed), segments(orig.segments) {}
  };

  class Net
  {
  public:
    LenType numSegments, numVias;
    CapType minWidth;
    vector<Point> pins;
    vector<SubNet> subnets;
    SegmentHash hashedSegments;

    Net(): numSegments(0), numVias(0), minWidth(0) {}
    Net(const Net &orig): numSegments(orig.numSegments), numVias(orig.numVias),
      minWidth(orig.minWidth), pins(orig.pins),
      subnets(orig.subnets), hashedSegments(orig.hashedSegments) {}
  };

  enum EdgeType { HORIZ, VERT, VIA };

  class Edge
  {
  public:
    IdType tile1, tile2;
    CoordType layer;
    CapType capacity, usage;
    CostType historyCost;
    EdgeType type;
    vector<SubNetIdType> subnets;

    Edge(): tile1(std::numeric_limits<IdType>::max()), tile2(std::numeric_limits<IdType>::max()),
      layer(std::numeric_limits<CoordType>::max()), capacity(0), usage(0),
      historyCost(1), type(VIA) {}
    Edge(const Edge &orig): tile1(orig.tile1), tile2(orig.tile2), layer(orig.layer),
      capacity(orig.capacity), usage(orig.usage),
      historyCost(orig.historyCost),
      type(orig.type), subnets(orig.subnets) {}
  };

  class Tile
  {
  public:
    IdType incX, decX, incY, decY, incZ, decZ;

    Tile(): incX(std::numeric_limits<IdType>::max()), decX(std::numeric_limits<IdType>::max()),
      incY(std::numeric_limits<IdType>::max()), decY(std::numeric_limits<IdType>::max()),
      incZ(std::numeric_limits<IdType>::max()), decZ(std::numeric_limits<IdType>::max()) {}
    Tile(const Tile &orig): incX(orig.incX), decX(orig.decX), incY(orig.incY), decY(orig.decY),
      incZ(orig.incZ), decZ(orig.decZ) {}
  };

  class CompareByBox
  {
    const vector<Net> &nets;
  public:
    CompareByBox(const vector<Net> &n): nets(n) {}

    bool operator()(const SubNetIdType &a, const SubNetIdType &b) const;
  };

  class BitBoard
  {
  private:
    vector<IdType> setBits;
    vector<bool> bits;
  public:

    BitBoard(): setBits(0), bits(0) {}
    BitBoard(IdType size): setBits(0), bits(size, false) {}
    const vector<IdType> &getSetBits(void) { return setBits; }
    void setBit(IdType id);
    bool isBitSet(IdType id) const;
    void clear(void);
    void resize(IdType size);
  };

  class PQueue
  {
    class tileData
    {
    public:
      IdType heapLoc;
      CostType totalCost;  // actualCost + estimated cost to end from A*
      CostType actualCost;
      IdType parent;
    };

    vector<tileData> data;
    BitBoard dataValid;
    vector<IdType> heap;

  public:

    PQueue(): data(), dataValid(), heap() {};

    void resize(unsigned newSize) { data.resize(newSize); dataValid.resize(newSize); }
    bool empty() const { return heap.empty(); }
    void clear();

    IdType getBestTile(void) const;
    void removeBestTile(void);
    void updateTile(IdType tileId, CostType totalCost, CostType actualCost, IdType parent);
    const tileData& getData(IdType tileId) const;
    bool visited(IdType tileId) const;
  };

  class FGR;

  class CostFunction
  {
  public:
    virtual CostType operator()(IdType, IdType) const = 0;
    virtual CostType viaCost(void) const = 0;
  };

  class DLMCost : public CostFunction
  {
    const FGR &fgr;
  public:
    DLMCost(const FGR &_fgr): fgr(_fgr) {}
    CostType operator()(IdType, IdType) const;
    CostType viaCost(void) const;
  };

  class UnitCost : public CostFunction
  {
    const FGR &fgr;
  public:
    UnitCost(const FGR &_fgr): fgr(_fgr) {}
    CostType operator()(IdType, IdType) const;
    CostType viaCost(void) const;
  };

  class LBFunction
  {
  public:
    virtual double operator()(const Point &a, const Point &b) const = 0;
  };

  class LBManhattan : public LBFunction
  {
    const FGR &fgr;
  public:
    LBManhattan(const FGR &_fgr): fgr(_fgr) {}
    CostType operator()(const Point &a, const Point &b) const;
  };

  class ExactManhattan : public LBFunction
  {
    const FGR &fgr;
  public:
    ExactManhattan(const FGR &_fgr): fgr(_fgr) {}
    CostType operator()(const Point &a, const Point &b) const;
  };

  class FGRParams
  {
    friend class FGR;

  private:
    void setDefault(void);
    void print(void) const;

  public:

    bool layerAssign, labyrinth;
    unsigned maxRipIter, maxGreedyIter;
    double timeOut;
    string outputFile;
    string resultsFile;

    FGRParams(void) { setDefault(); }
    FGRParams(int argc, char **argv);

    static void usage(const char* exename);
  };

  class FGR
  {
    friend class DLMCost;
    friend class UnitCost;
    friend class LBManhattan;
    friend class ExactManhattan;

  private:

    // benchmark stats
    IdType xTiles, yTiles, numLayers, routableNets, nonViaEdges, nonViaFullEdges;
    LenType minX, minY, tileWidth, tileHeight, halfWidth, halfHeight;

    // routing stats
    unsigned totalOverflow, overfullEdges, totalSegments, totalVias;

    // data structures
    vector<CapType> vertCaps, horizCaps, minWidths, minSpacings, viaSpacings;
    vector<Net> nets;
    vector<string> netNames;
    vector<IdType> netIds;
    vector<vector<vector<Tile> > > tiles, fulltiles;
    vector<Edge> edges, fulledges;
    PQueue pQ;
    map<string, IdType> netNameToIdx;

    ExactManhattan em;
    LBManhattan lbm;

    FGRParams params;

    IdType xyzToTileId(CoordType x, CoordType y, CoordType z);
    Point tileIdToXYZ(IdType id);

  public:

    FGR(const FGRParams &_params = FGRParams()):
        xTiles(0), yTiles(0), numLayers(0), routableNets(0), 
          nonViaEdges(0), nonViaFullEdges(0), totalOverflow(0),
          overfullEdges(0), totalSegments(0), totalVias(0),
          em(*this), lbm(*this), params(_params) {}

        void parseInput(const char *filename);
        void parseInputAsLab(HDPGrid& grid);
        void parseInputLab(const char *filename);
        void parseInputMapper(const char *filename);
        void parseSolution(const char *filename);

        void writeRoutes(const string &filename);

        void buildGrid(bool skipEmpty);

        double congEstScoreSegment(IdType netId, const Point &a, const Point &b);

        void addSegment(const SubNetIdType &subNetId, IdType edgeId);
        void ripUpSegment(const SubNetIdType &subNetId, IdType edgeId);
        void ripUpSubNet(const SubNetIdType &subNetId);
        void redoTree(IdType netId, bool allowOverflow, bool bboxConstrain, const CostFunction &f);

        void routeFlatSubNets(bool allowOverflow, const CostFunction &f);
        CostType routeSubNet(const SubNetIdType &subNetId, bool allowOverflow, bool bboxConstrain, const CostFunction &f);
        void routeSubNets(bool allowOverflow, const CostFunction &f);

        CostType routeMaze(const SubNetIdType &subNetId, bool allowOverflow, const Point &botleft, const Point &topright, 
          const CostFunction &f, vector<IdType> &path);

        void initialRouting(void);
        void doRRR(void);
        void layerAssignment(void);
        void greedyImprovement(void);

        void printParams(void) { params.print(); }
        void printStatistics(bool checkRouted = true, bool final = false);
        void printStatisticsLight(void);

        void plotXPM(const string &filename);
  };

  double cpuTime();
  int FGRRouting(HDPGrid& grid);

  const CostType powMax = 1.e12;
  const CostType powBase = 5.;
  const CostType edgeBase = 2.;
  const CostType viaFactor = 3.;
  const CostType epsilon = 1.;
  const CostType historyIncrement = 0.4;

  const string FGRversion = "1.0";
}; //namespace fgr

#endif
