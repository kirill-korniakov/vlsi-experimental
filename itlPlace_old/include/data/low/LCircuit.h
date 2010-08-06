#ifndef __LOW_CIRCUIT_H__
#define __LOW_CIRCUIT_H__

#include "stdTypes.h"

struct LCircuit
{
  double Height; //nm
  double Width;  //nm
  double MinX; //nm  coordinate of bottom left corner of benchmark
  double MinY; //nm  coordinate of bottom left corner of benchmark
  double DefDistanceDBU; //DEF benchmark characteristic
  double XScaleFactor; //custom coefficient to resize distances
  double YScaleFactor; //custom coefficient to resize distances

  double PlacementMinX; //nm
  double PlacementMinY; //nm
  double PlacementMaxX; //nm
  double PlacementMaxY; //nm

  string Name;

  LCircuit()
  {
    Height = 0.0;
    Width = 0.0;
    MinX = 0.0;
    MinY = 0.0;
    XScaleFactor = 1.0;
    YScaleFactor = 1.0;

    Name = "NONAME";
  }
};

#endif //__LOW_CIRCUIT_H__