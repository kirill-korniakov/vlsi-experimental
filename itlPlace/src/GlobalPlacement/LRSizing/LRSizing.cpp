#include "LRSizing.h"
#include <math.h>
#include "LinearRegression.h"
#include "TimingUtils.h"
#include <algorithm>
#include <limits>

#define accuracyForLRS_Mu 0.001
//#define DOUBLE_ACCURACY 1e-9
#define DOUBLE_ACCURACY 1e-4
#define errorBoundForLDP 0.01

//TODO: function for cell family name
//TODO: class CellFamily 
//TODO: GetCellFamilyRC()
//TODO: 

void LRSizer::ApplySizing(std::vector<double>& X)
{
  ASSERT(cells->size()==X.size());

  std::vector<HCell>::iterator cellFrom=cells->begin();
  std::vector<double>::iterator currentX=X.begin();
  int i=0;//DEBUG
  while(cellFrom!=cells->end() && currentX!=X.end())
  {
    HMacroType cellToType=roundCellToTypeFromLib(*cellFrom, *currentX);
    //DEBUG
    HMacroType cellFromType=design[*cellFrom].Type();
    i++;
    if (cellFromType != cellToType)
      ALERT("%d: We are changing it from %s to %s", i, (design.MacroTypes.GetString<HMacroType::Name>(cellFromType)).data(), 
        (design.MacroTypes.GetString<HMacroType::Name>(cellToType)).data());
    //END OF DEBUG
    

    (*cellFrom, design).SetType(cellToType);
    (*cellFrom, design).SetHeight(design.GetDouble<HMacroType::SizeY>(cellToType));
    (*cellFrom, design).SetWidth(design.GetDouble<HMacroType::SizeX>(cellToType));

    cellFrom++;
    currentX++;
  }
}

void LRSizer::getCellFamily(HCell cell, std::vector<HMacroType>& macroTypesInFamily)
{
  string cellName = design.MacroTypes.GetString<HMacroType::Name>((cell,design).Type());
  string cellFamily = cellName.substr( 0, cellName.length()-2 );//TODO: Correct looking for cell family

  for (HMacroTypes::EnumeratorW macroTypeE = design.MacroTypes.GetEnumeratorW(); macroTypeE.MoveNext();)
  {
    string macroTypeFamily = macroTypeE.Name().substr( 0, macroTypeE.Name().length()-2 );//TODO: Correct looking for cell family
    if (macroTypeFamily == cellFamily)
      macroTypesInFamily.push_back(macroTypeE);
  }
}

void LRSizer::getCellFamily(HCell cell, std::vector<double>& cellSizes)
{
  std::vector<HMacroType> macroTypesInFamily;
  getCellFamily(cell, macroTypesInFamily);

  for(std::vector<HMacroType>::iterator macroType = macroTypesInFamily.begin(); macroType != macroTypesInFamily.end(); macroType++)
  {
    double size=design.MacroTypes.GetDouble<HMacroType::SizeX>(*macroType);
    if (std::find(cellSizes.begin(), cellSizes.end(), size) == cellSizes.end())
      cellSizes.push_back(size);
  }
  std::sort(cellSizes.begin(), cellSizes.end());

  //Debugging code
  /*ALERT("size = %d", cellSizes.size());
  for (int i=0;i<cellSizes.size();i++)
  {
    ALERT("%d : %lf", i, cellSizes[i]);
  }*/
  //End of debugging code
}

void LRSizer::getPinFamilyC(HPin pin, std::vector<double>& C, std::vector<double>& X)
{
  HCell cell=(design,pin).Cell();
  string cellName = design.MacroTypes.GetString<HMacroType::Name>((cell,design).Type());
  string cellFamily = cellName.substr( 0, cellName.length()-2 );//TODO: Correct looking for cell family
  string pinName=(pin,design).Name();

  //ALERT(cellFamily);
  //ALERT(pinName);

  std::vector<double> cellSizes;

  for (HMacroTypes::EnumeratorW macroTypeE = design.MacroTypes.GetEnumeratorW(); macroTypeE.MoveNext();)
  {
    string macroTypeFamily = macroTypeE.Name().substr( 0, macroTypeE.Name().length()-2 );//TODO: Correct looking for cell family
    if (macroTypeFamily == cellFamily)
    {
      double size=design.MacroTypes.GetDouble<HMacroType::SizeX>(macroTypeE);
      if (std::find(cellSizes.begin(), cellSizes.end(), size) == cellSizes.end())
      {
        cellSizes.push_back(size);
        for (HMacroType::PinsEnumeratorW pinE = macroTypeE.GetEnumeratorW(); pinE.MoveNext();)
        {
          if ((pinE.Direction()==PinDirection_INPUT)||(pinE.Direction()==PinDirection_OUTPUT))
          {
            if(pinE.Name() == pinName)
            {
              C.push_back(Utils::GetSinkCapacitance(design, pinE, SignalDirection_None));
              X.push_back(macroTypeE.SizeX()); 
              break;
            }
          }
        }
      }

    }
  }
  ASSERT(C.size()==X.size());
  //int i=1;
  //ALERT(cellFamily.data());
  //if (cellFamily.data() == "DFFSRX")
  //{
//    i++;
    //ALERT("DFFSRX");
  //}
}

