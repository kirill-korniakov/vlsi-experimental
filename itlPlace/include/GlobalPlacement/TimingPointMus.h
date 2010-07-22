#ifndef __TIMING_POINT_MUS__
#define __TIMING_POINT_MUS__

#include "HDesign.h"
#include "MuReporter.h"

class TimingPointMus
{
public:
    TimingPointMus(HDesign& design);
    ~TimingPointMus();

    void UpdateMus(HDesign& design);

    void GetNetMus(HDesign& design, HNet net,
        std::vector<double>& cellArcMus, 
        std::vector<double>& netArcMus);

    int GetMuInCount(HTimingPoint pt) const { return (int)MuIn[::ToID(pt)].size() / 2; }
    int GetMuOutCount(HDesign& design, HTimingPoint pt)
    {
        Count counter;
        IterateOutMu(design, pt, counter);
        return counter.count;
    }

    double GetMuS(HTimingPoint pt) const { return MuS[::ToID(pt)]; }
    double GetInMuA(HTimingPoint pt, int index) const { return MuIn[::ToID(pt)][index]; }
    double GetInMuR(HTimingPoint pt, int index) const { return MuIn[::ToID(pt)][MuIn[::ToID(pt)].size() - 1 - index]; }

    double SumInMuA(HTimingPoint pt);
    double SumInMuR(HTimingPoint pt);
    double SumOutMuA(HDesign& design, HTimingPoint pt);
    double SumOutMuR(HDesign& design, HTimingPoint pt);

private:
    int size;
    double* MuS;
    std::vector<double>* MuIn;  //this class stores only muS, input muA and muR for the timing point,
    //out muA and muR are associated with next timing point

    MuReporter* reporter;

    void UpdateMuS(HDesign& design, double theta);
    void UpdateMuAOnTEP(HDesign& design, double theta);
    void UpdateMuROnTSP(HDesign& design, double theta);
    //void UpdateMuAMuROnPrimaries(HDesign& design, double theta);

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

    void ScaleOutMuR(HDesign& design, HTimingPoint pt, double multiplier);
    void ScaleInMuA(HTimingPoint pt, double multiplier);

    struct AccumulateMu
    {
        bool UseArrival;
        double sum;
        AccumulateMu(bool useArrival):UseArrival(useArrival), sum(0.0) {}

        void operator() (TimingPointMus& pm, HTimingPoint pt, int idx)
        {
            if (UseArrival)
                sum += pm.GetInMuA(pt, idx);
            else
                sum += pm.GetInMuR(pt, idx);
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
            muOut.push_back(pm.GetInMuA(pt, idx) + pm.GetInMuR(pt, idx));
        }
    };

    struct ScaleMuR
    {
        double Multiplier;
        ScaleMuR(double multiplier):Multiplier(multiplier) {}

        void operator() (TimingPointMus& pm, HTimingPoint pt, int idx)
        {
            pm.SetMuInR(pt, idx, Multiplier * pm.GetInMuR(pt, idx));
        }
    };

    struct Count
    {
        int count;
        Count():count(0) {}

        void operator() (TimingPointMus& pm, HTimingPoint pt, int idx)
        {
            count++;
        }
    };

    template<class Action>
    void IterateOutMu(HDesign& design, HTimingPoint pt, Action& todo);

    void InitPointMus(HDesign& design, HTimingPoint pt);
    double CalculateFactor(double slack, double theta);
};

#endif