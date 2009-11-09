#ifndef __MACROTYPEUTILS_H__
#define __MACROTYPEUTILS_H__

#include "HDesign.h"

namespace Utils
{
  HMacroType FindMacroTypeByName(const HDesign& hd, const string name);

  void TryDetectMacroTypeByName(HMacroTypeWrapper& self);

  const char* GetMacroFunctionName(MacroType mtype);
  MacroType GetMacroFunctionType(const char* name);

  void CalcElementTRC(HDesign& design, HMacroType elementType, double* T, double* R, double* C);
  void CalcElementLD(HDesign& design, HMacroType elementType, double* L, double* D);
  void CalcBufferLD(HDesign& design, const char* name, double* L, double* D);
};

#endif //__MACROTYPEUTILS_H__