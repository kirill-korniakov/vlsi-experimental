#include "ModificationVanGinnekenList.h"
#include "BufferPositions.h"
#include "NetBufferingAlgorithm.h"

AbstractModificationVanGinnekenList::AbstractModificationVanGinnekenList(NetBufferingAlgorithm* vGA)
{
  vGAlgorithm = vGA;
}

StandartModificationVanGinnekenList::StandartModificationVanGinnekenList(NetBufferingAlgorithm* vGA): 
AbstractModificationVanGinnekenList(vGA)
{
}

TemplateTypes<VGVariantsListElement>::list* 
StandartModificationVanGinnekenList::MergeList(TemplateTypes<VGVariantsListElement>::list* leftVGList, 
                                               TemplateTypes<VGVariantsListElement>::list* RightVGList)
{
  TemplateTypes<VGVariantsListElement>::list* result = new TemplateTypes<VGVariantsListElement>::list();

  TemplateTypes<VGVariantsListElement>::list::iterator left = leftVGList->begin(); 
  TemplateTypes<VGVariantsListElement>::list::iterator right = RightVGList->begin();
  {
    while ((left != leftVGList->end()) && (right != RightVGList->end()))
    {
      VGVariantsListElement newElement;

      newElement.SetC(left->GetC() + right->GetC());
      newElement.SetBufferPosition(*left->GetBufferPosition());
      for (TemplateTypes<BufferPositions>::list::iterator pos = right->GetBufferPosition()->begin(); 
        pos != right->GetBufferPosition()->end(); ++pos)
      {
        newElement.SetBufferPosition(*pos);
      }
      if (left->GetRAT() < right->GetRAT())
      {
        newElement.SetRAT(left->GetRAT());
        left++;
      }
      else      
      {
        newElement.SetRAT(right->GetRAT());
        right++;
      }
      result->push_back(newElement);

    }
  }
  if (vGAlgorithm->data->printVariantsList)
  {
    ALERT("left:\n");
    PrintVariants(leftVGList);
    ALERT("right:\n");
    PrintVariants(RightVGList);
    ALERT("merge:\n");
    PrintVariants(result);
  }

  delete leftVGList;
  delete RightVGList;

  return result;
}

void StandartModificationVanGinnekenList::UpdateValue(TemplateTypes<VGVariantsListElement>::list* vGList, 
                                                      double lengthEdge)
{
  for (TemplateTypes<VGVariantsListElement>::list::iterator i = vGList->begin(); i != vGList->end(); ++i)
  {    
    i->SetRAT(i->GetRAT() - vGAlgorithm->data->WirePhisics.RPerDist * lengthEdge * i->GetC() - 0.5 * 
      vGAlgorithm->data->WirePhisics.RPerDist * lengthEdge * lengthEdge * vGAlgorithm->data->WirePhisics.LinearC);
    i->SetC(i->GetC() + vGAlgorithm->data->WirePhisics.LinearC * lengthEdge);
  }
}

void StandartModificationVanGinnekenList::AddBuffer(TemplateTypes<VGVariantsListElement>::list* vGList, 
                                                    VanGinnekenTreeNode* node)
{  
  for(unsigned int j = 0; j < vGAlgorithm->data->Buffers.size(); j++)
  {
    double tMax = -INFINITY;
    double t;

    TemplateTypes<VGVariantsListElement>::list::iterator iOpt;
    bool findOpt = false;
    for (TemplateTypes<VGVariantsListElement>::list::iterator i = vGList->begin(); i != vGList->end(); ++i)
    {
      if ((i->GetPositionCount() >= vGAlgorithm->data->maxBufferCount) && (vGAlgorithm->data->maxBufferCount > 0))
        continue;

      t = i->GetRAT() - vGAlgorithm->data->Buffers[j].Tb() - vGAlgorithm->data->Buffers[j].Rb() * 
        i->GetC();
      if (t > tMax)
      {
        tMax = t;
        iOpt = i;
        findOpt = true;
      }
    }
    if (findOpt)
    {
      VGVariantsListElement newElement;
      newElement.SetBufferPosition(*iOpt->GetBufferPosition());
      BufferPositions bp(node, &vGAlgorithm->data->Buffers[j], 0);
      newElement.SetBufferPosition(bp);
      newElement.SetC(vGAlgorithm->data->Buffers[j].Cb());
      newElement.SetRAT(tMax);    
      vGAlgorithm->data->maxIndex = vGAlgorithm->data->maxIndex + 1;
      newElement.SetIndex(vGAlgorithm->data->maxIndex);
      InsertVGVariantsListElement(vGList, newElement);
    }
  }
}

