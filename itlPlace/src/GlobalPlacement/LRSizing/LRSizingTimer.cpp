#include "LRSizingTimer.h"
#include <algorithm>
#include "TimingUtils.h"

std::vector<double> LRSizingTimer::GetArrivalTimes(std::vector<double>& vX)
{
  std::vector<double> arrivalTimes;
  arrivalTimes.resize(size, 0);
  Maths::Regression::Linear* regressionR = NULL;
  for (HTimingPointWrapper tp = design[design.TimingPoints.TopologicalOrderRoot()].GoNext(); !::IsNull(tp); 
    tp.GoNext())
  {
    if(::IsNull(tp)) continue; 
    HPinWrapper pin = design[(design, tp).Pin()];
    double aT;
    if (pin.IsPrimaryInput()) 
      aT = 0;  
    else
    {
      if (pin.Direction() == PinDirection_INPUT || pin.IsPrimaryOutput())
        aT=GetInputTimingPointAT(vX, tp, arrivalTimes);
      else
        if (pin.Direction() == PinDirection_OUTPUT)
          aT=GetOutputTimingPointAT(vX, tp, arrivalTimes);
    }
    arrivalTimes[::ToID(tp)] = aT;
    ALERT("id: %d, arrivalTime: %lf", ::ToID(tp), aT);
  }
  return arrivalTimes;
}

double LRSizingTimer::GetInputTimingPointAT(std::vector<double>& vX,HTimingPoint& tp,std::vector<double>& arrivalTimes)
{
  HPinWrapper src = design[(design[design.Get<HPin::Net, HNet>(design.Get<HTimingPoint::Pin, HPin>(tp))]).Source()];
  HSteinerPointWrapper point = design[design.SteinerPoints[design[design.Get<HTimingPoint::Pin, HPin>(tp)]]];
  HSteinerPointWrapper srcPoint = design[design.SteinerPoints[src]];
  HSteinerPointWrapper nextPoint = srcPoint;  
  HSteinerPointWrapper nextRoot = point;
  HSteinerPointWrapper previousRoot = point;
  HSteinerPointWrapper srcP = srcPoint;
  TemplateTypes<HSteinerPoint>::stack points;
  double obsWireC, srcWireR,sinkCap;
  double aT = 0;
  while (GetParentStPoint(nextRoot, srcPoint) != nextRoot)
  {
    obsWireC = 0;
    srcWireR = 0;
    sinkCap = 0;
    nextRoot = GetParentStPoint(nextRoot, srcPoint);
    if (nextRoot.Left() == previousRoot)
      nextPoint = nextRoot.Left();
    else
      nextPoint = nextRoot.Right();
    obsWireC += design.GetDouble<HSteinerPoint::ExtractedC>(nextPoint) / 2;
    HPin nextPointPin = design.Get<HSteinerPoint::Pin, HPin>(nextPoint);
    if (!::IsNull(nextPointPin) && design.Get<HPin::Direction, PinDirection>(nextPointPin) == PinDirection_INPUT &&
      !::IsNull(design[nextPointPin].Cell()))
    {
      Maths::Regression::Linear* regressionC = GetRegressionC((design.TimingPoints[nextPointPin],design));
      int index = (int)distance(cells->begin(), find(cells->begin(), cells->end(), design[nextPointPin].Cell()));
      obsWireC += (regressionC->getValue(vX[index]));
    }
    srcWireR = design.GetDouble<HSteinerPoint::ExtractedR>(nextPoint);
    points.push(nextPoint);
    srcP = nextRoot;
    previousRoot = nextRoot;

    while (!points.empty()) points.pop(); //Clear stack
    points.push(nextPoint);   

    while (!points.empty())
    {
      srcP = points.top();
      points.pop();

      if (srcP.HasLeft())
      {
        nextPoint = srcP.Left();
        obsWireC += design.GetDouble<HSteinerPoint::ExtractedC>(nextPoint);
        HPin leftPointPin = design.Get<HSteinerPoint::Pin, HPin>(nextPoint);

        if (!::IsNull(leftPointPin) && design.Get<HPin::Direction, PinDirection>(leftPointPin) == PinDirection_INPUT &&
          !::IsNull(design[leftPointPin].Cell()))
        {
          Maths::Regression::Linear* regressionC = GetRegressionC((design.TimingPoints[leftPointPin],design));
          int index = (int)distance(cells->begin(), find(cells->begin(), cells->end(), design[leftPointPin].Cell()));
          obsWireC += (regressionC->getValue(vX[index]));
        }
        points.push(nextPoint);

        if (srcP.HasRight())
        {
          nextPoint = srcP.Right();
          obsWireC += design.GetDouble<HSteinerPoint::ExtractedC>(nextPoint);
          HPin rightPointPin = design.Get<HSteinerPoint::Pin, HPin>(nextPoint);
          if (!::IsNull(rightPointPin) && design.Get<HPin::Direction, PinDirection>(rightPointPin) == PinDirection_INPUT &&
            !::IsNull(design[rightPointPin].Cell()))
          {
            Maths::Regression::Linear* regressionC = GetRegressionC((design.TimingPoints[rightPointPin],design));
            int index = (int)distance(cells->begin(), find(cells->begin(), cells->end(), design[rightPointPin].Cell()));
            obsWireC += (regressionC->getValue(vX[index]));
          }
          points.push(nextPoint);
        }
      }
    }
    aT += srcWireR * obsWireC;
  }
  aT += arrivalTimes[::ToID(design.TimingPoints[src])];
  return aT;
}

