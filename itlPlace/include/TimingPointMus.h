#ifndef __TIMING_POINT_MUS__
#define __TIMING_POINT_MUS__

#include "HDesign.h"

class TimingPointMus;

class MuReporter
{
public:
    MuReporter(HDesign& design);
    void Report(HDesign& design, TimingPointMus* mus);

private:
    TimingPointMus* mus;
    int waitTime;

    void ReportMus(HDesign& design);
    void PlotMusInTopologicalOrder(HDesign& design);
    void PlotPathMus(HDesign& design, HCriticalPath path, int pathIdx);
    void PlotMusInCriticalPathOrder(HDesign& design);
};

class TimingPointMus
{
public:
  TimingPointMus(HDesign& design);
  ~TimingPointMus();

  void UpdateMus(HDesign& design);


  void GetNetMus(HDesign& design, HNet net,
                 std::vector<double>& cellArcMus, 
                 std::vector<double>& netArcMus);

  int GetMuInCount(HTimingPoint pt) const { return MuIn[::ToID(pt)].size() / 2; }
  double GetMuS(HTimingPoint pt) const { return MuS[::ToID(pt)]; }
  double GetMuInA(HTimingPoint pt, int index) const { return MuIn[::ToID(pt)][index]; }
  double GetMuInR(HTimingPoint pt, int index) const { return MuIn[::ToID(pt)][MuIn[::ToID(pt)].size() - 1 - index]; }
  double GetInMuA(HTimingPoint pt);
  double GetInMuR(HTimingPoint pt);

private:

  double* MuS;
  std::vector<double>* MuIn;
  int size;
  MuReporter* reporter;

  void EnforceFlowProperty(HDesign& design);
  void EnforceArrivalFlowProperty(HDesign& design, HTimingPoint pt);
  void EnforceRequiredFlowProperty(HDesign& design, HTimingPoint pt);

  void GetMuIn(HDesign& design, HTimingPoint pt, std::vector<double>& inMus);
  void GetMuOut(HDesign& design, HTimingPoint pt, std::vector<double>& outMus);

  double BorderValue(double value) { return max(0.01, min(value, 0.99)); }
  double ZeroBorderValue(double value) { return max(0.01, value); }
  void SetMuS(HTimingPoint pt, double value) { MuS[::ToID(pt)] = BorderValue(value); }
  void SetMuInA(HTimingPoint pt, int index, double value) { MuIn[::ToID(pt)][index] = ZeroBorderValue(value); }
  void SetMuInR(HTimingPoint pt, int index, double value) { MuIn[::ToID(pt)][MuIn[::ToID(pt)].size() - 1 - index] = ZeroBorderValue(value); }

  double SumInMuA(HTimingPoint pt);
  double SumInMuR(HTimingPoint pt);
  double SumOutMuA(HDesign& design, HTimingPoint pt);
  double SumOutMuR(HDesign& design, HTimingPoint pt);

  void UpdateOutMuR(HDesign& design, HTimingPoint pt, double multiplier);
  void UpdateInMuA(HTimingPoint pt, double multiplier);

  struct AccumulateMu
  {
    bool UseArrival;
    double sum;
    AccumulateMu(bool useArrival):UseArrival(useArrival), sum(0.0) {}

    void operator() (TimingPointMus& pm, HTimingPoint pt, int idx)
    {
      if (UseArrival)
        sum += pm.GetMuInA(pt, idx);
      else
        sum += pm.GetMuInR(pt, idx);
    }
  };

  struct GetMuOutVector
  {
    std::vector<double>& muOut;

    GetMuOutVector(std::vector<double>& vec): muOut(vec)
    {
      muOut.clear();
    } 

    void operator() (TimingPointMus& pm, HTimingPoint pt, int idx)
    {
      muOut.push_back(pm.GetMuInA(pt, idx) + pm.GetMuInR(pt, idx));
    }
  };

  struct UpdateMuR
  {
    double Multiplier;
    UpdateMuR(double multiplier):Multiplier(multiplier) {}

    void operator() (TimingPointMus& pm, HTimingPoint pt, int idx)
    {
      pm.SetMuInR(pt, idx, Multiplier * pm.GetMuInR(pt, idx));
    }
  };

  template<class Action>
  void IterateOutMu(HDesign& design, HTimingPoint pt, Action& todo);

  void InitPoint(HDesign& design, HTimingPoint pt);
};

#endif