#ifndef LRSizing_h
#define LRSizing_h

#include "HDesign.h"
#include "Utils.h"
#include <algorithm>
#include <hdpgrid.h>
#include <STA.h>
#include <stack>

enum ConnectionType 
{
		Input,
		Output,
		NDef
};

struct LamdbaData
{	
		double Lambda;
		ConnectionType Direction;

		LamdbaData(double _Lambda,ConnectionType _Direction):Lambda(_Lambda),Direction(_Direction){};
};

typedef std::vector<std::vector<LamdbaData>> LambdaMatrixType;

struct LambdaMatrix
{
		LambdaMatrixType lambdaMatrix;
		LambdaMatrix(HDPGrid& grid, HDesign& design, std::vector<HCell>& vCircuit);
};


std::vector<HCell>* InitVCircuit(HDPGrid& grid, HDesign& design);
//LambdaMatrix* NewLambdaMatrix(HDPGrid& grid, HDesign& design,std::vector<HCell>& vCircuit);
void SOLVE_LDP(HDPGrid& grid, HDesign& design,std::vector<HCell>& vCircuit, LambdaMatrix MLambda);
double FindInputLambdaSum(LambdaMatrix& MLambda, int index);
double FindOutputLambdaSum(LambdaMatrix& MLambda, int index);
void DoLRSizing(HDPGrid& grid, HDesign& design);

#endif