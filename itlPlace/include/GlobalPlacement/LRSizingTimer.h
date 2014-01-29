#ifndef __LRSIZINGTIMER__
#define __LRSIZINGTIMER__

#include "LinearRegression.h"
#include "HDesign.h"
#include "HDPGrid.h"
#include <vector>
class LRSizingTimer
{
private:
  int size;
  HDesign& design;
  std::vector<HCell>* cells;

public:
  LRSizingTimer(std::vector<HCell>* _cells, int _size, HDesign& hd): design(hd), size(_size), cells(_cells){}
  std::vector<double> GetArrivalTimes(std::vector<double>& vX);
  double GetMacroTypeSize(HMacroType macroType);
  void GetCellFamily(HCell cell, std::vector<HMacroType>& macroTypesInFamily);
  void GetCellFamily(HCell cell, std::vector<double>& cellSizes);
  void PrintMacroTypeCharacteristics();
  void PrintPinsCharacteristics();
private:
  Maths::Regression::Linear* GetRegressionC(HTimingPointWrapper tp);
  Maths::Regression::Linear* GetRegressionR(HPin inputPin, HPin outputPin);
 
  double GetObservedC(HTimingPoint tp,std::vector<double>& vX);

  void GetPinFamilyC(HPin pin, std::vector<double>& C, std::vector<double>& X);
  void GetArcFamilyR(HPin startPin, HPin endPin, std::vector<double>& R, std::vector<double>& invX);

  double GetInputTimingPointAT(std::vector<double>& vX,HTimingPoint& tp,std::vector<double>& arrivalTimes);
  double GetOutputTimingPointAT(std::vector<double>& vX, HTimingPoint& tp, std::vector<double>& arrivalTimes);

  HSteinerPointWrapper GetParentStPoint(HSteinerPointWrapper& point,HSteinerPointWrapper& srcPoint);
  std::string GetMacroTypeFamilyName(HMacroType macroType);
};

class MacroTypeSizesComparator{
public:
  HDesign& design;
  MacroTypeSizesComparator(HDesign& _design) : design(_design){}
  bool operator() (const HMacroType &a, const HMacroType &b){
    return design.GetDouble<HMacroType::SizeX>(a) < design.GetDouble<HMacroType::SizeX>(b);
  }
};
#endif
