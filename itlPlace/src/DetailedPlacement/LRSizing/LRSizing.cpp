#include <math.h>
#include <float.h>
#include "LRSizing.h"
#include "Legalization.h"
#include "Parser.h"
#include <time.h>
#include "Timing.h"

#include <iostream>
#include <vector>

using namespace std;

void DoLRSizing(HDPGrid& grid, HDesign& design)
{
		std::vector<HCell>* vCircuit=InitVCircuit(grid,design);
		LambdaMatrix Lambda(grid, design, *vCircuit);
		
		SOLVE_LDP(grid,design,*vCircuit, Lambda);
		
		//std::vector<double> OptX=SOLVE_LRS_mu(grid, design, *vCircuit,Lambda); //Not working yet
		
		/*
		TODO: Convert vector of sizes to discrete sizes and replace cells in netlist.
		*/
		
		delete vCircuit;
}

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

double FindInputLambdaSum(LambdaMatrix& Lambda, int index)
{
		double InputLambdaSum=0;
		for (unsigned int j=0;j<Lambda.matrix.size();j++)
		{
				if ((Lambda.matrix[index][j]).Direction==Input)
				InputLambdaSum+=(Lambda.matrix[index][j]).value;
		}
		return InputLambdaSum;
}

double FindOutputLambdaSum(LambdaMatrix& Lambda, int index)
{
		double OutputLambdaSum=0;
		for (unsigned int j=0;j<Lambda.matrix.size();j++)
		{
				if ((Lambda.matrix[index][j]).Direction==Output)
						OutputLambdaSum+=(Lambda.matrix[index][j]).value;
		}
		return OutputLambdaSum;
}

bool CheckKuhn_Tucker (LambdaMatrix& Lambda, int index)
{
		if (((FindOutputLambdaSum(Lambda,index))-FindInputLambdaSum(Lambda, index))<1e-9) 
				return true;
		else 
				return false;
}

void ProjectLambdaMatrix(LambdaMatrix& Lambda)
{
		double MuInput=0;
		double MuOutput=0;
		double* percentage = new double[Lambda.matrix.size()];

		for (unsigned int i=0; i<Lambda.matrix.size();i++)
		{
				for (unsigned int j=0; j<Lambda.matrix.size();j++)
				{
						if ((Lambda.matrix[i][j]).Direction==Input)
								MuInput+=(Lambda.matrix[i][j]).value;
						if ((Lambda.matrix[i][j]).Direction==Output)
								MuOutput+=(Lambda.matrix[i][j]).value;
				}
				for (unsigned int j=0; j<Lambda.matrix.size();j++)
				{
						if ((Lambda.matrix[i][j]).Direction==Input)
								percentage[j]=(Lambda.matrix[i][j]).value/MuInput;
				}
				for (unsigned int j=0; j<Lambda.matrix.size();j++)
				{
						if ((Lambda.matrix[i][j]).Direction==Input)
								(Lambda.matrix[i][j]).value=percentage[j]*MuOutput;
				}
		}		
		delete[] percentage;
}

void InitLambdaMatrix(LambdaMatrix& Lambda,std::vector<HCell>& vCircuit)
{
		for (unsigned int i=1; i<Lambda.matrix.size()-1;i++)
		{
				if (Lambda.matrix[i][0].Direction==Output)
				{
						Lambda.matrix[i][0].value=1;
						Lambda.matrix[0][i].value=1;
						ALERT("1Here!!!");
				}
		}
		
		double TempOutputLambdaSum;
		int InputCounter;
		for (unsigned int i=0; i<Lambda.matrix.size();i++)
		{
				TempOutputLambdaSum=FindOutputLambdaSum(Lambda,i);
				if (TempOutputLambdaSum<0)
				{
						ALERT("<0!!  i=%d",i);
				}
				InputCounter=0;
				for (unsigned int j=0; j<Lambda.matrix.size();j++)
				{
						if (Lambda.matrix[i][j].Direction==Input)
								InputCounter++;
				}
				for (unsigned int j=0; j<Lambda.matrix.size();j++)
				{
						if (Lambda.matrix[i][j].Direction==Input)
						{
								//if (Lambda.matrix[i][j].value!=-1) ALERT("ERROR #1");
								Lambda.matrix[i][j].value=TempOutputLambdaSum/InputCounter;
								//ALERT("qq=%f",TempOutputLambdaSum/InputCounter);
						}
				}
		}

		for (unsigned int i=0; i < Lambda.matrix.size();i++)
				for (unsigned int j=0; j < Lambda.matrix.size();j++)		
						if (Lambda.matrix[i][j].value!=Lambda.matrix[j][i].value) 
								ALERT("Not Simmetry, i=%d, j=%d",i,j);

		ALERT("LambdaMatrix Inited");
		ALERT("LambdaMatrix Inited");
}

