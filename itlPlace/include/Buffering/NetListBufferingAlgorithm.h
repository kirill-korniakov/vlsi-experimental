#ifndef __NetListBuffering_H__
#define __NetListBuffering_H__

#include "NetBufferingAlgorithm.h"

class BufferingAllCriticalPath : public NetBufferingAlgorithm
{
public:
  BufferingAllCriticalPath(HDesign& hd);
  virtual int BufferingCriticalPath();
};

#endif