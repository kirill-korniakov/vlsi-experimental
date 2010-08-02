#ifndef __TIMING_POINT_MUS__
#define __TIMING_POINT_MUS__

#include "HDesign.h"
#include "MuReporter.h"

class TimingPointMus
{
public:
    TimingPointMus(HDesign& design);
    ~TimingPointMus();

    void InitMuSOnTSP(HDesign &design, double defaultMu);
    void InitMusInternalAndTEP(HDesign& design, double defaultMu);
    void InitPointMus(HDesign& design, HTimingPoint pt, double defaultMu);

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
    MuReporter* reporter;
    int size;
    double* MuS;
    std::vector<double>* MuIn;  //this class stores only muS, input muA, input muR for the timing point,
                                //out muA and muR are associated with the next timing point

    double minFactor;
    double referenceValue;
    int nIncreased;
    int nDecreased;

    void UpdateMuS(HDesign& design, double theta);
    void UpdateLocalMuA(HDesign& design, double theta);
    void UpdateLocalMuR(HDesign& design, double theta);

    //TODO: move methods to the pin
    bool IsOutputPin(HDesign &design, HPin pin) 
    {
        return design.Get<HPin::Direction, PinDirection>(pin) == PinDirection_OUTPUT;
    }
    bool IsInputPin(HDesign &design, HPin pin) 
    {
        return design.Get<HPin::Direction, PinDirection>(pin) == PinDirection_INPUT;
    }

    void UpdateLocalMuB(HDesign& design, double theta);
    double CalculateInjectionFactor(double slack, double theta);
    double CalculateLocalFactor(double slack, double theta);

    void EnforceFlowProperty(HDesign& design);
    void EnforceArrivalFlowProperty(HDesign& design, HTimingPoint pt);
    void EnforceRequiredFlowProperty(HDesign& design, HTimingPoint pt);

    void GetMuIn(HDesign& design, HTimingPoint pt, std::vector<double>& inMus);
    void GetMuOut(HDesign& design, HTimingPoint pt, std::vector<double>& outMus);

    double BorderValue(double value) { return max(0.01, min(value, 0.99)); }
    double ZeroBorderValue(double value) { return max(0.01, value); }
    void SetMuS(HTimingPoint pt, double value) { MuS[::ToID(pt)] = BorderValue(value); }
    void SetInMuA(HTimingPoint pt, int index, double value) { MuIn[::ToID(pt)][index] = ZeroBorderValue(value); }
    void SetInMuR(HTimingPoint pt, int index, double value) { MuIn[::ToID(pt)][MuIn[::ToID(pt)].size() - 1 - index] = ZeroBorderValue(value); }
    
    void ScaleOutMuR(HDesign& design, HTimingPoint pt, double multiplier);
    void ScaleOutMuRByIndex(HDesign& design, HTimingPoint pt, double multiplier, int index);
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
        double Factor;
        int index;
        int iterationCounter;

        ScaleMuR(double val, int idx = -1) : Factor(val), index(idx), iterationCounter(0) {}

        void operator() (TimingPointMus& pm, HTimingPoint pt, int idx)
        {
            if (index == -1 || index == iterationCounter)
            {
                double oldValue = pm.GetInMuR(pt, idx);
                pm.SetInMuR(pt, idx, Factor * oldValue);
            }
            iterationCounter++;
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
};

#endif