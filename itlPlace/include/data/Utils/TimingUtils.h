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

  SinkPhisics GetSinkCapacitance(HDesign& design, HPinType sink, KSignalDirection ph);
  SinkPhisics GetSinkCapacitance(HDesign& design, HPin sink, KSignalDirection ph);
  DriverPhisics GetArcPhisics(HDesign& design, HTimingArcType timingArc, KSignalDirection ph);
  DriverPhisics GetDriverWorstPhisics(HDesign& design, HPinType driver, KSignalDirection ph);
  DriverPhisics GetDriverAveragePhisics(HDesign& design, HPinType driver, KSignalDirection ph);
  DriverPhisics GetDriverTimingPhisics(HDesign& design, HPin driver, KSignalDirection ph);
  DriverPhisics GetDriverWorstPhisics(HDesign& design, HPin driver, KSignalDirection ph);
  DriverPhisics GetDriverAveragePhisics(HDesign& design, HPin driver, KSignalDirection ph);
  DriverPhisics GetElementWorstPhisics(HDesign& design, HCell element, KSignalDirection ph);
  DriverPhisics GetElementWorstPhisics(HDesign& design, HMacroType elementType, KSignalDirection ph);
  DriverPhisics GetElementAveragePhisics(HDesign& design, HCell element, KSignalDirection ph);
  DriverPhisics GetElementAveragePhisics(HDesign& design, HMacroType elementType, KSignalDirection ph);

  double GetNetLoad(HDesign& design, HNet net, KSignalDirection ph);

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