void LRSizer::getPinFamilyR(HPin pin, std::vector<double>& R, std::vector<double>& X)
{
  /* UNCOMMENT!!!!!
  ASSERT((pin,design).Direction()==PinDirection_INPUT); //Now only for input pins
  */

  HCell cell=(design,pin).Cell();
  string cellName = design.MacroTypes.GetString<HMacroType::Name>((cell,design).Type());
  string cellFamily = cellName.substr( 0, cellName.length()-2 );//TODO: Correct looking for cell family
  string pinName=(pin,design).Name();

  std::vector<double> cellSizes;

  //ALERT(cellFamily);
  //ALERT(pinName);

  for (HMacroTypes::EnumeratorW macroTypeE = design.MacroTypes.GetEnumeratorW(); macroTypeE.MoveNext();)
  {
    string macroTypeFamily = macroTypeE.Name().substr( 0, macroTypeE.Name().length()-2 );//TODO: Correct looking for cell family
    if (macroTypeFamily == cellFamily)
    {
      double size=design.MacroTypes.GetDouble<HMacroType::SizeX>(macroTypeE);
      if (std::find(cellSizes.begin(), cellSizes.end(), size) == cellSizes.end())
      {
        cellSizes.push_back(size);
        for (HMacroType::PinsEnumeratorW pinE = macroTypeE.GetEnumeratorW(); pinE.MoveNext();)
        {
          if ((pinE.Direction()==PinDirection_INPUT)||(pinE.Direction()==PinDirection_OUTPUT))
          {
            if(pinE.Name() == pinName)
            {
              R.push_back(Utils::GetDriverWorstPhisics(design, pinE, SignalDirection_None).R);
              X.push_back(macroTypeE.SizeX());
              break;
            }
          }
        }
      }
    }
  }
  ASSERT(R.size()==X.size());
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
  //R = r*x+f
  size = design._Design.NetList.nPinsLimit;
  ::Grow(&LambdaIn, 0, size);

  //InitLambda(design, 1);
  cells = InitCells();

  HTimingPointWrapper tp = design[design.TimingPoints.FirstInternalPoint()];
  Maths::Regression::Linear* regressionC=getRegressionC(tp);
  if (regressionC) ALERT("RegressedC: %lf", regressionC->getValue(7));

  Maths::Regression::Linear* regressionR=getRegressionR(tp);
  ALERT("RegressedR: %lf", regressionR->getValue(7));

  HCell cell=(design,tp.Pin()).Cell();
  std::vector<double> cellsizes;
  getCellFamily(cell, cellsizes);
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

void LRSizer::SOLVE_LRS_mu(std::vector<double>& NewVX)
{
  std::vector<double> vMu;
  vMu.resize(size, -1);
  FillVMu(vMu);

  std::vector<double> vX;
  
  initVX(vX); //TODO Place for optimization //for all x_i=L_i
  NewVX.resize(vX.size());
  NewVX.assign(vX.begin(), vX.end());
  
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
    //ALERT("Size of vX before before =%d", vX.size());

    //vX=NewVX;
    vX.clear();
    //ALERT("Size of vX after clear   =%d", vX.size());
    vX.assign(NewVX.begin(), NewVX.end());

    //ALERT("Size of vX before        =%d", vX.size());

    UpdateVX(NewVX, vMu);
    //ALERT("Size of vX after         =%d", vX.size());


  } while(!CheckStopConditionForLRS_Mu(vX, NewVX, accuracyForLRS_Mu));
}

