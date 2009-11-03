#ifndef __REPORTING_H__
#define __REPORTING_H__

#include "HDesign.h"
#include "VanGinneken.h"

//macro types reports
void ReportCellsByMacroFunction(HDesign& design);
//количество cell имеющих тип MacroType
void ReportCellByMacroType(HDesign& design);
//метод выводит для каждого MacroType: имя, размер, тип, количество пинов, 
//для каждого пина: имя, направление, функциональное назначение, смещение относительно 
//элемента которому принадлежат
void ReportMacroTypeInfo(HDesign& design);

//circuit reports
void ReportPlacementArea(HDesign& design);
void ReportWithTheCharacteristicsOfCircuits(HDesign& design);
void ReportBenchmarkStatistics(HDesign& design);

//nets reports
//распечатка количества двупиновых, трёхпиновых и т.д. нэтов.
void ReportNetsInfo(HDesign& design);
//for converting placement to Fast Router input format
void PrintToFastRouterFormat(HDPGrid& grid, const char* fileName);
void PrintToBoxRouterFormat(HDPGrid& grid, const char* fileName);
//Информация о количестве нэтов каждого типа (all, skipped, buffered, removed, Active).
void ReportCountNetsWithTypes(HDesign& design);
void ReportNetPins(HDesign& hd, HNet net);
void ReportNetPinsCoordinates(HDesign& hd, HNet net);
void ReportNetTiming(HDesign& design, HNet net);

//timing reports
struct CriticalPathsCriticality
{
  double criticality;
  HCriticalPath path;
};

struct NameSortCPC
{  
  bool operator()
    (const CriticalPathsCriticality& a, 
    const CriticalPathsCriticality& b) 
    const { return a.criticality  < b.criticality;  }
};
void ReportNegativeSlacks(HDesign&);
void ReportCriticalPathsToLogFile(HDesign&, int n = -1);
void PlotCriticalPathsAndSaveImages(HDesign&);
void PlotTopCriticalPathsAndSaveImages(HDesign& design, int n = -1);

#endif //__REPORTING_H__