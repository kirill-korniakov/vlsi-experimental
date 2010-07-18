#include <math.h>
#include <float.h>
#include "LRSizing.h"
#include "Legalization.h"
#include "Parser.h"
#include <time.h>
#include "Timing.h"

#include <iostream>

using namespace std;

int cell2index(std::vector<HCell>& vCircuit, HCell cell)
{
		std::vector<HCell>::iterator iterCircuit=vCircuit.begin();
		int counter=1;
		for(;iterCircuit != vCircuit.end();iterCircuit++,counter++){
				if (*iterCircuit == cell) return counter;
		}
		return -1;
}

HCell index2cell(std::vector<HCell>& vCircuit, unsigned int index)
{
		if(index > 0 && index < vCircuit.size()+1)
				return vCircuit[index-1];
		else 
		{
				ALERT("index2cell:!!ERROR!!index out of vCircuit bounds!!");
				return vCircuit[0];
		}
}

std::vector<HCell>* InitVCircuit(HDPGrid& grid, HDesign& design)
{
		std::vector<HCell>* vCircuit = new std::vector<HCell>;
		int index = 1;
		HCell tempCell;

		HTimingPoint firstPoint = design.Get<HTimingPoint::PreviousPoint, HTimingPoint>(design.TimingPoints.FirstInternalPoint());

		for (HTimingPointWrapper pt = design[design.TimingPoints.TopologicalOrderRoot()];
				pt.GoPrevious() != firstPoint; )
		{
				CRITICAL_ASSERT(!::IsNull(pt));
				HPin pin = pt.Pin();
				tempCell=design[pin].Cell();
				if (::IsNull(tempCell))
				{
						ALERT("NULL");
						continue;
				}

				if (cell2index(*vCircuit,tempCell) < 0)
				{
						vCircuit->push_back(tempCell);
						ALERT(" inserted index=%d",index);
						index++;
				}
				else 
						ALERT("duplicate index=%d",cell2index(*vCircuit,tempCell));
		}
		return (vCircuit);
}

double FindInputLambdaSum(LambdaMatrix& MLambda, int index)
{
		double InputLambdaSum=0;
		for (int j=0;j<MLambda.lambdaMatrix.size();j++)
		{
				if ((MLambda.lambdaMatrix[index][j]).Direction==Input)
				InputLambdaSum+=(MLambda.lambdaMatrix[index][j]).Lambda;
		}
		return InputLambdaSum;
}

double FindOutputLambdaSum(LambdaMatrix& MLambda, int index)
{
		double OutputLambdaSum=0;
		for (int j=0;j<MLambda.lambdaMatrix.size();j++)
		{
				if ((MLambda.lambdaMatrix[index][j]).Direction==Output)
						OutputLambdaSum+=(MLambda.lambdaMatrix[index][j]).Lambda;
		}
		return OutputLambdaSum;
}

bool CheckKuhn_Tucker (LambdaMatrix& MLambda, int index)
{
		if (((FindOutputLambdaSum(MLambda,index))-FindInputLambdaSum(MLambda, index))<1e-9) 
				return true;
		else 
				return false;
}

void ProjectLambdaMatrix(LambdaMatrix& MLambda)
{
		double MuInput=0;
		double MuOutput=0;
		double* percentage = new double[MLambda.lambdaMatrix.size()];

		for (int i=0; i<MLambda.lambdaMatrix.size();i++)
		{
				for (int j=0; j<MLambda.lambdaMatrix.size();j++)
				{
						if ((MLambda.lambdaMatrix[i][j]).Direction==Input)
								MuInput+=(MLambda.lambdaMatrix[i][j]).Lambda;
						if ((MLambda.lambdaMatrix[i][j]).Direction==Output)
								MuOutput+=(MLambda.lambdaMatrix[i][j]).Lambda;
				}
				for (int j=0; j<MLambda.lambdaMatrix.size();j++)
				{
						if ((MLambda.lambdaMatrix[i][j]).Direction==Input)
								percentage[j]=(MLambda.lambdaMatrix[i][j]).Lambda/MuInput;
				}
				for (int j=0; j<MLambda.lambdaMatrix.size();j++)
				{
						if ((MLambda.lambdaMatrix[i][j]).Direction==Input)
								(MLambda.lambdaMatrix[i][j]).Lambda=percentage[j]*MuOutput;
				}
		}		
		delete[] percentage;
}

void InitLambdaMatrix(LambdaMatrix& MLambda,std::vector<HCell>& vCircuit)
{
		for (int i=1; i<MLambda.lambdaMatrix.size()-1;i++)
		{
				if (MLambda.lambdaMatrix[i][0].Direction==Output)
				{
						MLambda.lambdaMatrix[i][0].Lambda=1;
				}
		}
		
		double TempOutputLambdaSum;
		int InputCounter;
		for (int i=0; i<MLambda.lambdaMatrix.size();i++)
		{
				TempOutputLambdaSum=FindOutputLambdaSum(MLambda,i);
				InputCounter=0;
				for (int j=0; j<MLambda.lambdaMatrix.size();j++)
				{
						if (MLambda.lambdaMatrix[i][j].Direction==Input)
								InputCounter++;
				}
				for (int j=0; j<MLambda.lambdaMatrix.size();j++)
				{
						if (MLambda.lambdaMatrix[i][j].Direction==Input)
						{
								if (MLambda.lambdaMatrix[i][j].Lambda!=-1) ALERT("ERROR #1");
								MLambda.lambdaMatrix[i][j].Lambda=TempOutputLambdaSum/InputCounter;
						}
				}
		}
}

