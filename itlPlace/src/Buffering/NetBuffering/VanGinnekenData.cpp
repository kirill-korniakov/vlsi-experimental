#include "VanGinnekenData.h"

VGAlgorithmData::VGAlgorithmData(HDesign& hd): design(hd), WirePhisics(hd.RoutingLayers.Physics)
{
  DefaultValue();
}

VGAlgorithmData::VGAlgorithmData(HDesign& hd, NetBufferingAlgorithm* vGA): design(hd), WirePhisics(hd.RoutingLayers.Physics)
{
  vGAlgorithm = vGA;
  DefaultValue();
}

void VGAlgorithmData::DefaultValue()
{
  partitionPointCount = 0;
  plotVGTree = false;
  plotNets = false;
  printNetInfo = false;
  plotSteinerPoint = false;
  printVariantsList = false;
  plotterWaitTime = 1;
  isInitialize = false;
  isInsertInSourceAndSink = false;
  typeBufferingAlgorithm = CLASSIC_CREATE_VG_LIST;
  sizeBuffer = 0; 
  maxIndex = 0;
  vGTree = NULL;
  netVisit = NULL;
  maxBufferCount = 0;
  typeModificationVanGinnekenList = STANDART_MODIFICATION_VG_LIST;
  countPinInBufferingInterconnection = 0;
  isExactPinCountRequired = false;
  nameBufferingNet = "";
  typeBufferAddition = STANDART_ADDITION;
  sizeBufferMultiplier = 0;
  isNetContainPrimaryPin = false;
  totalAllowableBuffersArea = 0;
  totalAreaCells = 0;
  totalAreaBuffer = 0;
  plotBuffer = false;
  plotBinGridValue = false;
  totalTreeSize = 0;
  typeNetListBuffering = BUFFERING_ALL_CRITICAL_PATH;
  printCriticalPathsInfo = false;
  printTimingAfterBufferingCriticalPaths = false;
  textIdentifierBufferedNet = "__BufferedPart_";
  textIdentifierBuffer = "buf_";
  reRoutingSteinerTree = false;
  maxCountRepeatNet = 0;
  limitationCountCriticalPath = 0;
  numberBufferedAtOnceCriticalPaths = 0;
}


VGAlgorithmData::~VGAlgorithmData()
{
  if (netVisit != NULL)
    delete [] netVisit;
}

void VGAlgorithmData::ReportParameters() 
{
  ALERT("Partition type: %d", typePartition);
  if (typePartition == LINEAR)
    ALERT("Partition count: %d", partitionPointCount);

  ALERT("Buffering algorithm type type: %d", int (typeBufferingAlgorithm));
  ALERT("Type modification van Ginneken list: %d", int (typeModificationVanGinnekenList));
  ALERT("Maximum insert buffer in net: %d", maxBufferCount);
  ALERT("Type bufer addition: %d", int (typeBufferAddition));
  ALERT("Size buffer multiplier: %f", sizeBufferMultiplier);
  ALERT("Type net-list buffering: %d", int(typeNetListBuffering));
}

