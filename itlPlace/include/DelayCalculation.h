#ifndef __DELAY_CALCULATION_H__
#define __DELAY_CALCULATION_H__

#include <math.h>
#include "HDesign.h"
#include "Utils.h"

namespace DelayCalculationInternals
{
  class PointsContainer
  {
    int m_memorySize;
    int m_dataSize;
    HSteinerPoint* m_points;

  public:
    PointsContainer(int capacity)
    {
      m_points = new HSteinerPoint[capacity];
      m_memorySize = capacity;
      m_dataSize = 0;
    }

    ~PointsContainer()
    {
      delete[] m_points;
    }

    int Size() const
    {
      return m_dataSize;
    }

    void Push(HSteinerPoint point)
    {
      if (m_dataSize == m_memorySize)
        ::Grow(&m_points, m_dataSize, m_memorySize *= 2);
      m_points[m_dataSize++] = point;
    }

    void Reset() { m_dataSize = 0; }

    HSteinerPoint operator[](int idx) const { return m_points[idx]; }
  };

  class TwoDirectionalRCExtractor
  {
  public:
    void CalculateWireExtractedRC(HDesign& aDesign, HSteinerPoint aFirstPoint, HSteinerPoint aSecondPoint) const
    {
      HWirePhysicalParams& aPhysics = aDesign.RoutingLayers.Physics;
      double lh = fabs(aDesign.GetDouble<HSteinerPoint::X>(aFirstPoint)
        - aDesign.GetDouble<HSteinerPoint::X>(aSecondPoint));
      double lv = fabs(aDesign.GetDouble<HSteinerPoint::Y>(aFirstPoint)
        - aDesign.GetDouble<HSteinerPoint::Y>(aSecondPoint));
      //test if horizontal or vertical or diagonal
      if (lh <= aPhysics.VertPitch)
      {
        aDesign.Set<HSteinerPoint::ExtractedC>(aSecondPoint,
          aPhysics.VertCPerDist * lv + 2.0 * (aPhysics.VertWidth + lv) * aPhysics.VertEdgeCap);
        aDesign.Set<HSteinerPoint::ExtractedR>(aSecondPoint,
          aPhysics.VertRPerDist * lv);
      }
      else if (lv <= aPhysics.HorizPitch)
      {
        aDesign.Set<HSteinerPoint::ExtractedC>(aSecondPoint,
          aPhysics.HorizCPerDist * lh
          + 2.0 * (aPhysics.HorizWidth + lh) * aPhysics.HorizEdgeCap);
        aDesign.Set<HSteinerPoint::ExtractedR>(aSecondPoint,
          aPhysics.HorizRPerDist * lh);
      }
      else
      {
        aDesign.Set<HSteinerPoint::ExtractedC>(aSecondPoint,
          aPhysics.HorizCPerDist * lh 
          + 2.0 * (aPhysics.HorizWidth + lh) * aPhysics.HorizEdgeCap
          + aPhysics.VertCPerDist * lv
          + 2.0 * (aPhysics.VertWidth + lv) * aPhysics.VertEdgeCap);
        aDesign.Set<HSteinerPoint::ExtractedR>(aSecondPoint,
          aPhysics.HorizRPerDist * lh + aPhysics.VertRPerDist * lv);
      }
    }
  };

  class OneDirectionalRCExtractor
  {
  public:
    void CalculateWireExtractedRC(HDesign& aDesign, HSteinerPoint aFirstPoint, HSteinerPoint aSecondPoint) const
    {
      HWirePhysicalParams& aPhysics = aDesign.RoutingLayers.Physics;
      double l = fabs(aDesign.GetDouble<HSteinerPoint::X>(aFirstPoint)
          - aDesign.GetDouble<HSteinerPoint::X>(aSecondPoint))
        + fabs(aDesign.GetDouble<HSteinerPoint::Y>(aFirstPoint)
          - aDesign.GetDouble<HSteinerPoint::Y>(aSecondPoint));

      aDesign.Set<HSteinerPoint::ExtractedC>(aSecondPoint,
          aPhysics.CPerDist * l + 2.0 * (aPhysics.Width + l) * aPhysics.EdgeCap);
      aDesign.Set<HSteinerPoint::ExtractedR>(aSecondPoint,
          aPhysics.RPerDist * l);
    }
  };

