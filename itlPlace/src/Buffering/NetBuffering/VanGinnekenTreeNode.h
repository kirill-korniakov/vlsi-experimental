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
    CANDIDATE_INTERNAL = 5, // - кандидат на вставку буфера с координатой совподающей с внутреними точками дерева штейнера
    SOURCE_AND_SINK = 6 // - точка объеденяющая сорс и синк двух нетов в одну точку
};

class VanGinnekenTreeNode
{
public:
    VanGinnekenTreeNode();  
    ~VanGinnekenTreeNode();

    double x,y; //координаты узла дерев
    NodeType type; 
    int index;
    KSignalDirection signalDirection; //известен только у точек с типом SOURCE_AND_SINK

    bool HasLeft();
    bool HasRight();
    virtual HSteinerPoint GetSteinerPoint(bool first = true);
    virtual HNet GetNet(bool first = true);

    VanGinnekenTreeNode* GetLeft();
    VanGinnekenTreeNode* GetRight();
    VanGinnekenTree* GetTree(); 
    VanGinnekenTreeNode* GetRoot();

    virtual double GetSinkRAT();//RAT in sinc
    virtual double GetSourceRAT();//RAT in source
    virtual double GetC();//capacity in sinc
    virtual double GetR(bool isRequiredArc = false);//resistance in source
    virtual double GetT();

    virtual void SetSteinerPoint(HSteinerPoint sp, bool first = true);  
    virtual void SetNet(HNet n, bool first = true);
    void SetLeft(VanGinnekenTreeNode* node);
    void SetRight(VanGinnekenTreeNode* node);
    virtual void SetTree(VanGinnekenTree* t);
    void SetRoot(VanGinnekenTreeNode* node);

    virtual bool isSource();
    virtual bool isSink();
    virtual bool isBranchPoint();
    virtual bool isCandidate();
    virtual bool isCandidateAndRealPoint();
    virtual bool isInternal();
    virtual bool isSteinerTreeLeaf();
    virtual bool isSteinerTreeSource();

protected:
    HNet net;
    HSteinerPoint sPoint;
    VanGinnekenTreeNode* left;
    VanGinnekenTreeNode* right;
    VanGinnekenTree* tree;
    VanGinnekenTreeNode* root;
};

class VanGinnekenTreeNodePathBased: public VanGinnekenTreeNode
{
public:
    VanGinnekenTreeNodePathBased();
    virtual double GetSourceRAT();//RAT in source
    virtual double GetR(bool isRequiredArc = false);//resistance in source
    virtual double GetT();
    virtual HSteinerPoint GetSteinerPoint(bool first = true);
    virtual HNet GetNet(bool first = true);
    virtual void SetSteinerPoint(HSteinerPoint sp, bool first = true);  
    virtual void SetNet(HNet n, bool first = true);

protected:
    HNet secondNet;
    HSteinerPoint secondSPoint;
};

#endif
