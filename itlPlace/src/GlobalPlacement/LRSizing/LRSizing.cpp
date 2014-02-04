#include "LRSizing.h"
#include <math.h>
#include "LinearRegression.h"
#include "TimingUtils.h"
#include "AdaptiveRoute.h"
#include <algorithm>
#include <limits>

#define accuracyForLRS_Mu 0.001
#define DOUBLE_ACCURACY 1e-4
#define errorBoundForLDP 0.01

#define MODE_TEST 1

//TODO: function for cell family name
//TODO: class CellFamily 
//TODO: GetCellFamilyRC()
//TODO обход всех нетов и для каждого получаем вектор отр
void LRSizer::ApplySizing(std::vector<double>& X)
{
  ASSERT(cells->size() == X.size());
  std::vector<HCell>::iterator cellFrom = cells->begin();
  std::vector<double>::iterator currentX = X.begin();
  int i = 0;//DEBUG
  int numOfChanges = 0;
  while(cellFrom != cells->end() && currentX != X.end())
  {
    HMacroType cellToType = RoundCellToTypeFromLib(*cellFrom, *currentX);
    //DEBUG
    double roundingToSize = timer->GetMacroTypeSize(cellToType);
    //ALERT("Rounding from %lf to %lf", *currentX, roundingToSize);
    HMacroType cellFromType = design[*cellFrom].Type();
    i++;
    if (cellFromType != cellToType)
    {
      numOfChanges++;

      double fromWidth = timer->GetMacroTypeSize(cellFromType);
      double toWidth = timer->GetMacroTypeSize(cellToType);
      ALERT("%d: We are changing it from %s (%lf) to %s (%lf)", i, 
        (design.MacroTypes.GetString<HMacroType::Name>(cellFromType)).data(), fromWidth, 
        (design.MacroTypes.GetString<HMacroType::Name>(cellToType)).data(), toWidth);
    }
    //END OF DEBUG

    (*cellFrom, design).SetType(cellToType);
    (*cellFrom, design).SetHeight(design.GetDouble<HMacroType::SizeY>(cellToType));
    (*cellFrom, design).SetWidth(design.GetDouble<HMacroType::SizeX>(cellToType));
    cellFrom++;
    currentX++;
  }
  ALERT("Changed types of %d elements", numOfChanges);
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
  cells = new std::vector<HCell>;
  InitCells(cells);
  timer = new LRSizingTimer(cells, size, design);

  //DEBUG
  timer->PrintPinsCharacteristics();
  //GetStPEdges();
  timer->PrintMacroTypeCharacteristics();
  //END OF DEBUG
}

LRSizer::~LRSizer()
{
  ::Grow(&LambdaIn, size, 0);
  delete cells;
}

void LRSizer::InitLambda(double defaultLambda)
{
  for (HTimingPointWrapper tp = design[design.TimingPoints.FirstInternalPoint()]; !::IsNull(tp); tp.GoNext())
  {
    int arcsCount = CountLeftArcs(design, tp.Pin());
    if (arcsCount > 0)
      GetPointLambdas(tp).resize(arcsCount, defaultLambda);
  }
}

void LRSizer::InitCells(std::vector<HCell>* cells)
{
  int index = 1;
  HCell tempCell;
  //Going through whole circuit in reverse topological order
  for (HTimingPointWrapper tp = design[design.TimingPoints.TopologicalOrderRoot()]; 
    !::IsNull(tp.GoPrevious());)
  {
    CRITICAL_ASSERT(!::IsNull(tp));
    HPin pin = tp.Pin();
    tempCell = design[pin].Cell();
    if (::IsNull(tempCell)) continue;
    bool isCellNotInCells = true;
    for (std::vector<HCell>::iterator cell = cells->begin(); cell != cells->end(); cell++)
      if (*cell == tempCell){isCellNotInCells = false; break;}
      if (isCellNotInCells)
        cells->push_back(tempCell);
  }
}

