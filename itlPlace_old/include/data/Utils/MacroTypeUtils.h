#ifndef __MACROTYPEUTILS_H__
#define __MACROTYPEUTILS_H__

#include "HDesign.h"

namespace Utils
{
  HMacroType FindMacroTypeByName(const HDesign& hd, const string name);

  void TryDetectMacroTypeByName(HMacroTypeWrapper& self);

  const char* GetMacroFunctionName(MacroType mtype);
  MacroType GetMacroFunctionType(const char* name);
};

#endif //__MACROTYPEUTILS_H__