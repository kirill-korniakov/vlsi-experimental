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

LambdaMatrix::LambdaMatrix(HDPGrid& grid, HDesign& design)
{
		std::vector<HCell> vCircuit;
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

				if (cell2index(vCircuit,tempCell) < 0)
				{
						vCircuit.push_back(tempCell);
						ALERT(" inserted index=%d",index);
						index++;
				}
				else 
						ALERT("duplicate index=%d",cell2index(vCircuit,tempCell));
		}
		int CircuitSize=vCircuit.size();
		std::vector<std::vector<int>> MLambda;
		MLambda.reserve(CircuitSize+1);
		//int MLambda[vCircuit.size()][vCircuit.size()];

		std::vector<int> temp;
		for(int i=0;i<CircuitSize+1;i++)
		{	
				temp.reserve(CircuitSize+1);
				for(int j=0;j<CircuitSize+1;j++)
						temp.push_back(-2);
				MLambda.push_back(temp);
				temp.clear();
		}

		/*ALERT("Printing Matrix1:");
		for(std::vector<std::vector<int>>::iterator i=MLambda.begin();i!=MLambda.end();i++)
		{
				for(std::vector<int>::iterator j=(*i).begin();j!=(*i).end();j++)
						printf("+%d\t",*j);
				printf("\n");
		}*/

		for (std::vector<HCell>::iterator iterCircuit=vCircuit.begin();iterCircuit != vCircuit.end();iterCircuit++)
		{
				std::vector<HNet> vtempNet;
				
				for(HCell::PinsEnumeratorW pin=design.Get<HCell::Pins, HCell::PinsEnumeratorW>(*iterCircuit); pin.MoveNext();)
				{
						HNet net=design.Get<HPin::Net,HNet>(pin);
						if(!::IsNull(net)) vtempNet.push_back(net);
				}

				for (std::vector<HNet>::iterator iterTempNet = vtempNet.begin();iterTempNet != vtempNet.end();iterTempNet++)
				{
						for (HNet::PinsEnumeratorW pin = design[*iterTempNet].GetPinsEnumeratorW(); pin.MoveNext(); ) 
						{
								HCell cell = design[pin].Cell();
								if (::IsNull(cell)) continue;
								if (::IsNull(*iterCircuit)) ALERT("AAAAAAAAAAA");

								if(pin.IsPrimaryInput()) 
								{
										MLambda[0][cell2index(vCircuit,cell)]=-1;
										MLambda[cell2index(vCircuit,cell)][0]=-1;
								}
								else
								{
										if(pin.IsPrimaryOutput())
										{
												MLambda[CircuitSize+1][cell2index(vCircuit,cell)]=-1;
												MLambda[cell2index(vCircuit,cell)][CircuitSize+1]=-1;
										}
										else
												MLambda[cell2index(vCircuit,*iterCircuit)][cell2index(vCircuit,cell)]=-1;	
								}
						}		
				}
		}

		ALERT("Printing Matrix2:");
		int sum=0;
		for(std::vector<std::vector<int>>::iterator i=MLambda.begin();i!=MLambda.end();i++)
		{
				for(std::vector<int>::iterator j=(*i).begin();j!=(*i).end();j++)
						//printf("+%d\t",*j);
						if(*j==-1) sum++;
				//printf("\n");
		}
		ALERT("SUM=%d",sum);

		//HCell temptemp=index2cell(vCircuit,142);
}

void DOSizing(HDPGrid& grid, HDesign& design)
{
}