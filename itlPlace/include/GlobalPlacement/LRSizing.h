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
  void doLRSizing(){;}

private:

  std::vector<double>& GetPointLambdas(HTimingPoint pt)
  {
    return LambdaIn[::ToID(pt)];
  }
  

  void ProjectLambdaMatrix();
  void SOLVE_LRS_mu(std::vector<HCell>& cells, std::vector<double>& vX);
  bool CheckStopConditionForLRS_Mu( std::vector<double> prevVX, std::vector<double> nextVX, double accuracy);
  void SOLVE_LDP(std::vector<HCell>& cells);
  double CalculateQ(std::vector<double>& vX);
  bool CheckStopConditionForLDP(std::vector<double>& vX, double errorBound );
  void AdjustLambda(int step, std::vector<double> arrivalTimes );

  void InitLambda(double defaultLambda);
  std::vector<HCell>* InitCells();

  double GetObservedC(HTimingPoint tp);
  double GetWeightedResistance(HTimingPoint tp, std::vector<double>& vX, std::vector<double>& vMu);

  void FillVMu(std::vector<double>& vMu);
  void initVX( std::vector<double>& vX, std::vector<HCell>& cells );
  //void CalculateVC(std::vector<double>& vC, std::vector<HCell>& cells, std::vector<double> vX );
  //void CalculateVR(std::vector<double> vR, std::vector<HCell>& cells, std::vector<double> vX, std::vector<double> vMu );
  //void CalculateVX(std::vector<HCell>& cells, std::vector<double>& vX, std::vector<double> vMu, std::vector<double> vC, std::vector<double> vR );
  //void CalculateVRAndUpdateVX(std::vector<HCell>& cells, std::vector<double>& vX, std::vector<double>& vR,std::vector<double>& vC,std::vector<double>& vMu);
  //double CalcDownstrCapForCell(HCell& currentCell); 
  //double CalcUpstrResistForCell(HCell& currentCell,std::vector<double>& vX,std::vector<double>& vMu);
   
  double CalcB(HCell& cell,std::vector<double>& vMu);
  double CalcA(HCell& cell,std::vector<double>& vMu,std::vector<double>& vX);
  double CalcNewX(HCell& cell,std::vector<double>& vMu,std::vector<double>& vX);
  void UpdateVX(std::vector<HCell>& cells, std::vector<double>& newVX, std::vector<double>& vMu);

  std::vector<double> CalculateArrivalTimes();


  double FindOutputLambdaSum(HTimingPoint point);
  double FindInputLambdaSum(HTimingPoint point);
  
  static int CountLeftArcs(HDesign& design, HNet net);
  static int CountLeftArcs(HDesign& design, HPin pin);

  //debug functions
  bool CheckKuhn_Tucker(HTimingPoint point);
  void CheckKuhn_Tucker();
	
	
	void getPinFamilyC(HPin pin, std::vector<double>& C, std::vector<double>& X);
  Maths::Regression::Linear* getRegressionC( HTimingPointWrapper tp );
};
#endif
