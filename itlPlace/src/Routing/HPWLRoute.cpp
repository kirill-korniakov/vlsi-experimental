#include "HPWLRoute.h"
#include <Math.h>

void HPWLRoute(HDesign& aDesign, HNet& aNet)
{
  HNetWrapper net = aDesign[aNet];
  int numOfPins = net.PinsCount();
  DEBUG_ASSERT(numOfPins > 1);

  HWireWrapper wire = aDesign[aDesign.Wires[aNet]];

  //remove old tree if presents
  if(wire.RoutingType() != RoutingType_Unrouted)
    aDesign.SteinerPoints.RemoveSteinerTree(wire.Root());

  double wl = 0.0;
  int pos = 0;
  HSteinerPointWrapper current = aDesign[wire.Root()];
  current.InheritPinCoordinates();
  double root_x = current.X();
  double root_y = current.Y();

  for(HNet::SinksEnumeratorW piter = net.GetSinksEnumeratorW(); piter.MoveNext(); pos++)
  {
    aDesign.SteinerPoints.InheritPinCoordinates(aDesign.SteinerPoints[piter]);
    wl += fabs(piter.X() - root_x) + fabs(piter.Y() - root_y);
    if(pos < numOfPins - 1)
    {
      current.SetRight(aDesign.SteinerPoints[piter]);
      if(pos < numOfPins - 2)
      {
        current.SetLeft(aDesign.SteinerPoints.AllocateSteinerPoint());
        current.GoLeft();
        current.SetX(root_x);
        current.SetY(root_y);
      }
    }
    else
      current.SetLeft(aDesign.SteinerPoints[piter]);
  }// for(HNet::SinkPinEnumerator piter ...
  
  wire.SetRoutingType(RoutingType_HPWL);
  wire.SetLength(wl);
}

void HPWLRoute(HDesign &aDesign)
{
  for(HNets::ActiveNetsEnumeratorW niter = aDesign.Nets.GetNetsEnumeratorW(); niter.MoveNext(); )
    HPWLRoute(aDesign, niter);
}