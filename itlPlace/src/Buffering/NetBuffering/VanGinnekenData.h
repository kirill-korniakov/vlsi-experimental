#ifndef __VanGinnekenData_H__
#define __VanGinnekenData_H__

#include "BufferInfo.h"
#include "Utils.h"
#include "VanGinnekenTree.h"

class VanGinnekenTree;
class HWirePhysicalParams;
class NetBufferingAlgorithm;
using namespace Utils;

enum TypeBufferingAlgorithm
{
  CLASSIC_CREATE_VG_LIST = 0,
  LINE_BYPASS_CREATE_VG_LIST = 1,
  ADAPTIVE_BYPASS_CREATE_VG_LIST = 2
};

enum TypeModificationVanGinnekenList
{
 STANDART_MODIFICATION_VG_LIST = 0,
 ACCOUNTING_BORDER_MODIFICATION_VG_LIST = 1,
 EXHAUSTIVE_SEARCH = 2
};

enum TypeBufferAddition
{
 STANDART_ADDITION = 0,
 LEGAL_ADDITION = 1
};

class VGAlgorithmData
{
public:
  bool printNetInfo;
  bool printVariantsList;

  bool plotVGTree;
  bool plotNets;
  bool plotSteinerPoint;
  bool plotBuffer;
  bool plotBinGridValue;
  int plotterWaitTime;

  bool isInitialize;

  int partitionCount;
  VanGinnekenTree::PartitionType typePartition;
  int maxIndex;
  VanGinnekenTree* vGTree;
  
  bool isInsertInSourceAndSink;
  int maxBufferCount;
  TypeBufferingAlgorithm typeBufferingAlgorithm;  
  TypeModificationVanGinnekenList typeModificationVanGinnekenList;
  TypeBufferAddition typeBufferAddition;

  bool* netVisit;
  string nameBufferingNet;
  int countPinInBufferingNet;
  bool isExactPinCountRequired;
  bool isNetContainPrimaryPin;

  double totalAllowableBuffersArea;
  double totalAreaCells;
  double totalAreaBuffer;
  double sizeBufferMultiplier;
  double sizeBuffer;

  HDesign& design;
  HWirePhysicalParams& WirePhisics;
  TemplateTypes<BufferInfo>::vector Buffers;
  NetBufferingAlgorithm* vGAlgorithm;

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