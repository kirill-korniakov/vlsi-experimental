/*  
* Tile.h
* this is a part of itlAnalyticalPlacer
* Copyright (C) 2009, ITLab, Belyakov
* email: alexander.belyakov@mail.itlab.unn.ru
*/

#ifndef __TILE_H__
#define __TILE_H__

class Tile
{
protected:
  double x; //coordinates of the left down corner
  double y;
  int nWires; //number of wires in this tile
  int nCriticalWires;
  int nPins;

  //------------------------methods----------------------------------------------
public:
  Tile(): x(0), y(0), nWires(0), nCriticalWires(0), nPins(0) {}
  Tile(int _x, int _y, int _nPins): x(_x), y(_y), nPins(_nPins) {}
  void SetXY(double _x, double _y) { x = _x; y = _y; }
  double GetX() { return x; }
  double GetY() { return y; }
  int GetNWires() { return nWires; }
  int GetNCriticalWires() { return nCriticalWires; }
  int GetNPins() { return nPins; }
  void IncNWires() { nWires++; }
  void IncNCriticalWires() { nCriticalWires++; }
  void IncNPins() { nPins++; }
  ~Tile() {}

};

#endif