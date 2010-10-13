#ifndef __LRSIZING__
#define __LRSIZING__
#include "HDesign.h"
#include <hdpgrid.h>
#include <vector>
#include "LinearRegression.h"
#include "LRSizingTimer.h"

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
  LRSizingTimer* timer;

public:
  LRSizer(HDesign& design);
  ~LRSizer();
  void DoLRSizing();

private:
  void InitLambda(double defaultLambda);
  void InitCells(std::vector<HCell>* cells);
  void initVX(std::vector<double>& vX);

  std::vector<double>& GetPointLambdas(HTimingPoint pt);
  
  void SolveLrsMu(std::vector<double>& NewVX);
  void SolveLDP();
  void ProjectLambdaMatrix();
  void AdjustLambda(int step, std::vector<double>& arrivalTimes,std::vector<double>& vX);
  HMacroType RoundCellToTypeFromLib(HCell cellFrom, double currentX);
  void ApplySizing(std::vector<double>& X); 

  double CalculateQ(unsigned int size);
  
  bool CheckStopConditionForLDP(std::vector<double>& vX, double errorBound);
  bool CheckStopConditionForLRS_Mu(std::vector<double> prevVX, std::vector<double> nextVX, double accuracy);

  double GetWeightedResistance(HTimingPoint tp, std::vector<double>& vX, std::vector<double>& vMu);

  void FillVMu(std::vector<double>& vMu);
  void InitVxByLowerBound( std::vector<double>& vX);
  void UpdateVX(std::vector<double>& newVX, std::vector<double>& vMu);
  double CalcA(HCell& cell,std::vector<double>& vMu,std::vector<double>& vX);
  double CalcB(HCell& cell,std::vector<double>& vMu,std::vector<double>& vX);
  double CalcNewX(HCell& cell,std::vector<double>& vMu, std::vector<double>& vX);
  double CalcDelay(HTimingPointWrapper& tp, std::vector<double>& vX);
 
  double FindOutputLambdaSum(HTimingPoint point);
  double FindInputLambdaSum(HTimingPoint point);

  static int CountLeftArcs(HDesign& design, HNet net);
  static int CountLeftArcs(HDesign& design, HPin pin);

  //debug functions
  double TestCalculateInputTPDelay(std::vector<double>& vX, HTimingPoint& tp, std::vector<double>& arrivalTimes);
  bool CheckKuhn_Tucker(HTimingPoint point);
  void CheckKuhn_Tucker();
  void PrintPinsCharacteristics();
  void PrintMacroTypeCharacteristics();
  double CalcCriterion(std::vector<double>& vCurrentX);
};

#endif
