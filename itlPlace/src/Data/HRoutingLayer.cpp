#include "HDesign.h"

COLLECTIONCONSTRUCTOR(HRoutingLayers)

void HRoutingLayers::Initialize(int layersLimit)
{
  CHECKIFINITIALIZED();

  Grow(layersLimit);
  m_ld->Tech->layerCPerDist[0] = 0;
  m_ld->Tech->layerDirection[0] = LayerDirection_Default;
  m_ld->Tech->layerEdgeCap[0] = 0;
  m_ld->Tech->layerName[0] = "NULL";
  m_ld->Tech->layerPitch[0] = 0;
  m_ld->Tech->layerRPerDist[0] = 0;
  m_ld->Tech->layerWidth[0] = 0;

  m_ld->Tech->nLayers = 1;
  OnInitialize(layersLimit);
}

void HRoutingLayers::Grow(int layersLimit)
{
  int old_size = m_ld->Tech->nLayersLimit;
  ::Grow(&m_ld->Tech->layerCPerDist, old_size, layersLimit);
  ::Grow(&m_ld->Tech->layerDirection, old_size, layersLimit);
  ::Grow(&m_ld->Tech->layerEdgeCap, old_size, layersLimit);
  ::Grow(&m_ld->Tech->layerName, old_size, layersLimit);
  ::Grow(&m_ld->Tech->layerPitch, old_size, layersLimit);
  ::Grow(&m_ld->Tech->layerRPerDist, old_size, layersLimit);
  ::Grow(&m_ld->Tech->layerWidth, old_size, layersLimit);
  m_ld->Tech->nLayersLimit = layersLimit;
  OnGrow(old_size, layersLimit);
}

HRoutingLayer HRoutingLayers::AllocateLayer()
{
  while (m_ld->Tech->nLayers >= m_ld->Tech->nLayersLimit)
    Grow(GetNewSize(m_hd->cfg, m_ld->Tech->nLayersLimit, "data.routingLayersLimitGrow", 1, 0, 1.25));
  
  HRoutingLayer res = ::__ConstructPtr<HRoutingLayer>(m_ld->Tech->nLayers);
  m_ld->Tech->nLayers++;

  OnRoutingLayerAllocation(res);

  return res;
}