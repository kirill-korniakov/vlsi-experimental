/*
 *     This  file  is  part  of  the  Cadence  LEF/DEF  Open   Source
 *  Distribution,  Product Version 5.7, and is subject to the Cadence
 *  LEF/DEF Open Source License Agreement.   Your  continued  use  of
 *  this file constitutes your acceptance of the terms of the LEF/DEF
 *  Open Source License and an agreement to abide by its  terms.   If
 *  you  don't  agree  with  this, you must remove this and any other
 *  files which are part of the distribution and  destroy any  copies
 *  made.
 *
 *     For updates, support, or to become part of the LEF/DEF Community,
 *  check www.openeda.org for details.
 */

#ifndef defiBLOCKAGES_h
#define defiBLOCKAGES_h

#include <stdio.h>
#include "defiKRDefs.hpp"
#include "defiMisc.hpp"

class defiBlockage {
public:
  defiBlockage();
  void Init();

  void Destroy();
  ~defiBlockage();

  void clear();
  void clearPoly();

  void setLayer(const char* name);
  void setPlacement();
  void setComponent(const char* name);
  void setSlots();
  void setFills();
  void setPushdown();
  void setExceptpgnet();                            // 5.7
  void setSoft();                                   // 5.7
  void setPartial(double maxDensity);               // 5.7
  void setSpacing(int minSpacing);
  void setDesignRuleWidth(int width);
  void addRect(int xl, int yl, int xh, int yh);
  void addPolygon(defiGeometries* geom);

  int hasLayer() const;
  int hasPlacement() const;
  int hasComponent() const;
  int hasSlots() const;
  int hasFills() const;
  int hasPushdown() const;
  int hasExceptpgnet() const;                       // 5.7
  int hasSoft() const;                              // 5.7
  int hasPartial() const;                           // 5.7
  int hasSpacing() const;                           // 5.6
  int hasDesignRuleWidth() const;                   // 5.6
  int minSpacing() const;                           // 5.6
  int designRuleWidth() const;                      // 5.6
  double placementMaxDensity() const;               // 5.7
  const char* layerName() const;
  const char* layerComponentName() const;
  const char* placementComponentName() const;

  int numRectangles() const;
  int xl(int index) const;
  int yl(int index) const;
  int xh(int index) const;
  int yh(int index) const;

  int numPolygons() const;                          // 5.6
  struct defiPoints getPolygon(int index) const;    // 5.6

  void print(FILE* f) const;

protected:
  int    hasLayer_;
  char*  layerName_;
  int    layerNameLength_;
  int    hasPlacement_;
  int    hasComponent_;
  char*  componentName_;
  int    componentNameLength_;
  int    hasSlots_;
  int    hasFills_;
  int    hasPushdown_;                   // 5.7
  int    hasExceptpgnet_ ;               // 5.7
  int    hasSoft_;                       // 5.7
  double maxDensity_;                    // 5.7
  int    minSpacing_;
  int    width_;
  int    numRectangles_;
  int    rectsAllocated_;
  int*   xl_;
  int*   yl_;
  int*   xh_;
  int*   yh_;
  int    numPolys_;                      // 5.6
  int    polysAllocated_;                // 5.6
  struct defiPoints** polygons_;         // 5.6
};


#endif