void LRSizer::SolveLrsMu(std::vector<double>& NewVX)
{
  std::vector<double> vMu;
  vMu.resize(size, -1);
  FillVMu(vMu);
  std::vector<double> vX;
  InitVxByLowerBound(vX); //TODO Place for optimization //for all x_i=L_i
  NewVX.resize(vX.size());
  NewVX.assign(vX.begin(), vX.end());
  int stepCounter = 1;
  do
  {
    ///for (HTimingPointWrapper tp = design[design.TimingPoints.TopologicalOrderRoot()]; 
    /// !::IsNull(tp.GoNext()); )
    ///{
    ///  double Ui = 2.0;//TODO: replace these constants with correct bounds
    ///  double Li = 0.5;
    ///  double ri = 1;
    ///  double ci = 1;

    ///  double mu = vMu[::ToID(tp)];
    ///  //NewVX[] = min(
    ///}
    //ALERT("Size of vX before before =%d", vX.size());

    vX.clear();
    vX=NewVX;
    UpdateVX(NewVX, vMu);
    ALERT("Criterion on %d-th iteration of LRS_MU is %lf", stepCounter, CalcCriterion(NewVX));
    stepCounter++;
  } while(!CheckStopConditionForLRS_Mu(vX, NewVX, accuracyForLRS_Mu));

  ALERT("Number of iterations of LRS_MU is %d", stepCounter - 1);
}

double LRSizer::CalcB(HCell& cell,std::vector<double>& vMu,std::vector<double>& vX )
{
  double B = 0;
  /*TODO: Rewrite method according to the fact that R belongs to ARC
  Maths::Regression::Linear* regressionR;
  HTimingPoint tp;
  for (HCell::PinsEnumeratorW pin = (design, cell).GetPinsEnumeratorW(); pin.MoveNext(); )
  {
    if (pin.Direction() == PinDirection_OUTPUT && !::IsNull(pin.Net()))
    {
      tp = design.TimingPoints[pin];
      regressionR = GetRegressionR((design.TimingPoints[pin], design));
      int index = (int)distance(cells->begin(), find(cells->begin(), cells->end(), cell));
      B += vMu[::ToID(tp)] * GetObservedC(tp,vX) * (regressionR->getValue(1 / vX[index])) * vX[index];
    }
  }*/
  return B;
}

double LRSizer::CalcA(HCell& cell, std::vector<double>& vMu, std::vector<double>& vX)
{
  double A = 0;
  /*TODO: Rewrite method according to the fact that R belongs to ARC
  Maths::Regression::Linear* regressionC;
  HTimingPoint tp;
  for (HCell::PinsEnumeratorW pin = (design, cell).GetPinsEnumeratorW(); pin.MoveNext(); )
  {
    if (pin.Direction() == PinDirection_INPUT && !::IsNull(pin.Net()))
    {
      tp = design.TimingPoints[pin];
      regressionC = GetRegressionR((design.TimingPoints[pin], design));
      int index = (int)distance(cells->begin(), find(cells->begin(), cells->end(), cell));
      A += (GetWeightedResistance(tp, vX, vMu) + 1) * (regressionC->getValue(vX[index])) / vX[index];
    }
  }*/
  return A;
}

double LRSizer::CalcNewX(HCell& cell,std::vector<double>& vMu,std::vector<double>& vX)
{
  double root = sqrt(CalcB(cell,vMu,vX) / CalcA(cell, vMu, vX)); 
  std::vector<double> cellSizes;
  timer->GetCellFamily(cell, cellSizes);
  return std::min(cellSizes.back(), std::max(cellSizes.front(), root));
}

void LRSizer::UpdateVX(std::vector<double>& newVX, std::vector<double>& vMu)
{
  //newVX.reserve(cells.size());
  std::vector<double> tempNewVX;
  double newX = 0;
  for(unsigned int i = 0; i < cells->size(); i++)
  {
    newX = CalcNewX((*cells)[i], vMu, newVX);
    tempNewVX.push_back(newX);
  }
  newVX.clear();
  newVX.assign(tempNewVX.begin(), tempNewVX.end());
}

void LRSizer::FillVMu(std::vector<double>& vMu)
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

void LRSizer::InitVxByLowerBound(std::vector<double>& vX)
{
  vX.reserve(cells->size());
  std::vector<double> cellSizes;
  for (std::vector<HCell>::iterator cellIter = cells->begin(); cellIter != cells->end(); cellIter++)
  {
    timer->GetCellFamily(*cellIter, cellSizes);
    vX.push_back(*cellSizes.begin());
  }
}

void LRSizer::initVX(std::vector<double>& vX)
{
  vX.clear();
  for(std::vector<HCell>::iterator iter = cells->begin(); iter != cells->end(); iter++)
    vX.push_back(timer->GetMacroTypeSize((*iter, design).Type()));
}

