#ifndef __LRSIZING__
#define __LRSIZING__
#include "HDesign.h"
#include <hdpgrid.h>
#include <vector>
#include "LinearRegression.h"

struct StPEdge
{
  double startX, startY;
  double endX, endY;
};

class LRSizer{
private:
  HDesign& design;
  int size;
  std::vector<double>* LambdaIn;
  std::vector<HCell>* cells;

public:
  LRSizer(HDesign& design);
  ~LRSizer();
  void DoLRSizing();

private:
  std::vector<double>& GetPointLambdas(HTimingPoint pt)
  {
    return LambdaIn[::ToID(pt)];
  }

  void ProjectLambdaMatrix();
  void SolveLrsMu(std::vector<double>& NewVX);
  bool CheckStopConditionForLRS_Mu(std::vector<double> prevVX, std::vector<double> nextVX, double accuracy);
  void SolveLDP();
  double CalculateQ(unsigned int size);
  bool CheckStopConditionForLDP(std::vector<double>& vX, double errorBound);
  void AdjustLambda(int step, std::vector<double>& arrivalTimes,std::vector<double>& vX);

  void InitLambda(double defaultLambda);
  void InitCells(std::vector<HCell>* cells);

  double GetObservedC(HTimingPoint tp, std::vector<double>& vX);
  double GetWeightedResistance(HTimingPoint tp, std::vector<double>& vX, std::vector<double>& vMu);

  void FillVMu(std::vector<double>& vMu);
  void InitVxByLowerBound( std::vector<double>& vX);
  double CalcB(HCell& cell,std::vector<double>& vMu,std::vector<double>& vX);
  double CalcA(HCell& cell,std::vector<double>& vMu,std::vector<double>& vX);
  double CalcNewX(HCell& cell,std::vector<double>& vMu, std::vector<double>& vX);
  double CalcDelay(HTimingPointWrapper& tp, std::vector<double>& vX);
  void UpdateVX(std::vector<double>& newVX, std::vector<double>& vMu);

  double FindOutputLambdaSum(HTimingPoint point);
  double FindInputLambdaSum(HTimingPoint point);

  static int CountLeftArcs(HDesign& design, HNet net);
  static int CountLeftArcs(HDesign& design, HPin pin);

  //debug functions
  HSteinerPointWrapper GetParentStPoint(HSteinerPointWrapper& point,HSteinerPointWrapper& srcPoint);
  double GetInputTimingPointAT(std::vector<double>& vX,HTimingPoint& tp,std::vector<double>& arrivalTimes);
  std::vector<double> GetArrivalTimes(std::vector<double>& vX);
  void LRSizer::initVX(std::vector<double>& vX);
  double TestCalculateInputTPDelay(std::vector<double>& vX, HTimingPoint& tp, 
    std::vector<double>& arrivalTimes);
  double GetOutputTimingPointAT(std::vector<double>& vX, HTimingPoint& tp, 
    std::vector<double>& arrivalTimes);
  bool CheckKuhn_Tucker(HTimingPoint point);
  void CheckKuhn_Tucker();
  double CalcCriterion(std::vector<double>& vCurrentX);
  void PrintPinsCharacteristics();
  void PrintMacroTypeCharacteristics();
  
  void GetCellFamily(HCell cell, std::vector<double>& cellSizes);	
  void GetCellFamily(HCell cell, std::vector<HMacroType>& macroTypesInFamily);

  void GetPinFamilyC(HPin pin, std::vector<double>& C, std::vector<double>& X);
  void GetArcFamilyR(HPin startPin, HPin endPin, std::vector<double>& R, std::vector<double>& invX);


  Maths::Regression::Linear* GetRegressionC(HTimingPointWrapper tp);
  Maths::Regression::Linear* GetRegressionR(HPin inputPin, HPin outputPin);
  HMacroType RoundCellToTypeFromLib(HCell cellFrom, double currentX);

  void ApplySizing(std::vector<double>& X);
  double GetMacroTypeSize(HMacroType macroType);
  string GetMacroTypeFamilyName(HMacroType macroType);
 
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