void SOLVE_LDP(HDPGrid& grid, HDesign& design,std::vector<HCell>& vCircuit, LambdaMatrix MLambda)
{
		int StepCounter=1;
		InitLambdaMatrix(MLambda, vCircuit);

		for (int i=1; i < MLambda.lambdaMatrix.size()-1; i++)
				if(!CheckKuhn_Tucker(MLambda, i)) ALERT("Check KuhnTucker=false on i=%d",i);


}

void DoLRSizing(HDPGrid& grid, HDesign& design)
{
		std::vector<HCell>* vCircuit=InitVCircuit(grid,design);
		LambdaMatrix MLambda(grid, design, *vCircuit);
		SOLVE_LDP(grid,design,*vCircuit, MLambda);
		
		delete vCircuit;
}

LambdaMatrix::LambdaMatrix(HDPGrid& grid, HDesign& design, std::vector<HCell>& vCircuit)
{
		int CircuitSize=vCircuit.size();
		lambdaMatrix.reserve(CircuitSize+1);

		std::vector<LamdbaData> temp;
		LamdbaData tempLD_NDef(-2, NDef);
		for(int i=0;i<CircuitSize+1;i++)
		{	
				temp.reserve(CircuitSize+1);
				for(int j=0;j<CircuitSize+1;j++)
						temp.push_back(tempLD_NDef);
				lambdaMatrix.push_back(temp);
				temp.clear();
		}

		for (std::vector<HCell>::iterator iterCircuit=vCircuit.begin();iterCircuit != vCircuit.end();iterCircuit++)
		{
				std::vector<HNet> vtempNetINPUT;
				std::vector<HNet> vtempNetOUTPUT;
				for(HCell::PinsEnumeratorW pin=design.Get<HCell::Pins, HCell::PinsEnumeratorW>(*iterCircuit); pin.MoveNext();)
				{
						HNet net=design.Get<HPin::Net,HNet>(pin);
						if(!::IsNull(net))
						{
								if (design.Get<HPin::Direction, PinDirection>(pin) == PinDirection_INPUT)//??
										vtempNetINPUT.push_back(net);
								else//??
										vtempNetOUTPUT.push_back(net);
						}
				}

				for (std::vector<HNet>::iterator iterTempNet = vtempNetINPUT.begin();iterTempNet != vtempNetINPUT.end();iterTempNet++)
				{
						LamdbaData tempLD_Input(-1, Input);
						for (HNet::PinsEnumeratorW pin = design[*iterTempNet].GetPinsEnumeratorW(); pin.MoveNext(); ) 
						{
								HCell cell = design[pin].Cell();
								if (::IsNull(cell)) continue;
								if (::IsNull(*iterCircuit)) ALERT("AAAAAAAAAAA");

								if(pin.IsPrimaryInput()) 
								{
										//(*MLambda)[0][cell2index(vCircuit,cell)]=-1;
										lambdaMatrix[cell2index(vCircuit,cell)][0]=tempLD_Input;
								}
								else
								{
										if(pin.IsPrimaryOutput())
										{
												//(*MLambda)[CircuitSize+1][cell2index(vCircuit,cell)]=-1;
												lambdaMatrix[cell2index(vCircuit,cell)][CircuitSize+1]=tempLD_Input;
										}
										else
												lambdaMatrix[cell2index(vCircuit,*iterCircuit)][cell2index(vCircuit,cell)]=tempLD_Input;	
								}
						}		
				}
				for (std::vector<HNet>::iterator iterTempNet = vtempNetOUTPUT.begin();iterTempNet != vtempNetOUTPUT.end();iterTempNet++)
				{
						LamdbaData tempLD_Output(-1,Output);
						for (HNet::PinsEnumeratorW pin = design[*iterTempNet].GetPinsEnumeratorW(); pin.MoveNext(); ) 
						{
								HCell cell = design[pin].Cell();
								if (::IsNull(cell)) continue;
								if (::IsNull(*iterCircuit)) ALERT("AAAAAAAAAAA");

								if(pin.IsPrimaryInput()) 
								{
										//(*MLambda)[0][cell2index(vCircuit,cell)]=-1;
										lambdaMatrix[cell2index(vCircuit,cell)][0]=tempLD_Output;
								}
								else
								{
										if(pin.IsPrimaryOutput())
										{
												//(*MLambda)[CircuitSize+1][cell2index(vCircuit,cell)]=-1;
												lambdaMatrix[cell2index(vCircuit,cell)][CircuitSize+1]=tempLD_Output;
										}
										else
												lambdaMatrix[cell2index(vCircuit,*iterCircuit)][cell2index(vCircuit,cell)]=tempLD_Output;	
								}
						}		
				}
		}

		ALERT("Printing Matrix2:");
		int sum=0;
		for(LambdaMatrixType::iterator i=lambdaMatrix.begin();i!=lambdaMatrix.end();i++)
		{
				for(std::vector<LamdbaData>::iterator j=(*i).begin();j!=(*i).end();j++)
						//printf("+%d\t",*j);
						if(j->Lambda==-1) sum++;
				//printf("\n");
		}
		ALERT("SUM=%d",sum);
}