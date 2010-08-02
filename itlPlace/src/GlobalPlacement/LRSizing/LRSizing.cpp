#include "LRSizing.h"
#include <math.h>

#define accuracyForLRS_Mu 0.001
#define DOUBLE_ACCURACY 1e-9
#define errorBoundForLDP 0.01

int LRSizer::CountLeftArcs(HDesign& design, HNet net)
{
		HPin source = design[net].Source();
		HPinType sourceType = design.Get<HPin::Type, HPinType>(source);

		int i = 0;
		for (HPinType::ArcsEnumeratorW arc = design[sourceType].GetArcsEnumeratorW(); arc.MoveNext(); )
				if (arc.TimingType() == TimingType_Combinational)
						i++;

		return i;
}

LRSizer::LRSizer(HDesign& design): LambdaIn(0)
{
		size = design._Design.NetList.nPinsLimit;
		::Grow(&LambdaIn, 0, size);

		InitLambda(design, 1);
		cells = InitCells(design);
}

LRSizer::~LRSizer()
{
    ::Grow(&LambdaIn, size, 0);
		delete cells;
}

void LRSizer::InitLambda(HDesign& design, double defaultLambda)
{
		for (HTimingPointWrapper tp = design[design.TimingPoints.FirstInternalPoint()]; !::IsNull(tp); tp.GoNext() )
		{
				//WRITELINE("InitMus at %s", reporter->GetCellPinName(design, tp).c_str());
				InitPointLambda(design, tp, defaultLambda); 
		}
}

void LRSizer::InitPointLambda(HDesign& design, HTimingPoint tp, double defaultLambda)
{
		HPin pin = design.Get<HTimingPoint::Pin, HPin>(tp);
		if (design.Get<HPin::Direction, PinDirection>(pin) == PinDirection_INPUT)
		{
				LambdaIn[::ToID(tp)].resize(1, defaultLambda);
		}
		else
		{
				if (!design.GetBool<HPin::IsPrimary>(pin))
						LambdaIn[::ToID(tp)].resize(CountLeftArcs(design, design.Get<HPin::Net, HNet>(pin)), defaultLambda);
		}
}

std::vector<HCell>* LRSizer::InitCells(HDesign& design)
{
		HDPGrid grid(design);
		std::vector<HCell>* cells = new std::vector<HCell>;
		int index = 1;
		HCell tempCell;

		//Going through whole circuit in reverse topological order
		HTimingPointWrapper tp = design[design.TimingPoints.TopologicalOrderRoot()];
		tp.GoPrevious();
		for (; !::IsNull(tp); tp.GoPrevious())
		{
				CRITICAL_ASSERT(!::IsNull(tp));
				HPin pin = tp.Pin();
				tempCell=design[pin].Cell();
				if (::IsNull(tempCell)) continue;
				
				bool isCellNotInCells=true;
				for (std::vector<HCell>::iterator cell=cells->begin(); cell!=cells->end(); cell++)
						if (*cell == tempCell){isCellNotInCells=false; break;}

				if (isCellNotInCells)
						cells->push_back(tempCell);
		}
		return (cells);
}

std::vector<double> LRSizer::SOLVE_LRS_mu(HDesign& design, std::vector<HCell>& cells)
{
		std::vector<double> vMu;
		vMu.resize(size, -1);
		FillVMu(design, vMu);

		std::vector<double> vX;
		initVX(vX, cells); //for all x_i=L_i

		std::vector<double> NewVX=vX;

		std::vector<double> vC;
		std::vector<double> vR;

		bool stop;
		do 
		{
				vX=NewVX;
				CalculateVC(design, vC, cells, vX);
				CalculateVR(design, vR, cells, vX, vMu);

				CalculateVX(cells, NewVX, vMu,vC,vR);
				stop=CheckStopConditionForLRS_Mu(vX, NewVX, accuracyForLRS_Mu);
		}while(!stop);

		return NewVX;
}

void LRSizer::FillVMu( HDesign& design, std::vector<double>& vMu )
{
//Sum of all Lambda inputs of the point
		for (HTimingPointWrapper tp = design[design.TimingPoints.FirstInternalPoint()]; !::IsNull(tp); tp.GoNext() )
		{
				double lambdaSum=.0;
				for (std::vector<double>::iterator lambda = LambdaIn[::ToID(tp)].begin(); lambda!=LambdaIn[::ToID(tp)].end(); lambda++)
				{
						lambdaSum+=*lambda;
				}
				vMu[::ToID(tp)]=lambdaSum;
		}
}

