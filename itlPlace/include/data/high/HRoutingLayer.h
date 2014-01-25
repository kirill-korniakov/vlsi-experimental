#ifndef __HIGH_ROUTINGLAYER_H__
#define __HIGH_ROUTINGLAYER_H__

#include "HBaseDesignItem.h"
#include "HWirePhysicalParams.h"

BEGINITEM(HRoutingLayer, HRoutingLayerWrapper)

  enum Properties
  {
    Name,
    Direction,
    RPerDist,
    CPerDist,
    EdgeCap,
    Width,
    Pitch
  };
ENDITEM(HRoutingLayer)

BEGINHCOLLECTION(HRoutingLayers, HRoutingLayer)

  HWirePhysicalParams Physics;

  int Count() const {return m_ld->Tech->nLayers - 1;}

  ///Enumerator
  typedef HConstEnumerator<HRoutingLayer,1> Enumerator;
  typedef HEnumeratorW<HRoutingLayerWrapper, HRoutingLayers> EnumeratorW;
  Enumerator GetEnumerator() const {return Enumerator(m_ld->Tech->nLayers);}
  EnumeratorW GetEnumeratorW();

  ///Events
  Event<2, int, int> OnGrow; ///int nLayersLimit_before, int nLayersLimit_after
  Event<1, int> OnInitialize; ///int nLayersLimit
  Event<1, HRoutingLayer> OnRoutingLayerAllocation;

  ///Operations
  void Initialize(int layersLimit);
  void Grow(int layersLimit);
  HRoutingLayer AllocateLayer();

  //Getters & Setters
  GETTERS_SETTERS_DEFINITION()

ENDHCOLLECTION(HRoutingLayers)

//properties specializations
PROPERTYADECL(HRoutingLayers, string, HRoutingLayer::Name, m_ld->Tech->layerName)
PROPERTYADECL(HRoutingLayers, LayerDirection, HRoutingLayer::Direction, m_ld->Tech->layerDirection)
PROPERTYADECL(HRoutingLayers, double, HRoutingLayer::RPerDist, m_ld->Tech->layerRPerDist)
PROPERTYADECL(HRoutingLayers, double, HRoutingLayer::CPerDist, m_ld->Tech->layerCPerDist)
PROPERTYADECL(HRoutingLayers, double, HRoutingLayer::EdgeCap, m_ld->Tech->layerEdgeCap)
PROPERTYADECL(HRoutingLayers, double, HRoutingLayer::Width, m_ld->Tech->layerWidth)
PROPERTYADECL(HRoutingLayers, double, HRoutingLayer::Pitch, m_ld->Tech->layerPitch)

BEGINWRAPPER(HRoutingLayerWrapper, HRoutingLayers)

  //properties
  PROPERTYWD(string, Name)
  PROPERTYWD(LayerDirection, Direction)
  PROPERTYWD(double, RPerDist)
  PROPERTYWD(double, CPerDist)
  PROPERTYWD(double, EdgeCap)
  PROPERTYWD(double, Width)
  PROPERTYWD(double, Pitch)

ENDWRAPPER(HRoutingLayerWrapper)

inline HRoutingLayers::EnumeratorW HRoutingLayers::GetEnumeratorW()
{ return EnumeratorW(this, 1, m_ld->Tech->nLayers); }

#endif //__HIGH_ROUTINGLAYER_H__
