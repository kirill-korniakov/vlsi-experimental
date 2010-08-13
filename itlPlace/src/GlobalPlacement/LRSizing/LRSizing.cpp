#include "LRSizing.h"
#include <math.h>
#include "LinearRegression.h"

#define accuracyForLRS_Mu 0.001
#define DOUBLE_ACCURACY 1e-9
#define errorBoundForLDP 0.01

void LRSizer::getPinFamilyC(HPin pin, std::vector<double>& C, std::vector<double>& X)
{
  HCell cell=(design,pin).Cell();
  string cellName = design.MacroTypes.GetString<HMacroType::Name>((cell,design).Type());
  string cellFamily = cellName.substr( 0, cellName.length()-2 );//TODO: Correct looking for cell family
  string pinName=(pin,design).Name();

  //ALERT(cellFamily);
  //ALERT(pinName);

  for (HMacroTypes::EnumeratorW macroTypeE = design.MacroTypes.GetEnumeratorW(); macroTypeE.MoveNext();)
  {
    string macroTypeFamily = macroTypeE.Name().substr( 0, macroTypeE.Name().length()-2 );//TODO: Correct looking for cell family
    if (macroTypeFamily == cellFamily)
      for (HMacroType::PinsEnumeratorW pinE = macroTypeE.GetEnumeratorW(); pinE.MoveNext();)
      {
        if ((pinE.Direction()==PinDirection_INPUT)||(pinE.Direction()==PinDirection_OUTPUT))
        {
          if(pinE.Name() == pinName)
          {
            C.push_back(pinE.Capacitance());
            X.push_back(macroTypeE.SizeX()); //TODO: Check, what is it!!
            break;
          }
        }
      }
  }
  ASSERT(C.size()==X.size());
}

int LRSizer::CountLeftArcs(HDesign& design, HNet net)
{
  return CountLeftArcs(design, design[net].Source());
}

int LRSizer::CountLeftArcs(HDesign& design, HPin pin)
{
  if (design.Get<HPin::Direction, PinDirection>(pin) == PinDirection_INPUT)
  {
    if (!::IsNull(design.Get<HPin::Net, HNet>(pin)))
      return 1;
    return 0;
  }

  HPinType pinType = design.Get<HPin::Type, HPinType>(pin);

  int i = 0;
  for (HPinType::ArcsEnumeratorW arc = design[pinType].GetArcsEnumeratorW(); arc.MoveNext(); )
    if (arc.TimingType() == TimingType_Combinational)
      i++;

  return i;
}

LRSizer::LRSizer(HDesign& hd): design(hd), LambdaIn(0)
{
  size = design._Design.NetList.nPinsLimit;
  ::Grow(&LambdaIn, 0, size);

  //InitLambda(design, 1);
  cells = InitCells();
  
  HTimingPointWrapper tp = design[design.TimingPoints.FirstInternalPoint()];
  Maths::Regression::Linear* regressionC=getRegressionC(tp);
  ALERT("Regressed: %lf", regressionC->getValue(7));
}

LRSizer::~LRSizer()
{
  ::Grow(&LambdaIn, size, 0);
  delete cells;
}

void LRSizer::InitLambda(double defaultLambda)
{
  for (HTimingPointWrapper tp = design[design.TimingPoints.FirstInternalPoint()]; !::IsNull(tp); tp.GoNext() )
  {
    int arcsCount = CountLeftArcs(design, tp.Pin());
    if (arcsCount > 0)
      GetPointLambdas(tp).resize(arcsCount, defaultLambda);
  }
}