double LRSizer::CalcB(HCell& cell,std::vector<double>& vMu,std::vector<double>& vX )
{

  //DEBUG
  //string cellName = design.MacroTypes.GetString<HMacroType::Name>((cell,design).Type());
  //ALERT("CALC B: cellname: %s", cellName.data());

  Maths::Regression::Linear* regressionR;
  double B=0;
  HTimingPoint tp;
  //for(HCell::PinsEnumeratorW pin = cell.GetPinsEnumeratorW(); pin.MoveNext(); )
  for (HCell::PinsEnumeratorW pin = (design, cell).GetPinsEnumeratorW(); pin.MoveNext(); )
  {
    if (pin.Direction() == PinDirection_OUTPUT && !::IsNull(pin.Net()))
    {
      tp = design.TimingPoints[pin];
      regressionR=getRegressionR((design.TimingPoints[pin],design));
      int index = distance(cells->begin(), find(cells->begin(), cells->end(), cell));
      B=B+vMu[::ToID(tp)]*GetObservedC(tp,vX)*(regressionR->getValue(vX[index]))*vX[index];
    }
  }
  return B;
}

double LRSizer::CalcA( HCell& cell,std::vector<double>& vMu,std::vector<double>& vX )
{
  double A=0;
  Maths::Regression::Linear* regressionC;
  HTimingPoint tp;
  for (HCell::PinsEnumeratorW pin = (design, cell).GetPinsEnumeratorW(); pin.MoveNext(); )
  {
    if (pin.Direction() == PinDirection_INPUT && !::IsNull(pin.Net()))
    {
      tp = design.TimingPoints[pin];
      regressionC=getRegressionR((design.TimingPoints[pin],design));
      int index = distance(cells->begin(), find(cells->begin(), cells->end(), cell));
      A=A+(GetWeightedResistance(tp, vX, vMu)+1)*(regressionC->getValue(vX[index]))/vX[index];
    }
  }
  return A;
}

double LRSizer::CalcNewX( HCell& cell,std::vector<double>& vMu,std::vector<double>& vX )
{
  double root;
  root = CalcB(cell,vMu,vX)/CalcA(cell, vMu, vX); 
  std::vector<double> cellSizes;
  getCellFamily(cell,cellSizes);
  return min(cellSizes.back(), max(cellSizes.front(),root));
}