void LRSizer::initVX( std::vector<double> vX, std::vector<HCell>& cells )
{
//Init all X_i by its lower bound
		for (std::vector<HCell>::iterator cell=cells.begin(); cell!=cells.end(); cell++){
		
				vX.push_back(1);
				;

		}
		//throw std::exception("The method or operation is not implemented.");
}

void LRSizer::CalculateVC(HDesign& design, std::vector<double>& C, std::vector<HCell>& cells, std::vector<double> vX )
{
		C.resize(size, 0);

		HTimingPointWrapper tp = design[design.TimingPoints.TopologicalOrderRoot()];
		tp.GoPrevious();
		for (; tp!=design.TimingPoints.LastInternalPoint(); tp.GoPrevious())
		{
				double TempC=3;// C_i^L
				C[::ToID(tp)]=TempC;
		}

		tp = design[design.TimingPoints.LastInternalPoint()];
		for (; !::IsNull(tp); tp.GoPrevious())
		{
				//Выходные арки
				HPin pin = design.Get<HTimingPoint::Pin, HPin>(tp);

				if (design.Get<HPin::Direction, PinDirection>(pin) == PinDirection_OUTPUT)
				{//cell output pin, iterate net arcs
						for (HNet::SinksEnumeratorW sink = design.Get<HNet::Sinks, HNet::SinksEnumeratorW>(design.Get<HPin::Net, HNet>(pin));sink.MoveNext();)
						{
								HTimingPoint timingPoint = design.TimingPoints[sink];
								//C[::ToID(timingPoint)]=C[::ToID(timingPoint)] + c_k x_k + f_k
						}
				}
				else
				{//cell input pin, iterate cell arcs
						HCell cell = design.Get<HPin::Cell, HCell>(pin);

						for (HCell::PinsEnumeratorW _pin = design.Get<HCell::Pins, HCell::PinsEnumeratorW>(cell); _pin.MoveNext(); )
						{
								if (_pin.Direction() != PinDirection_OUTPUT || ::IsNull(_pin.Net())) 
										continue;

								HTimingPoint point = design.TimingPoints[_pin];
								for (HPinType::ArcsEnumeratorW arc = design.Get<HPinType::ArcTypesEnumerator, HPinType::ArcsEnumeratorW>(_pin.Type());arc.MoveNext();)
								{
										if (arc.TimingType() == TimingType_Combinational)
										{
												if (arc.GetStartPin(_pin) == pin)
												{
														HTimingPoint timingPoint = point;
														//C[::ToID(timingPoint)]=C[::ToID(timingPoint)] + c_k x_k + f_k
												}
										}
								}
						}
				}
		}
//	throw std::exception("The method or operation is not implemented.");
}

void LRSizer::CalculateVR( HDesign& design, std::vector<double> R, std::vector<HCell>& cells, std::vector<double> vX, std::vector<double> vMu )
{
		R.resize(size, 0);

		HTimingPointWrapper tp = design[design.TimingPoints.TopologicalOrderRoot()];
		for (; !::IsNull(tp); tp.GoNext())
		{
				for(HTimingPointWrapper _tp = design[design.TimingPoints.TopologicalOrderRoot()]; _tp != design.TimingPoints.FirstInternalPoint(); tp.GoNext())
				{
						HPin pin = design.Get<HTimingPoint::Pin, HPin>(tp);
						if (design.Get<HPin::Direction, PinDirection>(pin) == PinDirection_INPUT)
						{
								HNet net = design.Get<HPin::Net, HNet>(pin);
								HPin source = design.Get<HNet::Source, HPin>(net);
								
								HTimingPoint point = design.TimingPoints[source];

								if(point == _tp)
								{
										//Here input point for tp is STP
								}
								else
								{
										//Here input point for tp is NOT STP
								}
						}
						else
						{
								if (!design.GetBool<HPin::IsPrimary>(pin))
								{
										HNet net = design.Get<HPin::Net, HNet>(pin);
										HPin source = design[net].Source();
										HPinType sourceType = design.Get<HPin::Type, HPinType>(source);

										for (HPinType::ArcsEnumeratorW arc = design[sourceType].GetArcsEnumeratorW(); arc.MoveNext(); )
												if (arc.TimingType() == TimingType_Combinational)
												{
														HPin tp_Pin=design.Get<HTimingPoint::Pin, HPin>(tp);
														HPin startPin = arc.GetStartPin(tp_Pin);
														HTimingPoint point = design.TimingPoints[startPin];

														if(point == _tp)
														{
																//Here input point for tp is STP
														}
														else
														{
																//Here input point for tp is NOT STP
														}
												}
								}
						}
				}
		}
			
//			throw std::exception("The method or operation is not implemented.");
}

