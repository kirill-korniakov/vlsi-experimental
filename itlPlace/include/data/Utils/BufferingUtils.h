#ifndef __BufferingUtils_H__
#define __BufferingUtils_H__

#include "VGVariantsListElement.h"

namespace Utils
{
  void PrintVariantsNode(VGVariantsListElement* vGE, int i);
  void PrintVariants(TemplateTypes<VGVariantsListElement>::list* vGList);
  int FindRepeat(string str, string find);
}

#endif //__BufferingUtils_H__
