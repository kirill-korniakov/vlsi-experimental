#ifndef __VanGinnekenAlgorithm_H__
#define __VanGinnekenAlgorithm_H__
#include "VanGinnekenData.h"

#define	INFINITY	20000000000.0
#define MAXBUFFERTYPE 64

class VGAlgorithm
{
protected:
	HDesign& design;
	HWirePhysicalParams& WirePhisics;
	TemplateTypes<BufferInfo>::vector Buffers;

	void LoadBuffers();

public:
	VGAlgorithm(HDesign& hd);

	int BufferingPlacement();
	int BufferingNen(HNet& net);
	VGVariantsListElement* Algorithm(VanGinnekenTree* vGTree);
	TemplateTypes<VGVariantsListElement>::list* CreateVGList(VanGinnekenTreeNode& node);
	TemplateTypes<VGVariantsListElement>::list* MergeList(TemplateTypes<VGVariantsListElement>::list* leftVGList, TemplateTypes<VGVariantsListElement>::list* RightVGList);
	void UpdateValue(TemplateTypes<VGVariantsListElement>::list* vGList, double lengthEdge);
	void AddBuffer(TemplateTypes<VGVariantsListElement>::list* vGList, VanGinnekenTreeNode& node);
	double GetLength(VanGinnekenTreeNode& node1, VanGinnekenTreeNode& node2);
	void InsertBuffer(BufferPositions& position);

	~VGAlgorithm()
	{
		int i = 0;
	}
};

#endif //__VanGinnekenAlgorithm_H__