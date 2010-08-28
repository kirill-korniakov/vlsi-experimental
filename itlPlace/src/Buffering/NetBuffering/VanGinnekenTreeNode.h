#ifndef __VanGinnekenTreeNode_H__
#define __VanGinnekenTreeNode_H__

#include "HSteinerPoint.h"
#include "HNet.h"
class VanGinnekenTree;

class VanGinnekenTreeNode
{
protected:
  double x,y; //координаты узла дерев
  int type; // тип звена: 0 - source; 1 - sink; 2 - точка ветвлени€; 3 - кандидат на вставку буфера; 
  //4 - кандидат на вставку буфера с координатоми совподающими с source или sink;
  //5 - кандидат на вставку буфера с координатой совподающей с внутреними точками дерева штейнера
  // TODO: ѕ–»ƒ”ћј“№ Ќќ–ћјЋ№Ќ”ё ‘ќ–ћ”Ћ»–ќ¬ ”

  int index;
  HSteinerPoint sPoint;
  VanGinnekenTreeNode* left;
  VanGinnekenTreeNode* right;
  VanGinnekenTree* tree;

public:
  HNet net;
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

  int GetIndex();
  VanGinnekenTreeNode* GetLeft();
  VanGinnekenTreeNode* GetRight();
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

#endif