std::vector<HCell>* LRSizer::InitCells()
{
  std::vector<HCell>* cells = new std::vector<HCell>;
  int index = 1;
  HCell tempCell;

  //Going through whole circuit in reverse topological order
  for (HTimingPointWrapper tp = design[design.TimingPoints.TopologicalOrderRoot()];
    !::IsNull(tp.GoPrevious()); )
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

void LRSizer::SOLVE_LRS_mu(std::vector<HCell>& cells, std::vector<double>& NewVX)
{
  std::vector<double> vMu;
  vMu.resize(size, -1);
  FillVMu(vMu);

  std::vector<double> vX;
  vX.resize(cells.size());

  initVX(vX, cells); //for all x_i=L_i
  //NewVX.resize(vX.size());
  //NewVX.assign(vX.begin(), vX.end());



  do
  {
    ///for (HTimingPointWrapper tp = design[design.TimingPoints.TopologicalOrderRoot()]; !::IsNull(tp.GoNext()); )
    ///{
    ///  double Ui = 2.0;//TODO: replace these constants with correct bounds
    ///  double Li = 0.5;
    ///  double ri = 1;
    ///  double ci = 1;

    ///  double mu = vMu[::ToID(tp)];
    ///  //NewVX[] = min(
    ///}

    //vX=NewVX;

    //UpdateVX(cells, NewVX, vMu);


  } while(!CheckStopConditionForLRS_Mu(vX, NewVX, accuracyForLRS_Mu));
}
double LRSizer::CalcB(HCell& cell,std::vector<double>& vMu)
{
  double B=0;
  HTimingPoint tp;
  //for(HCell::PinsEnumeratorW pin = cell.GetPinsEnumeratorW(); pin.MoveNext(); )
  for (HCell::PinsEnumeratorW pin = (design, cell).GetPinsEnumeratorW(); pin.MoveNext(); )
  {
    if (pin.Direction() == PinDirection_OUTPUT && !::IsNull(pin.Net()))
    {
      tp = design.TimingPoints[pin];
      B+= vMu[::ToID(tp)] /* *r_pin */ *GetObservedC(tp);
    }
  }
  return B;
}
double LRSizer::CalcA(HCell& cell,std::vector<double>& vMu,std::vector<double>& vX)
{
  double A=0;
  HTimingPoint tp;
  for (HCell::PinsEnumeratorW pin = (design, cell).GetPinsEnumeratorW(); pin.MoveNext(); )
  {
    if (pin.Direction() == PinDirection_INPUT && !::IsNull(pin.Net()))
    {
      tp = design.TimingPoints[pin];
      A+= GetWeightedResistance(tp, vX, vMu)/* *c_pin  */;
    }
  }
  return A;
}
double LRSizer::CalcNewX(HCell& cell,std::vector<double>& vMu,std::vector<double>& vX)
{
  double NewX=0;
  double root;
  root = CalcB(cell,vMu)/CalcA(cell, vMu, vX); 
  //NewX=min(U_i, max(L_i,root));
  return NewX;
}
void LRSizer::UpdateVX(std::vector<HCell>& cells, std::vector<double>& newVX, std::vector<double>& vMu)
{
  newVX.reserve(cells.size());
  double newX=0;
  for(int i=0; i<cells.size();i++)
  {
    newX=CalcNewX(cells[i],vMu,newVX);
    newVX.push_back(newX);
  }
}

void LRSizer::FillVMu( std::vector<double>& vMu )
{
  //Sum of all Lambda inputs of the point
  for (HTimingPointWrapper tp = design[design.TimingPoints.FirstInternalPoint()]; !::IsNull(tp); tp.GoNext() )
  {
    double lambdaSum = 0;
    for (std::vector<double>::iterator lambda = GetPointLambdas(tp).begin();
      lambda != GetPointLambdas(tp).end(); lambda++)
    {
      lambdaSum += *lambda;
    }

    vMu[::ToID(tp)] = lambdaSum;
  }
}

void LRSizer::initVX( std::vector<double>& vX, std::vector<HCell>& cells)
{
  //Init all X_i by its lower bound
  vX.assign(cells.size(), 1);
}

double LRSizer::GetObservedC(HTimingPoint tp)
{
  HPinWrapper pin = design[(design,tp).Pin()];

  if (pin.Direction() == PinDirection_INPUT)
    return 0; //c * x_pin + f //TODO: implement formulas

  HNet net = pin.Net();
  if (pin.Direction() == PinDirection_OUTPUT && !::IsNull(net))
  {
    double sum = (design.SteinerPoints[pin],design).ObservedC();
    for(HNet::SinksEnumeratorW sink = (design,net).GetSinksEnumeratorW(); sink.MoveNext(); )
    {
      sum += 0 /*c*x_sink+f*/ - (sink.Type(),design).Capacitance();//TODO: implement formulas// убрать "-"?
    }
    return sum;
  }

  return 0;
}

double LRSizer::GetWeightedResistance(HTimingPoint tp, std::vector<double>& vX, std::vector<double>& vMu)
{
  HPinWrapper pin = design[(design,tp).Pin()];

  if (pin.Direction() == PinDirection_INPUT)
  {
    if (pin.IsPrimaryInput())
      return 0; // u_pin * R_pin_from_EVR;

    HPin src = (design, pin.Net()).Source();

    return 0;// u_src * r_src / x_src + R_wire(from srs to pin);
  }

  HNet net = pin.Net();
  if (pin.Direction() == PinDirection_OUTPUT && !::IsNull(net))
  {
    HCellWrapper cell = design[pin.Cell()];
    double sum = 0;
    for(HCell::PinsEnumeratorW inputPin = cell.GetPinsEnumeratorW(); inputPin.MoveNext(); )
      if (inputPin.Direction() == PinDirection_INPUT && !::IsNull(inputPin.Net()))
      {
        sum += 0; //u_inputPin * r_inputPin / x_inputPin;
      }
  }

  return 0;
}

//void LRSizer::CalculateVC(std::vector<double>& C, std::vector<HCell>& cells, std::vector<double> vX )
//{
//  C.resize(size, 0);
//
//  HTimingPointWrapper tep = design[design.TimingPoints.TopologicalOrderRoot()];
//  for (tep.GoPrevious(); tep!=design.TimingPoints.LastInternalPoint(); tep.GoPrevious())
//  {
//    C[::ToID(tep)] = ((tep.Pin(),design).Type(),design).Capacitance();
//  }
//
//  HTimingPointWrapper tp = design[design.TimingPoints.LastInternalPoint()];
//  HTimingPoint lastStartPoint = design[design.TimingPoints.FirstInternalPoint()].GoPrevious();
//  for (; tp != lastStartPoint; tp.GoPrevious())
//  {
//    Выходные арки
//    HPin pin = tp.Pin();
//
//    if (design.Get<HPin::Direction, PinDirection>(pin) == PinDirection_OUTPUT)
//    {//cell output pin, iterate net arcs
//      for (HNet::SinksEnumeratorW sink = design.Get<HNet::Sinks, HNet::SinksEnumeratorW>(design.Get<HPin::Net, HNet>(pin));sink.MoveNext();)
//      {
//        HTimingPoint timingPoint = design.TimingPoints[sink];
//        C[::ToID(timingPoint)]=C[::ToID(timingPoint)] + c_k x_k + f_k
//      }
//    }
//    else
//    {//cell input pin, iterate cell arcs
//      HCell cell = design.Get<HPin::Cell, HCell>(pin);
//
//      for (HCell::PinsEnumeratorW _pin = design.Get<HCell::Pins, HCell::PinsEnumeratorW>(cell); _pin.MoveNext(); )
//      {
//        if (_pin.Direction() != PinDirection_OUTPUT || ::IsNull(_pin.Net())) 
//          continue;
//
//        HTimingPoint point = design.TimingPoints[_pin];
//        for (HPinType::ArcsEnumeratorW arc = design.Get<HPinType::ArcTypesEnumerator, HPinType::ArcsEnumeratorW>(_pin.Type());arc.MoveNext();)
//        {
//          if (arc.TimingType() == TimingType_Combinational)
//          {
//            if (arc.GetStartPin(_pin) == pin)
//            {
//              HTimingPoint timingPoint = point;
//              C[::ToID(timingPoint)]=C[::ToID(timingPoint)] + c_k x_k + f_k
//            }
//          }
//        }
//      }
//    }
//  }
//  	throw std::exception("The method or operation is not implemented.");
//}



//void LRSizer::CalculateVR( std::vector<double> R, std::vector<HCell>& cells, std::vector<double> vX, std::vector<double> vMu )
//{
//  R.resize(size, 0);
//
//  HTimingPointWrapper tp = design[design.TimingPoints.TopologicalOrderRoot()];
//  for (; !::IsNull(tp); tp.GoNext())
//  {
//    for(HTimingPointWrapper _tp = design[design.TimingPoints.TopologicalOrderRoot()]; _tp != design.TimingPoints.FirstInternalPoint(); tp.GoNext())
//    {
//      HPin pin = design.Get<HTimingPoint::Pin, HPin>(tp);
//      if (design.Get<HPin::Direction, PinDirection>(pin) == PinDirection_INPUT)
//      {
//        HNet net = design.Get<HPin::Net, HNet>(pin);
//        HPin source = design.Get<HNet::Source, HPin>(net);
//
//        HTimingPoint point = design.TimingPoints[source];
//
//        if(point == _tp)
//        {
//          //Here input point for tp is STP
//        }
//        else
//        {
//          //Here input point for tp is NOT STP
//        }
//      }
//      else
//      {
//        if (!design.GetBool<HPin::IsPrimary>(pin))
//        {
//          HNet net = design.Get<HPin::Net, HNet>(pin);
//          HPin source = design[net].Source();
//          HPinType sourceType = design.Get<HPin::Type, HPinType>(source);
//
//          for (HPinType::ArcsEnumeratorW arc = design[sourceType].GetArcsEnumeratorW(); arc.MoveNext(); )
//            if (arc.TimingType() == TimingType_Combinational)
//            {
//              HPin tp_Pin=design.Get<HTimingPoint::Pin, HPin>(tp);
//              HPin startPin = arc.GetStartPin(tp_Pin);
//              HTimingPoint point = design.TimingPoints[startPin];
//
//              if(point == _tp)
//              {
//                //Here input point for tp is STP
//              }
//              else
//              {
//                //Here input point for tp is NOT STP
//              }
//            }
//        }
//      }
//    }
//  }
//
//  //			throw std::exception("The method or operation is not implemented.");
//}

//void LRSizer::CalculateVX( std::vector<HCell>& cells, std::vector<double>& vX, std::vector<double> vMu, std::vector<double> vC, std::vector<double> vR )
//{
//  vX.clear();
//  //double x;
//  for (unsigned int i=0;i<cells.size();i++)
//  {
//    //x=min(U_i, max (L_i, sqrt()));
//    //vX.push_back(x);
//  }
//  //		throw std::exception("The method or operation is not implemented.");
//}

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

void LRSizer::CheckKuhn_Tucker()
{
#ifdef DEBUG
  HTimingPointWrapper tp = design[design.TimingPoints.TopologicalOrderRoot()];
  for (; !::IsNull(tp); tp.GoNext())
  {
    if (!CheckKuhn_Tucker(tp)) ALERT("Check KuhnTucker=false on ::toID(tp)=%d",::ToID(tp));
  }
#endif
}


void LRSizer::SOLVE_LDP(std::vector<HCell>& cells)
{
  int stepCounter = 1;

  //1. Set initial lambdas
  InitLambda(1);
  ProjectLambdaMatrix();
  CheckKuhn_Tucker();

  std::vector<double> vX;

  do{

    SOLVE_LRS_mu(cells,vX);
    std::vector<double> arrivalTimes=CalculateArrivalTimes();

    AdjustLambda(stepCounter, arrivalTimes);
    ProjectLambdaMatrix();

    stepCounter++;
  } while (!CheckStopConditionForLDP(vX, errorBoundForLDP));
}

double LRSizer::FindOutputLambdaSum(HTimingPoint point)
{
  double sumOfLambda = 0;

  //Выходные арки
  HPin pin = design.Get<HTimingPoint::Pin, HPin>(point);

  if (design.Get<HPin::Direction, PinDirection>(pin) == PinDirection_OUTPUT)
  {
    HNet net = (design,pin).Net();
    ASSERT(design[net].Source() == pin);

    for(HNet::SinksEnumeratorW sink = (design,net).GetSinksEnumeratorW();
      sink.MoveNext(); )
    {
      std::vector<double>& lambdas = GetPointLambdas(design.TimingPoints[sink]);
      ASSERT(lambdas.size() == 1);
      sumOfLambda += lambdas[0];
    }
    return sumOfLambda;
  }

  if (design.Get<HPin::Direction, PinDirection>(pin) == PinDirection_INPUT)
  {
    //cell input pin, iterate cell arcs
    HPin inputPin = pin;
    HCell cell = (design,inputPin).Cell();
    //шпаргалка
    // (design,cell) \
    // (cell,design) --> HCellWrapper
    // design[cell]  /
    // design.Cells.MakeWrapper(cell)

    for (HCell::PinsEnumeratorW outputPin = (design, cell).GetPinsEnumeratorW();
      outputPin.MoveNext(); )
    {
      if (outputPin.Direction() != PinDirection_OUTPUT || ::IsNull(outputPin.Net())) 
        continue;

      int i=0;
      HTimingPoint timingPointOfOutputPin = design.TimingPoints[outputPin];

      for (HPinType::ArcsEnumeratorW arc = (design,outputPin.Type()).GetArcsEnumeratorW(); arc.MoveNext();)
      {
        if (arc.TimingType() != TimingType_Combinational) continue;
        if (arc.GetStartPin(outputPin) == inputPin)
          sumOfLambda += GetPointLambdas(timingPointOfOutputPin)[i];
        i++;
      }
    }
  }

  return 0;
}

double LRSizer::FindInputLambdaSum(HTimingPoint point)
{
  double lambdaSum = 0;
  for (std::vector<double>::iterator lambda = GetPointLambdas(point).begin();
    lambda != GetPointLambdas(point).end(); lambda++)
    lambdaSum += *lambda;

  return lambdaSum;
}

bool LRSizer::CheckKuhn_Tucker( HTimingPoint point)
{
  return (FindOutputLambdaSum(point) - FindInputLambdaSum(point) < DOUBLE_ACCURACY);
}

std::vector<double> LRSizer::CalculateArrivalTimes()
{
  std::vector<double> arrivalTimes;
  arrivalTimes.reserve(size);
  HTimingPointWrapper tp = design[design.TimingPoints.TopologicalOrderRoot()];
  for (; tp!=design.TimingPoints.FirstInternalPoint(); tp.GoNext())
  {
    //arrivalTimes[::ToID(tp)]=r_i*DownstreamCapacitance
    ;
  }

  double maxInArrivalTime;

  tp = design[design.TimingPoints.FirstInternalPoint()];
  for (; !::IsNull(tp); tp.GoNext())
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
      if (!design.GetBool<HPin::IsPrimary>(pin))//??
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

  return arrivalTimes;
  //throw std::exception("The method or operation is not implemented.");
}

void LRSizer::AdjustLambda( int step, std::vector<double> arrivalTimes )
{
  throw std::exception("The method or operation is not implemented.");
}

void LRSizer::ProjectLambdaMatrix()
{
  double MuIn,MuOut; 
  std::vector<double> percentage;
  //double* lambda;

  HTimingPointWrapper tp = design[design.TimingPoints.TopologicalOrderRoot()];
  for (tp.GoPrevious(); !::IsNull(tp); tp.GoPrevious())
  {
    percentage.reserve(GetPointLambdas(tp).size());
    MuIn=FindInputLambdaSum(tp);
    MuOut=FindOutputLambdaSum(tp);

    for (std::vector<double>::iterator lambdaIter = GetPointLambdas(tp).begin();
      lambdaIter != GetPointLambdas(tp).end(); lambdaIter++)
      percentage.push_back(*lambdaIter/MuIn);
    for(int i=0;i<GetPointLambdas(tp).size();i++)
    {
      GetPointLambdas(tp)[i]=percentage[i]*MuOut;
    }
  }
}
double LRSizer::CalculateQ(std::vector<double>& vX)
{
  double SumInputLambdaForPoint; 
  double SumAllInputLambda=0;
  double QFunction;
  HTimingPointWrapper tp = design[design.TimingPoints.TopologicalOrderRoot()];
  for (; tp!=design.TimingPoints.FirstInternalPoint(); tp.GoNext())
  {
    SumInputLambdaForPoint=FindInputLambdaSum(tp);
    //SumAllInputLambda+=SumInputLambdaForPoint*(r_i*DownStreamCap_i);//при x_i = lower_bound_X[i]
  }

  double SumMinX=0;
  //for(unsigned int i=0; i < vX.size(); i++)
  //SumMinX+=lower_bound_X[i];
  //QFunction=SumMinX+SumAllInputLambda;
  return QFunction;
}
bool LRSizer::CheckStopConditionForLDP(std::vector<double>& vX, double errorBound )
{
  double Sum=0;
  for(unsigned int i=0; i < vX.size(); i++)
    Sum+=vX[i];
  if ((Sum-CalculateQ(vX))<errorBound) return true;
  else false;
}

Maths::Regression::Linear* LRSizer::getRegressionC( HTimingPointWrapper tp )
{
  HPin pin=tp.Pin();

  std::vector<double> C, X;

  getPinFamilyC(pin, C, X);

  int numOfAlternatives=C.size();
  double* arrC=new double[numOfAlternatives];
  copy(C.begin(), C.end(), arrC);

  double* arrX=new double[numOfAlternatives];
  copy(X.begin(), X.end(), arrX);

  return new Maths::Regression::Linear(C.size(), arrX, arrC);
}