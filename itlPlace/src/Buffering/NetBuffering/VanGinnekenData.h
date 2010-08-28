#ifndef __VanGinnekenData_H__
#define __VanGinnekenData_H__

#include "BufferInfo.h"
#include "Utils.h"

class VanGinnekenTree;
class HWirePhysicalParams;
class NetBufferingAlgorithm;
using namespace Utils;

class VGAlgorithmData
{
public:
  bool printNetInfo;
  bool printVariantsList;

  bool plotVGTree;
  bool plotNets;
  bool plotSteinerPoint;
  int plotterWaitTime;
  bool plotBuffer;
  bool plotBinGridValue;

  int partitionCount;
  bool isInitialize;
  bool isInsertInSourceAndSink;
  int typeBufferingAlgorithm;
  int typePartition;
  double sizeBuffer;
  int maxIndex;
  bool* netVisit;
  int maxBufferCount;
  int typeModificationVanGinnekenList;
  int countPinInBufferingNet;
  bool isTypeLimitationOnCountPinInBufferingNetEquality;
  string nameBufferingNet;
  int typeBuferAddition;
  double sizeBufferMultiplier;
  bool isBufferingNetContainPrimaryPin;
  double totalAreaOfBuffersInRelationToAllCells;
  double totalAreaCells;
  double totalAreaBuffer;

  HDesign& design;
  HWirePhysicalParams& WirePhisics;
  TemplateTypes<BufferInfo>::vector Buffers;
  NetBufferingAlgorithm* vGAlgorithm;

  VanGinnekenTree* vGTree;

  VGAlgorithmData(HDesign& hd,  NetBufferingAlgorithm* vGA);
  ~VGAlgorithmData();
  void Initialize();

  void ReportParameters();
  void ReportBufferInfo(BufferInfo buf);

  void LoadBuffers();
  bool IsBuffering();
  double PercentAreaComposeBuffers();

  void CalculationTotalAreaCells();
  void AddAreaBuffer(double bA);
};

#endif