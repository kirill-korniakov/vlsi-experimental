#include "AdaptiveRoute.h"
#include "FLUTERoute.h"
#include "HPWLRoute.h"

void AdaptiveRoute(HDesign& aDesign, HNet& aNet)
{
  FLUTERoute(aDesign, aNet);
  if(aDesign.Get<HWire::RoutingType, RoutingType>(aDesign.Wires[aNet]) == RoutingType_Unrouted)
    HPWLRoute(aDesign, aNet);
}

void AdaptiveRoute(HDesign& aDesign)
{
  ConfigContext ctx(aDesign.cfg.OpenContext("AdaptiveRoute"));
  //ALERT("ADAPTIVE (FLUTE/HPWL) ROUTING STARTED...");
  for(HNets::ActiveNetsEnumeratorW niter = aDesign.Nets.GetActiveNetsEnumeratorW(); niter.MoveNext(); )
    AdaptiveRoute(aDesign, niter);
  //ALERT("ADAPTIVE ROUTING FINISHED");
}