void StandartModificationVanGinnekenList::SortVGVariantsListElement(TemplateTypes<VGVariantsListElement>::list* vGList)
{
  TemplateTypes<VGVariantsListElement>::list::iterator i = vGList->begin();
  TemplateTypes<VGVariantsListElement>::list::iterator j = i;
  bool isDeleteVariantsListElement = false;

  while (i != vGList->end())
  {
    if ((!isDeleteVariantsListElement) || (i == j))
      j++;

    isDeleteVariantsListElement = false;
    if (j == vGList->end())
      break;

    if ((i->GetRAT() > j->GetRAT()) && (i->GetC() < j->GetC()))
    {
      TemplateTypes<VGVariantsListElement>::list::iterator j1 = j;
      j++;
      vGList->erase(j1);
      isDeleteVariantsListElement = true;
      continue;
    }
    else
    {
      if ((i->GetRAT() < j->GetRAT()) && (i->GetC() > j->GetC()))
      {
        TemplateTypes<VGVariantsListElement>::list::iterator i1 = i;
        i++;
        vGList->erase(i1);
        isDeleteVariantsListElement = true;
        continue;
      }
      else
      {
        if ((i->GetRAT() > j->GetRAT()) && (fabs(i->GetC() - j->GetC()) < ZERO))
        {
          TemplateTypes<VGVariantsListElement>::list::iterator j1 = j;
          j++;
          vGList->erase(j1);
          isDeleteVariantsListElement = true;
          continue;
        }
        else
          if ((i->GetRAT() < j->GetRAT()) && (fabs(i->GetC() - j->GetC()) < ZERO))
          {
            TemplateTypes<VGVariantsListElement>::list::iterator i1 = i;
            i++;
            vGList->erase(i1);
            isDeleteVariantsListElement = true;
            continue;
          }

          if ((fabs(i->GetRAT() - j->GetRAT()) < ZERO) && (i->GetC() < j->GetC()))
          {
            TemplateTypes<VGVariantsListElement>::list::iterator j1 = j;
            j++;
            vGList->erase(j1);
            isDeleteVariantsListElement = true;
            continue;
          }
          else
            if ((fabs(i->GetRAT() - j->GetRAT()) < ZERO) && (i->GetC() > j->GetC()))
            {
              TemplateTypes<VGVariantsListElement>::list::iterator i1 = i;
              i++;
              vGList->erase(i1);
              isDeleteVariantsListElement = true;
              continue;
            }
      }
    }
    i = j;   
  }
}

void StandartModificationVanGinnekenList::InsertVGVariantsListElement(
  TemplateTypes<VGVariantsListElement>::list* vGList, VGVariantsListElement& element)
{
  if (vGList->size() == 0)
  {
    vGList->push_front(element);
    return;
  }
  bool isIns = false;
  bool stop = false;
  if ((vGList->front().GetRAT() > element.GetRAT()) && (vGList->front().GetC() > element.GetC()))
  {
    vGList->push_front(element);
  }
  else
  {
    TemplateTypes<VGVariantsListElement>::list::iterator i = vGList->end();
    --i;
    for (; ((!stop) && (!isIns)); )
    {
      if ((i->GetRAT() >= element.GetRAT()) && (i->GetC() <= element.GetC()))
      {
        isIns = true;
      }
      else
        if ((i->GetRAT() < element.GetRAT()) && (i->GetC() > element.GetC()))
        {
          (*i) = element;
          isIns = true;
        }
        else
          if ((i->GetRAT() < element.GetRAT()) && (i->GetC() < element.GetC()))
          {
            i++;
            if (i != vGList->end())
              vGList->insert(i, element);
            else
              vGList->push_back(element);
            isIns = true;
          }
          if (i == vGList->begin())
            stop = true;
          else
            --i;
    }
  }
}

double StandartModificationVanGinnekenList::GetLength(VanGinnekenTreeNode* node1, VanGinnekenTreeNode* node2)
{
  return fabs(node1->x - node2->x) + fabs(node1->y - node2->y);
}

TemplateTypes<VGVariantsListElement>::list* 
StandartModificationVanGinnekenList::CreateNewVGList(VanGinnekenTreeNode* node)
{
  TemplateTypes<VGVariantsListElement>::list* newList = new TemplateTypes<VGVariantsListElement>::list();
  VGVariantsListElement element;
  element.SetC(node->GetC());
  element.SetRAT(node->GetSinkRAT());  

  vGAlgorithm->data->maxIndex = vGAlgorithm->data->maxIndex + 1;
  element.SetIndex(vGAlgorithm->data->maxIndex);
  newList->push_back(element);

  if (vGAlgorithm->data->printVariantsList)
  { 
    ALERT("sink id: %d", node->index);
    PrintVariants(newList);
  }
  return newList;
}

