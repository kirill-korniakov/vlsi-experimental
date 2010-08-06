#ifndef __SUM_OF_DELAYS_DATA__
#define __SUM_OF_DELAYS_DATA__

#include "HDesign.h"

struct NetSODData
{
  double Lnet;
  double braces;
};

class SODData
{
 public:
  double    Lbuf;
  double    Dbuf;
  double    DbufLbufDifferenceOfSquares;
  double    gradientBalance;
  std::vector<NetSODData> netSODData;
  
  void Initialize(HDesign& hd);

 private:
  void InitializeNets(HDesign& hd);
};

#endif