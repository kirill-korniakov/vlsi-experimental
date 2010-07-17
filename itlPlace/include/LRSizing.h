#ifndef LRSizing_h
#define LRSizing_h

#include "HDesign.h"
#include "Utils.h"
#include <algorithm>
#include <hdpgrid.h>
#include <STA.h>
#include <stack>

struct LambdaMatrix{
		std::vector<std::vector<int>> MLambda;
		LambdaMatrix(HDPGrid& grid, HDesign& design);
};

void DOSizing(HDPGrid& grid, HDesign& design);

#endif