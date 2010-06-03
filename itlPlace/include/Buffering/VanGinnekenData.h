#ifndef __VanGinnekenData_H__
#define __VanGinnekenData_H__

//#include "HDesign.h"
#include "BufferInfo.h"
#include "HDPGrid.h"

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

  void CreateTree();

  VanGinnekenTreeNode* CreateNode(HSteinerPoint node, int type, int& index, int rootIndex, bool isRight = false, VanGinnekenTree* tree = NULL);
public:

  HDesign& design;
  HDPGrid DPGrid;

  VanGinnekenTree(HDesign& hd);
  VanGinnekenTree(HDesign& hd, int partitionCount);
  VanGinnekenTree(HDesign& hd, int partitionCount, HSteinerPoint& source);

  void UpdateTree(HSteinerPoint& source);

  void SetEdgePartitionCount(int partitionCount);

  VanGinnekenTreeNode GetSource();
  double GetR();

  void ClearTree();

  int TreeSize()
  {
    return treeSize;
  }

  ~VanGinnekenTree();
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

class NetId
{
public:
  string name;
  int id;
  bool isCreateVGTree;
  NetId()
  {
    name = "";
    id = -1;
    isCreateVGTree = false;
  }
};

class VGTreeRepository
{
protected:
  int lastId;
  HDesign& design;
  int netCount;
  int partitionCount;
  TemplateTypes<NetId>::vector NetIdRepository;
public:
  VGTreeRepository(HDesign& hd);
  void CreateVGTree(HNet net);
  int GetLastId();
  bool NoVGTree(HNet net);
  int SetNet(string s, int id);
  int FindId(string s);

};

#endif //__VanGinnekenData_H__