/*  
* Line.h
* this is a part of itlAnalyticalPlacer
* Copyright (C) 2009, ITLab, Belyakov
* email: alexander.belyakov@mail.itlab.unn.ru
*/

#ifndef __LINE_H__
#define __LINE_H__

bool IsEqual(double value1, double value2, double eps = 0.0001);

//enum LineType {vertical, horizontal, general}
//class Line  for storing lines
//y=kx+b
//or x=c
class Line
{
protected:  
  int lineType; //type of the line:
                //0 - general, 1 - vertical, 2 - horizontal
  double k;
  double b;
  double x1;
  double y1;
  double x2;
  double y2;
  
 public:
  Line(double _x1, double _y1, double _x2, double _y2);
  ~Line() {}
  double GetX1() { return x1; }
  double GetY1() { return y1; }
  double GetX2() { return x2; }
  double GetY2() { return y2; }
  double GetX(double _y);
  double GetY(double _x);
  void Print();
};

#endif