void SOLVE_LDP(HDPGrid& grid, HDesign& design,std::vector<HCell>& vCircuit, LambdaMatrix& Lambda)
{
		int StepCounter=1;
		InitLambdaMatrix(Lambda, vCircuit);

		for (unsigned int i=1; i < Lambda.matrix.size()-1; i++)
				if(!CheckKuhn_Tucker(Lambda, i)) ALERT("Check KuhnTucker=false on i=%d",i);


		/*
		TODO
		*/
}

void FillVMu(std::vector<double>& vMu, LambdaMatrix& Lambda)
{
	double sumOfMu;
	vMu.clear();
	
	vMu.reserve(Lambda.matrix.size()); // can be bug, check!
	
	//Do not push -1 !! It is not a bug, it is okay. Последний фиктиынй элемент имеет инпуты, поэтому для него оно не ноль.
	for (unsigned int i=0;i<Lambda.matrix.size()-1;i++)
	{
		sumOfMu=0;
		for (unsigned int j=0;i<Lambda.matrix.size();j++)
		{
			if (Lambda.matrix[i][j].Direction==Input)
				sumOfMu+=Lambda.matrix[i][j].value;
		}
		vMu.push_back(sumOfMu);
	}
	vMu.push_back(-1);
	

}

void initVX(std::vector<double>& vX, LambdaMatrix& MLambda)
{
	vX.clear();
	vX.push_back(-1);
	HCell TempCell;
	double TempLowerBound;
	for (unsigned int i=1;i<MLambda.matrix.size()-1;i++)
	{
		//TempCell=index2cell(vCircuit,i);
		vX.push_back(2);// L_i
	}
	vX.push_back(-1);
}

void CalculateVC(std::vector<double>& vC, LambdaMatrix& Lambda,std::vector<double>& vX)
{
	vC.clear();
	vC.push_back(-1);
	double TempC;
	//HCell TempCell;
	for (unsigned int i=1;i<Lambda.matrix.size()-1;i++)
	{
		TempC=0;
		if(Lambda.matrix[i][0].Direction==Output)
		{
			//TempCell=index2cell(vCircuit,i);
			TempC=3;// C_i^L
		}
		else
		{
			TempC=0;
			for(unsigned int j=0;j<Lambda.matrix.size();j++)
			{
				if (Lambda.matrix[i][j].Direction==Output)
				;//TempC=TempC + c_k x_k + f_k
			}
		}
		vC.push_back(TempC);
	}
	vC.push_back(-1);
}

void CalculateVR(std::vector<double>& vR, LambdaMatrix& Lambda,std::vector<double>& vX,std::vector<double>& vMu)
{
	vR.clear();
	vR.push_back(-1);
	double TempR;
	unsigned int Size = (unsigned int) (Lambda.matrix.size()); 
	for (unsigned int i=Size-2;i > 0;i--)
	{
		TempR=0;
		for(unsigned int j=1;j<Size-1;j++)
		{
			if(Lambda.matrix[i][Size-1].Direction==Input)
			{
				;
				//TempR=TempR + vMu[j] * R_j-n^D
			}
			else
			{
				//TempR=TempR + vMu[j] * r_j / x_j
			}
		}
		vR.push_back(TempR);
	}
	vR.push_back(-1);
}

void CalculateVX(std::vector<double>& vX, LambdaMatrix& Lambda,std::vector<double>& vMu,std::vector<double>& vC,std::vector<double>& vR)
{
	vX.clear();
	vX.push_back(-1);
	double tempMax,tempMin,Temp;
	for (unsigned int i=1;i<Lambda.matrix.size()-1;i++)
	{
		//Temp=sqrt()..
		//if (Temp > L_i)  tempMax=Temp;
		//else  tempMax=L_i
		//if (tempMax < U_i)  tempMin=tempMax;
		//else  tempMin=U_i	
		vX.push_back(tempMin);
	}
	vX.push_back(-1);
}

bool CheckStopConditionForLRS_Mu(std::vector<double>& prevVX,std::vector<double>& nextVX,double accuracy)
{
	double max=0;
	if (prevVX.size()!=nextVX.size())
	{
		ALERT("different size!!!");
		return false;		
	}

	for(unsigned int i=0;i<prevVX.size();i++)
	{
		if (abs(prevVX[i]-nextVX[i])>max) 
			max=abs(prevVX[i]-nextVX[i]);
	}

	if (max<accuracy) return true;
	else return false;
}

std::vector<double> SOLVE_LRS_mu(HDPGrid& grid, HDesign& design,std::vector<HCell>& vCircuit, LambdaMatrix& Lambda)
{

	std::vector<double> vMu;
	FillVMu(vMu,Lambda);

	std::vector<double> vX;
	initVX(vX,Lambda); //for all x_i=L_i

	std::vector<double> NewVX=vX;
	
	std::vector<double> vC;
	std::vector<double> vR;
	
	bool stop;
	do 
	{
		vX=NewVX;
		CalculateVC(vC,Lambda,vX);
		CalculateVR(vR,Lambda,vX,vMu);
		
		CalculateVX(NewVX,Lambda,vMu,vC,vR);
		stop=CheckStopConditionForLRS_Mu(vX,NewVX,accuracyForLRS_Mu);
	}while(!stop);

	return NewVX;
}

