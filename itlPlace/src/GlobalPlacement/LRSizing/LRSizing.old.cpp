#include <math.h>
#include <float.h>
#include "LRSizing.old.h"
#include "Legalization.h"
#include "Parser.h"
#include <time.h>
#include "Timing.h"

#include <iostream>
#include <vector>

using namespace std;

std::vector<HTimingPoint>* InitTimingPoints( HDPGrid& grid, HDesign& design ) 
{
		ALERT("initTimingPoints started");
		std::vector<HTimingPoint>* timingPoints=new std::vector<HTimingPoint>;
		//Passing design in REVERS topological order
		HTimingPoint timimgPointsEnd = design.TimingPoints.FirstInternalPoint();
		if (::IsNull(timimgPointsEnd)) ALERT("init: TPEnd is null");
		
		/*int i=0;

		HTimingPoint root=design.TimingPoints.TopologicalOrderRoot();
		if (::IsNull(root)) ALERT("init: root is null");
		HTimingPointWrapper rootWr=design[root];
		rootWr.GoPrevious();
		HTimingPoint maybeend=rootWr;
		if (::IsNull(maybeend)) ALERT("init: maybeend is null");
		*/
		//HTimingPointWrapper timingPoint=design[design.TimingPoints.TopologicalOrderRoot()];

		for (HTimingPointWrapper timingPoint = design[design.TimingPoints.TopologicalOrderRoot()];
				timingPoint.GoNext() != timimgPointsEnd; )
		{
				//if (::IsNull(timingPoint)) ALERT("init: TP is null, i=%d",i);
				timingPoints->push_back(timingPoint);
				//i++;
		} 
		ALERT("initTimingPoints finished");
		return timingPoints;
}

void DoLRSizing(HDPGrid& grid, HDesign& design)
{
		std::vector<HCell>* vCircuit=InitVCircuit(grid,design);
		std::vector<HTimingPoint>* timingPoints=InitTimingPoints(grid,design);
		LambdaMatrix Lambda(design, *timingPoints);
		SOLVE_LDP(grid,design, *vCircuit, *timingPoints, Lambda);
		std::vector<double> OptX=SOLVE_LRS_mu(design, *vCircuit, *timingPoints, Lambda); //Not working yet
		
		/*
		TODO: Convert vector of sizes to discrete sizes and replace cells in netlist.
		*/
		
		delete timingPoints;
}

int cell2index(std::vector<HCell>& vCircuit, HCell cell)
{
		std::vector<HCell>::iterator iterCircuit=vCircuit.begin();
		int counter=0;
		for(;iterCircuit != vCircuit.end();iterCircuit++,counter++){
				if (*iterCircuit == cell) return counter;
		}
		return -1;
}

/*HCell index2cell(std::vector<HCell>& vCircuit, unsigned int index)
{
		if(index >= 0 && index < vCircuit.size())
				return vCircuit[index];
		else 
		{
				ALERT("index2cell:!!ERROR!!index out of vCircuit bounds!!");
				return vCircuit[0];
		}
}*/

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
						//ALERT("NULL");
						continue;
				}

				if (cell2index(*vCircuit,tempCell) < 0)
				{
						vCircuit->push_back(tempCell);
						//ALERT(" inserted index=%d",index);
						index++;
				}
				//else ALERT("duplicate index=%d",cell2index(*vCircuit,tempCell));
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

void InitLambdaMatrix(LambdaMatrix& Lambda,std::vector<HTimingPoint>& timingPoints)
{
		for (unsigned int i=0; i<Lambda.matrix.size();i++)
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
		
		//Check symmetry
		for (unsigned int i=0; i < Lambda.matrix.size();i++)
				for (unsigned int j=0; j < Lambda.matrix.size();j++)		
						if (Lambda.matrix[i][j].value!=Lambda.matrix[j][i].value) 
								ALERT("Not Simmetry, i=%d, j=%d",i,j);

		ALERT("LambdaMatrix Inited");
}

