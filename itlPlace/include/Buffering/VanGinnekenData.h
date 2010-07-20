#ifndef __VanGinnekenData_H__
#define __VanGinnekenData_H__

#include "BufferInfo.h"
#include "HSteinerPoint.h"
#include "PlacementGridUtils.h"

class HDPGrid;
class VanGinnekenTree;
class HWirePhysicalParams;

using namespace Utils;

class VanGinnekenTreeNode
{
protected:
  double x,y; //координаты узла дерев
  int type; // тип звена: 0 - source; 1 - sink; 2 - точка ветвлени€; 3 - кандидат на вставку буфера; 
            //4 - кандидат на вставку буфера с координатоми совподающими с source или sink;
            //5 - кандидат на вставку буфера с координатой совподающей с внутреними точками дерева штейнера
 // (TO DO: ѕ–»ƒ”ћј“№ Ќќ–ћјЋ№Ќ”ё ‘ќ–ћ”Ћ»–ќ¬ ”)

  HSteinerPoint sPoint;
  VanGinnekenTreeNode* left;
  VanGinnekenTreeNode* right;
  int index;
  VanGinnekenTree* tree;

public:
  VanGinnekenTreeNode();
  bool isSource();
  bool isSink();
  bool isBranchPoint();
  bool isCandidate();
  bool isCandidateAndRealPoint();
  bool isInternal();
  bool HasLeft();
  bool HasRight();
  HSteinerPoint GetSteinerPoint();  
  double GetX();
  double GetY();
  VanGinnekenTreeNode* GetLeft();
  VanGinnekenTreeNode* GetRight();
  int GetIndex();
  VanGinnekenTree* GetTree();

  double GetRAT();//RAT in sinc
  double GetC();//capacity in sinc
  double GetR();//resistance in source

  void SetSteinerPoint(HSteinerPoint sp);  
  void SetX(double value);
  void SetY(double value);
  void SetLeft(VanGinnekenTreeNode* node);
  void SetRight(VanGinnekenTreeNode* node);
  void SetType(int t);
  void SetIndex(int i);
  void SetTree(VanGinnekenTree* t);

  ~VanGinnekenTreeNode();

};

class VanGinnekenTree
{
protected:
  int partitionPointCount;
  int totalTreeSize;  
  int treeSize;
  int TypePartition;

  virtual void CreateTree() = 0;
  virtual VanGinnekenTreeNode* CreateNode(HSteinerPoint node, int type, int& index, int rootIndex, bool isRight = false, VanGinnekenTree* tree = NULL) = 0;


public:
  HDesign& design;
  HDPGrid* DPGrid;
  VanGinnekenTreeNode* vGTree;

  VanGinnekenTree(HDesign& hd);
  VanGinnekenTree(HDesign& hd, int partitionCount);
  VanGinnekenTree(HDesign& hd, int partitionCount, HSteinerPoint& source);

  virtual void UpdateTree(HSteinerPoint& source);

  virtual void SetEdgePartitionCount(int partitionCount);

  virtual VanGinnekenTreeNode GetSource();
  virtual double GetR();

  virtual void ClearTree();

  virtual int GetTreeSize();


};

class VGTreeUniformDistribution: public VanGinnekenTree
{
protected:
  virtual void CreateTree();
  virtual VanGinnekenTreeNode* CreateNode(HSteinerPoint node, int type, int& index, int rootIndex, bool isRight = false, VanGinnekenTree* tree = NULL);

public:
  VGTreeUniformDistribution(HDesign& hd);
  VGTreeUniformDistribution(HDesign& hd, int partitionCount);
  VGTreeUniformDistribution(HDesign& hd, int partitionCount, HSteinerPoint& source);

  virtual ~VGTreeUniformDistribution();
};

class VGTreeDynamicDistribution: public VanGinnekenTree
{
protected:
  virtual void CreateTree();
  virtual VanGinnekenTreeNode* CreateNode(HSteinerPoint node, int type, int& index, int rootIndex, bool isRight = false, VanGinnekenTree* tree = NULL);

public:
  VGTreeDynamicDistribution(HDesign& hd);
  VGTreeDynamicDistribution(HDesign& hd, int partitionCount);
  VGTreeDynamicDistribution(HDesign& hd, int partitionCount, HSteinerPoint& source);

  virtual ~VGTreeDynamicDistribution();
};

class VGTreeLegalDynamicDistribution: public VanGinnekenTree
{
protected:
  HPlacementGrid pGrid;

  virtual void CreateTree();
  virtual VanGinnekenTreeNode* CreateNode(HSteinerPoint node, int type, int& index, int rootIndex, bool isRight = false, VanGinnekenTree* tree = NULL);

public:
  VGTreeLegalDynamicDistribution(HDesign& hd);
  VGTreeLegalDynamicDistribution(HDesign& hd, int partitionCount);
  VGTreeLegalDynamicDistribution(HDesign& hd, int partitionCount, double sizeBuffer);
  VGTreeLegalDynamicDistribution(HDesign& hd, int partitionCount, HSteinerPoint& source);

