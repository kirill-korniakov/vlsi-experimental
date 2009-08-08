/*  
* Line.h
* this is a part of itlAnalyticalPlacer
* Copyright (C) 2009, ITLab, Belyakov
* email: alexander.belyakov@mail.itlab.unn.ru
*/

#include "Line.h"
#include <math.h>
#include <stdio.h>

bool IsEqual(double value1, double value2, double eps)
{
  if (fabs(value1 - value2) < eps)
    return true;
  else
    return false;
}

Line::Line(double _x1, double _y1, double _x2, double _y2): x1(_x1), y1(_y1),
                                                            x2(_x2), y2(_y2)
{
  if ((IsEqual(x1, x2)) && (IsEqual(y1, y2)))
  {
    printf("error: points are equal!\n");
    lineType = 1;
    k = x1;
  }
  
  else if (IsEqual(x1, x2))
  {
    lineType = 1;
    k = x1;
  }
  
  else if (IsEqual(y1, y2))
  {
    lineType = 2;
    b = y1;
  }
  
  else
  {
    lineType = 0;
    k = (y1 - y2) / (x1 - x2);
    b = (y2 * x1 - y1 * x2) / (x1 - x2);
    
    if (fabs(b) < 0.001)
      b = 0;
  }
}

double Line::GetX(double _y)
{
  switch (lineType)
  {
    case 0: //general
      return ((_y - b) / k);
      break;
    case 1: //vertical
      return x1;
      break;
    default:
      printf("error: this line is horizontal\n");
      return -1;
  }
}

double Line::GetY(double _x)
{
  switch (lineType)
  {
  case 0: //general
    return (k * _x + b);
    break;  
  case 2:
    return b; //horizontal
    break;
  default:
    printf("error: this line is vertical\n");
    return -1;
  }    
}

void Line::Print()
{
  printf("type = %d\nk = %f\nb = %f\n", lineType, k, b);
}