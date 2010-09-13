#ifndef __VanGinnekenTreeNode_H__
#define __VanGinnekenTreeNode_H__

#include "HSteinerPoint.h"
#include "HNet.h"
class VanGinnekenTree;

enum NodeType
{
    SOURCE = 0, //- source;
    SINK = 1, // - sink;
    BRANCH = 2, // - точка ветвления;
    CANDIDATE = 3, // - кандидат на вставку буфера; 
    CANDIDATE_ON_PIN = 4, // - кандидат на вставку буфера с координатоми совподающими с source или sink;
    CANDIDATE_INTERNAL = 5 // - кандидат на вставку буфера с координатой совподающей с внутреними точками дерева штейнера
};

class VanGinnekenTreeNode
{
public:
  HNet net;
  VanGinnekenTreeNode();  
  ~VanGinnekenTreeNode();

  double x,y; //координаты узла дерев
  NodeType type; 
  int index;

  bool HasLeft();
  bool HasRight();
  HSteinerPoint GetSteinerPoint();  

  VanGinnekenTreeNode* GetLeft();
  VanGinnekenTreeNode* GetRight();
  VanGinnekenTree* GetTree(); 

  double GetRAT();//RAT in sinc
  double GetC();//capacity in sinc
  double GetR();//resistance in source

  void SetSteinerPoint(HSteinerPoint sp);  
  void SetLeft(VanGinnekenTreeNode* node);
  void SetRight(VanGinnekenTreeNode* node);
  void SetTree(VanGinnekenTree* t);

  bool isSource();
  bool isSink();
  bool isBranchPoint();
  bool isCandidate();
  bool isCandidateAndRealPoint();
  bool isInternal();

protected:
  HSteinerPoint sPoint;
  VanGinnekenTreeNode* left;
  VanGinnekenTreeNode* right;
  VanGinnekenTree* tree;
};

#endif