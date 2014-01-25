#pragma once

//#include "HDesign.h"
class HDesign;
class HNet;
class HPin;
class HTimingPoint;
class HCriticalPath;

#include <string>
using std::string;

struct CrossingReport
{
	string name;
	int crossing;
	int onlyRight;
	int noTree;
};

//macro types reports
void ReportCellsByMacroFunction(HDesign& design);
void ReportCellByMacroType(HDesign& design);
void ReportMacroTypeInfo(HDesign& design);

//circuit reports
void ReportPlacementArea(HDesign& design);
void ReportWithTheCharacteristicsOfCircuits(HDesign& design);
void ReportBenchmarkStatistics(HDesign& design);

//nets reports
void ReportNetsInfo(HDesign& design);
CrossingReport SteinerTreeReport(HDesign& design, HNet net, bool isReport = false, int intWaitTime = 0);
void SteinerWoodReport(HDesign& design, bool IsStepReport = false);

void ReportCountNetsWithTypes(HDesign& design);
void ReportNetPins(HDesign& hd, HNet net);
void ReportNetPinsCoordinates(HDesign& hd, HNet net);
void ReportNetTiming(HDesign& design, HNet net);
void ReportRouting(HDesign& design);
void PrintSteinerTree(HDesign& design, HNet net);

string GetCellPinName(HDesign& design, HPin pin);
string GetCellPinName(HDesign& design, HTimingPoint pt);

void ReportTEPNames(HDesign& design);
void ReportTopologicalOrder(HDesign& design);
void ReportVectorOfSlacks(HDesign& design);
void ReportNegativeSlacks(HDesign&);
void ReportTNSWNSSequence(HDesign& hd, string &tnsStr, string &wnsStr);

void PrintTimingReport(HDesign& hd, int nPaths = -1);
void ReportMacrotypesShort(HDesign& design);
void ReportLibraryPhisics(HDesign& design, int/*SignalDirection*/ sd);
void PrintPath(HDesign& design, HCriticalPath path, int pathNumber);
