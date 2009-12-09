#ifndef __VANGINNEKENWIRES_H__
#define __VANGINNEKENWIRES_H__

namespace Buffering
{
  //temporary struct containing information about buffer added into netlist
  struct InsertedBuffer
  {
    HPin sink;
    HPin driver;
    double coordinate;
  };

  inline bool operator <(const InsertedBuffer& ib1, const InsertedBuffer& ib2)
  {
    return ib1.coordinate < ib2.coordinate;
  }

  struct WireTreeEdges;

  //wrapper for edge of steiner tree
  struct WireTreeEdge
  {
    WireTreeEdges* Parent;
    int LeftEdge;
    int RightEdge;

    HSteinerPoint Start;
    HSteinerPoint End;
    double Length;
    bool IsStartPointRoot;
    bool IsEndPointSink;
    std::vector<InsertedBuffer> InsertedBuffers;

    void Set(WireTreeEdges* parent, HSteinerPoint start, HSteinerPoint end);
    bool HasLeft();
    bool HasRight();

    //int GetLeftEdge();
    //int GetRightEdge();

    double Xs();
    double Ys();
    double Xe();
    double Ye();

    void Clear();
  };

  //set of edges covering all steiner tree
  struct WireTreeEdges
  {
    GrowOnlyArray<WireTreeEdge> Edges;
    HDesign& Design;

    WireTreeEdges(HDesign& hd): Design(hd), Edges(20,20,0.002) {}
    int CreateNewEdge();

    int GetLeftEdge(int rootIndex);
    int GetRightEdge(int rootIndex);
  };
}

#endif //__VANGINNEKENWIRES_H__