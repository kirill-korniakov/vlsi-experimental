#ifndef __PINTYPEUTILS_H__
#define __PINTYPEUTILS_H__

#include "HDesign.h"

namespace Utils
{
  PinDirection GetPinDirection(const char* dir);
  const char* GetPinDirectionStr(PinDirection dir);

  HPinType FindMacroTypePinByName(HDesign& design, HMacroType macro, const string& name);
  const char * GetPinFunctionStr(PinFunction pf);
  PinFunction GetPinFunction(const char * name);
};

#endif //__PINTYPEUTILS_H__