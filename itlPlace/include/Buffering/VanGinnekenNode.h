#ifndef __VanGinnekenNode_H__
#define __VanGinnekenNode_H__

#include "HDesign.h"

#define	INFINITY	20000000000.0

const double FBI_WIRE_RESISTANCE_SCALING  = 1000.0;
const double FBI_WIRE_CAPACITANCE_SCALING = 1.0;
const double FBI_CELL_RESISTANCE_SCALING  = 1000.0;
const double FBI_CELL_CAPACITANCE_SCALING = 1.0;
const double FBI_TIME_SCALING             = 1000.0;
const double FBI_LENGTH_SCALING           = 0.001;


//NOTE: –езультат вз€ти€ звена VGNode по индексу
class VGItem
{
public:
  VGItem()
  {
    x = 0;
    y = 0;
  }
  HSteinerPoint steinerPoint;
  double x, y;
};

//NOTE: класс хран€щий поддерево:
//
//     ќ
//     |
//     х
//     |
//     o
//
//или
//     o - x
//     |   |
//     x   0
//     |
//     0
//или
// o
//
// "O" - сурс, "o" - синки, "0" - внутрен€€ точка штейнерского дерева, "x" - виртуальное звено, 
// "|" - переход(ребро) влево, " - " - переход(ребро) в право(необ€зательно существует)
class VGNode
{
public:
  VGNode(HSteinerPoint start, HSteinerPoint lEnd, HSteinerPoint rEnd, double capacitance, 
    double requiredArrivalTime, int steps, int step_num, int typePoint, 
    HDesign& hd, int act, int index);

  ~VGNode();
  //отладочна€ функци€.
  void UpdatingTree();


  //NOTE: »нициализаци€ дерева начина€ с корн€, по Ўтейнерскому дереву
  int InitializeTree(HSteinerPoint start,  double capacitance, double requiredArrivalTime, int steps, 
    int typePoint, int act, int index);

  //NOTE: выдает координаты x и y, если isNew = 0-старый, 1-новый 
  double X(int isNew = 0) const ;
  double Y(int isNew = 0) const ;

  double Cap() const ;  //NOTE: node capacitance 
  double Time() const ; //NOTE: required arrival time at the node 

  //NOTE: €вл€етс€ сурсом, внутреним, синком
  bool IsSource() const ;
  bool IsInternal() const ;
  bool IsSink() const ;

  bool IsRightNodeExist();
  bool IsLeftNodeExist();

  VGNode* LeftStep() ;
  VGNode* RightStep() ;
  void    BackStep();

  VGNode* GetLeft() ;
  VGNode* GetRight() ;

  int Index() const ; //NOTE: индекс текущего звена

  //NOTE: возвращает по индексу: штейнерскую точку если она есть, координаты звена, 
  //флаги €вл€етс€ ли точка внутреней, буфером, VGNode в этой точке
  VGNode& GetSteinerPoint(int index, VGNode &node, VGItem& result, bool doIndexesClear = false);

  void Destroy(); //NOTE: уничтожает дерево
  void NULLPointer(); //NOTE: занул€ет левый и правый указатель
  bool IsReal();  //NOTE: по звену можно получить штейнерскую точку
  void IndexesClear();  //NOTE: обнуление индексов в дереве

  double LengthTree(bool doIndexesClear = false);

protected:
  HDesign&      m_hd;

  VGNode* left; //NOTE: указатель на левое поддерево
  VGNode* right; //NOTE:  указатель на правое поддерево

  int    m_typePoint;//NOTE: тип точки 0 - Source; 1 - Internal; 2 - Sink;
  double m_Capacitance; //NOTE: node capacitance 
  double m_RAT; //NOTE: required arrival time at the node  

  HSteinerPoint m_SubtreeRoot; //NOTE: корень поддерева
  HSteinerPoint m_LeftEnd; //NOTE: лева€ вершина поддерева
  HSteinerPoint m_RightEnd; //NOTE: права€ вершина поддерева

  int           m_steps;//NOTE:количество точек на отрезке от начала до конца
  int           m_indexStart;//NOTE: номер в сквозной нумерации
  int           m_RelativePosition; //текуща€ позици€ 0 - левый, 1 - правый, 2 - сурс (или начало)
  int           m_nLeftSteps, m_nRightSteps; //NOTE: индексы, указывающие на звено в поддереве (если = 0 то начало, если = m_steps то конец)
  bool          isTransitionL, isTransitionR; //NOTE: был переход в право, влево
  //NOTE:координаты точек начала и конца
  double m_xs, m_xle, m_xre;
  double m_ys, m_yle, m_yre;

  int m_leftCount; //NOTE: число звеньев в левом поддереве
  bool isRightNodeExist;



};

#endif //__VanGinnekenNode_H__