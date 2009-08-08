#ifndef __ROUTINGLAYERSBUILDER_H__
#define __ROUTINGLAYERSBUILDER_H__

#include "HDesign.h"

namespace Builders
{
  class RoutingLayersBuilder
  {
  private:
    HDesign* m_hd;
    bool m_layers_started;
    bool m_layers_finished;
    bool m_layer_started;
    bool m_completed;
    HRoutingLayerWrapper m_CurrLayer;

  public:
    RoutingLayersBuilder(HDesign* design);
    ~RoutingLayersBuilder();

    void LayersStart(int size = 0);
    void LayersFinished();

    void LayerStart();
    void LayerFinished();

    HRoutingLayerWrapper* operator->();

    void CompleteBuild();
  };
}

#endif //__ROUTINGLAYERSBUILDER_H__