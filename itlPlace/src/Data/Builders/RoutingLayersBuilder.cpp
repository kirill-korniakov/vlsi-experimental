#include "RoutingLayersBuilder.h"
#include "RoutingLayerUtils.h"

namespace Builders
{
  RoutingLayersBuilder::RoutingLayersBuilder(HDesign* design): m_hd(design), m_CurrLayer(&design->RoutingLayers)
  {
    m_layers_started = false;
    m_layers_finished = false;
    m_layer_started = false;
    m_completed = false;
  }

  RoutingLayersBuilder::~RoutingLayersBuilder()
  {
    if(!m_completed)
      CompleteBuild();
  }

  void RoutingLayersBuilder::LayersStart(int size)
  {
    ERROR_ASSERT(!m_layers_started);

    m_hd->RoutingLayers.Initialize(size + 1);

    m_layers_started = true;
  }
  
  void RoutingLayersBuilder::LayersFinished()
  {
    ERROR_ASSERT(!m_layer_started && m_layers_started && !m_layers_finished);

    m_layers_finished = true;
  }

  void RoutingLayersBuilder::LayerStart()
  {
    ERROR_ASSERT(!m_layer_started && m_layers_started && !m_layers_finished);

    m_CurrLayer = m_hd->RoutingLayers.AllocateLayer();

    m_layer_started = true;
  }

  void RoutingLayersBuilder::LayerFinished()
  {
    ERROR_ASSERT(m_layer_started && m_layers_started && !m_layers_finished);

    m_layer_started = false;
  }

  HRoutingLayerWrapper* RoutingLayersBuilder::operator->()
  {
    ERROR_ASSERT(m_layer_started);
    return &m_CurrLayer;
  }

  void RoutingLayersBuilder::CompleteBuild()
  {
    if(!m_layers_started)
      LayersStart(0);
    if(!m_layers_finished)
      LayersFinished();
    Utils::ExtractWirePhysicalParameters(*m_hd);
    m_hd = 0;
    m_completed = true;
  }
}