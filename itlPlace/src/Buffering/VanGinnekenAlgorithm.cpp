#include "VanGinnekenAlgorithm.h"
#include <math.h>

void VGAlgorithm::LoadBuffers()
{
	BufferInfo buf = BufferInfo::Create(design);
	Buffers.push_back(buf);
}

VGAlgorithm::VGAlgorithm(HDesign& hd): design(hd), WirePhisics(hd.RoutingLayers.Physics)
{
	LoadBuffers();
}

int VGAlgorithm::BufferingPlacement()
{
	for (HNets::ActiveNetsEnumeratorW inw = design.Nets.GetActiveNetsEnumeratorW(); inw.MoveNext();)
	{
		if (inw.PinsCount() == 2)
		{
			BufferingNen(inw);
			string s = design.Nets.GetString<HNet::Name>(inw);
				break;
		}
	}
	return 0;
}

int VGAlgorithm::BufferingNen(HNet& net)
{
	VanGinnekenTree vGTree(design, 3, design.SteinerPoints[(net, design).Source()]);
	vGTree.GetR();
	VGVariantsListElement* best = Algorithm(&vGTree);
	int bufCount = best->GetPositionCount();
	return bufCount;
}

VGVariantsListElement* VGAlgorithm::Algorithm(VanGinnekenTree* vGTree)
{

	double t = 0;
	VGVariantsListElement* best;
	double tMax = -INFINITY;

	TemplateTypes<VGVariantsListElement>::list* vGList = CreateVGList(vGTree->GetSource().GetLeft());
	for (TemplateTypes<VGVariantsListElement>::list::iterator i = vGList->begin(); i != vGList->end(); ++i)
	{
		t = i->GetRAT() - i->GetC() * vGTree->GetR();

		if (t > tMax)
		{
			tMax = t;
			best = &(*i);
		}
	}
	
	return best;
}

TemplateTypes<VGVariantsListElement>::list* VGAlgorithm::CreateVGList( VanGinnekenTreeNode& node)
{
	if (node.isSink())
	{
		TemplateTypes<VGVariantsListElement>::list* newList = new TemplateTypes<VGVariantsListElement>::list();
		VGVariantsListElement element;
		element.SetC(node.GetC());
		element.SetRAT(node.GetRAT());
		newList->push_back(element);
		return newList;
	}
	else
	if (node.isBranchPoint())
	{
		TemplateTypes<VGVariantsListElement>::list* leftVGList = CreateVGList(node.GetLeft());
		TemplateTypes<VGVariantsListElement>::list* RightVGList;
		if (node.HasRight())
		{
			RightVGList = CreateVGList(node.GetLeft());
			return MergeList(leftVGList, RightVGList);
		}
		return leftVGList;
	}
	else
	{
		TemplateTypes<VGVariantsListElement>::list* newList = CreateVGList(node.GetLeft());
		UpdateValue(newList, GetLength(node, node.GetLeft()));
		AddBuffer(newList, node);
		return newList;
	}

}

TemplateTypes<VGVariantsListElement>::list* VGAlgorithm::MergeList(TemplateTypes<VGVariantsListElement>::list* leftVGList, TemplateTypes<VGVariantsListElement>::list* RightVGList)
{
	TemplateTypes<VGVariantsListElement>::list* result = new TemplateTypes<VGVariantsListElement>::list();
	
	for (TemplateTypes<VGVariantsListElement>::list::iterator left = leftVGList->begin(); left != leftVGList->end(); ++left)
	{
		for (TemplateTypes<VGVariantsListElement>::list::iterator right = RightVGList->begin(); right != RightVGList->end(); ++right)
		{
			VGVariantsListElement newElement;
			if (left->GetRAT() < right->GetRAT())
				newElement.SetRAT(left->GetRAT());
			else
				newElement.SetRAT(right->GetRAT());
			newElement.SetC(left->GetC() + right->GetC());
			left->SortBufferPosition();
			right->SortBufferPosition();
			newElement.SetBufferPosition(left->GetBufferPosition());			
			newElement.GetBufferPosition().merge(right->GetBufferPosition());
			result->push_back(newElement);

		}
	}
	return result;
}

void VGAlgorithm::UpdateValue(TemplateTypes<VGVariantsListElement>::list* vGList, double lengthEdge)
{
	for (TemplateTypes<VGVariantsListElement>::list::iterator i = vGList->begin(); i != vGList->end(); ++i)
	{
		i->SetRAT(i->GetRAT() - WirePhisics.RPerDist * lengthEdge * i->GetC() - 0.5 * WirePhisics.RPerDist * lengthEdge * lengthEdge * WirePhisics.LinearC);
		i->SetC(i->GetC() + WirePhisics.LinearC * lengthEdge);
	}
}

void VGAlgorithm::AddBuffer(TemplateTypes<VGVariantsListElement>::list* vGList, VanGinnekenTreeNode& node)
{
	double tMax = -INFINITY;
	double t;

	TemplateTypes<VGVariantsListElement>::list::iterator iOpt;
	for (TemplateTypes<VGVariantsListElement>::list::iterator i = vGList->begin(); i != vGList->end(); ++i)
	{
		t = i->GetRAT() - Buffers[0].Tb() - Buffers[0].Rb() * i->GetC();
		if (t > tMax)
		{
			tMax = t;
			iOpt = i;
		}
	}
	VGVariantsListElement newElement;
	newElement.SetBufferPosition(iOpt->GetBufferPosition());
	BufferPositions bp(&node, &Buffers[0], 0);
	newElement.SetBufferPosition(bp);
	newElement.SetC(Buffers[0].Cb());
	newElement.SetRAT(tMax);
	newElement.SortBufferPosition();
	vGList->push_back(newElement);
}

double VGAlgorithm::GetLength(VanGinnekenTreeNode& node1, VanGinnekenTreeNode& node2)
{

	return fabs(node1.GetX() - node2.GetX()) + fabs(node1.GetY() - node2.GetY());
}

  void VGAlgorithm::InsertBuffer(BufferPositions& position)
  {
    if (position.GetBufferInfo() == 0) return;

    HCellWrapper buffer = design[design.Cells.AllocateCell()];
    buffer.SetType(position.GetBufferInfo()->Type());
    buffer.SetPlacementStatus(PlacementStatus_Movable);
    buffer.SetHeight(design.GetDouble<HMacroType::SizeY>(position.GetBufferInfo()->Type()));
    buffer.SetWidth(design.GetDouble<HMacroType::SizeX>(position.GetBufferInfo()->Type()));
    
    char bufferName[32];
    sprintf(bufferName, "buf_%d", ::ToID(buffer));//TODO: create unique name
    buffer.SetName(bufferName);
    buffer.SetX(position.GetPosition()->GetX() - buffer.Width() * 0.5);
    buffer.SetY(position.GetPosition()->GetY() - buffer.Height() * 0.5);
    buffer.SetOrientation(Orientation_N);
    design.Pins.AllocatePins(buffer);
  }