  virtual ~VGTreeLegalDynamicDistribution();
};

class BufferPositions
{
protected:
  VanGinnekenTreeNode* position;
  BufferInfo* bufferInfo;
  int index;
public:
  BufferPositions();
  BufferPositions(VanGinnekenTreeNode* pos, BufferInfo* bufInfo, int i);

  bool operator > (BufferPositions& element);
  bool operator < (BufferPositions& element);

  VanGinnekenTreeNode* GetPosition();
  BufferInfo* GetBufferInfo();
  int GetIndex();

  void SetPosition(VanGinnekenTreeNode* pos);
  void SetBufferInfo(BufferInfo* bufInfo);
  void SetIndex(int i);
  ~BufferPositions()
  {
    position = NULL;
    bufferInfo = NULL;

  }
};

class VGVariantsListElement
{

protected:
  double RAT;
  double c;
  TemplateTypes<BufferPositions>::list bufferPositions;
  int positionCount;
  int index;
public:

  VGVariantsListElement();

  bool operator > (VGVariantsListElement& element);
  bool operator < (VGVariantsListElement& element);
  bool operator == (VGVariantsListElement& element);
  VGVariantsListElement& operator = (VGVariantsListElement& element);
  void SortBufferPosition();

  double GetRAT();
  double GetC();
  BufferPositions GetStartBufferPosition();
  BufferPositions GetEndBufferPosition();
  BufferPositions GetNextBufferPosition();
  BufferPositions GetbufferPosition(int i);
  int GetPositionCount();
  TemplateTypes<BufferPositions>::list* GetBufferPosition();
  int GetIndex();

  void SetBufferPosition(BufferPositions position);
  void SetBufferPosition(TemplateTypes<BufferPositions>::list position);
  void SetRAT(double rat);
  void SetC(double capacity);
  void SetIndex(int i);


};

class VGAlgorithmData
{
protected:
  int partitionCount;
  bool plotVGTree;
  bool plotNets;
  bool printNetInfo;
  bool plotSteinerPoint;
  bool printVariantsList;
  int plotterWaitTime;
  bool isInitialize;
  bool isInsertInSourceAndSink;
  int typeBufferingAlgorithm;
  int typePartition;
  double sizeBuffer;
  int maxIndex;
  bool* netVisit;
  int maxBufferCount;
  int typeModificationVanGinnekenList;

public:
  HDesign& design;
  HWirePhysicalParams& WirePhisics;
  TemplateTypes<BufferInfo>::vector Buffers;

  VanGinnekenTree* vGTree;

  VGAlgorithmData(HDesign& hd);
  ~VGAlgorithmData();
  void Initialize();
  void LoadBuffers();

  HDesign& GetDesign();
  HWirePhysicalParams& GetWirePhisics();
  TemplateTypes<BufferInfo>::vector GetBuffers();
  int GetPartitionCount();
  bool GetPlotVGTree();
  bool GetPlotNets();
  bool GetPrintNetInfo();
  bool GetPlotSteinerPoint();
  bool GetPrintVariantsList();
  int GetPlotterWaitTime();
  bool GetIsInitialize();
  bool GetIsInsertInSourceAndSink();
  int GetTypeBufferingAlgorithm();
  int GetTypePartition();
  double GetSizeBuffer();
  int GetMaxIndex();
  VanGinnekenTree* GetVGTree();
  bool* GetNetVisit();
  int GetMaxBufferCount();
  int GetTypeModificationVanGinnekenList();

  void SetWirePhisics(HWirePhysicalParams& wPP);
  void SetBuffers(TemplateTypes<BufferInfo>::vector buf);
  void SetPartitionCount(int pC);
  void SetPlotVGTree(bool pVGT);
  void SetPlotNets(bool pN);
  void SetPrintNetInfo(bool pNI);
  void SetPlotSteinerPoint(bool pSP);
  void SetPrintVariantsList(bool pVL);
  void SetPlotterWaitTime(int pWT);
  void SetIsInitialize(bool iIL);
  void SetIsInsertInSourceAndSink(bool iIISAS);
  void SetTypeBufferingAlgorithm(int tBA);
  void SetTypePartition(int tP);
  void SetSizeBuffer(double sB);
  void SetMaxIndex(int mI);
  void SetVGTree(VanGinnekenTree* vgT);
  void SetNetVisit(bool* nV);
  void SetMaxBufferCount(int mBC);
  void SetTypeModificationVanGinnekenList(int tMVGL);

};
#endif //__VanGinnekenData_H__