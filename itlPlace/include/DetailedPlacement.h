#ifndef __DETAILED_PLACEMENT_H__
#define __DETAILED_PLACEMENT_H__

#include "HDPGrid.h"

int GetOptimalRowIdx(HDPGrid& grid, HCell cell);
void GetOptimalPosition(HDPGrid& grid, HCell cell, int& rowIdx, int& columnIdx);
                
void HorizontalSearch(HDPGrid& grid);
void VerticalSearch(HDPGrid& grid);
void GlobalSwap(HDPGrid& grid);

void DetailedPlacement(HDPGrid& grid);

#endif