  class LumpedRCExtractor
  {
  public:
    void CalculateWireExtractedRC(HDesign& aDesign, HSteinerPoint aFirstPoint, HSteinerPoint aSecondPoint) const
    {
      HWirePhysicalParams& aPhysics = aDesign.RoutingLayers.Physics;
      double l = fabs(aDesign.GetDouble<HSteinerPoint::X>(aFirstPoint)
          - aDesign.GetDouble<HSteinerPoint::X>(aSecondPoint))
        + fabs(aDesign.GetDouble<HSteinerPoint::Y>(aFirstPoint)
          - aDesign.GetDouble<HSteinerPoint::Y>(aSecondPoint));

      aDesign.Set<HSteinerPoint::ExtractedC>(aSecondPoint,
          aPhysics.LinearC * l);
      aDesign.Set<HSteinerPoint::ExtractedR>(aSecondPoint,
          aPhysics.RPerDist * l);
    }
  };

  class HippocrateRCExtractor
  {
  public:
    void CalculateWireExtractedRC(HDesign& aDesign, HSteinerPoint aFirstPoint, HSteinerPoint aSecondPoint) const
    {
      HWirePhysicalParams& aPhysics = aDesign.RoutingLayers.Physics;

      if (!(aDesign,aSecondPoint).IsInternal())
      {
        HPin sink = (aDesign,aSecondPoint).Pin();
        HPin src = ((aDesign,sink).Net(),aDesign).Source();
        if (src != sink)//net sink detected :)
        {
          double l = abs((aDesign,sink).X() - (aDesign,src).X())
            + abs((aDesign,sink).Y() - (aDesign,src).Y());

          aDesign.Set<HSteinerPoint::ExtractedC>(aSecondPoint,
            aPhysics.LinearC * l);
          aDesign.Set<HSteinerPoint::ExtractedR>(aSecondPoint,
            aPhysics.RPerDist * l);
        }
      }
      else if (!(aDesign,aFirstPoint).IsInternal())
      {
        HPin src = (aDesign,aFirstPoint).Pin();
        HNetWrapper net = ((aDesign,src).Net(),aDesign);
        if (net.Source() == src)//net source detected :)
        {
          double L = Utils::CalcNetHPWL(aDesign, net);
          for (HNet::SinksEnumeratorW sink = net.GetSinksEnumeratorW(); sink.MoveNext(); )
            L -= abs(aDesign.GetDouble<HPin::X>(src) - sink.X()) + abs(aDesign.GetDouble<HPin::Y>(src) - sink.Y());

          aDesign.Set<HSteinerPoint::ExtractedC>(aSecondPoint, L * aPhysics.LinearC);
          aDesign.Set<HSteinerPoint::ExtractedR>(aSecondPoint, 0.0);
        }
      }
      else
      {
        aDesign.Set<HSteinerPoint::ExtractedC>(aSecondPoint, 0.0);
        aDesign.Set<HSteinerPoint::ExtractedR>(aSecondPoint, 0.0);
      }
    }
  };

  class TwoDirectionalDelaysCalculator
  {
  public:
    void CalcObservedC(HDesign& design, HSteinerPoint point) const
    {
      double fallC = 0.0;
      double riseC = 0.0;
      
      //add own point capacitance
      if (!design.GetBool<HSteinerPoint::IsInternal>(point))
      {
        HPinType pin_type = design.Get<HPin::Type, HPinType>(design.Get<HSteinerPoint::Pin, HPin>(point));

        fallC = design.GetDouble<HPinType::FallCapacitance>(pin_type);
        riseC = design.GetDouble<HPinType::RiseCapacitance>(pin_type);
      }

      //add left edge
      if (design.GetBool<HSteinerPoint::HasLeft>(point))
      {
        HSteinerPoint leftPoint = design.Get<HSteinerPoint::Left, HSteinerPoint>(point);
        double leftEdgeC = design.GetDouble<HSteinerPoint::ExtractedC>(leftPoint);

        fallC += leftEdgeC + design.GetDouble<HSteinerPoint::FallObservedC>(leftPoint);
        riseC += leftEdgeC + design.GetDouble<HSteinerPoint::RiseObservedC>(leftPoint);

        //OPTIMIZATION: right edge may exist only if exists left edge
        //add right edge
        if (design.GetBool<HSteinerPoint::HasRight>(point))
        {
          HSteinerPoint rightPoint = design.Get<HSteinerPoint::Right, HSteinerPoint>(point);
          double rightEdgeC = design.GetDouble<HSteinerPoint::ExtractedC>(rightPoint);

          fallC += rightEdgeC + design.GetDouble<HSteinerPoint::FallObservedC>(rightPoint);
          riseC += rightEdgeC + design.GetDouble<HSteinerPoint::RiseObservedC>(rightPoint);
        }
      }

      design.Set<HSteinerPoint::FallObservedC>(point, fallC);
      design.Set<HSteinerPoint::RiseObservedC>(point, riseC);
    }

