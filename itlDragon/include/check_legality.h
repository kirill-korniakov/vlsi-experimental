/* check_legality.h
 * this is a part of itlDragon
 * Copyright (C) 2006, ITLab, Kirill Kornyakov
 * email: kirillkornyakov@yandex.ru
 */

#ifndef _CHECK_LEGALITY_H_
#define _CHECK_LEGALITY_H_

#include "..\include\global.h"

// checks the legality of the current placement
ITLDRAGON_ERROR CheckLegalityOfPlacement(const Circuit& circuit);
ITLDRAGON_ERROR CheckSiteBelonging(const Circuit& circuit);
ITLDRAGON_ERROR CheckRowBelonging(const Circuit& circuit);
ITLDRAGON_ERROR CheckBorders(const Circuit& circuit);
ITLDRAGON_ERROR CheckXBorders(const Circuit& circuit, int rowIdx);
ITLDRAGON_ERROR CheckOverlaps(const Circuit& circuit);
ITLDRAGON_ERROR CheckOverlaps(const Circuit& circuit, int rowIdxToCheck);

// different chek-ups of bin's structure
ITLDRAGON_ERROR CheckBins(const Circuit& circuit);
ITLDRAGON_ERROR IsNotEmpty(const Circuit& circuit);
ITLDRAGON_ERROR CheckXEquality(const Circuit& circuit);
ITLDRAGON_ERROR CheckYEquality(const Circuit& circuit);
ITLDRAGON_ERROR CheckOrder(const Circuit& circuit);
ITLDRAGON_ERROR CheckBinsOnMacros(const Circuit& circuit);

int CheckArrOfSites(const Circuit& circuit, int** arrOfSites);

ITLDRAGON_ERROR CheckRowLength(const Circuit& circuit, double availableRowLength, int rowIdx);

int PrintBinsSize(const Circuit& circuit);
int PrintBinsCoordinates(const Circuit& circuit);


#endif