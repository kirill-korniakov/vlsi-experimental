#ifndef __TIMINGUTILS_H__
#define __TIMINGUTILS_H__

#include "HDesign.h"
#include <math.h>

namespace Utils
{
  struct DriverPhisics
  {
    double T;
    double R;
    double C;
  };

  typedef double SinkPhisics;

  SinkPhisics GetSinkCapacitance(HDesign& design, HPinType sink, SignalDirection ph);
  SinkPhisics GetSinkCapacitance(HDesign& design, HPin sink, SignalDirection ph);
  DriverPhisics GetArcPhisics(HDesign& design, HTimingArcType timingArc, SignalDirection ph);
  DriverPhisics GetDriverWorstPhisics(HDesign& design, HPinType driver, SignalDirection ph);
  DriverPhisics GetDriverAveragePhisics(HDesign& design, HPinType driver, SignalDirection ph);
  DriverPhisics GetDriverTimingPhisics(HDesign& design, HPin driver, SignalDirection ph);
  DriverPhisics GetDriverWorstPhisics(HDesign& design, HPin driver, SignalDirection ph);
  DriverPhisics GetDriverAveragePhisics(HDesign& design, HPin driver, SignalDirection ph);
  DriverPhisics GetElementWorstPhisics(HDesign& design, HCell element, SignalDirection ph);
  DriverPhisics GetElementWorstPhisics(HDesign& design, HMacroType elementType, SignalDirection ph);
  DriverPhisics GetElementAveragePhisics(HDesign& design, HCell element, SignalDirection ph);
  DriverPhisics GetElementAveragePhisics(HDesign& design, HMacroType elementType, SignalDirection ph);

  double GetNetLoad(HDesign& design, HNet net, SignalDirection ph);

  inline double LBuf(const DriverPhisics& bdriver, const SinkPhisics& bsink, const HWirePhysicalParams& wires)
  {
    return bdriver.R / wires.RPerDist + bsink / wires.LinearC;
  }

  inline double LNet(const DriverPhisics& ndriver, double netLoad, const HWirePhysicalParams& wires)
  {
    return ndriver.R / wires.RPerDist + netLoad / wires.LinearC;
  }

  inline double Lext(double wireLength, double lbuf, double lnet)
  {
    return wireLength + lnet - lbuf;
  }

  inline double DBuf(const DriverPhisics& bdriver, const SinkPhisics& bsink, const HWirePhysicalParams& wires)
  {
    return sqrt(2.0 * (bdriver.T + bdriver.R * bsink) / wires.RPerDist / wires.LinearC);
  }

  inline double DNet(const DriverPhisics& ndriver, double netLoad, const HWirePhysicalParams& wires)
  {
    return sqrt(2.0 * ndriver.R * netLoad / wires.RPerDist / wires.LinearC);
  }

  bool VerifyTimingCalculationOrder(HDesign& design);

}

#endif //__TIMINGUTILS_H__