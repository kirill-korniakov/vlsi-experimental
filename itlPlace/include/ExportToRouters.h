#ifndef __EXPORT_TO_ROUTERS__
#define __EXPORT_TO_ROUTERS__

#include "HDPGrid.h"

//for converting placement to Fast Router input format
void PrintToFastRouterFormat(HDPGrid& grid, const char* fileName);
void PrintToBoxRouterFormat(HDPGrid& grid, const char* fileName);

#endif