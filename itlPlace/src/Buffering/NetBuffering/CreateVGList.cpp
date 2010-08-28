#include "CreateVGList.h"
#include "NetBufferingAlgorithm.h"
#include "VanGinnekenTree.h"

AbstractCreateVGListAlgorithm::AbstractCreateVGListAlgorithm(NetBufferingAlgorithm* vGA)
{
  vGAlgorithm = vGA;
}

ClassicCreateVGListAlgorithm::ClassicCreateVGListAlgorithm(NetBufferingAlgorithm* vGA): 
AbstractCreateVGListAlgorithm(vGA)
{
}

TemplateTypes<VGVariantsListElement>::list* 
ClassicCreateVGListAlgorithm::CreateVGList(VanGinnekenTree* tree)
{
  return CreateVGList(tree->GetSource().GetLeft());
}

TemplateTypes<VGVariantsListElement>::list* 
ClassicCreateVGListAlgorithm::CreateVGList( VanGinnekenTreeNode* node)
{
  if (node->isSink())
  {    
    return vGAlgorithm->modificationVanGinnekenList->CreateNewVGList(node);
  }
  else
    if (node->isBranchPoint())
    {
      TemplateTypes<VGVariantsListElement>::list* leftVGList = CreateVGList(node->GetLeft());

      TemplateTypes<VGVariantsListElement>::list* RightVGList;
      if (node->HasRight())
      {
        RightVGList = CreateVGList(node->GetRight());        
        if (vGAlgorithm->data->printVariantsList)
          ALERT("Branch Point id: %d", node->GetIndex());
        return vGAlgorithm->modificationVanGinnekenList->MergeList(leftVGList, RightVGList);
      }
      if (vGAlgorithm->data->printVariantsList)
      { 
        ALERT("BranchPoint id: %d", node->GetIndex());
        PrintVariants(leftVGList);
      }
      return leftVGList;
    }
    else
    {
      TemplateTypes<VGVariantsListElement>::list* newList = CreateVGList(node->GetLeft());
      vGAlgorithm->modificationVanGinnekenList->UpdateValue(newList, 
        vGAlgorithm->modificationVanGinnekenList->GetLength(node, node->GetLeft()));
      vGAlgorithm->modificationVanGinnekenList->SortVGVariantsListElement(newList);
      if (node->isCandidateAndRealPoint())
      {
        if (vGAlgorithm->data->isInsertInSourceAndSink)
          vGAlgorithm->modificationVanGinnekenList->AddBuffer(newList, node);
      }
      else
        vGAlgorithm->modificationVanGinnekenList->AddBuffer(newList, node);

      if (vGAlgorithm->data->printVariantsList)
      { 
        ALERT("point id: %d", node->GetIndex());
        ALERT("Length line %d and %d: %.2f",node->GetIndex(), node->GetLeft()->GetIndex(), 
          vGAlgorithm->modificationVanGinnekenList->GetLength(node, node->GetLeft()));
        PrintVariants(newList);
      }
      return newList;
    }
}

LineBypassAtCreateVGListAlgorithm::LineBypassAtCreateVGListAlgorithm(NetBufferingAlgorithm* vGA): 
AbstractCreateVGListAlgorithm(vGA)
{
}

TemplateTypes<VGVariantsListElement>::list* 
LineBypassAtCreateVGListAlgorithm::CreateVGList(VanGinnekenTree* tree)
{
  TemplateTypes<TemplateTypes<VGVariantsListElement>::list*>::stack stackList;
  int lastIndexTree = tree->GetTreeSize() - 1;

  TemplateTypes<VGVariantsListElement>::list* newList = NULL;
  TemplateTypes<VGVariantsListElement>::list* currentList = NULL;
  TemplateTypes<VGVariantsListElement>::list* leftList = NULL;
  TemplateTypes<VGVariantsListElement>::list* rightList = NULL;

  for (int i = lastIndexTree; i > 0; i--)
  {
    if (tree->vGTree[i].isSink())
    {
      newList = vGAlgorithm->modificationVanGinnekenList->CreateNewVGList(&tree->vGTree[i]);
      stackList.push(newList);
      newList = NULL;
    }
    else
      if (tree->vGTree[i].isBranchPoint())
      {
        leftList = stackList.top();
        stackList.pop();

        if (tree->vGTree[i].HasRight())
        {
          rightList = stackList.top();
          stackList.pop();
          if (vGAlgorithm->data->printVariantsList)
            ALERT("Branch Point id: %d", tree->vGTree[i].GetIndex());          
          currentList = vGAlgorithm->modificationVanGinnekenList->MergeList(leftList, rightList);
          stackList.push(currentList);
          leftList = NULL;
          rightList = NULL;
          currentList = NULL;
        }
        else
        {
          if (vGAlgorithm->data->printVariantsList)
          { 
            ALERT("BranchPoint id: %d", tree->vGTree[i].GetIndex());
            PrintVariants(leftList);
          }
          stackList.push(leftList);
          leftList = NULL;
        }
      }
      else
      {
        currentList = stackList.top();
        stackList.pop();

        vGAlgorithm->modificationVanGinnekenList->UpdateValue(currentList, 
          vGAlgorithm->modificationVanGinnekenList->GetLength(&(tree->vGTree[i]), 
          (tree->vGTree[i].GetLeft())));
        vGAlgorithm->modificationVanGinnekenList->SortVGVariantsListElement(currentList);
        if (tree->vGTree[i].isCandidateAndRealPoint())
        {
          if (vGAlgorithm->data->isInsertInSourceAndSink)
            vGAlgorithm->modificationVanGinnekenList->AddBuffer(currentList, &tree->vGTree[i]);
        }
        else
          vGAlgorithm->modificationVanGinnekenList->AddBuffer(currentList, &tree->vGTree[i]);

        if (vGAlgorithm->data->printVariantsList)
        { 
          ALERT("point id: %d", tree->vGTree[i].GetIndex());
          ALERT("Length line %d and %d: %.2f",tree->vGTree[i].GetIndex(), 
            tree->vGTree[i].GetLeft()->GetIndex(), vGAlgorithm->modificationVanGinnekenList->GetLength(&tree->vGTree[i], tree->vGTree[i].GetLeft()));
          PrintVariants(currentList);
        }
        stackList.push(currentList);
        currentList = NULL;
      }
  }
  if (stackList.size() > 1)
    ALERT("Error: length stackList > 1");

  newList = stackList.top();
  stackList.pop();
  return newList;
}

