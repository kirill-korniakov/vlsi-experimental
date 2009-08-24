#ifndef __VanGinneken_H__
#define __VanGinneken_H__


#include "HDesign.h"
#include "HDPGrid.h"
#include "AdaptiveRoute.h"
#include "DelayCalculation.h"
#include "Timing.h" 
#include "Legalization.h"
#include <crtdbg.h>
#include <fstream>
#include "VanGinnekenNode.h"
#include "PlacementQualityAnalyzer.h"


class HDesign;

#define EPS	0.000000000000000000000001 //!!!
#define MAXBUFFERTYPE 64
#define nullSP m_hd.SteinerPoints.Null()
#define QM 1
#define PREDICT 1 //NOTE: predictive pruning 
#define TREATZERO 1

class VanGinneken
{
private:
  struct BufferInfo
  {
    HMacroType BufferMacroType;
    double TIntrinsic;
    double Capacitance;
    double Resistance;
  };

  //NOTE: хранит индексы 
  typedef struct comp
  { 
    struct comp *left;  
    struct comp *right; 
    int buffertype; 
    int x;  //(x, y) means the buffer is inserted at point with index y with the direction x->y  
    int y;  
    int counter;  //NOTE: use for comp deletion and comp chare 
  } Comp;

  //NOTE: Ordered list used in Van Ginneken algorithm 
  typedef struct rlnode
  {
    struct rlnode *next;
    double cap;   //NOTE: downstream capacitance 
    double time;  //NOTE: required arrival time at the node 
    Comp *com;    //NOTE: buffer location 
  } RLnode;

public:
  VanGinneken(HDesign& design);

  //NOTE: двупроходная буферизация, сначало выбираем "хорошие" пути, потом их буферизуем
  int Buffering2Pass(HDPGrid& DPGrid, int argc, char** argv, const char* exportFileName);
  int BufferingTillDegradation(); //NOTE: буферизуем до ухудшения

  int NetBuffering(HNet &net);    //NOTE: буферизация нета

  int BufferingOfMostCriticalPaths(int nPaths = 0); //NOTE: буферизуются все критические пути
  int CriticalPathBuffering(HCriticalPath aPath);   //NOTE: буферизация критического пути
  
  HWirePhysicalParams GetPhysical();
  BufferInfo* GetBufferInfo();
  VGNode GetVGTree();
  int NetBufferNotDegradation(HNet &net);

private:
  typedef TemplateTypes<BufferInfo>::vector BuffersVector;

  RLnode *create_list(VGNode *t); //NOTE: создает RLnode
  void list_delete(RLnode *l);  //NOTE: удаляет RLnode
  void comp_delete(Comp *com);   //NOTE: удаляет comp
  RLnode *redundent(RLnode *list);
  RLnode *add_buffer(double distance, RLnode *list, int px, int py);
  double distance(VGNode *t, VGNode *t1);
  RLnode* MergeLists(RLnode* sleft, RLnode* sright, VGNode *t);

  //NOTE: метод запускающий алгоритм Ван Генникена
  RLnode *van(VGNode *t, double& van_answer, double rd);

  void LoadAvailableBuffers();

  void ParsingFinalLocationVan(VanGinneken::Comp *com);

  //NOTE: создает и инициализирует VGnode
  int RunVG(HNet& net);

  void InitializeBuffersIdxs();

  //NOTE: считает количество пинов в новом нете
  void PinsCountCalculation(VGNode& startNode, int startNodeIdx, int& nPins, bool doIndexesClear = false);

  //NOTE: Создает новые неты и целы
  void CreateNetsAndCells(HNet& net);  
  void CreateCells(string bufferName, HCell* insertedBuffers);
  void CreateNets(HNet& net, HCell* insertedBuffers, HNet* newNet);
  void AddSinks2Net(HCell* insertedBuffers, HNet& net, VGNode& startNode, int startNodeIdx, 
    HNetWrapper::PinsEnumeratorW& pinEnum, bool doIndexesClear = false);

  void print(RLnode* x);
  void printbuffer(Comp *x, int *i);

  double CalculationOptimumNumberBuffers(HNet net);

  BuffersVector       m_AvailableBuffers;  //NOTE: библиотека доступных буферов для вставки
  HWirePhysicalParams m_WirePhisics;
  HDesign&            m_hd;
  VGNode              m_vgNetSplitted;
  int                 m_nCandidatesForBuffering;  //количество нетов которые хотели буферизовать
  int*                m_buffersIdxsAtNetSplitted; //индексы звеньев в которые рекомендуется вставить буферы,
                                                  //m_buffersIdxsAtNetSplitted[i][0] храним длину значимой области 

  RLnode* m_VGOutput;                     //результирующий RLNode выдаваемый методом Ван Генникена
  int     m_nCriticalPaths;     //количество критических путей, которые пытались буферизовать
  int     m_RATidx;             //текущий индекс в массиве rAT
  double  m_freeSpace;          //количество оставшегося свободного пространства на плате
  bool    m_isFreeSpaceEnded;   //кончилось ли свободное пространство

  bool    m_doReportBuffering;
  Comp*   m_finalLocationVan;   //NOTE: последняя структура Comp созданая Ван Генникеном
  double  bestTNS, bestWNS;
  PlacementQualityAnalyzer* m_QualityAnalyzer; 
};

#endif //__VanGinneken_H__