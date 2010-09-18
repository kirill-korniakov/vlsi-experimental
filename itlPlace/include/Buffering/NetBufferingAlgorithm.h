#ifndef __NetBufferingAlgorithm_H__
#define __NetBufferingAlgorithm_H__

#include "VanGinnekenData.h"
#include "BufferPositions.h"
#include "AdditionNewElement.h"
#include "ModificationVanGinnekenList.h"
#include "CreateVGList.h"
#include "HCell.h"
#include "HPin.h"
#include "HPinType.h"
#include "HNet.h"

#define	INFINITY 20000000000.0
#define MAXBUFFERTYPE 64

using namespace Utils;

class NetBufferingAlgorithm
{
public:
  VGAlgorithmData* data;
  AbstractCreateVGListAlgorithm* createVGListAlgorithm;
  AbstractModificationVanGinnekenList* modificationVanGinnekenList;
  AbstractAdditionNewElement* additionNewElement;
  CalculateVGBranchPoint* branchPoint;

  NetBufferingAlgorithm();
  NetBufferingAlgorithm(HDesign& hd);
  ~NetBufferingAlgorithm();
  virtual void Initialize(bool isAgainInitialize = false);
  virtual VGVariantsListElement BufferingNet(HNet& net, bool isRealBuffering = true);

protected:
  bool isInitialize;
  virtual VGVariantsListElement Algorithm(VanGinnekenTree* vGTree);
};

#endif //__NetBufferingAlgorithm_H__