double LRSizer::GetWeightedResistance(HTimingPoint tp, std::vector<double>& vX, std::vector<double>& vMu)
{
  /*TODO: Rewrite method according to the fact that R belongs to ARC
  HPinWrapper pin = design[(design,tp).Pin()];
  Maths::Regression::Linear* regressionR;
  if (pin.Direction() == PinDirection_INPUT)
  {
    if (pin.IsPrimaryInput())
      return 0; // u_pin * R_pin_from_EVR;
    HPin src = (design, pin.Net()).Source();
    if ((src, design).IsPrimary()) return 0; //TODO: It is a DIRTY HACK
    regressionR = GetRegressionR((design.TimingPoints[src], design));
    HCell cell = (src, design).Cell();
    int index = (int)distance(cells->begin(), find(cells->begin(), cells->end(), cell));
    return (vMu[::ToID(design.TimingPoints[src])] * (regressionR->getValue(1 / vX[index]))); // + R_wire(from srs to pin);
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
        regressionR = GetRegressionR((design.TimingPoints[inputPin], design));
        HCell cell = (inputPin,design).Cell();
        int index = (int)distance(cells->begin(), find(cells->begin(), cells->end(), cell));
        sum += vMu[::ToID(design.TimingPoints[inputPin])] * (regressionR->getValue(1 / vX[index]));
        //sum += 0; //u_inputPin * r_inputPin / x_inputPin;
      }
  }
  */
  return 0;
}

bool LRSizer::CheckStopConditionForLRS_Mu(std::vector<double> prevVX, std::vector<double> nextVX, 
                                          double accuracy)
{
  double max = 0;
  if (prevVX.size() != nextVX.size())
  {
    ALERT("delta size!!! = %d", nextVX.size() - prevVX.size());
    return false;		
  }
  for(unsigned int i = 0; i < prevVX.size(); i++)
  {
    if (fabs(prevVX[i] - nextVX[i]) > max) 
      max = fabs(prevVX[i] - nextVX[i]);
  }

  if (max < accuracy) return true;
  else return false;
}

void LRSizer::CheckKuhn_Tucker()
{
#ifdef DEBUG
  HTimingPointWrapper tp = design[design.TimingPoints.TopologicalOrderRoot()];
  for (; !::IsNull(tp); tp.GoNext())
  {
    if (!CheckKuhn_Tucker(tp)) ALERT("Check KuhnTucker=false on ::toID(tp)=%d", ::ToID(tp));
  }
#endif
}