HSteinerPointWrapper LRSizingTimer::GetParentStPoint(HSteinerPointWrapper& point,HSteinerPointWrapper& srcPoint)
{
  TemplateTypes<HSteinerPoint>::stack points;
  HSteinerPointWrapper nextPoint = srcPoint;
  HSteinerPointWrapper parentPoint = srcPoint;
  points.push(srcPoint);
  while (!points.empty())
  {
    parentPoint = points.top();
    points.pop();
    if (parentPoint.HasLeft())
    {
      nextPoint = parentPoint.Left();
      if (nextPoint == point)
        return parentPoint;
      points.push(nextPoint);
      if (parentPoint.HasRight())
      {
        nextPoint = parentPoint.Right();
        if (nextPoint == point)
          return parentPoint;
        points.push(nextPoint);
      }
    }
  }
  return point;
}

Maths::Regression::Linear* LRSizingTimer::GetRegressionC(HTimingPointWrapper tp)
{
  HPin pin = tp.Pin();
  std::vector<double> C, X;
  GetPinFamilyC(pin, C, X);
  int numOfAlternatives = (int)C.size();
  double* arrC = new double[numOfAlternatives];
  copy(C.begin(), C.end(), arrC);
  double* arrX = new double[numOfAlternatives];
  copy(X.begin(), X.end(), arrX);
  ASSERT(C.size() > 0);
  return new Maths::Regression::Linear((int)C.size(), arrX, arrC);
}

Maths::Regression::Linear* LRSizingTimer::GetRegressionR(HPin inputPin, HPin outputPin)
{
  std::vector<double> R, invX;
  GetArcFamilyR(inputPin, outputPin, R, invX);
  int numOfAlternatives = (int)R.size();
  double* arrR = new double[numOfAlternatives];
  copy(R.begin(), R.end(), arrR);
  double* arrInvX = new double[numOfAlternatives];
  copy(invX.begin(), invX.end(), arrInvX);
  ASSERT(R.size() > 0);
  return new Maths::Regression::Linear((int)R.size(), arrInvX, arrR);
}

void LRSizingTimer::GetPinFamilyC(HPin pin, std::vector<double>& C, std::vector<double>& X)
{
  string cellFamily = GetMacroTypeFamilyName(design[design[pin].Cell()].Type());
  string pinName = design[pin].Name();
  std::vector<double> cellSizes;
  std::vector<HMacroType> macroTypesInFamily;
  GetCellFamily(design[pin].Cell(), macroTypesInFamily);
  for (std::vector<HMacroType>::iterator macroType = macroTypesInFamily.begin(); 
    macroType != macroTypesInFamily.end(); macroType++)
  {
    double size = GetMacroTypeSize(*macroType);
    if (std::find(cellSizes.begin(), cellSizes.end(), size) == cellSizes.end())
    {
      cellSizes.push_back(size);
      for (HMacroType::PinsEnumeratorW pinE = design[*macroType].GetEnumeratorW(); pinE.MoveNext(); )
      {
        if ((pinE.Direction() == PinDirection_INPUT) || (pinE.Direction() == PinDirection_OUTPUT))
        {
          if(pinE == design[pin].Type())
          {
            C.push_back(Utils::GetSinkCapacitance(design, pinE, SignalDirection_None));
            X.push_back(GetMacroTypeSize(*macroType)); 
            break;
          }
        }
      }
    }
  }
  ASSERT(C.size() == X.size());
}

std::string LRSizingTimer::GetMacroTypeFamilyName(HMacroType macroType)
{
  std::string macroTypeName = design.MacroTypes.GetString<HMacroType::Name>(macroType);
  return macroTypeName.substr(0, macroTypeName.length()-2);
}

double LRSizingTimer::GetMacroTypeSize(HMacroType macroType)
{
  std::string macroTypeName = design[macroType].Name();
  int i = (int)macroTypeName.length() - 1;
  while(i >= 0 && macroTypeName[i] != 'x' && macroTypeName[i] != 'X') i--;
  ASSERT(i > 0);
  std::string macroTypeSizeString = macroTypeName.substr(i + 1);
  return atof(macroTypeSizeString.c_str());
}

