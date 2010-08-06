#ifndef __PARSER_H__
#define __PARSER_H__

#include "HDesign.h"

void ParseLEF(HDesign& design);
void ParseDEF(HDesign& design);
void ParseLIB(HDesign& design);

void ExportDEF(HDesign& design, const string& defname);
void ExportDEF(HDesign& design, const string& defname, bool useOriginalNames);

void TranslateTechnology(HDesign& design);

#endif //__PARSER_H__