    void CalcPathDelay(HDesign& design, HSteinerPoint edgeStart, HSteinerPoint edgeEnd) const
    {
      ASSERT(!::IsNull(edgeEnd));

      design.Set<HSteinerPoint::FallPathDelay>(edgeEnd,
        design.GetDouble<HSteinerPoint::FallPathDelay>(edgeStart)
        + design.GetDouble<HSteinerPoint::ExtractedR>(edgeEnd)
          * (design.GetDouble<HSteinerPoint::ExtractedC>(edgeEnd) * 0.5
            + design.GetDouble<HSteinerPoint::FallObservedC>(edgeEnd))
        );

      design.Set<HSteinerPoint::RisePathDelay>(edgeEnd,
        design.GetDouble<HSteinerPoint::RisePathDelay>(edgeStart)
        + design.GetDouble<HSteinerPoint::ExtractedR>(edgeEnd)
          * (design.GetDouble<HSteinerPoint::ExtractedC>(edgeEnd) * 0.5
            + design.GetDouble<HSteinerPoint::RiseObservedC>(edgeEnd))
        );
    }
  };

  class OneDirectionalDelaysCalculator
  {
  public:
    void CalcObservedC(HDesign& design, HSteinerPoint point) const
    {
      double cap = 0.0;
      
      //add own point capacitance
      if (!design.GetBool<HSteinerPoint::IsInternal>(point))
        cap = design.GetDouble<HPinType::Capacitance>(
                design.Get<HPin::Type, HPinType>(
                  design.Get<HSteinerPoint::Pin, HPin>(
                    point
                  )
                )
              );
      
      //add left edge
      if (design.GetBool<HSteinerPoint::HasLeft>(point))
      {
        HSteinerPoint leftPoint = design.Get<HSteinerPoint::Left, HSteinerPoint>(point);

        cap += design.GetDouble<HSteinerPoint::ExtractedC>(leftPoint)
          + design.GetDouble<HSteinerPoint::ObservedC>(leftPoint);

        //OPTIMIZATION: right edge may exist only if exists left edge
        //add right edge
        if (design.GetBool<HSteinerPoint::HasRight>(point))
        {
          HSteinerPoint rightPoint = design.Get<HSteinerPoint::Right, HSteinerPoint>(point);
          cap += design.GetDouble<HSteinerPoint::ExtractedC>(rightPoint)
            + design.GetDouble<HSteinerPoint::ObservedC>(rightPoint);
        }
      }

      design.Set<HSteinerPoint::ObservedC>(point, cap);
    }

    void CalcPathDelay(HDesign& design, HSteinerPoint edgeStart, HSteinerPoint edgeEnd) const
    {
      ASSERT(!::IsNull(edgeEnd));

      design.Set<HSteinerPoint::PathDelay>(edgeEnd,
        design.GetDouble<HSteinerPoint::PathDelay>(edgeStart)
        + design.GetDouble<HSteinerPoint::ExtractedR>(edgeEnd)
          * (design.GetDouble<HSteinerPoint::ExtractedC>(edgeEnd) * 0.5
            + design.GetDouble<HSteinerPoint::ObservedC>(edgeEnd))
        );
    }
  };