void LRSizer::UpdateVX( std::vector<double>& newVX, std::vector<double>& vMu )
{
  //newVX.reserve(cells.size());
  std::vector<double> tempNewVX;
  double newX=0;
  for(int i=0; i<cells->size();i++)
  {
    newX=CalcNewX((*cells)[i],vMu,newVX);
    tempNewVX.push_back(newX);
  }
  newVX.clear();
  newVX.assign(tempNewVX.begin(), tempNewVX.end());
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

void LRSizer::initVX( std::vector<double>& vX )
{
  //Init all X_i by its lower bound
  vX.reserve(cells->size());
  std::vector<double> cellSizes;
  for (std::vector<HCell>::iterator cellIter=cells->begin(); cellIter!=cells->end();cellIter++)
  {
    getCellFamily(*cellIter,cellSizes);
    vX.push_back(*cellSizes.begin());
  }
}

double LRSizer::GetObservedC( HTimingPoint tp,std::vector<double>& vX )
{
  HPinWrapper pin = design[(design,tp).Pin()];
  Maths::Regression::Linear* regressionC;
  if (pin.Direction() == PinDirection_INPUT)
  { 
    regressionC=getRegressionC((tp,design));

    HCell cell = pin.Cell();
    int index = distance(cells->begin(), find(cells->begin(), cells->end(), cell));
    return (regressionC->getValue(vX[index])) /* +f */; //TODO: implement formulas
    //return 0; //c * x_pin + f 
  }

  HNet net = pin.Net();

  if (pin.Direction() == PinDirection_OUTPUT && !::IsNull(net))
  {
    double sum = (design.SteinerPoints[pin],design).ObservedC();
    for(HNet::SinksEnumeratorW sink = (design,net).GetSinksEnumeratorW(); sink.MoveNext(); )
    {
      //sink can be PO!! ERROR TODO
      if(sink.IsPrimaryOutput()) continue;//This is a DIRTY patch!

      regressionC=getRegressionC((design.TimingPoints[sink],design));
      HCell cell = sink.Cell();
      int index = distance(cells->begin(), find(cells->begin(), cells->end(), cell));
      sum=sum+(regressionC->getValue(vX[index])) /* +f */-(sink.Type(),design).Capacitance();
      double cSink = Utils::GetSinkCapacitance(design, sink.Type(), SignalDirection_None);
      //sum += 0 /*c*x_sink+f*/ - cSink;//TODO: implement formulas// убрать "-"?
      double rWire = design.RoutingLayers.Physics.RPerDist;
    }
    return sum;
  }

  return 0;
}

double LRSizer::GetWeightedResistance( HTimingPoint tp, std::vector<double>& vX, std::vector<double>& vMu )
{
  HPinWrapper pin = design[(design,tp).Pin()];
  Maths::Regression::Linear* regressionR;

  if (pin.Direction() == PinDirection_INPUT)
  {
    if (pin.IsPrimaryInput())
      return 0; // u_pin * R_pin_from_EVR;

    HPin src = (design, pin.Net()).Source();

    if ((src,design).IsPrimary()) return 0; //TODO: It is a DIRTY HACK
    regressionR=getRegressionR((design.TimingPoints[src],design));
    HCell cell = (src,design).Cell();
    int index = distance(cells->begin(), find(cells->begin(), cells->end(), cell));
    return (vMu[::ToID(design.TimingPoints[src])] * (regressionR->getValue(vX[index]))) /* + R_wire(from srs to pin)*/;
    //return 0;// u_src * r_src / x_src + R_wire(from srs to pin);
  }

  HNet net = pin.Net();
  if (pin.Direction() == PinDirection_OUTPUT && !::IsNull(net))
  {
    HCellWrapper cell = design[pin.Cell()];
    double sum = 0;
    for(HCell::PinsEnumeratorW inputPin = cell.GetPinsEnumeratorW(); inputPin.MoveNext(); )
      if (inputPin.Direction() == PinDirection_INPUT && !::IsNull(inputPin.Net()))
      {
        regressionR=getRegressionR((design.TimingPoints[inputPin],design));
        HCell cell = (inputPin,design).Cell();
        int index = distance(cells->begin(), find(cells->begin(), cells->end(), cell));
        sum=sum+vMu[::ToID(design.TimingPoints[inputPin])]*(regressionR->getValue(vX[index]));
        //sum += 0; //u_inputPin * r_inputPin / x_inputPin;
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
    ALERT("delta size!!! = %d", nextVX.size()-prevVX.size());
    return false;		
  }

  for(unsigned int i=0;i<prevVX.size();i++)
  {
    if (fabs(prevVX[i]-nextVX[i])>max) 
      max=fabs(prevVX[i]-nextVX[i]);
  }

  ALERT("max = %lf",max);
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


void LRSizer::SOLVE_LDP()
{
  int stepCounter = 1;

  //1. Set initial lambdas
  InitLambda(1);
  ProjectLambdaMatrix();
  CheckKuhn_Tucker();

  std::vector<double> vX;

  do{

    SOLVE_LRS_mu(vX);
    std::vector<double> arrivalTimes=CalculateArrivalTimes(vX);

    AdjustLambda(stepCounter, arrivalTimes,vX);
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
  return (fabs(FindOutputLambdaSum(point) - FindInputLambdaSum(point)) < DOUBLE_ACCURACY);
}

std::vector<double> LRSizer::CalculateArrivalTimes(std::vector<double>& vX)
{
  std::vector<double> arrivalTimes;
  arrivalTimes.reserve(size);
  for (int i=0;i<size;i++)
    arrivalTimes.push_back(0);

  HTimingPointWrapper tp = design[design.TimingPoints.TopologicalOrderRoot()].GoNext();//TODO Check this GoNEXT
  Maths::Regression::Linear* regressionR;
  for (; tp!=design.TimingPoints.FirstInternalPoint(); tp.GoNext())
  {
    //arrivalTimes[::ToID(tp)]=r_i*DownstreamCapacitance

    if(::IsNull(tp)) continue; //TODO Dirty HACK
    HPinWrapper pin = design[(design,tp).Pin()];
    double aT;
    if(pin.IsPrimary() || ::IsNull(tp)) 
      aT = 0;            //TODO it is DIRTY HACK
    else{
      regressionR=getRegressionR(tp);
      //HPinWrapper pin = design[(design,tp).Pin()];//TODO write a function for getting regression value
      HCell cell = pin.Cell();
      int index = distance(cells->begin(), find(cells->begin(), cells->end(), cell));
      aT = regressionR->getValue(vX[index])*GetObservedC(tp,vX);
    }
    int id=::ToID(tp);
    ALERT("id= %d", id);
    arrivalTimes[id]= aT;
  }

  double maxInArrivalTime;

  tp = design[design.TimingPoints.FirstInternalPoint()];//TODO Check is GoNext needed?
  for (; !::IsNull(tp); tp.GoNext())
  {
    if(design[(design,tp).Pin()].IsPrimary() || ::IsNull(tp)) continue; //TODO Dirty HACK
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
    HCell cell = (pin,design).Cell();
    int index = distance(cells->begin(), find(cells->begin(), cells->end(), cell));
    arrivalTimes[::ToID(tp)]= maxInArrivalTime+regressionR->getValue(vX[index])*GetObservedC(tp,vX);
    //arrivalTimes[::ToID(tp)]=maxInArrivalTime+r_i*DownstreamCapacitance;
  }

  return arrivalTimes;
  //throw std::exception("The method or operation is not implemented.");
}
double LRSizer::CalcDelay(HTimingPointWrapper& tp,std::vector<double>& vX)
{
  Maths::Regression::Linear* regressionR=getRegressionR(tp);
  HPinWrapper pin = design[(design,tp).Pin()];
  HCell cell = pin.Cell();
  int index = distance(cells->begin(), find(cells->begin(), cells->end(), cell));
  return (regressionR->getValue(vX[index])*GetObservedC(tp,vX));  
}

void LRSizer::AdjustLambda( int step, std::vector<double>& arrivalTimes,std::vector<double>& vX)
{
  HTimingPointWrapper tp = design[design.TimingPoints.TopologicalOrderRoot()];
  for (tp.GoPrevious(); tp!=design.TimingPoints.LastInternalPoint(); tp.GoPrevious())
  {
    HNet net = design.Get<HPin::Net, HNet>(design.Get<HTimingPoint::Pin, HPin>(tp));
    HPin source = design[net].Source();
    HTimingPointWrapper tpSrc = design[design.TimingPoints[source]];
    GetPointLambdas(tp)[0]=GetPointLambdas(tp)[0]+(1/step)*(arrivalTimes[::ToID(tpSrc)]/*-A_0*/);// ?? EndPoint always has the only input point ??
  }
  HTimingPointWrapper tmpTp=design[design.TimingPoints.FirstInternalPoint()].GoPrevious();
  for (tp.GoPrevious(); tp!=tmpTp; tp.GoPrevious())
  {
    //HCell cell=(design,design.Get<HTimingPoint::Pin, HPin>(tp)).Cell();

    if (((design.Get<HTimingPoint::Pin, HPin>(tp)),design).Direction() == PinDirection_INPUT)
    {
      HNet net = design.Get<HPin::Net, HNet>(design.Get<HTimingPoint::Pin, HPin>(tp));
      HPin source = design[net].Source();
      HTimingPointWrapper tpSrc = design[design.TimingPoints[source]];
      GetPointLambdas(tp)[0]=GetPointLambdas(tp)[0]+(1/step)*(arrivalTimes[::ToID(tpSrc)]+CalcDelay(tp,vX)-arrivalTimes[::ToID(tp)]);
    }

    int i=0;
    if (((design.Get<HTimingPoint::Pin, HPin>(tp)),design).Direction() == PinDirection_OUTPUT)
    {
      HPin outputPin = design.Get<HTimingPoint::Pin, HPin>(tp);
      HCell cell = (design,outputPin).Cell();
      for (HCell::PinsEnumeratorW inputPin = (design, cell).GetPinsEnumeratorW(); inputPin.MoveNext(); )
      {
        HTimingPointWrapper tpInput = design[design.TimingPoints[inputPin]];
        if (inputPin.Direction() != PinDirection_INPUT || ::IsNull(inputPin.Net())) 
          continue;
        for (HPinType::ArcsEnumeratorW arc = (design,(design,outputPin).Type()).GetArcsEnumeratorW(); arc.MoveNext();)
        {
          if (arc.TimingType() != TimingType_Combinational) continue;
          if (arc.GetStartPin(inputPin) == outputPin)
          {
            GetPointLambdas(tp)[i]=GetPointLambdas(tp)[i]+(1/step)*(arrivalTimes[::ToID(tpInput)]+CalcDelay(tp,vX)-arrivalTimes[::ToID(tp)]);
            i++;
          }

        }
      }   
    }


  }


  //throw std::exception("The method or operation is not implemented.");
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
double LRSizer::CalculateQ(unsigned int size)
{
  double SumInputLambdaForPoint; 
  double SumAllInputLambda=0;
  double QFunction=0;//TODO Implement Q!!!
  std::vector<double> XLower;
  XLower.reserve(size);
  initVX(XLower);
  HTimingPointWrapper tp = design[design.TimingPoints.TopologicalOrderRoot()];
  for (; tp!=design.TimingPoints.FirstInternalPoint(); tp.GoNext())
  {
    SumInputLambdaForPoint=FindInputLambdaSum(tp);
    
    HPinWrapper pin = design[(design,tp).Pin()];
    HCell cell = pin.Cell();
    if (::IsNull(cell)) continue;
    Maths::Regression::Linear* regressionR=getRegressionR(tp);
    
    int index = distance(cells->begin(), find(cells->begin(), cells->end(), cell));
    SumAllInputLambda+=SumInputLambdaForPoint*(regressionR->getValue(XLower[index])*GetObservedC(tp,XLower));
    //SumAllInputLambda+=SumInputLambdaForPoint*(r_i*DownStreamCap_i);//при x_i = lower_bound_X[i]
  }

  double SumMinX=0;
  for(unsigned int i=0; i < XLower.size(); i++)
  SumMinX+=XLower[i];
  QFunction=SumMinX+SumAllInputLambda;
  return QFunction;
}
bool LRSizer::CheckStopConditionForLDP(std::vector<double>& vX, double errorBound)
{
  double Sum=0;
  for(unsigned int i=0; i < vX.size(); i++)
    Sum+=vX[i];
  if ((Sum-CalculateQ(vX.size()))<errorBound) return true;
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

  //if (C.size() <= 1) ALERT("c.size<=1");

  return new Maths::Regression::Linear(C.size(), arrX, arrC);
}

Maths::Regression::Linear* LRSizer::getRegressionR( HTimingPointWrapper tp )
{
  HPin pin=tp.Pin();

  std::vector<double> R, X;

  getPinFamilyR(pin, R, X);

  int numOfAlternatives=R.size();
  double* arrR=new double[numOfAlternatives];
  copy(R.begin(), R.end(), arrR);

  double* arrX=new double[numOfAlternatives];
  copy(X.begin(), X.end(), arrX);

  return new Maths::Regression::Linear(R.size(), arrX, arrR);
}

HMacroType LRSizer::roundCellToTypeFromLib(HCell cellFrom, double currentX)
{
  std::vector<HMacroType> macroTypesInFamily;
  getCellFamily(cellFrom, macroTypesInFamily);
  double smallestDeltaX=std::numeric_limits<double>::max();
  HMacroType bestMacroType;
  for(std::vector<HMacroType>::iterator macroType = macroTypesInFamily.begin(); macroType != macroTypesInFamily.end(); macroType++)
  {
    double deltaX = abs(currentX - design.MacroTypes.GetDouble<HMacroType::SizeX>(*macroType));
    if (deltaX < smallestDeltaX)
    {
      smallestDeltaX = design.MacroTypes.GetDouble<HMacroType::SizeX>(*macroType);
      bestMacroType = *macroType;
    }
  }

  return bestMacroType;
}

void LRSizer::doLRSizing()
{
  FILE* fbefore = fopen("./log/fbefore.csv","w");
  for (int i=0; i<cells->size(); i++)
  {
    HCell cell=(*cells)[i];
    HCellWrapper cellW=(design, cell);
    
    double width = design.Cells.GetDouble<HCell::Width>(cellW);
    fprintf (fbefore, "%d;%lf\n", i, width);

  }
  fclose(fbefore);

  std::vector<double> newVX;
  SOLVE_LDP();
  SOLVE_LRS_mu(newVX);
  
  FILE* fafter = fopen("./log/fafter.csv","w"); 
  for (int i=0; i<newVX.size(); i++)
  {
    fprintf (fafter, "%d;%lf\n", i, newVX[i]);
  }
  fclose(fafter);

  ApplySizing(newVX);

  FILE* fafter2 = fopen("./log/fafter2.csv","w");
  for (int i=0; i<cells->size(); i++)
  {
    HCell cell=(*cells)[i];
    HCellWrapper cellW=(design, cell);
    
    double width = design.Cells.GetDouble<HCell::Width>(cellW);
    fprintf ( fafter2, "%d;%lf\n", i, width);

  }
  fclose( fafter2);
}