ModificationVanGinnekenListAccountingBorder::ModificationVanGinnekenListAccountingBorder(NetBufferingAlgorithm* vGA): 
StandartModificationVanGinnekenList(vGA)
{
}
TemplateTypes<VGVariantsListElement>::list* 
ModificationVanGinnekenListAccountingBorder::MergeList(TemplateTypes<VGVariantsListElement>::list* leftVGList, 
                                                       TemplateTypes<VGVariantsListElement>::list* RightVGList)
{
  TemplateTypes<VGVariantsListElement>::list* result = new TemplateTypes<VGVariantsListElement>::list();

  for (TemplateTypes<VGVariantsListElement>::list::iterator left = leftVGList->begin(); left != leftVGList->end(); left++)
    for (TemplateTypes<VGVariantsListElement>::list::iterator right = RightVGList->begin(); 
      right != RightVGList->end(); right++)
    {
      if (((left->GetPositionCount() + right->GetPositionCount()) > vGAlgorithm->data->maxBufferCount) 
        && (vGAlgorithm->data->maxBufferCount > 0))
        continue;

      VGVariantsListElement newElement;

      newElement.SetC(left->GetC() + right->GetC());
      newElement.SetBufferPosition(*left->GetBufferPosition());
      for (TemplateTypes<BufferPositions>::list::iterator pos = right->GetBufferPosition()->begin(); 
        pos != right->GetBufferPosition()->end(); ++pos)
      {
        newElement.SetBufferPosition(*pos);
      }
      if (left->GetRAT() < right->GetRAT())
      {
        newElement.SetRAT(left->GetRAT());
      }
      else      
      {
        newElement.SetRAT(right->GetRAT());
      }

      InsertVGVariantsListElement(result, newElement);
    }  

    if (vGAlgorithm->data->printVariantsList)
    {
      ALERT("left:\n");
      PrintVariants(leftVGList);
      ALERT("right:\n");
      PrintVariants(RightVGList);
      ALERT("merge:\n");
      PrintVariants(result);
    }

    delete leftVGList;
    delete RightVGList;

    return result;
}

ExhaustiveSearch::ExhaustiveSearch(NetBufferingAlgorithm* vGA):ModificationVanGinnekenListAccountingBorder(vGA)
{
}

void ExhaustiveSearch::AddBuffer(TemplateTypes<VGVariantsListElement>::list* vGList, 
                                 VanGinnekenTreeNode* node)
{  
  for(unsigned int j = 0; j < vGAlgorithm->data->Buffers.size(); j++)
  {
    double tMax;
    for (TemplateTypes<VGVariantsListElement>::list::iterator iOpt = vGList->begin(); iOpt != vGList->end(); ++iOpt)
    {
      if ((iOpt->GetPositionCount() >= vGAlgorithm->data->maxBufferCount) && (vGAlgorithm->data->maxBufferCount > 0))
        continue;

      tMax = iOpt->GetRAT() - vGAlgorithm->data->Buffers[j].Tb() - vGAlgorithm->data->Buffers[j].Rb() * 
        iOpt->GetC();

      VGVariantsListElement newElement;
      newElement.SetBufferPosition(*iOpt->GetBufferPosition());
      BufferPositions bp(node, &vGAlgorithm->data->Buffers[j], 0);
      newElement.SetBufferPosition(bp);
      newElement.SetC(vGAlgorithm->data->Buffers[j].Cb());
      newElement.SetRAT(tMax);    
      vGAlgorithm->data->maxIndex = vGAlgorithm->data->maxIndex + 1;
      newElement.SetIndex(vGAlgorithm->data->maxIndex);
      InsertVGVariantsListElement(vGList, newElement);
    }
  }
}

void ExhaustiveSearch::SortVGVariantsListElement(TemplateTypes<VGVariantsListElement>::list* vGList)
{
}

void ExhaustiveSearch::InsertVGVariantsListElement(TemplateTypes<VGVariantsListElement>::list* vGList, 
                                                   VGVariantsListElement& element)
{
  bool isIns = false;
  bool stop = false;
  if (vGList->size() == 0)
  {
    vGList->push_front(element);
    return;
  }

  if ((vGList->front().GetRAT() > element.GetRAT()) && (vGList->front().GetC() > element.GetC()))
  {
    vGList->push_front(element);
  }
  else
  {
    TemplateTypes<VGVariantsListElement>::list::iterator i = vGList->end();
    --i;
    for (; ((!stop) && (!isIns)); )
    {
      if ((i->GetRAT() < element.GetRAT()) && (i->GetC() < element.GetC()))
      {
        i++;
        if (i != vGList->end())
          vGList->insert(i, element);
        else
          vGList->push_back(element);
        isIns = true;
      }
      if (i == vGList->begin())
        stop = true;
      else
        --i;
    }
  }
  if (!isIns)
    vGList->push_front(element);
}