std::vector<double> CalculateArrivalTimes(LambdaMatrix& Lambda )
{
		std::vector<double> arrivalTimes;
		arrivalTimes.reserve(Lambda.matrix.size());

		//1.For all inputs (ends in the timing points tree)
		//arrivalTimes[i]=r_i*DownStreamCap;
		//2.
		
		double MaxInputArrivalTime;
		for (unsigned int i=Lambda.matrix.size()-1; i<=0;i++)//should concern only middle points and start! (from input to output)! 
		{
				MaxInputArrivalTime=0;
				for (unsigned int j=0; j<Lambda.matrix.size();j++)
				{
						if (Lambda.matrix[i][j].Direction==Input)
								if (arrivalTimes[j]>MaxInputArrivalTime)
										MaxInputArrivalTime=arrivalTimes[j];
				}
				//arrivalTimes[i]=MaxInputArrivalTime+r_i*DownStreamCap;
		}
		//3.For all outputs (starts in the timing points tree)
		//if (arrivalTimes[i]>A_0); ALERT("Smth wrong in timing"); //A_0
		return arrivalTimes;
}

void AdjustingLambda(LambdaMatrix& Lambda,int k,std::vector<double>& arrivalTimes)
{
		for (unsigned int i=0; i<Lambda.matrix.size();i++)//for all
		{
				for (unsigned int j=0; j<Lambda.matrix.size();j++)//for all j - input i
				{
						if (Lambda.matrix[i][j].Direction==Input)
						{
								;
								//if (i==root)
								//{
										//Lambda.matrix[j][i].value=Lambda.matrix[j][i].value+(1/k)*(arrivalTimes[j]-A_0);
										//continue;
								//}
								//if (i==from 1 to n)
								//{
										//Lambda.matrixv[j][i].value=Lambda.matrix[j][i].value+(1/k)*(arrivalTimes[j]+r_i*DownStreamCap_i-arrivalTimes[i]);
										//continue;
								//}
								//if (i==from end points)
								//{
										//Lambda.matrixv[j][i].value=Lambda.matrix[j][i].value+(1/k)*(r_i*DownStreamCap_i-arrivalTimes[i]);
										//continue;
								//}

						}
								
				}
				//arrivalTimes[i]=MaxInputArrivalTime+r_i*DownStreamCap;
		}
}

double CalculateQ(std::vector<double>& vX, LambdaMatrix& Lambda)
{
		double SumInputLambdaForPoint; 
		double SumAllInputLambda=0;
		double QFunction;
		for (unsigned int i=0; i < Lambda.matrix.size(); i++)
		{
				SumInputLambdaForPoint=0;
				for (unsigned int j=0; j<Lambda.matrix.size();j++)//for all j - input i
				{
						if (Lambda.matrix[i][j].Direction==Input)
						{
								SumInputLambdaForPoint+=Lambda.matrix[j][i].value;
						}
				}
				//SumAllInputLambda=SumInputLambdaForPoint*(r_i*DownStreamCap_i);//при x_i = lower_bound_X[i]
		}
		double SumMinX=0;
		for(unsigned int i=0; i < vX.size(); i++)
				;//SumMinX+=lower_bound_X[i];
		//QFunction=SumMinX+SumAllInputLambda;
		return QFunction;
}

bool CheckStopConditionForLDP(std::vector<double> vX,double accuracy,LambdaMatrix& Lambda)
{
		double Sum=0;
		for(unsigned int i=0; i < vX.size(); i++)
				Sum+=vX[i];
		if ((Sum-CalculateQ(vX, Lambda))<accuracy) return true;
		else false;
}

void SOLVE_LDP( HDPGrid& grid, HDesign& design, std::vector<HCell>& vCircuit, std::vector<HTimingPoint>& timingPoints, LambdaMatrix& Lambda )
{
		int StepCounter=1;

		InitLambdaMatrix(Lambda, timingPoints);

		for (unsigned int i=0; i < Lambda.matrix.size(); i++)
				if(!CheckKuhn_Tucker(Lambda, i)) ALERT("Check KuhnTucker=false on i=%d",i);

		//bool stop;
		//do{
		//std::vector<double> vX=SOLVE_LRS_mu(design,vCircuit,timingPoints,Lambda);
		//std::vector<double> arrivalTimes=CalculateArrivalTimes(LambdaMatrix& Lambda );
		//AdjustingLambda(Lambda,StepCounter,arrivalTimes);
		//ProjectLambdaMatrix(Lambda);
		//StepCounter++;
		//stop=CheckStopConditionForLDP(vX,errorBoundForLDP,Lambda)
		//}
		//while(!stop)
		/*
		TODO
		*/
}

