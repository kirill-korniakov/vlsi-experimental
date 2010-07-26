#ifndef LRSizing_h
#define LRSizing_h

#include "HDesign.h"
#include "Utils.h"
#include <algorithm>
#include <hdpgrid.h>
#include <STA.h>
#include <stack>

#define accuracyForLRS_Mu 0.1

enum ConnectionType 
{
		Input,
		Output,
		NDef
};

struct LamdbaData
{	
		double value;
		ConnectionType Direction;

		LamdbaData(double _Lambda,ConnectionType _Direction):value(_Lambda),Direction(_Direction){};
};

typedef std::vector<std::vector<LamdbaData>> LambdaMatrixType;

struct LambdaMatrix
{
		LambdaMatrixType matrix;
		LambdaMatrix(HDPGrid& grid, HDesign& design, std::vector<HCell>& vCircuit);
};


std::vector<HCell>* InitVCircuit(HDPGrid& grid, HDesign& design);
//LambdaMatrix* NewLambdaMatrix(HDPGrid& grid, HDesign& design,std::vector<HCell>& vCircuit);
void SOLVE_LDP(HDPGrid& grid, HDesign& design,std::vector<HCell>& vCircuit, LambdaMatrix& Lambda);
double FindInputLambdaSum(LambdaMatrix& Lambda, int index);
double FindOutputLambdaSum(LambdaMatrix& Lambda, int index);
void DoLRSizing(HDPGrid& grid, HDesign& design);
std::vector<double> SOLVE_LRS_mu(HDPGrid& grid, HDesign& design,std::vector<HCell>& vCircuit, LambdaMatrix& Lambda);

#endif