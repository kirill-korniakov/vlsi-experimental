#ifndef __LRSIZING__
#define __LRSIZING__
#include "HDesign.h"
#include <hdpgrid.h>
#include <vector>
#include "LinearRegression.h"

class LRSizer{
private:
  HDesign& design;
  int size;
  std::vector<double>* LambdaIn;
  std::vector<HCell>* cells;

public:
  LRSizer(HDesign& design);

  ~LRSizer();
  void doLRSizing();

private:

  std::vector<double>& GetPointLambdas(HTimingPoint pt)
  {
    return LambdaIn[::ToID(pt)];
  }
  

  void ProjectLambdaMatrix();
  void SOLVE_LRS_mu(std::vector<double>& NewVX);
  bool CheckStopConditionForLRS_Mu( std::vector<double> prevVX, std::vector<double> nextVX, double accuracy);
  void SOLVE_LDP();
  double CalculateQ(unsigned int size);
  bool CheckStopConditionForLDP(std::vector<double>& vX, double errorBound);
  void AdjustLambda(int step, std::vector<double>& arrivalTimes,std::vector<double>& vX );

  void InitLambda(double defaultLambda);
  std::vector<HCell>* InitCells();

  double GetObservedC(HTimingPoint tp,std::vector<double>& vX);
  double GetWeightedResistance(HTimingPoint tp, std::vector<double>& vX, std::vector<double>& vMu);

  void FillVMu(std::vector<double>& vMu);
  void initVX( std::vector<double>& vX);
  //void CalculateVC(std::vector<double>& vC, std::vector<HCell>& cells, std::vector<double> vX );
  //void CalculateVR(std::vector<double> vR, std::vector<HCell>& cells, std::vector<double> vX, std::vector<double> vMu );
  //void CalculateVX(std::vector<HCell>& cells, std::vector<double>& vX, std::vector<double> vMu, std::vector<double> vC, std::vector<double> vR );
  //void CalculateVRAndUpdateVX(std::vector<HCell>& cells, std::vector<double>& vX, std::vector<double>& vR,std::vector<double>& vC,std::vector<double>& vMu);
  //double CalcDownstrCapForCell(HCell& currentCell); 
  //double CalcUpstrResistForCell(HCell& currentCell,std::vector<double>& vX,std::vector<double>& vMu);
   
  double CalcB(HCell& cell,std::vector<double>& vMu,std::vector<double>& vX);
  double CalcA(HCell& cell,std::vector<double>& vMu,std::vector<double>& vX);
  double CalcNewX(HCell& cell,std::vector<double>& vMu,std::vector<double>& vX);
  double CalcDelay(HTimingPointWrapper& tp,std::vector<double>& vX);
  void UpdateVX(std::vector<double>& newVX, std::vector<double>& vMu);

  std::vector<double> CalculateArrivalTimes(std::vector<double>& vX);


  double FindOutputLambdaSum(HTimingPoint point);
  double FindInputLambdaSum(HTimingPoint point);
  
  static int CountLeftArcs(HDesign& design, HNet net);
  static int CountLeftArcs(HDesign& design, HPin pin);

  //debug functions
  bool CheckKuhn_Tucker(HTimingPoint point);
  void CheckKuhn_Tucker();
  double CalcCriterion(std::vector<double>& vCurrentX);
	
  void getCellFamily(HCell cell, std::vector<double>& cellSizes);	
  void getCellFamily(HCell cell, std::vector<HMacroType>& macroTypesInFamily);

  void getPinFamilyC(HPin pin, std::vector<double>& C, std::vector<double>& X);
  void getPinFamilyR(HPin pin, std::vector<double>& R, std::vector<double>& X);

  Maths::Regression::Linear* getRegressionC( HTimingPointWrapper tp );
  Maths::Regression::Linear* getRegressionR( HTimingPointWrapper tp );
  HMacroType roundCellToTypeFromLib( HCell cellFrom, double currentX );
 
  void ApplySizing(std::vector<double>& X);
};

class MacroTypeSizesComparator{
public:
  HDesign& design;
  MacroTypeSizesComparator(HDesign& _design):design(_design){}
  bool operator() (const HMacroType &a, const HMacroType &b){
    return design.GetDouble<HMacroType::SizeX>(a)<design.GetDouble<HMacroType::SizeX>(b);
  }
};

#endif
