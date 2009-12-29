/*  
* TDDPAlgorithm4.h
* this is a part of itlAnalyticalPlacer
* Copyright (C) 2009, ITLab, Belyakov
* email: alexander.belyakov@mail.itlab.unn.ru
*/

#ifndef __TDDP_ALGORITHM4_H__
#define __TDDP_ALGORITHM4_H__

#include "HDesign.h"
#include "Utils.h"

HPin FindNextSrc(HPin src, const HDesign& hd);

struct NetSensitivity
{
  HNetWrapper net;
  double slackByWeight;
  double FOMByWeight;

  NetSensitivity(HNetWrapper _net, double slByW, double FOMByW):
                 net(_net), slackByWeight(slByW), FOMByWeight(FOMByW) {}
};

typedef std::list<NetSensitivity> NetSensitivities;

class PinsStorage
{
private:
  class Node
  {
    public:
      HPinWrapper pin;
      HCell cell;
      unsigned int FOMByT;
      unsigned int* cellFOMByT;
      
      Node* next;
      Node* prev;

      Node(HPinWrapper _pin): pin(_pin), FOMByT(0), cell(pin.Cell()), cellFOMByT(NULL),
                              next(NULL), prev(NULL) {}
      
      ~Node()
      {
        if (!(IsSource()) && (cellFOMByT != NULL))
        {
          delete cellFOMByT;
          cellFOMByT = NULL;
        }
      }

      bool IsSource() { return (pin.Direction() == PinDirection_OUTPUT); }
  };

  Node* head;
  Node* tail;
  HDesign& design;

  void InsertNode(HPinWrapper pin);
  void ConsiderNewNode(HPinWrapper pin);
  void CalculateFOMSensitivities();
  
  Node* FindNode(HCell cell)
  {
    Node* pv = tail;
  
    while (pv != NULL)
    {
      if (pv->cell == cell)
        break;
      
      pv = pv->prev;
    }
  
    return pv;
  }

  Node* FindSourceNode(HPinWrapper src)
  {
    Node* pv = tail;
  
    while (pv != NULL)
    {
      if (pv->pin == src)
        break;
      
      pv = pv->prev;
    }
  
    return pv;
  }

public:
  PinsStorage(HDesign& hd): design(hd), head(NULL), tail(NULL) { CalculateFOMSensitivities(); }
  ~PinsStorage();
  unsigned int FindFOMByTForSource(HPinWrapper src); //find FOM sensitivity for source
};

void SensitivityGuidedNetWeighting(HDesign& hd);

#endif