#ifndef __DETAILED_PLACEMENT_H__
#define __DETAILED_PLACEMENT_H__

#include "HDPGrid.h"
#include "ConstraintsController.h"

int GetOptimalRowIdx(HDPGrid& grid, HCell cell);
void GetOptimalPosition(HDPGrid& grid, HCell cell, int& rowIdx, int& columnIdx);
                
void HorizontalSearch(HDPGrid& grid, ConstraintsController* checker);
void VerticalSearch(HDPGrid& grid, ConstraintsController* checker);
void GlobalSwap(HDPGrid& grid, ConstraintsController* checker);

void DetailedPlacement(HDPGrid& grid);

#endif