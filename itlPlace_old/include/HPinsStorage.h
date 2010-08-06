/*  
* HPinsStorage.h
* this is a part of itlAnalyticalPlacer
* Copyright (C) 2009, ITLab, Belyakov
* email: alexander.belyakov@mail.itlab.unn.ru
*/

#ifndef __HIGH_PINS_STORAGE_H__
#define __HIGH_PINS_STORAGE_H__

#include "HDesign.h"
#include "Utils.h"

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

#endif