void FillVMu(std::vector<double>& vMu, LambdaMatrix& Lambda)
{
	double sumOfMu;
	vMu.clear();
	
	vMu.reserve(Lambda.matrix.size()); // can be bug, check!
	
	//Do not push -1 !! It is not a bug, it is ok. Последний фиктиынй элемент имеет инпуты, поэтому для него оно не ноль.
	for (unsigned int i=0;i<Lambda.matrix.size();i++)
	{
		sumOfMu=0;
		for (unsigned int j=0;i<Lambda.matrix.size();j++)
		{
			if (Lambda.matrix[i][j].Direction==Input)
				sumOfMu+=Lambda.matrix[i][j].value;
		}
		vMu.push_back(sumOfMu);
	}
	//vMu.push_back(-1);
	

}

void initVX(std::vector<double>& vX, std::vector<HCell>& vCircuit)
{
	vX.clear();
	//vX.push_back(-1);
	//HCell TempCell;
	//double TempLowerBound;
	for (unsigned int i=1;i<vCircuit.size();i++)
	{
		//TempCell=index2cell(vCircuit,i);
		vX.push_back(2);// L_i
	}
	//vX.push_back(-1);
}

void CalculateVC(HDesign& design, std::vector<double>& vC, LambdaMatrix& Lambda, std::vector<HCell>& vCircuit, std::vector<HTimingPoint>& timingPoints, std::vector<double>& vX)
{
	vC.clear();
	double TempC;
	unsigned int nCells=vCircuit.size();
	double* C=new double[nCells]; 
	for(int i=0;i<nCells;i++) C[i]=-1;

	unsigned int nTimingPoints=Lambda.matrix.size();
	for (unsigned int i=0;i<nTimingPoints;i++)
	{
		TempC=0;
		if(Lambda.matrix[i][0].Direction==Output)//for all Outputs
		{
			TempC=3;// C_i^L
		}
		else//for all other points
		{
			TempC=0;
			for(unsigned int j=0;j<nTimingPoints;j++)
			{
				if (Lambda.matrix[i][j].Direction==Output)// for all outputs of i
				;//TempC=TempC + c_k x_k + f_k
			}
		}
		
		HPin pin = design.Get<HTimingPoint::Pin, HPin>(timingPoints[i]);
		HPinWrapper pinWrapper=design[pin];
		HCell cell=pinWrapper.Cell();
		int cellIndex=cell2index(vCircuit, cell);
		if (C[cellIndex] < 0) C[cellIndex]=TempC; //if C[i] not empty
	}
}

void CalculateVR(HDesign& design, std::vector<double>& vR, LambdaMatrix& Lambda, std::vector<HCell>& vCircuit, std::vector<HTimingPoint>& timingPoints, std::vector<double>& vX,std::vector<double>& vMu)
{
	vR.clear();
	double TempR;
	
	unsigned int nCells=vCircuit.size();
	double* R=new double[nCells]; 
	for(int i=0;i<nCells;i++) R[i]=-1;

	unsigned int nTimingPoints = (unsigned int) (Lambda.matrix.size()); 
	for (unsigned int i=nTimingPoints-1;i >= 0;i--)
	{
		TempR=0;
		for(unsigned int j=0;j<nTimingPoints;j++)
		{
			if (Lambda.matrix[i][j].Direction==Input)//for all inputs of i
			{
				//if(j from Outputs)
				{
					;
					//TempR=TempR + vMu[j] * R_j-n^D
				}
				//else// for all other j points
				{
					//TempR=TempR + vMu[j] * r_j / x_j
				}
			}
		}
		HPin pin = design.Get<HTimingPoint::Pin, HPin>(timingPoints[i]);
		HPinWrapper pinWrapper=design[pin];
		HCell cell=pinWrapper.Cell();
		int cellIndex=cell2index(vCircuit, cell);
		if (R[cellIndex] < 0) R[cellIndex]=TempR; //if C[i] not empty
	}
}

