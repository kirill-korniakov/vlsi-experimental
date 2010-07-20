#ifndef __BufferingUtils_H__
#define __BufferingUtils_H__

#include "VanGinnekenData.h"

namespace Utils
{
  void PrintVariantsNode(VGVariantsListElement* vGE, int i);
  void PrintVariants(TemplateTypes<VGVariantsListElement>::list* vGList);
}

#endif //__BufferingUtils_H__
