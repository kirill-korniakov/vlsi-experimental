/*  
* HPinsStorage.cpp
* this is a part of itlAnalyticalPlacer
* Copyright (C) 2009, ITLab, Belyakov
* email: alexander.belyakov@mail.itlab.unn.ru
*/

#include "HDesign.h"
#include "Utils.h"
#include "HPinsStorage.h"
#include <math.h>

PinsStorage::~PinsStorage()
{
  Node* pv = head;
  Node* pt = head;

  while (pv != NULL)
  {
    pt = pv;
    pv = pv->next;
    delete pt;
    pt = NULL;
  }

  head = NULL;
}

void PinsStorage::InsertNode(HPinWrapper pin)
{
  Node* pv = new Node(pin);
  
  if (tail != NULL)
  {
    tail->next = pv;
    pv->prev   = tail;
    tail       = pv;
  }
  
  else //first node
  {
    head = tail = pv;
  }
}

void PinsStorage::ConsiderNewNode(HPinWrapper pin)
{
  if (pin.Direction() == PinDirection_INPUT) //sink
  {
    if (!::IsNull(pin.Cell()))
    {
      Node* pv = FindNode(pin.Cell());

      if (pv != NULL)
      {

        if (pv->IsSource()) //source was found
        {
          InsertNode(pin); //add to the end of the list
          tail->cellFOMByT = pv->cellFOMByT; //set pointer
        }

        else //sink
        {
          HTimingPointWrapper newPoint = design[design.TimingPoints[pin]];
          HTimingPointWrapper oldPoint = design[design.TimingPoints[pv->pin]];

          if (newPoint.Slack() < oldPoint.Slack())
            pv->pin = pin; //this pin is more critical than previous
        }
      }

      else //no sources, no sinks on the cell were not found
        InsertNode(pin);
    }
    
    else //this pin isn't on a cell
    {
      HTimingPointWrapper point = design[design.TimingPoints[pin]];
      
      if (point.Slack() < 0) //if it's critical
      {
        InsertNode(pin);
        tail->FOMByT = 1;
      }
    }
  }

  else //this pin is source
  {
    InsertNode(pin);
    Node* pv = FindNode(pin.Cell());

    if (pv != NULL)
    {
      if (pv->cellFOMByT != NULL)
        tail->cellFOMByT = pv->cellFOMByT;

      else
        tail->cellFOMByT = new unsigned int(0);
    }

    else
    {
      tail->cellFOMByT = new unsigned int(0);
    }
  }
}

void PinsStorage::CalculateFOMSensitivities()
{
  //first - fill pins storage
  for (HTimingPointWrapper pt = design[design.TimingPoints.TopologicalOrderRoot()];
    !::IsNull(pt.GoPrevious()); )
  {
    HPinWrapper pin  = design[design.Get<HTimingPoint::Pin, HPin>(pt)];
    ConsiderNewNode(pin);
  }

  //calculate FOM sensitivities
  Node* pv = head;
  
  while (pv != NULL)
  {
    if (!(pv->IsSource()))
    {
      //Find node pt with source of a pin's net;
      HNetWrapper net = design[pv->pin.Net()];
      HPinWrapper src = design[net.Source()];
      Node* pt = FindSourceNode(src);

      if (pt == NULL)
        LOGERROR("no source for this pin");

      unsigned int fomByT = pv->FOMByT;

      if (pv->cellFOMByT != NULL)
        fomByT += *(pv->cellFOMByT);

      *(pt->cellFOMByT) += fomByT;
      pt->FOMByT        += fomByT;
    }
    
    pv = pv->next;
  }
}

unsigned int PinsStorage::FindFOMByTForSource(HPinWrapper src)
{
  Node* pv = head;

  while (pv != NULL)
  {
    if (pv->pin == src)
      break;

    pv = pv->next;
  }

  if (pv != NULL)
    return pv->FOMByT;
  
  return 0;
}