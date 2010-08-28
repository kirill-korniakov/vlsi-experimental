#ifndef __WIRESEGMENTATIONSTRATEGIES_H__
#define __WIRESEGMENTATIONSTRATEGIES_H__

#include "VanGinnekenWires.h"

namespace Buffering
{
  struct ConstantSegmentCountStrategy
  {
    typedef double State;

    State StartPointValue(const WireTreeEdge& e) const {return 0.0;}

    double GetX(const State& s, WireTreeEdge& e)
    {
      return e.Xs() + (e.Xe() - e.Xs()) * s;
    }

    double GetY(const State& s, WireTreeEdge& e)
    {
      return e.Ys() + (e.Ye() - e.Ys()) * s;
    }

    bool IsStartPoint(const State& s, const WireTreeEdge& e)
    {
      return Equals(s, 0.0);
    }

    bool IsEndPoint(const State& s, const WireTreeEdge& e)
    {
      return Equals(s, 1.0);
    }

    State GetNextState(const State& s, const WireTreeEdge& e)
    {
      return s + 0.05;
    }

    double Distance(const State& s1, const State& s2, const WireTreeEdge& e)
    {
      if (s2 >= s1)
        return (s2 - s1) * e.Length;
      else
        return (1.0 - s1) * e.Length;
    }
  };
}

#endif //__WIRESEGMENTATIONSTRATEGIES_H__