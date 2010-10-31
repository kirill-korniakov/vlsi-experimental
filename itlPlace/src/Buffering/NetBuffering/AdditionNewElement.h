#ifndef __AdditionNewElement_H__
#define __AdditionNewElement_H__

#include "Utils.h"
#include "NewBuffer.h"

class NetBufferingAlgorithm;
using namespace Utils;

class AbstractAdditionNewElement
{
public:
    AbstractAdditionNewElement(NetBufferingAlgorithm* vGA);
    virtual void InsertsBuffer(TemplateTypes<NewBuffer>::list& newBuffer, VGVariantsListElement* best) = 0;
    virtual void InsertBuffer(TemplateTypes<NewBuffer>::list& newBuffer,BufferPositions& position) = 0;
    virtual void AddSinksToNet(HNet& subNet, VanGinnekenTreeNode* node, HNetWrapper::PinsEnumeratorW& subNetPinEnumW, TemplateTypes<NewBuffer>::list& newBuffer) = 0;
    virtual NewBuffer* FindBufferNumberByIndex(VanGinnekenTreeNode* node, TemplateTypes<NewBuffer>::list& newBuffer) = 0;
    virtual void PinsCountCalculation(VanGinnekenTreeNode* node, int& nPins, TemplateTypes<NewBuffer>::list& newBuffer) = 0;
    virtual void CreateNets(HNet& net, TemplateTypes<NewBuffer>::list& newBuffer, HNet* newNet, VanGinnekenTreeNode* node, int newNetCount) = 0;
    HCriticalPath currentCriticalPath;

protected:
    NetBufferingAlgorithm* vGAlgorithm;
};

class StandartAdditionNewElement: public AbstractAdditionNewElement
{
public:
    StandartAdditionNewElement(NetBufferingAlgorithm* vGA);
    virtual void InsertsBuffer(TemplateTypes<NewBuffer>::list& newBuffer, VGVariantsListElement* best);
    virtual void InsertBuffer(TemplateTypes<NewBuffer>::list& newBuffer,BufferPositions& position);
    virtual void AddSinksToNet(HNet& subNet, VanGinnekenTreeNode* node, HNetWrapper::PinsEnumeratorW& subNetPinEnumW, TemplateTypes<NewBuffer>::list& newBuffer);
    virtual NewBuffer* FindBufferNumberByIndex(VanGinnekenTreeNode* node, TemplateTypes<NewBuffer>::list& newBuffer);
    virtual void PinsCountCalculation(VanGinnekenTreeNode* node, int& nPins, TemplateTypes<NewBuffer>::list& newBuffer);
    virtual void AddPointToSteinerTree(NewBuffer& buffer, TemplateTypes<NewBuffer>::list& newBuffer);
    HSteinerPoint FindStartPoindInEdge(NewBuffer& buffer, bool& isRight, TemplateTypes<NewBuffer>::list& newBuffer);
    HSteinerPoint FindEndPoindInEdge(NewBuffer& buffer, TemplateTypes<NewBuffer>::list& newBuffer);
    virtual void CreateNets(HNet& net, TemplateTypes<NewBuffer>::list& newBuffer, HNet* newNet, VanGinnekenTreeNode* node, int newNetCount);
};

class LegalAdditionNewElement: public StandartAdditionNewElement
{
public:
    LegalAdditionNewElement(NetBufferingAlgorithm* vGA);
    virtual void InsertBuffer(TemplateTypes<NewBuffer>::list& newBuffer,BufferPositions& position);
};

#endif