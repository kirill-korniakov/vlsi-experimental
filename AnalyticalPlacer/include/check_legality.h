/* check_legality.h
 * this is a part of itlAnalyticalPlacer
 * Copyright (C) 2006, ITLab, Kirill Kornyakov
 * email: kirillkornyakov@yandex.ru
 */

#ifndef _CHECK_LEGALITY_H_
#define _CHECK_LEGALITY_H_

#include "..\include\global.h"

// checks the legality of the current placement
MULTIPLACER_ERROR CheckLegalityOfPlacement(const Circuit& circuit);
MULTIPLACER_ERROR CheckSiteBelonging(const Circuit& circuit);
MULTIPLACER_ERROR CheckRowBelonging(const Circuit& circuit);
MULTIPLACER_ERROR CheckBorders(const Circuit& circuit);
MULTIPLACER_ERROR CheckXBorders(const Circuit& circuit, int rowIdx);
MULTIPLACER_ERROR CheckOverlaps(const Circuit& circuit);
MULTIPLACER_ERROR CheckOverlaps(const Circuit& circuit, int rowIdxToCheck);

// different chek-ups of bin's structure
MULTIPLACER_ERROR CheckBins(const Circuit& circuit);
MULTIPLACER_ERROR IsNotEmpty(const Circuit& circuit);
MULTIPLACER_ERROR CheckXEquality(const Circuit& circuit);
MULTIPLACER_ERROR CheckYEquality(const Circuit& circuit);
MULTIPLACER_ERROR CheckOrder(const Circuit& circuit);
MULTIPLACER_ERROR CheckBinsOnMacros(const Circuit& circuit);

int CheckArrOfSites(const Circuit& circuit, int** arrOfSites);

MULTIPLACER_ERROR CheckRowLength(const Circuit& circuit, double availableRowLength, int rowIdx);

int PrintBinsSize(const Circuit& circuit);
int PrintBinsCoordinates(const Circuit& circuit);


#endif