void VGAlgorithmData::Initialize()
{
  isInitialize = true;
  LoadBuffers();

  plotSteinerPoint = design.cfg.ValueOf("Plotting.PlotSteinerPoint", false);
  plotVGTree = design.cfg.ValueOf("Plotting.PlotVGTree", false);
  plotNets = design.cfg.ValueOf("Plotting.PlotNets", false);
  plotterWaitTime = design.cfg.ValueOf("Plotting.PlotterWaitTime", 1);
  plotBuffer = design.cfg.ValueOf("Plotting.PlotBuffer", false);
  plotBinGridValue = design.cfg.ValueOf("Plotting.PlotBinGridValue", false);

  printNetInfo = design.cfg.ValueOf("Reporting.PrintNetInfo", false);
  printVariantsList = design.cfg.ValueOf("Reporting.PrintVGVariantsList", false);

  partitionPointCount = design.cfg.ValueOf("Interval", 1);  
  isInsertInSourceAndSink = design.cfg.ValueOf("IsInsertInSourceAndSink", true);
  typeBufferingAlgorithm = TypeBufferingAlgorithm(design.cfg.ValueOf("TypeBufferingAlgorithm", 0));
  typePartition = PartitionType(design.cfg.ValueOf("TypePartition", 0));

  maxBufferCount = design.cfg.ValueOf("MaxBufferCount", 0);
  typeModificationVanGinnekenList = TypeModificationVanGinnekenList (design.cfg.ValueOf("TypeModificationVanGinnekenList", 0));

  countPinInBufferingInterconnection = design.cfg.ValueOf("CountPinInBufferingInterconnection", 0);
  isExactPinCountRequired = design.cfg.ValueOf("IsNotExactPinCountRequired", false);
  nameBufferingNet = design.cfg.ValueOf("NameBufferingNet", "");
  typeBufferAddition = TypeBufferAddition (design.cfg.ValueOf("TypeBufferAddition", 0));
  sizeBufferMultiplier = design.cfg.ValueOf("SizeBufferMultiplier", 1.0);
  isNetContainPrimaryPin = design.cfg.ValueOf("IsNetContainPrimaryPin", false);
  totalAllowableBuffersArea = design.cfg.ValueOf("TotalAllowableBuffersArea", 0.0);
  typeNetListBuffering = TypeNetListBuffering(design.cfg.ValueOf("TypeNetListBuffering", 0));
  printCriticalPathsInfo = design.cfg.ValueOf("Reporting.PrintCriticalPathsInfo", false);
  printTimingAfterBufferingCriticalPaths = design.cfg.ValueOf("Reporting.PrintTimingAfterBufferingCriticalPaths", false);
  reRoutingSteinerTree = design.cfg.ValueOf("ReRoutingSteinerTree", false);
  maxCountRepeatNet = design.cfg.ValueOf("MaxCountRepeatNet", 0);
  limitationCountCriticalPath = design.cfg.ValueOf("LimitationCountCriticalPath", 0);
  numberBufferedAtOnceCriticalPaths = design.cfg.ValueOf("NumberBufferedAtOnceCriticalPaths", 0);

  totalAreaCells = 0;
  totalAreaBuffer = 0;
  for (HCells::CellsEnumeratorW cw = design.Cells.GetEnumeratorW(); cw.MoveNext();)
    totalAreaCells += cw.Width() * cw.Height();

  if (typeNetListBuffering == BUFFERING_ALL_CRITICAL_PATH)
  {
    netVisit = new bool [design.Nets.Count() * 2];
    for (int i = 0; i < design.Nets.Count() * 2; i++)
    {
      netVisit[i] = false;
    }
  }
  if (typeNetListBuffering == PATH_BASED)
  {
    netVisit = new bool [design.CriticalPaths.Count() * 2];

    for (int i = 0; i < design.CriticalPaths.Count() * 2; i++)
    {
      netVisit[i] = false;
    }
  }

  if (design.cfg.ValueOf("Reporting.reportParameters", false))
    ReportParameters();
}

void VGAlgorithmData::ReportBufferInfo(BufferInfo buf) 
{
  if (design.cfg.ValueOf("Reporting.reportBufferInfo", false) == false)
    return;

  ALERT("Buffer type: %s\t input pin: %s\t output pin: %s", (design, buf.Type()).Name().c_str(),
    (design, buf.InPin()).Name().c_str(), (design, buf.OutPin()).Name().c_str());
  sizeBuffer = design[buf.Type()].SizeX() * design[buf.Type()].SizeY();
}

void VGAlgorithmData::LoadBuffers()
{
  if (design.cfg.ValueOf("UseOnlyDefaultBuffer", false))
  {
    BufferInfo buf = BufferInfo::Create(design);    
    Buffers.push_back(buf);
    sizeBuffer = design[buf.Type()].SizeX() * design[buf.Type()].SizeY();
    ReportBufferInfo(buf);
  }
  else
  {
    string sBufferList = design.cfg.ValueOf("BufferList", "");
    unsigned int n = design.cfg.ValueOf("BufferListLength", 0);
    string* bufferList = NULL;
    if (n > 0)
    {
      bufferList = new string [n];     
      for (unsigned int i = 0, j = 0, t = 0; (i < sBufferList.length()) && (j < n); i++, t++)
      {
        if(sBufferList[i] != ',')
          bufferList[j].push_back(sBufferList[i]);
        else
        {
          t = -1;
          j++;
        }
      }
    }

    Buffers = BufferInfo::CreateVector(design, bufferList);
    for (unsigned int i = 0; i < Buffers.size(); i++)
    {
      if ((design[Buffers[i].Type()].SizeX() * design[Buffers[i].Type()].SizeY()) > sizeBuffer)
        sizeBuffer = design[Buffers[i].Type()].SizeX() * design[Buffers[i].Type()].SizeY();
      ReportBufferInfo(Buffers[i]);
    }
  }
}

bool VGAlgorithmData::IsBuffering()
{
  if (totalAllowableBuffersArea != 0)
    if (totalAreaBuffer > totalAllowableBuffersArea * totalAreaCells)
      return false;  
  return true;
}

bool VGAlgorithmData::IslimitationsCountRepeatNet()
{
  if (maxCountRepeatNet != 0)
    return true;
  return false;
}

double VGAlgorithmData::PercentAreaComposeBuffers()
{
  return totalAreaBuffer / totalAreaCells * 100.0;
}

void VGAlgorithmData::CalculationTotalAreaCells()
{
  totalAreaCells = 0;
  for (HCells::CellsEnumeratorW cw = design.Cells.GetEnumeratorW(); cw.MoveNext();)
    totalAreaCells += cw.Width() * cw.Height();
}

void VGAlgorithmData::AddAreaBuffer(double bA)
{
  totalAreaBuffer += bA;
}
