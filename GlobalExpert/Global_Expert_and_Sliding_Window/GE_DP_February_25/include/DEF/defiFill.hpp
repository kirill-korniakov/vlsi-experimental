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

#ifndef defiFILL_h
#define defiFILL_h

#include <stdio.h>
#include "defiKRDefs.hpp"
#include "defiMisc.hpp"

class defiFill {
public:
  defiFill();
  void Init();

  void Destroy();
  ~defiFill();

  void clear();
  void clearPoly();
  void clearPts();

  void setLayer(const char* name);
  void setLayerOpc();                             // 5.7
  void addRect(int xl, int yl, int xh, int yh);
  void addPolygon(defiGeometries* geom);
  void setVia(const char* name);                  // 5.7
  void setViaOpc();                               // 5.7
  void addPts(defiGeometries* geom);              // 5.7

  int hasLayer() const;
  const char* layerName() const;
  int hasLayerOpc() const;                        // 5.7

  int numRectangles() const;
  int xl(int index) const;
  int yl(int index) const;
  int xh(int index) const;
  int yh(int index) const;

  int numPolygons() const;                        // 5.6
  struct defiPoints getPolygon(int index) const;  // 5.6

  int hasVia() const;                             // 5.7
  const char* viaName() const;                    // 5.7
  int hasViaOpc() const;                          // 5.7

  int numViaPts() const;                          // 5.7
  struct defiPoints getViaPts(int index) const;   // 5.7

  void print(FILE* f) const;

protected:
  int   hasLayer_;
  char* layerName_;
  int   layerNameLength_;
  int   layerOpc_;                  // 5.7
  int   numRectangles_;
  int   rectsAllocated_;
  int*  xl_;
  int*  yl_;
  int*  xh_;
  int*  yh_;
  int   numPolys_;                  // 5.6
  int   polysAllocated_;            // 5.6
  struct defiPoints** polygons_;    // 5.6
  int   hasVia_;                    // 5.7
  char* viaName_;                   // 5.7
  int   viaNameLength_;             // 5.7
  int   viaOpc_;                    // 5.7
  int   numPts_;                    // 5.7
  int   ptsAllocated_;              // 5.7
  struct defiPoints** viaPts_;      // 5.7

};


#endif