void LRSizer::CalculateVX( std::vector<HCell>& cells, std::vector<double>& vX, std::vector<double> vMu, std::vector<double> vC, std::vector<double> vR )
{
		vX.clear();
		//double x;
		for (unsigned int i=0;i<cells.size();i++)
		{
				//x=min(U_i, max (L_i, sqrt()));
				//vX.push_back(x);
		}
//		throw std::exception("The method or operation is not implemented.");
}

bool LRSizer::CheckStopConditionForLRS_Mu( std::vector<double> prevVX, std::vector<double> nextVX, double accuracy )
{
		double max=0;
		if (prevVX.size()!=nextVX.size())
		{
				ALERT("different size!!!");
				return false;		
		}

		for(unsigned int i=0;i<prevVX.size();i++)
		{
				if (fabs(prevVX[i]-nextVX[i])>max) 
						max=fabs(prevVX[i]-nextVX[i]);
		}

		if (max<accuracy) return true;
		else return false;
}

void LRSizer::SOLVE_LDP(HDesign& design, std::vector<HCell>& cells)
{
		HDPGrid grid(design);
		int StepCounter=1;

		InitLambda(design, 1);
		//InitLambdaMatrix(Lambda, timingPoints);
		HTimingPointWrapper tp = design[design.TimingPoints.TopologicalOrderRoot()];
		for (; !::IsNull(tp); tp.GoNext())
		{
				if (!CheckKuhn_Tucker(design,tp)) ALERT("Check KuhnTucker=false on ::toID(tp)=%d",::ToID(tp));
		}

		bool stop;
		do{
				std::vector<double> vX=SOLVE_LRS_mu(design,cells);
				std::vector<double> arrivalTimes=CalculateArrivalTimes(design);
				
				AdjustingLambda(design, StepCounter,arrivalTimes);

				ProjectLambdaMatrix(design);
				StepCounter++;
				stop=CheckStopConditionForLDP(design, vX, errorBoundForLDP);
		}while(!stop);
		/*
		TODO
		*/
}

double LRSizer::FindOutputLambdaSum(HDesign& design, HTimingPoint point)
{
		double sumOfLambda=.0;
		//Выходные арки
		HPin pin = design.Get<HTimingPoint::Pin, HPin>(point);

		if (design.Get<HPin::Direction, PinDirection>(pin) == PinDirection_OUTPUT)
		{
				sumOfLambda = LambdaIn[::ToID(point)][0];
				if (LambdaIn[::ToID(point)].size() > 1) ALERT("Everything is wrong in FindOutputLambdaSum!!!111");
		}
		else
		{//cell input pin, iterate cell arcs
				HPin& inputPin=pin;
				HCell cell = design.Get<HPin::Cell, HCell>(inputPin);

				for (HCell::PinsEnumeratorW outputPin = design.Get<HCell::Pins, HCell::PinsEnumeratorW>(cell); outputPin.MoveNext(); )
				{
						if (outputPin.Direction() != PinDirection_OUTPUT || ::IsNull(outputPin.Net())) 
								continue;
						
						int i=0;
						HTimingPoint timingPointOfOutputPin = design.TimingPoints[outputPin];
						for (HPinType::ArcsEnumeratorW arc = design.Get<HPinType::ArcTypesEnumerator, HPinType::ArcsEnumeratorW>(outputPin.Type());arc.MoveNext();)
						{
								if (arc.TimingType() == TimingType_Combinational)
								{
										if (arc.GetStartPin(outputPin) == inputPin)
										{
												sumOfLambda+=LambdaIn[::ToID(timingPointOfOutputPin)][i];
										}
										i++;
								}
						}
				}
		}

	return 0;	
}

double LRSizer::FindInputLambdaSum(HDesign& design, HTimingPoint point)
{
		double lambdaSum=.0;
		for (std::vector<double>::iterator lambda = LambdaIn[::ToID(point)].begin(); lambda != LambdaIn[::ToID(point)].end(); lambda++)
				lambdaSum+=*lambda;

		return lambdaSum;
}

bool LRSizer::CheckKuhn_Tucker( HDesign& design, HTimingPoint point)
{
		return (FindOutputLambdaSum(design, point)-
				FindInputLambdaSum(design, point) < DOUBLE_ACCURACY);
}

