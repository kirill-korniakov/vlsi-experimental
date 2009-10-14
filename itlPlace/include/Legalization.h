#ifndef __LEGALIZATION_H__
#define __LEGALIZATION_H__

#include "HDPGrid.h"

void Legalization(HDPGrid& grid);

void AbacusLegalization(HDPGrid& grid);

//legality checks

bool CheckGridConsistency(HDPGrid& grid, unsigned maxCellsToReport, double sensitivity = 1e-6, bool drawMismatchses = false);
bool CheckGridBoundary(HDPGrid& grid, unsigned maxCellsToReport, bool drawMismatchses = false);
bool CheckOverlaps(HDPGrid& grid, unsigned maxCellsToReport, bool drawOverlaps = false);

#endif //__LEGALIZATION_H__