void LRSizingTimer::GetCellFamily(HCell cell, std::vector<HMacroType>& macroTypesInFamily)
{
  string cellFamily = GetMacroTypeFamilyName((cell,design).Type());
  for (HMacroTypes::EnumeratorW macroTypeE = design.MacroTypes.GetEnumeratorW(); macroTypeE.MoveNext(); )
  {
    string macroTypeFamily = GetMacroTypeFamilyName(macroTypeE);
    if (macroTypeFamily == cellFamily)
      macroTypesInFamily.push_back(macroTypeE);
  }
  std::sort(macroTypesInFamily.begin(), macroTypesInFamily.end(), MacroTypeSizesComparator(design));
}

void LRSizingTimer::GetCellFamily(HCell cell, std::vector<double>& cellSizes)
{
  cellSizes.clear();
  std::vector<HMacroType> macroTypesInFamily;
  GetCellFamily(cell, macroTypesInFamily);

  for(std::vector<HMacroType>::iterator macroType = macroTypesInFamily.begin(); 
    macroType != macroTypesInFamily.end(); macroType++)
  {
    double size = GetMacroTypeSize(*macroType);
    if (std::find(cellSizes.begin(), cellSizes.end(), size) == cellSizes.end())
      cellSizes.push_back(size);
  }
  std::sort(cellSizes.begin(), cellSizes.end());
}

double LRSizingTimer::GetOutputTimingPointAT(std::vector<double>& vX, HTimingPoint& tp, 
                                       std::vector<double>& arrivalTimes)
{
  Maths::Regression::Linear* regressionR;
  HPin pin = design.Get<HTimingPoint::Pin, HPin>(tp);
  HCell cell = design[(design, tp).Pin()].Cell();
  int index = (int)distance(cells->begin(), find(cells->begin(), cells->end(), cell));
  double x = vX[index];
  double R = 0;
  double T = 0, aT = 0, maxDelay = 0;
  HPin startPin;

  HPinType sourceType = design.Get<HPin::Type, HPinType>(pin);

  for (HPinType::ArcsEnumeratorW arc = design[sourceType].GetArcsEnumeratorW(); arc.MoveNext(); )
  {
    if (arc.TimingType() == TimingType_Combinational)
    {
      startPin = arc.GetStartPin(pin);
      aT = arrivalTimes[::ToID(design.TimingPoints[startPin])];
      T = arc.TIntrinsicRise();
      regressionR = GetRegressionR(startPin, pin);
      R = regressionR->getValue(1 / x);
      if (maxDelay < (T + aT + R * GetObservedC(tp, vX))) 
        maxDelay = T + aT + R * GetObservedC(tp, vX);
    }
  }
  return (maxDelay);
}

double LRSizingTimer::GetObservedC(HTimingPoint tp,std::vector<double>& vX)
{
  HPinWrapper pin = design[(design,tp).Pin()];
  Maths::Regression::Linear* regressionC;
  if (pin.Direction() == PinDirection_INPUT)
  { 
    regressionC = GetRegressionC((tp, design));
    HCell cell = pin.Cell();
    int index = (int)distance(cells->begin(), find(cells->begin(), cells->end(), cell));
    return (regressionC->getValue(vX[index]));
  }
  HNet net = pin.Net();
  if (pin.Direction() == PinDirection_OUTPUT && !::IsNull(net))
  {
    double sum = (design.SteinerPoints[pin],design).ObservedC();
    for(HNet::SinksEnumeratorW sink = (design,net).GetSinksEnumeratorW(); sink.MoveNext(); )
    {
      //sink can be PO!! ERROR TODO
      if(sink.IsPrimaryOutput()) continue;//This is a DIRTY patch!
      regressionC = GetRegressionC((design.TimingPoints[sink], design));
      HCell cell = sink.Cell();
      int index = (int)distance(cells->begin(), find(cells->begin(), cells->end(), cell));
      sum += (regressionC->getValue(vX[index])) - (sink.Type(), design).Capacitance();
    }
    return sum;
  }
  return 0;
}

