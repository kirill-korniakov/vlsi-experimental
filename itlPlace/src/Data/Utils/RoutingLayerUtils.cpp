#include "RoutingLayerUtils.h"

namespace Utils
{
  void SetHorizontalLayer(HDesign& aDesign, HRoutingLayer layer)
  {
    HWirePhysicalParams& pars = aDesign.RoutingLayers.Physics;

    pars.SetHorizCPerDist(aDesign.RoutingLayers.GetDouble<HRoutingLayer::CPerDist>(layer));
    pars.SetHorizWidth(aDesign.RoutingLayers.GetDouble<HRoutingLayer::Width>(layer));
    pars.SetHorizEdgeCap(aDesign.RoutingLayers.GetDouble<HRoutingLayer::EdgeCap>(layer));
    pars.SetHorizPitch(aDesign.RoutingLayers.GetDouble<HRoutingLayer::Pitch>(layer));
    pars.SetHorizRPerDist(aDesign.RoutingLayers.GetDouble<HRoutingLayer::RPerDist>(layer));
    pars.SetHorizLinearC(pars.HorizCPerDist + 2.0 * pars.HorizEdgeCap);
  }

  void SetVerticalLayer(HDesign& aDesign, HRoutingLayer layer)
  {
    HWirePhysicalParams& pars = aDesign.RoutingLayers.Physics;

    pars.SetVertCPerDist(aDesign.RoutingLayers.GetDouble<HRoutingLayer::CPerDist>(layer));
    pars.SetVertWidth(aDesign.RoutingLayers.GetDouble<HRoutingLayer::Width>(layer));
    pars.SetVertEdgeCap(aDesign.RoutingLayers.GetDouble<HRoutingLayer::EdgeCap>(layer));
    pars.SetVertPitch(aDesign.RoutingLayers.GetDouble<HRoutingLayer::Pitch>(layer));
    pars.SetVertRPerDist(aDesign.RoutingLayers.GetDouble<HRoutingLayer::RPerDist>(layer));
    pars.SetVertLinearC(pars.VertCPerDist + 2.0 * pars.VertEdgeCap);
  }

  void SetUnifiedLayer(HDesign& aDesign)
  {
    HWirePhysicalParams& pars = aDesign.RoutingLayers.Physics;

    pars.SetCPerDist(0.5 * (pars.HorizCPerDist + pars.VertCPerDist));
    pars.SetWidth(0.5 * (pars.HorizWidth + pars.VertWidth));
    pars.SetEdgeCap(0.5 * (pars.HorizEdgeCap + pars.VertEdgeCap));
    pars.SetPitch(0.5 * (pars.HorizPitch + pars.VertPitch));
    pars.SetRPerDist(0.5 * (pars.HorizRPerDist + pars.VertRPerDist));
    pars.SetLinearC(0.5 * (pars.HorizLinearC + pars.VertLinearC));
  }

  void ExtractWirePhysicalParameters(HDesign& aDesign)
  {
    bool hlayer_found = false;
    bool vlayer_found = false;
    HRoutingLayer skipped_layer = aDesign.RoutingLayers.Null();
    HRoutingLayer vert_layer = aDesign.RoutingLayers.Null();
    HRoutingLayer horiz_layer = aDesign.RoutingLayers.Null();

    for(HRoutingLayers::Enumerator i = aDesign.RoutingLayers.GetEnumerator(); i.MoveNext() && !(vlayer_found && hlayer_found); )
      if(::IsNull(skipped_layer))
      {
        skipped_layer = i;
      }
      else if(!hlayer_found && aDesign.RoutingLayers.Get<HRoutingLayer::Direction,LayerDirection>(i) == LayerDirection_Horizontal)
      {
        SetHorizontalLayer(aDesign, horiz_layer = i);
        hlayer_found = true;
      }
      else if(!vlayer_found && aDesign.RoutingLayers.Get<HRoutingLayer::Direction,LayerDirection>(i) == LayerDirection_Vertical)
      {
        SetVerticalLayer(aDesign, vert_layer = i);
        vlayer_found = true;
      }

    if(::IsNull(skipped_layer))
    {
      LOGERROR("No routing layers found. Unable to extract physics for wires.");
      return;
    }
    
    if(!hlayer_found && !vlayer_found)
    {
      LOGWARNING("Only one routing layer is founded. Will use this layer both as horisontal and vertical.");
      SetHorizontalLayer(aDesign, skipped_layer);
      SetVerticalLayer(aDesign, skipped_layer);
    }
    else if(!hlayer_found)
    {
      LOGWARNING("Horizontal routing layer is not founded. Physics from vertical layer will be used.");
      SetHorizontalLayer(aDesign, vert_layer);
    }
    else if(!vlayer_found)
    {
      LOGWARNING("Vertical routing layer is not founded. Physics from horizontal layer will be used.");
      SetVerticalLayer(aDesign, horiz_layer);
    }

    SetUnifiedLayer(aDesign);
  }
}