std::vector<double> LRSizer::CalculateArrivalTimes( HDesign& design )
{
		std::vector<double> arrivalTimes;
		arrivalTimes.reserve(size);
		HTimingPointWrapper tp = design[design.TimingPoints.TopologicalOrderRoot()];
		for (; tp!=design.TimingPoints.FirstInternalPoint(); tp.GoNext())
		{
				//arrivalTimes[::ToID(tp)]=r_i*DownstreamCapacitance
				;
		}

		double maxInArrivalTime=7;
		
		tp = design[design.TimingPoints.FirstInternalPoint()];
		for (; tp!=design.TimingPoints.LastInternalPoint(); tp.GoNext())
		{
				maxInArrivalTime=0;

				HPin pin = design.Get<HTimingPoint::Pin, HPin>(tp);
				if (design.Get<HPin::Direction, PinDirection>(pin) == PinDirection_INPUT)
				{
						HNet net = design.Get<HPin::Net, HNet>(pin);
						HPin source = design.Get<HNet::Source, HPin>(net);
						HTimingPoint point = design.TimingPoints[source];
						if (arrivalTimes[::ToID(point)] > maxInArrivalTime) maxInArrivalTime=arrivalTimes[::ToID(point)];
				}
				else
				{
						if (!design.GetBool<HPin::IsPrimary>(pin))
						{
								HNet net = design.Get<HPin::Net, HNet>(pin);
								HPin source = design[net].Source();
								HPinType sourceType = design.Get<HPin::Type, HPinType>(source);

								for (HPinType::ArcsEnumeratorW arc = design[sourceType].GetArcsEnumeratorW(); arc.MoveNext(); )
										if (arc.TimingType() == TimingType_Combinational)
										{
												HPin tp_Pin=design.Get<HTimingPoint::Pin, HPin>(tp);
												HPin startPin = arc.GetStartPin(tp_Pin);
												HTimingPoint point = design.TimingPoints[startPin];
												if (arrivalTimes[::ToID(point)] > maxInArrivalTime) maxInArrivalTime=arrivalTimes[::ToID(point)];
										}
						}
				}

				//arrivalTimes[::ToID(tp)]=maxInArrivalTime+r_i*DownstreamCapacitance;
		}

		maxInArrivalTime=0;
		HPin pin = design.Get<HTimingPoint::Pin, HPin>(design.TimingPoints.LastInternalPoint());
		if (design.Get<HPin::Direction, PinDirection>(pin) == PinDirection_INPUT)
		{
				HNet net = design.Get<HPin::Net, HNet>(pin);
				HPin source = design.Get<HNet::Source, HPin>(net);
				HTimingPoint point = design.TimingPoints[source];
				if (arrivalTimes[::ToID(point)] > maxInArrivalTime) maxInArrivalTime=arrivalTimes[::ToID(point)];
		}
		else
		{
				if (!design.GetBool<HPin::IsPrimary>(pin))
				{
						HNet net = design.Get<HPin::Net, HNet>(pin);
						HPin source = design[net].Source();
						HPinType sourceType = design.Get<HPin::Type, HPinType>(source);

						for (HPinType::ArcsEnumeratorW arc = design[sourceType].GetArcsEnumeratorW(); arc.MoveNext(); )
								if (arc.TimingType() == TimingType_Combinational)
								{
										HPin tp_Pin=design.Get<HTimingPoint::Pin, HPin>(tp);
										HPin startPin = arc.GetStartPin(tp_Pin);
										HTimingPoint point = design.TimingPoints[startPin];
										if (arrivalTimes[::ToID(point)] > maxInArrivalTime) maxInArrivalTime=arrivalTimes[::ToID(point)];
								}
				}
		}
		//arrivalTimes[::ToID(design.TimingPoints.LastInternalPoint())]=maxInArrivalTime+r_i*DownstreamCapacitance;
		
		return arrivalTimes;
		//throw std::exception("The method or operation is not implemented.");
}

void LRSizer::AdjustingLambda( HDesign& design, int step, std::vector<double> arrivalTimes )
{
		
//		throw std::exception("The method or operation is not implemented.");
}

void LRSizer::ProjectLambdaMatrix( HDesign& design )
{
		throw std::exception("The method or operation is not implemented.");
}

bool LRSizer::CheckStopConditionForLDP( HDesign& design, std::vector<double> vX, double errorBound )
{
		throw std::exception("The method or operation is not implemented.");
}