void LRSizingTimer::GetArcFamilyR(HPin startPin, HPin endPin, std::vector<double>& R, std::vector<double>& invX)
{
  ASSERT((startPin, design).Direction() == PinDirection_INPUT);
  ASSERT((endPin, design).Direction() == PinDirection_OUTPUT);
  ASSERT((endPin, design).Cell() == (startPin, design).Cell());//Pins are on the same cell
  std::vector<HMacroType> macroTypesInFamily;
  GetCellFamily(design[endPin].Cell(), macroTypesInFamily);
  for (std::vector<HMacroType>::iterator macroType = macroTypesInFamily.begin(); 
    macroType != macroTypesInFamily.end(); macroType++)
  {
    for(HMacroType::PinsEnumeratorW pinE = design[*macroType].GetEnumeratorW(); pinE.MoveNext();)
    {
      if(pinE.Direction() == PinDirection_OUTPUT && pinE == design[endPin].Type())
      {
        for(HPinType::ArcsEnumeratorW arcE = pinE.GetArcsEnumeratorW(); arcE.MoveNext();)
        {
          HPinType possibleStartPinConnectedToPinE = arcE.GetStartPinType(pinE);
          if (possibleStartPinConnectedToPinE == (startPin, design).Type())
          {
            R.push_back(arcE.ResistanceRise());
            invX.push_back(1.0 / GetMacroTypeSize(*macroType));
          }
        }
      }
    }
  }
  ASSERT(R.size() == invX.size());
}

void LRSizingTimer::PrintMacroTypeCharacteristics()
{
  FILE* macroCharacteristics = fopen("./log/macroCharacteristics.csv", "w");
  fprintf (macroCharacteristics, "cellName;cellSize;pinName;pinDirection;C;signalDirection;R;T;\n");
  for (HMacroTypes::EnumeratorW macroTypeE = design.MacroTypes.GetEnumeratorW(); macroTypeE.MoveNext(); )
  {
    double cellSize = GetMacroTypeSize(macroTypeE);
    std::string cellName = macroTypeE.Name();
    for(HMacroType::PinsEnumeratorW pinE = macroTypeE.GetEnumeratorW(); pinE.MoveNext();)
    {
      std::string pinName = pinE.Name();
      if (pinE.Direction() == PinDirection_INPUT)
      {
        Utils::DriverPhisics driverPhisics = Utils::GetDriverWorstPhisics(design, pinE, SignalDirection_None);
        fprintf(macroCharacteristics, "%s;%lf;%s;Input;%lf;-; ; \n", 
          cellName.data(), cellSize, pinName.data(), driverPhisics.C);
      }
      else if(pinE.Direction() == PinDirection_OUTPUT)
      {
        for(HPinType::ArcsEnumeratorW arcE = pinE.GetArcsEnumeratorW(); arcE.MoveNext();)
        {
          HPinType pinWhichConnectsToPinE = arcE.GetStartPinType(pinE);
          std::string signalDirection = design[pinWhichConnectsToPinE].Name() + "->" + pinName;
          fprintf(macroCharacteristics, "%s;%lf;%s;Output; ;%s;%lf;%lf\n", 
            cellName.data(), cellSize, pinName.data(), signalDirection.data(), arcE.ResistanceRise(), 
            arcE.TIntrinsicRise());
        }
      }
    }
  }
  fclose(macroCharacteristics);
}

void LRSizingTimer::PrintPinsCharacteristics()
{
  FILE* pinsCharacteristics = fopen("./log/pinsCharacteristics.csv","w");
  fprintf (pinsCharacteristics, "cellName;pinName;id;pinDirection;ArrivalTime;C;R;T\n");
  HPin pin;
  string pinName, cellName, direction="null";
  HCell cell;
  Utils::DriverPhisics DPhisics;
  for (HTimingPointWrapper tp = design[design.TimingPoints.TopologicalOrderRoot()].GoNext(); !::IsNull(tp);
    tp.GoNext())
  {
    if(!::IsNull(tp))
    {
      pin = tp.Pin(); 
      if (design[(design,tp).Pin()].IsPrimaryInput())
      {

        fprintf (pinsCharacteristics, "%s;;;;%lf\n", "PrimaryInput", tp.ArrivalTime());
        continue;
      }
      if (design[(design,tp).Pin()].IsPrimaryOutput())
      {
        fprintf (pinsCharacteristics, "%s;;;;%lf\n", "PrimaryOutput", tp.ArrivalTime());
        continue;
      }
      DPhisics = Utils::GetDriverWorstPhisics(design, pin, SignalDirection_None);
      pinName = (pin,design).Name();
      cell = (design,pin).Cell();
      cellName = design.MacroTypes.GetString<HMacroType::Name>((cell,design).Type());
      if ((pin,design).Direction() == PinDirection_INPUT) 
        direction="INPUT";
      else 
      {
        if ((pin,design).Direction() == PinDirection_OUTPUT) direction = "OUTPUT";
      }
      fprintf (pinsCharacteristics, "%s;%s;%d;%s;%lf;%lf;%lf;%lf\n", cellName.data(), pinName.data(), 
        ::ToID(tp),direction.data(), tp.ArrivalTime(), DPhisics.C, DPhisics.R, DPhisics.T);
    }
  }
  fclose(pinsCharacteristics);
}