void CalculateVX(std::vector<HCell>& vCircuit, std::vector<double>& vX, LambdaMatrix& Lambda,std::vector<double>& vMu,std::vector<double>& vC,std::vector<double>& vR)
{
	vX.clear();
	double tempMax,tempMin,Temp;
	for (unsigned int i=0;i<vCircuit.size();i++)
	{
		//Temp=sqrt()..
		//if (Temp > L_i)  tempMax=Temp;
		//else  tempMax=L_i
		//if (tempMax < U_i)  tempMin=tempMax;
		//else  tempMin=U_i	
		vX.push_back(tempMin);
	}
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

std::vector<double> SOLVE_LRS_mu(HDesign& design,std::vector<HCell>& vCircuit, std::vector<HTimingPoint>& timingPoints, LambdaMatrix& Lambda )
{

	std::vector<double> vMu;
	FillVMu(vMu,Lambda);

	std::vector<double> vX;
	initVX(vX, vCircuit); //for all x_i=L_i

	std::vector<double> NewVX=vX;
	
	std::vector<double> vC;
	std::vector<double> vR;
	
	bool stop;
	do 
	{
		vX=NewVX;
		CalculateVC(design, vC, Lambda, vCircuit, timingPoints, vX);
		CalculateVR(design, vR, Lambda, vCircuit, timingPoints, vX, vMu);
		
		CalculateVX(vCircuit, NewVX,Lambda,vMu,vC,vR);
		stop=CheckStopConditionForLRS_Mu(vX,NewVX,accuracyForLRS_Mu);
	}while(!stop);

	return NewVX;
}


LambdaMatrix::LambdaMatrix(HDesign& design, std::vector<HTimingPoint>& timingPoints)
{
		HDPGrid grid(design);

		LamdbaData lambdaOutput(-2, Output, UndefinedType);
		LamdbaData lambdaInput(-2, Input, UndefinedType);
		LamdbaData lambdaNDef(-3, NDef, UndefinedType);

		//Filling matrix with "Undef" values
		std::vector<LamdbaData> lambdaRow;
		unsigned int MatrixSize=timingPoints.size();
		for(unsigned int i=0;i<MatrixSize;i++)
		{	
				lambdaRow.reserve(MatrixSize);
				for(unsigned int j=0;j<MatrixSize;j++)
						lambdaRow.push_back(lambdaNDef);
				matrix.push_back(lambdaRow);
				lambdaRow.clear();
		}

		//Filling matrix with real values
		/*for (unsigned int timingPointIndex=0; timingPointIndex < timingPoints.size(); timingPointIndex++)
		{
				HTimingPoint tp=timingPoints[timingPointIndex];
				HPin pin = design.Get<HTimingPoint::Pin, HPin>(tp);

				for (HPinType::ArcsEnumeratorW arc = design.Get<HPinType::ArcTypesEnumerator, HPinType::ArcsEnumeratorW>(pin.Type()); 
						arc.MoveNext();)
				{
						//Мы считаем, что GetStartPin возвращает просто стартовый пин арки, вне зависимости, стартовый или конечный пин мы передали в кач-ве параметра
						if (pin == arc.GetStartPin(pin)) //our pin is start. This arc is output
						{
								HPin endPin=arc.GetEndPin(pin);
								HTimingPoint endTimingPoint = design.TimingPoints[endPin];
								matrix[timingPointIndex][TimingPoint2index(timingPoints, endTimingPoint)]=lambdaOutput;
						} else 
								if (pin == arc.GetEndPin(pin)) //our pin is end. This arc is input
								{
										HPin startPin=arc.GetStartPin(pin);
										HTimingPoint startTimingPoint = design.TimingPoints[startPin];
										matrix[timingPointIndex][TimingPoint2index(timingPoints, startTimingPoint)]=lambdaInput;
								} else ALERT("UZHOZZZ in LambdaMatix constructor");
				}
		}*/
		
		
		
		//Print matrix
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

		//Check symmetry
		/*for (unsigned int i=0; i < matrix.size();i++)
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
		*/

		ALERT("LambdaMatrix Constructed");
}