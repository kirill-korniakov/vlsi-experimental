#ifndef __LOW_DETAILEDPLACEMENT_H__
#define __LOW_DETAILEDPLACEMENT_H__

#include "stdTypes.h"
#include "LTech.h"
#include "LNetList.h"

struct LDetailedPlacement
{
  //Placement rows
  string *rowName;
  IDType *rowSite;
  CellOrientation *rowSiteOrient;
  double *rowX;
  double *rowY;
  double *rowStepX;
  double *rowStepY;
  int *rowXSitesNum;
  int *rowYSitesNum;

  IDType *rowNextRow;
  IDType *rowPreviousRow;

  int nRows;
  int nRowsLimit;

  LDetailedPlacement()
  {
    rowName = 0;
    rowSite = 0;
    rowSiteOrient = 0;
    rowX = 0;
    rowY = 0;
    rowStepX = 0;
    rowStepY = 0;
    rowXSitesNum = 0;
    rowYSitesNum = 0;
    nRows = 0;
    nRowsLimit = 0;
    rowNextRow = 0;
    rowPreviousRow = 0;
  }
};

#endif //__LOW_DETAILEDPLACEMENT_H__

