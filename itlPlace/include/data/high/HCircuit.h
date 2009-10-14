#ifndef __HIGH_CIRCUIT_H__
#define __HIGH_CIRCUIT_H__

#include "LDesign.h"

class HCircuit
{

  LDesign *m_ld;

public:
  HCircuit(LDesign *_d): m_ld(_d)
  {
  }

  //getters
  string Name() const {return m_ld->Circuit.Name; }
  double Height() const {return m_ld->Circuit.Height; }
  double Width() const {return m_ld->Circuit.Width; }
  double MinX() const {return m_ld->Circuit.MinX; }
  double MinY() const {return m_ld->Circuit.MinY; }
  double DefDistanceDBU() const {return m_ld->Circuit.DefDistanceDBU; }
  double XScaleFactor() const {return m_ld->Circuit.XScaleFactor; }
  double YScaleFactor() const {return m_ld->Circuit.YScaleFactor; }

  double PlacementMinX() const {return m_ld->Circuit.PlacementMinX; }
  double PlacementMinY() const {return m_ld->Circuit.PlacementMinY; }
  double PlacementMaxX() const {return m_ld->Circuit.PlacementMaxX; }
  double PlacementMaxY() const {return m_ld->Circuit.PlacementMaxY; }

  double PlacementWidth() const {return m_ld->Circuit.PlacementMaxX - m_ld->Circuit.PlacementMinX; }
  double PlacementHeight() const {return m_ld->Circuit.PlacementMaxY - m_ld->Circuit.PlacementMinY; }

  //setters
  void SetName(const string& name) {m_ld->Circuit.Name = name;}
  void SetHeight(const double& height) {m_ld->Circuit.Height = height;}
  void SetWidth(const double& width) {m_ld->Circuit.Width = width;}
  void SetMinX(const double& minx) {m_ld->Circuit.MinX = minx;}
  void SetMinY(const double& miny) {m_ld->Circuit.MinY = miny;}
  void SetDefDistanceDBU(const double& dbu) {m_ld->Circuit.DefDistanceDBU = dbu;}
  void SetXScaleFactor(const double& xf) { m_ld->Circuit.XScaleFactor = xf; }
  void SetYScaleFactor(const double& yf) { m_ld->Circuit.YScaleFactor = yf; }

  void SetPlacementMinX(const double& minx) { m_ld->Circuit.PlacementMinX = minx; }
  void SetPlacementMinY(const double& miny) { m_ld->Circuit.PlacementMinY = miny; }
  void SetPlacementMaxX(const double& maxx) { m_ld->Circuit.PlacementMaxX = maxx; }
  void SetPlacementMaxY(const double& maxy) { m_ld->Circuit.PlacementMaxY = maxy; }
};


#endif //__HIGH_CIRCUIT_H__