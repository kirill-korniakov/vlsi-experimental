#ifndef __VanGinnekenData_H__
#define __VanGinnekenData_H__

//#include "HDesign.h"
#include "BufferInfo.h"
#include "HSteinerPoint.h"
//#include "HDPGrid.h
#include "PlacementGrid.h"

class HDPGrid;
class VanGinnekenTree;

class VanGinnekenTreeNode
{
protected:
  double x,y; //координаты узла дерев
  int type; // тип звена: 0 - source; 1 - sink; 2 - точка ветвления; 3 - кандидат на вставку буфера
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
  VanGinnekenTreeNode* vGTree;
  int partitionPointCount;
  int treeSize;  
  int TypePartition;

  virtual void CreateTree() = 0;
  virtual VanGinnekenTreeNode* CreateNode(HSteinerPoint node, int type, int& index, int rootIndex, bool isRight = false, VanGinnekenTree* tree = NULL) = 0;


public:
  HDesign& design;
  HDPGrid* DPGrid;

  VanGinnekenTree(HDesign& hd);
  VanGinnekenTree(HDesign& hd, int partitionCount);
  VanGinnekenTree(HDesign& hd, int partitionCount, HSteinerPoint& source);

  virtual void UpdateTree(HSteinerPoint& source);

  virtual void SetEdgePartitionCount(int partitionCount);

  virtual VanGinnekenTreeNode GetSource();
  virtual double GetR();

  virtual void ClearTree();

  virtual int TreeSize();


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

  void SetBufferPosition(BufferPositions position);
  void SetBufferPosition(TemplateTypes<BufferPositions>::list position);
  void SetRAT(double rat);
  void SetC(double capacity);


};
/*
class VGVariantsList
{
protected:
  BufferPositions* bufferPositions;
  int sizeList;
  int lastVariantId;
public:

  VGVariantsList();
  VGVariantsList(int sizeVariantsList);

  void push_front(VGVariantsListElement element);
  void push_back(VGVariantsListElement element);
  void insert(VGVariantsListElement element, int index);
  void erase(int id);
  void UpdateVariant(VGVariantsListElement element, int id);
  bufferPositions GetVariants(int id);

  

};
*/
#endif //__VanGinnekenData_H__