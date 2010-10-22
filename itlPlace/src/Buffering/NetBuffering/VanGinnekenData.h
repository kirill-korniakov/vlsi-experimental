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

enum TypeNetListBuffering
{
  BUFFERING_ALL_CRITICAL_PATH = 0,
  PATH_BASED = 1
};

class VGAlgorithmData
{
public:
  bool isInitialize;

  bool printNetInfo;
  bool printVariantsList;
  bool printCriticalPathsInfo;
  bool printTimingAfterBufferingCriticalPaths;

  bool plotVGTree;
  bool plotNets;
  bool plotSteinerPoint;
  bool plotBuffer;
  bool plotBinGridValue;
  int plotterWaitTime;

  PartitionType typePartition;
  int maxIndex;
  VanGinnekenTree* vGTree;
  int totalTreeSize;
  int partitionPointCount;
  
  bool isInsertInSourceAndSink;
  int maxBufferCount;
  TypeBufferingAlgorithm typeBufferingAlgorithm;  
  TypeModificationVanGinnekenList typeModificationVanGinnekenList;
  TypeBufferAddition typeBufferAddition;
  string textIdentifierBufferedNet;
  string textIdentifierBuffer;
  

  bool* netVisit;
  string nameBufferingNet;
  int countPinInBufferingInterconnection;
  bool isExactPinCountRequired;
  bool isNetContainPrimaryPin;
  TypeNetListBuffering typeNetListBuffering;
  bool reRoutingSteinerTree;
  int maxCountRepeatNet;
  int limitationCountCriticalPath;
  int numberBufferedAtOnceCriticalPaths;


  double totalAllowableBuffersArea;
  double totalAreaCells;
  double totalAreaBuffer;
  double sizeBufferMultiplier;
  double sizeBuffer;

  HDesign& design;
  HWirePhysicalParams& WirePhisics;
  TemplateTypes<BufferInfo>::vector Buffers;
  NetBufferingAlgorithm* vGAlgorithm;

  VGAlgorithmData(HDesign& hd);
  VGAlgorithmData(HDesign& hd,  NetBufferingAlgorithm* vGA);
  ~VGAlgorithmData();
  void DefaultValue();
  void Initialize();

  void ReportParameters();
  void ReportBufferInfo(BufferInfo buf);

  void LoadBuffers();
  bool IsBuffering();
  bool IslimitationsCountRepeatNet();
  double PercentAreaComposeBuffers();

  void CalculationTotalAreaCells();
  void AddAreaBuffer(double bA);
};

#endif