AdaptiveBypassAtCreateVGListAlgorithm::AdaptiveBypassAtCreateVGListAlgorithm(NetBufferingAlgorithm* vGA): 
AbstractCreateVGListAlgorithm(vGA)
{
}

TemplateTypes<VGVariantsListElement>::list* 
AdaptiveBypassAtCreateVGListAlgorithm::CreateVGList(VanGinnekenTree* tree)
{
  TemplateTypes<TemplateTypes<VGVariantsListElement>::list*>::stack stackList;
  int lastIndexTree = tree->GetTreeSize() - 1;

  TemplateTypes<VGVariantsListElement>::list* newList = NULL;
  TemplateTypes<VGVariantsListElement>::list* currentList = NULL;
  TemplateTypes<VGVariantsListElement>::list* leftList = NULL;
  TemplateTypes<VGVariantsListElement>::list* rightList = NULL;

  for (int i = lastIndexTree; i > 0; i--)
  {
    if (tree->vGTree[i].isSink())
    {
      newList = vGAlgorithm->modificationVanGinnekenList->CreateNewVGList(&tree->vGTree[i]);
      stackList.push(newList);
      newList = NULL;
    }
    else
      if (tree->vGTree[i].isBranchPoint())
      {
        leftList = stackList.top();
        stackList.pop();

        if (tree->vGTree[i].HasRight())
        {
          rightList = stackList.top();
          stackList.pop();
          if (vGAlgorithm->data->printVariantsList)
            ALERT("Branch Point id: %d", tree->vGTree[i].GetIndex());          
          currentList = vGAlgorithm->modificationVanGinnekenList->MergeList(leftList, rightList);
          stackList.push(currentList);
          leftList = NULL;
          rightList = NULL;
          currentList = NULL;
        }
        else
        {
          if (vGAlgorithm->data->printVariantsList)
          { 
            ALERT("BranchPoint id: %d", tree->vGTree[i].GetIndex());
            PrintVariants(leftList);
          }
          stackList.push(leftList);
          leftList = NULL;
        }
      }
      else
      {
        currentList = stackList.top();
        stackList.pop();
        if (tree->vGTree[i].isInternal() || tree->vGTree[i].isCandidateAndRealPoint())
        {
          vGAlgorithm->modificationVanGinnekenList->UpdateValue(currentList, 
            vGAlgorithm->modificationVanGinnekenList->GetLength(&(tree->vGTree[i]), 
            (tree->vGTree[i].GetLeft())));
          vGAlgorithm->modificationVanGinnekenList->SortVGVariantsListElement(currentList);
        }
        else
        {
          TemplateTypes<VGVariantsListElement>::list copyList(*currentList);

          int currentMaxIndex = vGAlgorithm->data->maxIndex;

          vGAlgorithm->modificationVanGinnekenList->UpdateValue(&copyList, 
            vGAlgorithm->modificationVanGinnekenList->GetLength(&(tree->vGTree[i]), 
            (tree->vGTree[i].GetLeft())));
          vGAlgorithm->modificationVanGinnekenList->SortVGVariantsListElement(&copyList);

          if (tree->vGTree[i].isCandidateAndRealPoint())
          {
            if (vGAlgorithm->data->isInsertInSourceAndSink)
              vGAlgorithm->modificationVanGinnekenList->AddBuffer(&copyList, &tree->vGTree[i]);
          }
          else
            vGAlgorithm->modificationVanGinnekenList->AddBuffer(&copyList, &tree->vGTree[i]);
          if (currentMaxIndex != vGAlgorithm->data->maxIndex)
          {
            if (copyList.begin()->GetIndex() > currentMaxIndex)
            {
              currentList->push_front(*copyList.begin());
            }
            else
            {

              TemplateTypes<VGVariantsListElement>::list::iterator j = currentList->begin();
              for (TemplateTypes<VGVariantsListElement>::list::iterator k = copyList.begin(); 
                k != copyList.end(); k++)
              {
                if ((*k) == (*j))
                  j++;
                else
                  vGAlgorithm->modificationVanGinnekenList->InsertVGVariantsListElement(currentList, *k);
              }
            }
          }
        }

        if (vGAlgorithm->data->printVariantsList)
        { 
          ALERT("point id: %d", tree->vGTree[i].GetIndex());
          ALERT("Length line %d and %d: %.2f",tree->vGTree[i].GetIndex(), 
            tree->vGTree[i].GetLeft()->GetIndex(), vGAlgorithm->modificationVanGinnekenList->GetLength(&tree->vGTree[i], tree->vGTree[i].GetLeft()));
          PrintVariants(currentList);
        }
        stackList.push(currentList);
        currentList = NULL;
      }
  }
  if (stackList.size() > 1)
    ALERT("Error: length stackList > 1");

  newList = stackList.top();
  stackList.pop();
  return newList;
}