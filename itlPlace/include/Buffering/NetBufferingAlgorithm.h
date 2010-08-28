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

struct AppCtx;
class HWirePhysicalParams;

#define	INFINITY	20000000000.0
#define MAXBUFFERTYPE 64

using namespace Utils;

class NetBufferingAlgorithm
{
protected:
  bool isInitialize;
  virtual VGVariantsListElement Algorithm(VanGinnekenTree* vGTree);
public:
  VGAlgorithmData* data;
  AbstractCreateVGListAlgorithm* createVGListAlgorithm;
  AbstractModificationVanGinnekenList* modificationVanGinnekenList;
  AbstractAdditionNewElement* additionNewElement;

  NetBufferingAlgorithm(HDesign& hd);
  virtual void Initialize(bool isAgainInitialize = false);

  virtual VGVariantsListElement BufferingNen(HNet& net, bool isRealBuffering = true, AppCtx* context = NULL);
};

#endif //__NetBufferingAlgorithm_H__