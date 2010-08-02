#ifdef nothing

#include "HDesign.h"
#include "Utils.h"
#include <algorithm>
#include <hdpgrid.h>
#include <STA.h>
#include <stack>
#include "DelayPropagation.h"

#include "Timing.h"
#include "Reporting.h"

#define accuracyForLRS_Mu 0.1
#define errorBoundForLDP 0.1

enum ConnectionType 
{
		Input,
		Output,
		NDef
};

enum FirstOrLast 
{
		First,
		Last,
		General,
		UndefinedType
};


struct LamdbaData
{	
		double value;
		ConnectionType Direction;
		FirstOrLast type;

		LamdbaData(double _Lambda,ConnectionType _Direction, FirstOrLast _type):value(_Lambda),Direction(_Direction),type(_type){};
};

typedef std::vector<std::vector<LamdbaData>> LambdaMatrixType;

struct LambdaMatrix
{
		LambdaMatrixType matrix;
		LambdaMatrix(HDesign&, std::vector<HTimingPoint>&);

		int TimingPoint2index(std::vector<HTimingPoint>& timingPoints, HTimingPoint& timingPoint ) 
		{
				for(unsigned int i = 0; i < timingPoints.size();i++)
				{
						if (timingPoints[i]==timingPoint) return i;
				}
				ALERT("TimingPoint2index error: tp is not in vector!");
				return -1;
		}
};


std::vector<HCell>* InitVCircuit(HDPGrid& grid, HDesign& design);
//LambdaMatrix* NewLambdaMatrix(HDPGrid& grid, HDesign& design,std::vector<HCell>& vCircuit);
void SOLVE_LDP(HDPGrid& grid, HDesign& design,std::vector<HCell>& vCircuit, std::vector<HTimingPoint>& timingPoints, LambdaMatrix& Lambda);
double FindInputLambdaSum(LambdaMatrix& Lambda, int index);
double FindOutputLambdaSum(LambdaMatrix& Lambda, int index);
void DoLRSizing(HDPGrid& grid, HDesign& design);
std::vector<double> SOLVE_LRS_mu(HDesign& design,std::vector<HCell>& vCircuit, std::vector<HTimingPoint>& timingPoints, LambdaMatrix& Lambda);

#endif