  template<class RCExtractor, class DelayCalculator>
  void CalcNetDelays(HDesign& design,
                     HNet net,
                     PointsContainer& ptContainer,
                     const RCExtractor& extractor,
                     const DelayCalculator& calculator)
  {
    ptContainer.Reset();
    //put source into container
    HSteinerPointWrapper activePoint = design[design.Get<HWire::RootPoint, HSteinerPoint>(design.Wires[net])];
    ptContainer.Push(activePoint);

    //extract RC for wires and remember traversing order
    for (int i = 0; i < ptContainer.Size(); ++i)
    {
      activePoint = ptContainer[i];

      if (activePoint.HasLeft())
      {
        HSteinerPoint leftPoint = activePoint.Left();

        ptContainer.Push(leftPoint);
        extractor.CalculateWireExtractedRC(design, activePoint, leftPoint);

        if (activePoint.HasRight())
        {
          HSteinerPoint rightPoint = activePoint.Right();

          ptContainer.Push(rightPoint);
          extractor.CalculateWireExtractedRC(design, activePoint, rightPoint);
        }
      }
    }

    //accumulate observed C
    for (int j = ptContainer.Size() - 1; j >= 0; --j)
      calculator.CalcObservedC(design, ptContainer[j]);

    //propagate delays
    for (int k = 0; k < ptContainer.Size(); ++k)
    {
      activePoint = ptContainer[k];
      
      if (activePoint.HasLeft())
      {
        calculator.CalcPathDelay(design, activePoint, activePoint.Left());

        if (activePoint.HasRight())
          calculator.CalcPathDelay(design, activePoint, activePoint.Right());
      }
    }
  }
}//namespace DelayCalculationInternals

enum LayersModel
{
  LayersModel_OneDirection,
  LayersModel_TwoDirections,
  LayersModel_Lumped,
  LayersModel_Hippocrate
};

enum SignalModel
{
  SignalModel_Universal,
  SignalModel_RiseFall
};

template<LayersModel lmodel, SignalModel smodel>
inline void CalculateNetDelays(HDesign& design, HNet net, DelayCalculationInternals::PointsContainer& ptContainer)
{
  /*This_method_should_not_be_instantiated*/;
}

template<>
inline void CalculateNetDelays<LayersModel_Hippocrate, SignalModel_Universal>
  (HDesign& design, HNet net, DelayCalculationInternals::PointsContainer& ptContainer)
{
  DelayCalculationInternals::CalcNetDelays(
    design,
    net,
    ptContainer,
    DelayCalculationInternals::HippocrateRCExtractor(),
    DelayCalculationInternals::OneDirectionalDelaysCalculator()
  );
}

template<>
inline void CalculateNetDelays<LayersModel_Hippocrate, SignalModel_RiseFall>
  (HDesign& design, HNet net, DelayCalculationInternals::PointsContainer& ptContainer)
{
  DelayCalculationInternals::CalcNetDelays(
    design,
    net,
    ptContainer,
    DelayCalculationInternals::HippocrateRCExtractor(),
    DelayCalculationInternals::TwoDirectionalDelaysCalculator()
  );
}

template<>
inline void CalculateNetDelays<LayersModel_Lumped, SignalModel_Universal>
  (HDesign& design, HNet net, DelayCalculationInternals::PointsContainer& ptContainer)
{
  DelayCalculationInternals::CalcNetDelays(
    design,
    net,
    ptContainer,
    DelayCalculationInternals::LumpedRCExtractor(),
    DelayCalculationInternals::OneDirectionalDelaysCalculator()
  );
}

template<>
inline void CalculateNetDelays<LayersModel_Lumped, SignalModel_RiseFall>
  (HDesign& design, HNet net, DelayCalculationInternals::PointsContainer& ptContainer)
{
  DelayCalculationInternals::CalcNetDelays(
    design,
    net,
    ptContainer,
    DelayCalculationInternals::LumpedRCExtractor(),
    DelayCalculationInternals::TwoDirectionalDelaysCalculator()
  );
}

template<>
inline void CalculateNetDelays<LayersModel_OneDirection, SignalModel_Universal>
  (HDesign& design, HNet net, DelayCalculationInternals::PointsContainer& ptContainer)
{
  DelayCalculationInternals::CalcNetDelays(
    design,
    net,
    ptContainer,
    DelayCalculationInternals::OneDirectionalRCExtractor(),
    DelayCalculationInternals::OneDirectionalDelaysCalculator()
  );
}

