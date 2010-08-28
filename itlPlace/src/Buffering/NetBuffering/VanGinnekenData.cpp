#include "VanGinnekenData.h"

VGAlgorithmData::VGAlgorithmData(HDesign& hd, NetBufferingAlgorithm* vGA): design(hd), WirePhisics(hd.RoutingLayers.Physics)
{
  vGAlgorithm = vGA;
  partitionCount = 0;
  plotVGTree = false;
  plotNets = false;
  printNetInfo = false;
  plotSteinerPoint = false;
  printVariantsList = false;
  plotterWaitTime = 1;
  isInitialize = false;
  isInsertInSourceAndSink = false;
  typeBufferingAlgorithm = 0;
  sizeBuffer = 0; 
  maxIndex = 0;
  vGTree = NULL;
  netVisit = NULL;
  maxBufferCount = 0;
  typeModificationVanGinnekenList = 0;
  countPinInBufferingNet = 0;
  isTypeLimitationOnCountPinInBufferingNetEquality = false;
  nameBufferingNet = "";
  typeBuferAddition = 0;
  sizeBufferMultiplier = 0;
  isBufferingNetContainPrimaryPin = false;
  totalAreaOfBuffersInRelationToAllCells = 0;
  totalAreaCells = 0;
  totalAreaBuffer = 0;
  plotBuffer = false;
  plotBinGridValue = false;
}

VGAlgorithmData::~VGAlgorithmData()
{
  if (netVisit != NULL)
    delete [] netVisit;
}

void VGAlgorithmData::ReportParameters() 
{
  ALERT("Partition type: %d", typePartition);
  if (typePartition == 0)
    ALERT("Partition count: %d", partitionCount);

  ALERT("Buffering algorithm type type: %d", typeBufferingAlgorithm);
  ALERT("Type modification van Ginneken list: %d", typeModificationVanGinnekenList);
  ALERT("Maximum insert buffer in net: %d", maxBufferCount);
  ALERT("Type bufer addition: %d", typeBuferAddition);
  ALERT("Size buffer multiplier: %f", sizeBufferMultiplier);
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

  partitionCount = design.cfg.ValueOf("Interval", 1);  
  isInsertInSourceAndSink = design.cfg.ValueOf("IsInsertInSourceAndSink", true);
  typeBufferingAlgorithm = design.cfg.ValueOf("TypeBufferingAlgorithm", 0);
  typePartition = design.cfg.ValueOf("TypePartition", 0);

  maxBufferCount = design.cfg.ValueOf("MaxBufferCount", 0);
  typeModificationVanGinnekenList = design.cfg.ValueOf("TypeModificationVanGinnekenList", 0);

  countPinInBufferingNet = design.cfg.ValueOf("CountPinInBufferingNet", 0);
  isTypeLimitationOnCountPinInBufferingNetEquality = design.cfg.ValueOf("IsTypeLimitationOnCountPinInBufferingNetEquality", false);
  nameBufferingNet = design.cfg.ValueOf("NameBufferingNet", "");
  typeBuferAddition = design.cfg.ValueOf("TypeBuferAddition", 0);
  sizeBufferMultiplier = design.cfg.ValueOf("SizeBufferMultiplier", 1.0);
  isBufferingNetContainPrimaryPin = design.cfg.ValueOf("IsBufferingNetContainPrimaryPin", false);
  totalAreaOfBuffersInRelationToAllCells = design.cfg.ValueOf("TotalAreaOfBuffersInRelationToAllCells", 0.0);

  totalAreaCells = 0;
  totalAreaBuffer = 0;
  for (HCells::CellsEnumeratorW cw = design.Cells.GetEnumeratorW(); cw.MoveNext();)
    totalAreaCells += cw.Width() * cw.Height();

  netVisit = new bool [design.Nets.Count() * 2];
  for (int i = 0; i < design.Nets.Count() * 2; i++)
  {
    netVisit[i] = false;
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
  if (totalAreaOfBuffersInRelationToAllCells != 0)
    if (totalAreaBuffer > totalAreaOfBuffersInRelationToAllCells * totalAreaCells)
      return false;  
  return true;
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