LambdaMatrix::LambdaMatrix(HDPGrid& grid, HDesign& design, std::vector<HCell>& vCircuit)
{
		unsigned int CircuitSize=(unsigned int) (vCircuit.size());
		unsigned int MatrixSize=CircuitSize+2;
		matrix.reserve(MatrixSize);

		std::vector<LamdbaData> temp;
		LamdbaData tempLD_NDef(-2, NDef);
		for(unsigned int i=0;i<MatrixSize;i++)
		{	
				temp.reserve(MatrixSize);
				for(unsigned int j=0;j<MatrixSize;j++)
						temp.push_back(tempLD_NDef);
				matrix.push_back(temp);
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
								if (design.Get<HPin::Direction, PinDirection>(pin) == PinDirection_INPUT)
										vtempNetINPUT.push_back(net);
								else
										vtempNetOUTPUT.push_back(net);
						}
				}

				for (std::vector<HNet>::iterator iterTempNet = vtempNetINPUT.begin();iterTempNet != vtempNetINPUT.end();iterTempNet++)
				{
						LamdbaData tempLD_Input(-2, Input);
						for (HNet::PinsEnumeratorW pin = design[*iterTempNet].GetPinsEnumeratorW(); pin.MoveNext(); ) 
						{
								HCell cell = design[pin].Cell();
								//if (::IsNull(cell)) continue;
								//if (::IsNull(*iterCircuit)) ALERT("AAAAAAAAAAA");
								if (cell==*iterCircuit) {
										ALERT("Pzhzhzhzh");
										continue;
								}

								if(pin.IsPrimaryInput()) 
								{
										//(*value)[0][cell2index(vCircuit,cell)]=-1;
										matrix[cell2index(vCircuit,*iterCircuit)][MatrixSize-1]=tempLD_Input;
										//ALERT("PrimaryInput!!!!!!!!!!!!");
								}
								else
								{
										if(pin.IsPrimaryOutput())
										{
												//(*value)[CircuitSize+1][cell2index(vCircuit,cell)]=-1;
												matrix[cell2index(vCircuit,*iterCircuit)][0]=tempLD_Input;
												//if (::IsNull(cell))ALERT("AAAAAAAAAAA");
												ALERT("PrimaryOutput!!!!!!!!!!!!");
										}
										else
												matrix[cell2index(vCircuit,*iterCircuit)][cell2index(vCircuit,cell)]=tempLD_Input;	
								}
						}		
				}
				for (std::vector<HNet>::iterator iterTempNet = vtempNetOUTPUT.begin();iterTempNet != vtempNetOUTPUT.end();iterTempNet++)
				{
						LamdbaData tempLD_Output(-2,Output);
						for (HNet::PinsEnumeratorW pin = design[*iterTempNet].GetPinsEnumeratorW(); pin.MoveNext(); ) 
						{
								HCell cell = design[pin].Cell();
								//if (::IsNull(cell)) continue;
								//if (::IsNull(*iterCircuit)) ALERT("AAAAAAAAAAA");
								
								if (cell==*iterCircuit) {
										ALERT("Pchchchch");
										continue;
								}
								if(pin.IsPrimaryInput()) 
								{
										//(*value)[0][cell2index(vCircuit,cell)]=-1;
										matrix[cell2index(vCircuit,*iterCircuit)][MatrixSize-1]=tempLD_Output;
								}
								else
								{
										if(pin.IsPrimaryOutput())
										{
												//(*value)[CircuitSize+1][cell2index(vCircuit,cell)]=-1;
												matrix[cell2index(vCircuit,*iterCircuit)][0]=tempLD_Output;
										}
										else
												matrix[cell2index(vCircuit,*iterCircuit)][cell2index(vCircuit,cell)]=tempLD_Output;	
								}
						}		
				}
		}

		/*ALERT("Printing Matrix2:");
		int sum=0;
		for(LambdaMatrixType::iterator i=matrix.begin();i!=matrix.end();i++)
		{
				for(std::vector<LamdbaData>::iterator j=(*i).begin();j!=(*i).end();j++)
						//printf("+%d\t",*j);
						if(j->value==-1) sum++;
				//printf("\n");
		}
		ALERT("SUM=%d",sum);*/

		for (unsigned int i=0; i < matrix.size();i++)
				for (unsigned int j=0; j < matrix.size();j++)		
				{
						if (matrix[i][j].value!=matrix[j][i].value) 
								ALERT("Not Simmetry, i=%d, j=%d",i,j);
						else 
								if ((matrix[i][j].Direction==Input && matrix[j][i].Direction==Output)||
										(matrix[i][j].Direction==Output && matrix[j][i].Direction==Input)||
										(matrix[i][j].Direction==NDef && matrix[j][i].Direction==NDef))
										i==i;
										//ALERT("Direction is OK, i=%d, j=%d",i,j);
								else 
										ALERT("Direction is Wrong, i=%d, j=%d",i,j);
				}

		ALERT("LambdaMatrix Constructed");
		ALERT("LambdaMatrix Constructed");
}