template<>
inline void CalculateNetDelays<LayersModel_OneDirection, SignalModel_RiseFall>
  (HDesign& design, HNet net, DelayCalculationInternals::PointsContainer& ptContainer)
{
  DelayCalculationInternals::CalcNetDelays(
    design,
    net,
    ptContainer,
    DelayCalculationInternals::OneDirectionalRCExtractor(),
    DelayCalculationInternals::TwoDirectionalDelaysCalculator()
  );
}

template<>
inline void CalculateNetDelays<LayersModel_TwoDirections, SignalModel_Universal>
  (HDesign& design, HNet net, DelayCalculationInternals::PointsContainer& ptContainer)
{
  DelayCalculationInternals::CalcNetDelays(
    design,
    net,
    ptContainer,
    DelayCalculationInternals::TwoDirectionalRCExtractor(),
    DelayCalculationInternals::OneDirectionalDelaysCalculator()
  );
}

template<>
inline void CalculateNetDelays<LayersModel_TwoDirections, SignalModel_RiseFall>
  (HDesign& design, HNet net, DelayCalculationInternals::PointsContainer& ptContainer)
{
  DelayCalculationInternals::CalcNetDelays(
    design,
    net,
    ptContainer,
    DelayCalculationInternals::TwoDirectionalRCExtractor(),
    DelayCalculationInternals::TwoDirectionalDelaysCalculator()
  );
}

template<LayersModel lmodel, SignalModel smodel>
inline void CalculateWireDelays(HDesign& design)
{
  ConfigContext ctx(design.cfg.OpenContext("DelayCalculator"));
  //ALERT("RC EXTRACTION & DELAY CALCULATION STARTED...");

  DelayCalculationInternals::PointsContainer ptContainer(design.cfg.ValueOf(".initialNetPointsReserve", 90));

  for (HNets::ActiveNetsEnumeratorW net = design.Nets.GetActiveNetsEnumeratorW(); net.MoveNext(); )
    CalculateNetDelays<lmodel, smodel>(design, net, ptContainer);

  //ALERT("RC EXTRACTION & DELAY CALCULATION FINISHED");
}

template<LayersModel lmodel, SignalModel smodel>
inline void CalculateNetDelays(HDesign& design, HNet net)
{
  ConfigContext ctx(design.cfg.OpenContext("DelayCalculator"));

  DelayCalculationInternals::PointsContainer ptContainer(design.cfg.ValueOf(".initialNetPointsReserve", 90));
  CalculateNetDelays<lmodel, smodel>(design, net, ptContainer);
}

inline void CalculateWireDelays(HDesign& design, LayersModel lmodel, SignalModel smodel)
{
  if (lmodel == LayersModel_OneDirection)
    if (smodel == SignalModel_Universal)
      CalculateWireDelays<LayersModel_OneDirection, SignalModel_Universal>(design);
    else
      CalculateWireDelays<LayersModel_OneDirection, SignalModel_RiseFall>(design);
  else if (lmodel == LayersModel_TwoDirections)
    if (smodel == SignalModel_Universal)
      CalculateWireDelays<LayersModel_TwoDirections, SignalModel_Universal>(design);
    else
      CalculateWireDelays<LayersModel_TwoDirections, SignalModel_RiseFall>(design);
  else if (lmodel == LayersModel_Lumped)
    if (smodel == SignalModel_Universal)
      CalculateWireDelays<LayersModel_Lumped, SignalModel_Universal>(design);
    else
      CalculateWireDelays<LayersModel_Lumped, SignalModel_RiseFall>(design);
  else if (lmodel == LayersModel_Hippocrate)
    if (smodel == SignalModel_Universal)
      CalculateWireDelays<LayersModel_Hippocrate, SignalModel_Universal>(design);
    else
      CalculateWireDelays<LayersModel_Hippocrate, SignalModel_RiseFall>(design);
  else
  {
    LOGCRITICAL("Unsupported Layers Model");
  }
}

#endif //__DELAY_CALCULATION_H__
