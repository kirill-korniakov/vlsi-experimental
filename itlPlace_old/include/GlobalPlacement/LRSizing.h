#ifndef __LRSIZING__
#define __LRSIZING__
#include "HDesign.h"
#include <hdpgrid.h>
#include <vector>

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
  bool CheckStopConditionForLDP(std::vector<double> vX, double errorBound );
  void AdjustLambda(int step, std::vector<double> arrivalTimes );

  void InitLambda(double defaultLambda);
  std::vector<HCell>* InitCells();

  double GetObservedC(HTimingPoint tp);
  double GetWeightedResistance(HTimingPoint tp, std::vector<double>& vX, std::vector<double>& vMu);

  void FillVMu(std::vector<double>& vMu);
  void initVX( std::vector<double>& vX, std::vector<HCell>& cells );
  void CalculateVC(std::vector<double>& vC, std::vector<HCell>& cells, std::vector<double> vX );
  void CalculateVR(std::vector<double> vR, std::vector<HCell>& cells, std::vector<double> vX, std::vector<double> vMu );
  void CalculateVX(std::vector<HCell>& cells, std::vector<double>& vX, std::vector<double> vMu, std::vector<double> vC, std::vector<double> vR );
  std::vector<double> CalculateArrivalTimes();

  double FindOutputLambdaSum(HTimingPoint point);
  double FindInputLambdaSum(HTimingPoint point);
  
  static int CountLeftArcs(HDesign& design, HNet net);
  static int CountLeftArcs(HDesign& design, HPin pin);

  //debug functions
  bool CheckKuhn_Tucker(HTimingPoint point);
  void CheckKuhn_Tucker();
};
#endif
