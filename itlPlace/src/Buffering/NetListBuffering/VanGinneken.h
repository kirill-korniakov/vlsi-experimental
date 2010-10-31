#ifndef __VANGINNEKEN_H__
#define __VANGINNEKEN_H__

#include "HDesign.h"
#include "GrowOnlyArray.h"
#include "BufferInfo.h"

#include "WireSegmentationStrategies.h"

#define EPS	0.000000000000000000000001

namespace Buffering
{
    namespace Internal
    {
        template<class strategy>
        class VGNode2
        {
        private:
            typedef typename strategy::State State;
            typedef VGNode2<strategy> ThisType;
            int EdgeID;
            State Coord;
#define EDGE edges.Edges[EdgeID]

        public:
            VGNode2(int eid, const State& coord): EdgeID(eid), Coord(coord) {}
            double X(WireTreeEdges& edges, strategy& st) const { return st.GetX(Coord, EDGE); }
            double Y(WireTreeEdges& edges, strategy& st) const { return st.GetY(Coord, EDGE); }
            bool IsSource(WireTreeEdges& edges, strategy& st) const { return EDGE.IsStartPointRoot && st.IsStartPoint(Coord, EDGE); }
            bool IsSink(WireTreeEdges& edges, strategy& st) const { return EDGE.IsEndPointSink && st.IsEndPoint(Coord, EDGE); }

            ThisType Left(WireTreeEdges& edges, strategy& st) const
            {
                if (st.IsEndPoint(Coord, EDGE) || EDGE.Length < EPS)
                {
                    if (EDGE.HasLeft())
                    {
                        int left = EDGE.GetLeftEdge();
                        return ThisType(left, st.StartPointValue(edges.Edges[left]));
                    }
                    else
                        return ThisType(0, Coord);
                }
                else
                    return ThisType(EdgeID, st.GetNextState(Coord,EDGE));
            }

            ThisType Right(WireTreeEdges& edges, strategy& st) const
            {
                if ((st.IsEndPoint(Coord, EDGE) || EDGE.Length < EPS) && EDGE.HasRight())
                {
                    int right = EDGE.GetRightEdge();
                    return ThisType(right, st.StartPointValue(edges.Edges[right]));
                }
                else
                    return ThisType(0, Coord);
            }

            double DistanceTo(ThisType& node, WireTreeEdges& edges, strategy& st)//only for neighbour nodes
            {
                return st.Distance(Coord, node.Coord, EDGE);
            }

            double SinkCapacitance(WireTreeEdges& edges) const
            {
                HDesign& hd = edges.Design;
                return (((EDGE.End,hd).Pin(),hd).Type(),hd).Capacitance();
            }

            double SinkRequiredTime(WireTreeEdges& edges) const
            {
                HDesign& hd = edges.Design;
                return (hd.TimingPoints[(EDGE.End,hd).Pin()],hd).RequiredTime();
            }

            bool IsNull() const {return EdgeID == 0;}
#undef EDGE
        };

        //buffer insertion point
        struct VGNode
        {

            int EdgeID;
            double Coordinate;//0.0 <= Coordinate <= 1.0
        public:

#define EDGE edges.Edges[EdgeID]

            VGNode(int eid = 0, double coord = 0.0): EdgeID(eid), Coordinate(coord) {}

            double X(WireTreeEdges& edges) const { return EDGE.Xs() + (EDGE.Xe() - EDGE.Xs()) * Coordinate; }
            double Y(WireTreeEdges& edges) const { return EDGE.Ys() + (EDGE.Ye() - EDGE.Ys()) * Coordinate; }

            bool IsSource(WireTreeEdges& edges) const { return Equals(Coordinate, 0.0) && EDGE.IsStartPointRoot; }
            bool IsSink(WireTreeEdges& edges) const { return Equals(Coordinate, 1.0) && EDGE.IsEndPointSink; }
            //bool IsInternal(WireTreeEdges& edges) const { return !IsSink(edges) && !IsSource(edges); }

            double SinkCapacitance(WireTreeEdges& edges) const
            {
                ASSERT(IsSink(edges));
                HDesign& hd = edges.Design;
                return (((EDGE.End,hd).Pin(),hd).Type(),hd).Capacitance();
            }

            double SinkRequiredTime(WireTreeEdges& edges) const
            {
                ASSERT(IsSink(edges));
                HDesign& hd = edges.Design;
                return (hd.TimingPoints[(EDGE.End,hd).Pin()],hd).RequiredTime();
            }

            bool IsNull() const {return EdgeID == 0;}

            VGNode Left(WireTreeEdges& edges) const
            {
                if (Equals(Coordinate, 1.0))
                {
                    if (EDGE.HasLeft())
                        return VGNode(edges.GetLeftEdge(EdgeID), 0.0);//NOTE: if no right edge then can skip 0.0;
                    else
                    {
                        ASSERT(EDGE.IsEndPointSink);
                        if (!EDGE.IsEndPointSink)
                        {
                            WRITELINE("!! %s", ((EDGE.End,edges.Design).Pin(),edges.Design).Name().c_str());
                        }
                        return VGNode(0, 0.0);
                    }
                }
                else if (EDGE.Length < EPS)
                    return VGNode(EdgeID, 1.0);
                else
                    return VGNode(EdgeID, Coordinate + 0.05);
            }

            VGNode Right(WireTreeEdges& edges) const
            {
                if (Equals(Coordinate, 1.0))
                    return VGNode(EDGE.HasRight() ? edges.GetRightEdge(EdgeID) : 0, 0.0);
                else
                    return VGNode(0, 0.0);
            }

            double DistanceTo(VGNode& node, WireTreeEdges& edges)//only for neighbour nodes
            {
                if (EdgeID == node.EdgeID)
                    return (node.Coordinate - Coordinate) * EDGE.Length;
                else
                    return (1.0 - Coordinate) * EDGE.Length;
            }
#undef EDGE
        };

        //contains information of inserted buffer
        //part of temporary solutions
        struct Comp
        { 
            struct Comp *left;
            struct Comp *right;
            BufferInfo* BufferType;
            VGNode x;

            Comp(Comp* l, Comp*r, BufferInfo* binfo): counter(1), left(l), right(r), BufferType(binfo) {}
            Comp(Comp* l, Comp*r, BufferInfo* binfo, VGNode& node): counter(1), left(l), right(r), BufferType(binfo), x(node) {}

            void AddRef()
            {
                if (this == 0) return;
                counter++;
            }

            void Release()
            {
                if (this == 0) return;
                if (--counter <= 0)
                {
                    if (left != 0) left->Release();
                    if (right != 0) right->Release();
                    delete this;
                }
            }

        private:
            int counter;  /* use for comp deletion and comp chare */
        };

        // Ordered list used in Van Ginneken algorithm
        struct RLnode
        {
            struct RLnode *next;
            double DownstreamCapacitance;   /* downstream capacitance */
            double RequiredTime;  /* required arrival time at the node */
            Comp *com;    /* buffer locations */
            int BufsCount;

            RLnode(): next(0), com(0), DownstreamCapacitance(0.0), RequiredTime(0.0), BufsCount(0) {}
        };
    }

    class VanGinneken2;
}

#endif //__VANGINNEKEN_H__