void LRSizer::SolveLDP()
{
  int stepCounter = 1;
  //1. Set initial lambdas
  InitLambda(1);
  ProjectLambdaMatrix();
  CheckKuhn_Tucker();

  std::vector<double> vX;

  do{
    SolveLrsMu(vX);

    //ALERT("Criterion on %d-th iteration is %lf", stepCounter, CalcCriterion(vX));
    std::vector<double> arrivalTimes = timer->GetArrivalTimes(vX);
    AdjustLambda(stepCounter, arrivalTimes, vX);
    ProjectLambdaMatrix();
    stepCounter++;
  } while (!CheckStopConditionForLDP(vX, errorBoundForLDP));

  ALERT("Number of iterations in SOLVE LDP: %d", stepCounter - 1);
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
    for(HNet::SinksEnumeratorW sink = (design,net).GetSinksEnumeratorW(); sink.MoveNext(); )
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
    HCell cell = (design, inputPin).Cell();
    //шпаргалка
    // (design,cell) \
    // (cell,design) --> HCellWrapper
    // design[cell]  /
    // design.Cells.MakeWrapper(cell)

    for (HCell::PinsEnumeratorW outputPin = (design, cell).GetPinsEnumeratorW(); outputPin.MoveNext(); )
    {
      if (outputPin.Direction() != PinDirection_OUTPUT || ::IsNull(outputPin.Net())) 
        continue;
      int i=0;
      HTimingPoint timingPointOfOutputPin = design.TimingPoints[outputPin];
      for (HPinType::ArcsEnumeratorW arc = (design,outputPin.Type()).GetArcsEnumeratorW(); arc.MoveNext(); )
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

double LRSizer::CalcDelay(HTimingPointWrapper& tp, std::vector<double>& vX)
{
  /*TODO: Rewrite method according to the fact that R belongs to ARC  
  Maths::Regression::Linear* regressionR = GetRegressionR(tp);
  HPinWrapper pin = design[(design, tp).Pin()];
  HCell cell = pin.Cell();
  int index = (int)distance(cells->begin(), find(cells->begin(), cells->end(), cell));
  return (regressionR->getValue(1 / vX[index]) * GetObservedC(tp, vX));  
  */
  return 1;
}

void LRSizer::AdjustLambda( int step, std::vector<double>& arrivalTimes, std::vector<double>& vX)
{
  HTimingPointWrapper tp = design[design.TimingPoints.TopologicalOrderRoot()];
  for (tp.GoPrevious(); tp != design.TimingPoints.LastInternalPoint(); tp.GoPrevious())
  {
    HNet net = design.Get<HPin::Net, HNet>(design.Get<HTimingPoint::Pin, HPin>(tp));
    HPin source = design[net].Source();
    HTimingPointWrapper tpSrc = design[design.TimingPoints[source]];
    GetPointLambdas(tp)[0] = GetPointLambdas(tp)[0] + (1 / step) * 
      (arrivalTimes[::ToID(tpSrc)]/*-A_0*/);// ?? EndPoint always has the only input point ??
  }
  HTimingPointWrapper tmpTp = design[design.TimingPoints.FirstInternalPoint()].GoPrevious();
  for (tp.GoPrevious(); tp != tmpTp; tp.GoPrevious())
  {
    if (((design.Get<HTimingPoint::Pin, HPin>(tp)), design).Direction() == PinDirection_INPUT)
    {
      HNet net = design.Get<HPin::Net, HNet>(design.Get<HTimingPoint::Pin, HPin>(tp));
      HPin source = design[net].Source();
      HTimingPointWrapper tpSrc = design[design.TimingPoints[source]];
      GetPointLambdas(tp)[0] = GetPointLambdas(tp)[0] + (1 / step) * 
        (arrivalTimes[::ToID(tpSrc)] + CalcDelay(tp, vX) - arrivalTimes[::ToID(tp)]);
    }
    int i=0;
    if (((design.Get<HTimingPoint::Pin, HPin>(tp)), design).Direction() == PinDirection_OUTPUT)
    {
      HPin outputPin = design.Get<HTimingPoint::Pin, HPin>(tp);
      HCell cell = (design,outputPin).Cell();
      for (HCell::PinsEnumeratorW inputPin = (design, cell).GetPinsEnumeratorW(); inputPin.MoveNext(); )
      {
        HTimingPointWrapper tpInput = design[design.TimingPoints[inputPin]];
        if (inputPin.Direction() != PinDirection_INPUT || ::IsNull(inputPin.Net())) 
          continue;
        for (HPinType::ArcsEnumeratorW arc = (design,(design,outputPin).Type()).GetArcsEnumeratorW(); 
          arc.MoveNext();)
        {
          if (arc.TimingType() != TimingType_Combinational) 
            continue;
          if (arc.GetStartPin(inputPin) == outputPin)
          {
            GetPointLambdas(tp)[i] = GetPointLambdas(tp)[i] + (1 / step) * 
              (arrivalTimes[::ToID(tpInput)] + CalcDelay(tp, vX) - arrivalTimes[::ToID(tp)]);
            i++;
          }
        }
      }   
    }
  }
}

void LRSizer::ProjectLambdaMatrix()
{
  double MuIn, MuOut; 
  std::vector<double> percentage;
  HTimingPointWrapper tp = design[design.TimingPoints.TopologicalOrderRoot()];
  for (tp.GoPrevious(); !::IsNull(tp); tp.GoPrevious())
  {
    percentage.reserve(GetPointLambdas(tp).size());
    MuIn = FindInputLambdaSum(tp);
    MuOut = FindOutputLambdaSum(tp);
    for (std::vector<double>::iterator lambdaIter = GetPointLambdas(tp).begin();
      lambdaIter != GetPointLambdas(tp).end(); lambdaIter++)
      percentage.push_back(*lambdaIter / MuIn);
    for(unsigned int i = 0;i < GetPointLambdas(tp).size(); i++)
    {
      GetPointLambdas(tp)[i] = percentage[i] * MuOut;
    }
  }
}

double LRSizer::CalculateQ(unsigned int size)
{
  double SumInputLambdaForPoint; 
  double SumAllInputLambda = 0;
  double QFunction = 0;
  /*TODO: Rewrite method according to the fact that R belongs to ARC
  std::vector<double> XLower;
  XLower.reserve(size);
  InitVxByLowerBound(XLower);
  HTimingPointWrapper tp = design[design.TimingPoints.TopologicalOrderRoot()];
  for (; tp != design.TimingPoints.FirstInternalPoint(); tp.GoPrevious())
  {
    SumInputLambdaForPoint=FindInputLambdaSum(tp);
    HPinWrapper pin = design[(design, tp).Pin()];
    HCell cell = pin.Cell();
    if (::IsNull(cell)) continue;
    Maths::Regression::Linear* regressionR = GetRegressionR(tp);
    int index = (int)distance(cells->begin(), find(cells->begin(), cells->end(), cell));
    SumAllInputLambda += SumInputLambdaForPoint*(regressionR->getValue(1 / XLower[index]) * 
      GetObservedC(tp, XLower));
  }
  double SumMinX=0;
  for(unsigned int i = 0; i < XLower.size(); i++)
    SumMinX += XLower[i];
  QFunction = SumMinX + SumAllInputLambda;
  */
  return QFunction;
}

bool LRSizer::CheckStopConditionForLDP(std::vector<double>& vX, double errorBound)
{
  double Sum = 0;
  for(unsigned int i = 0; i < vX.size(); i++)
    Sum += vX[i];
  if ((Sum - CalculateQ((int)vX.size())) < errorBound) 
    return true;
  else return false;
} 

double LRSizer::CalcCriterion(std::vector<double>& vCurrentX)
{
  double criterion = 0, Sum = 0, SumInputLambdaForPoint = 0, SumAllInputLambda = 0;
  /*TODO: Rewrite method according to the fact that R belongs to ARC
  for(unsigned int i = 0; i < vCurrentX.size(); i++)
    Sum+=vCurrentX[i];
  HTimingPointWrapper tp = design[design.TimingPoints.TopologicalOrderRoot()];
  for (; tp != design.TimingPoints.FirstInternalPoint(); tp.GoPrevious())
  {
    SumInputLambdaForPoint = FindInputLambdaSum(tp);
    HPinWrapper pin = design[(design, tp).Pin()];
    HCell cell = pin.Cell();
    if (::IsNull(cell)) continue;
    Maths::Regression::Linear* regressionR = GetRegressionR(tp);
    int index = (int)distance(cells->begin(), find(cells->begin(), cells->end(), cell));
    SumAllInputLambda += SumInputLambdaForPoint * (regressionR->getValue(1 / vCurrentX[index]) *
      GetObservedC(tp, vCurrentX));
  }
  criterion = Sum + SumAllInputLambda;
  */
  return criterion;
}

HMacroType LRSizer::RoundCellToTypeFromLib(HCell cellFrom, double currentX)
{
  std::vector<HMacroType> macroTypesInFamily;
  timer->GetCellFamily(cellFrom, macroTypesInFamily);
  double smallestDeltaX = std::numeric_limits<double>::max();
  HMacroType bestMacroType;
  for(std::vector<HMacroType>::iterator macroType = macroTypesInFamily.begin(); 
    macroType != macroTypesInFamily.end(); macroType++)
  {
    double sizeOfMacroType = timer->GetMacroTypeSize(*macroType);
    double deltaX = fabs(currentX - sizeOfMacroType);
    if (deltaX < smallestDeltaX)
    {
      smallestDeltaX = deltaX;
      bestMacroType = *macroType;
    }
  }
  return bestMacroType;
}

void LRSizer::DoLRSizing()
{
#ifdef MODE_TEST   
  ALERT("MODE TEST defined");
  std::vector<double> vX;
  InitVxByLowerBound(vX);
  std::vector<double> arrivalTimes = timer->GetArrivalTimes(vX);
  return;
#endif

  ALERT("MODE TEST not defined");
  std::vector<double> newVX;
  SolveLDP();
  SolveLrsMu(newVX);
  ApplySizing(newVX);
}

std::vector<double>& LRSizer::GetPointLambdas(HTimingPoint pt)
{
  return LambdaIn[::ToID(pt)];
}
