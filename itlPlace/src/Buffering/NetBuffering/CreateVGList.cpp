#include "CreateVGList.h"
#include "NetBufferingAlgorithm.h"
#include "VanGinnekenTree.h"
#include "Timing.h"

HTimingArcType FindArrivalArc(HDesign& hd,
                              HTimingPoint arcEndPoint,
                              KSignalDirection dir,
                              double& arcTime,
                              bool& isInversed);

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
    return CreateVGList(tree->GetSource()->GetLeft());
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
                    ALERT("Branch Point id: %d", node->index);
                return vGAlgorithm->modificationVanGinnekenList->MergeList(leftVGList, RightVGList);
            }
            if (vGAlgorithm->data->printVariantsList)
            { 
                ALERT("BranchPoint id: %d", node->index);
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
                ALERT("point id: %d", node->index);
                ALERT("Length line %d and %d: %.2f",node->index, node->GetLeft()->index, 
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


CalculateVGCandidatePoint::CalculateVGCandidatePoint(NetBufferingAlgorithm* vGA)
{
    vGAlgorithm = vGA;
}

void CalculateVGCandidatePoint::CalculateCandidatePoint(VanGinnekenTree* tree, TemplateTypes<TemplateTypes<VGVariantsListElement>::list*>::stack& stackList,
                                                        TemplateTypes<VGVariantsListElement>::list* currentList, int i)
{
    currentList = stackList.top();
    stackList.pop();

    vGAlgorithm->modificationVanGinnekenList->UpdateValue(currentList, 
        vGAlgorithm->modificationVanGinnekenList->GetLength(tree->vGTreeNodeList[i], 
        tree->vGTreeNodeList[i]->GetLeft()));
    vGAlgorithm->modificationVanGinnekenList->SortVGVariantsListElement(currentList);
    if (tree->vGTreeNodeList[i]->isCandidateAndRealPoint())
    {
        if (vGAlgorithm->data->isInsertInSourceAndSink)
            vGAlgorithm->modificationVanGinnekenList->AddBuffer(currentList, tree->vGTreeNodeList[i]);
    }
    else
        vGAlgorithm->modificationVanGinnekenList->AddBuffer(currentList, tree->vGTreeNodeList[i]);

    if (vGAlgorithm->data->printVariantsList)
    { 
        ALERT("point id: %d", tree->vGTreeNodeList[i]->index);
        ALERT("Length line %d and %d: %.2f",tree->vGTreeNodeList[i]->index, 
            tree->vGTreeNodeList[i]->GetLeft()->index, vGAlgorithm->modificationVanGinnekenList->GetLength(tree->vGTreeNodeList[i], tree->vGTreeNodeList[i]->GetLeft()));
        PrintVariants(currentList);
    }
    stackList.push(currentList);
    currentList = NULL;
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
        if (tree->vGTreeNodeList[i]->isSink())
        {
            newList = vGAlgorithm->modificationVanGinnekenList->CreateNewVGList(tree->vGTreeNodeList[i]);
            stackList.push(newList);
            newList = NULL;
        }
        else
        {
            if (tree->vGTreeNodeList[i]->isBranchPoint())
            {
                vGAlgorithm->calculateBranchPoint->CalculateBranchPoint(tree, stackList, currentList, leftList, rightList, i);
            }	
            else
            {
                vGAlgorithm->calculateCandidatePoint->CalculateCandidatePoint(tree, stackList, currentList, i);
            }
        }
    }
    ASSERT(stackList.size() == 1);

    newList = stackList.top();
    stackList.pop();
    return newList;
}

AdaptiveBypassAtCalculateVGCandidatePoint::AdaptiveBypassAtCalculateVGCandidatePoint(NetBufferingAlgorithm* vGA): 
CalculateVGCandidatePoint(vGA)
{
}

void AdaptiveBypassAtCalculateVGCandidatePoint::CalculateCandidatePoint(VanGinnekenTree* tree, TemplateTypes<TemplateTypes<VGVariantsListElement>::list*>::stack& stackList,
                                                                        TemplateTypes<VGVariantsListElement>::list* currentList, int i)
{
    currentList = stackList.top();
    stackList.pop();
    if (tree->vGTreeNodeList[i]->isInternal() || tree->vGTreeNodeList[i]->isCandidateAndRealPoint())
    {
        vGAlgorithm->modificationVanGinnekenList->UpdateValue(currentList, 
            vGAlgorithm->modificationVanGinnekenList->GetLength(tree->vGTreeNodeList[i], 
            tree->vGTreeNodeList[i]->GetLeft()));
        vGAlgorithm->modificationVanGinnekenList->SortVGVariantsListElement(currentList);
    }
    else
    {
        TemplateTypes<VGVariantsListElement>::list copyList(*currentList);

        int currentMaxIndex = vGAlgorithm->data->maxIndex;

        vGAlgorithm->modificationVanGinnekenList->UpdateValue(&copyList, 
            vGAlgorithm->modificationVanGinnekenList->GetLength(tree->vGTreeNodeList[i], 
            tree->vGTreeNodeList[i]->GetLeft()));
        vGAlgorithm->modificationVanGinnekenList->SortVGVariantsListElement(&copyList);

        if (tree->vGTreeNodeList[i]->isCandidateAndRealPoint())
        {
            if (vGAlgorithm->data->isInsertInSourceAndSink)
                vGAlgorithm->modificationVanGinnekenList->AddBuffer(&copyList, tree->vGTreeNodeList[i]);
        }
        else
            vGAlgorithm->modificationVanGinnekenList->AddBuffer(&copyList, tree->vGTreeNodeList[i]);
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
        ALERT("point id: %d", tree->vGTreeNodeList[i]->index);
        ALERT("Length line %d and %d: %.2f",tree->vGTreeNodeList[i]->index, 
            tree->vGTreeNodeList[i]->GetLeft()->index, vGAlgorithm->modificationVanGinnekenList->GetLength(tree->vGTreeNodeList[i], tree->vGTreeNodeList[i]->GetLeft()));
        PrintVariants(currentList);
    }
    stackList.push(currentList);
    currentList = NULL;
}

CalculateVGBranchPoint::CalculateVGBranchPoint(NetBufferingAlgorithm* vGA)
{
    vGAlgorithm = vGA;
}

void CalculateVGBranchPoint::CalculateBranchPoint(VanGinnekenTree* tree, TemplateTypes<TemplateTypes<VGVariantsListElement>::list*>::stack& stackList,
                                                  TemplateTypes<VGVariantsListElement>::list* currentList,
                                                  TemplateTypes<VGVariantsListElement>::list* leftList,
                                                  TemplateTypes<VGVariantsListElement>::list* rightList, int i)
{
    leftList = stackList.top();
    stackList.pop();

    if (tree->vGTreeNodeList[i]->HasRight())
    {
        rightList = stackList.top();
        stackList.pop();
        if (vGAlgorithm->data->printVariantsList)
            ALERT("Branch Point id: %d", tree->vGTreeNodeList[i]->index);          
        currentList = vGAlgorithm->modificationVanGinnekenList->MergeList(leftList, rightList);
        stackList.push(currentList);
        leftList = NULL;
        rightList = NULL;
        currentList = NULL;
    }
    else
    {
        UpdateInBranchPoint(tree, stackList, currentList, leftList,  i);
        if (vGAlgorithm->data->printVariantsList)
        { 
            ALERT("BranchPoint id: %d", tree->vGTreeNodeList[i]->index);
            PrintVariants(leftList);
        }
        stackList.push(leftList);
        leftList = NULL;
    }
}

void CalculateVGBranchPoint::UpdateInBranchPoint(VanGinnekenTree* tree, TemplateTypes<TemplateTypes<VGVariantsListElement>::list*>::stack& stackList,
                                                 TemplateTypes<VGVariantsListElement>::list* currentList,
                                                 TemplateTypes<VGVariantsListElement>::list* leftList, int i)

{
}

void PathBasedCalculateVGBranchPoint::UpdateInBranchPoint(VanGinnekenTree* tree, TemplateTypes<TemplateTypes<VGVariantsListElement>::list*>::stack& stackList,
                                                          TemplateTypes<VGVariantsListElement>::list* currentList,
                                                          TemplateTypes<VGVariantsListElement>::list* leftList, int i)
{
    if (tree->vGTreeNodeList[i]->type == SOURCE_AND_SINK)
    {
        double timingRAT = tree->vGTreeNodeList[i]->GetSinkRAT();
        double timingC = tree->vGTreeNodeList[i]->GetC();

        HPinWrapper driver = tree->vGAlgorithmData->design[tree->vGAlgorithmData->design.SteinerPoints.Get<HSteinerPoint::Pin, HPin>(tree->vGTreeNodeList[i]->GetSteinerPoint())];
        DriverPhisics result;
        result.T = result.R = 0.0;
        bool inverted;
        double arcTime;

        HTimingArcType arc = FindRequiredArc(tree->vGAlgorithmData->design, tree->vGAlgorithmData->design.TimingPoints[driver], SignalDirection_Rise, arcTime, inverted);
        result.R = tree->vGAlgorithmData->design.GetDouble<HTimingArcType::ResistanceRise>(arc);
        result.T = tree->vGAlgorithmData->design.GetDouble<HTimingArcType::TIntrinsicRise>(arc);

        for (TemplateTypes<VGVariantsListElement>::list::iterator vGElement = leftList->begin(); vGElement != leftList->end(); ++vGElement)
        {

            vGElement->SetRAT(vGElement->GetRAT() - result.T - result.R * 
                vGElement->GetC());
            vGElement->SetC(tree->vGTreeNodeList[i]->GetC());
            if (vGElement->GetPositionCount() == 0)
            {
                double rat = fabs(timingRAT - vGElement->GetRAT());
                if (rat > 0.00001)
                {
                    HPinWrapper pw = tree->vGAlgorithmData->design[tree->vGAlgorithmData->design.SteinerPoints.Get<HSteinerPoint::Pin, HPin>(tree->vGTreeNodeList[i]->GetSteinerPoint(false))];
                    HCellWrapper cw = tree->vGAlgorithmData->design[pw.Cell()];
                    ALERT("%s %s", cw.Name().c_str(), pw.Name().c_str());
                    ALERT("ERROR timingRAT %f\t!= vGElement->GetRAT %f", timingRAT, vGElement->GetRAT());
                    ALERT("c = %f", timingC);
                }

            }
        }
    }
}
