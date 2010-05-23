#include "VanGinnekenData.h"
#include "Utils.h"

//VanGinnekenTreeNode

class VanGinnekenTreeNode;

VanGinnekenTreeNode::VanGinnekenTreeNode()
{
  x = 0;
  y = 0;
  type = 3;
  //sPoint = tree->design.SteinerPoints.Null();
  index = 0;
  tree = NULL;
  left = NULL;
  right = NULL;
}

bool VanGinnekenTreeNode::isSource()
{
  return (type == 0) ? true : false;
}

bool VanGinnekenTreeNode::isSink()
{
  return (type == 1) ? true : false;
}

bool VanGinnekenTreeNode::isBranchPoint()
{
  return (type == 2) ? true : false;
}

bool VanGinnekenTreeNode::isCandidate()
{
  return (type == 3) ? true : false;
}

bool VanGinnekenTreeNode::HasLeft()
{
  if (left != NULL)
    return true;
  return false;
}

bool VanGinnekenTreeNode::HasRight()
{
  if (right != NULL)
    return true;
  return false;
}

HSteinerPoint VanGinnekenTreeNode::GetSteinerPoint()
{
  return sPoint;
}

double VanGinnekenTreeNode::GetX()
{
  return x;
}

double VanGinnekenTreeNode::GetY()
{
  return y;
}

VanGinnekenTreeNode* VanGinnekenTreeNode::GetLeft()
{
  return left;
}

VanGinnekenTreeNode* VanGinnekenTreeNode::GetRight()
{
  return right;
}

int VanGinnekenTreeNode::GetIndex()
{
  return index;
}

VanGinnekenTree* VanGinnekenTreeNode::GetTree()
{
  return tree;
}

double VanGinnekenTreeNode::GetRAT()	
{		
  if (sPoint != tree->design.SteinerPoints.Null())
  {
    return (tree->design.TimingPoints[(sPoint,tree->design).Pin()],tree->design).RequiredTime();
  }
  else
    return 0;
}

double VanGinnekenTreeNode::GetC()
{
  if (sPoint != tree->design.SteinerPoints.Null())
    return (((sPoint,tree->design).Pin(),tree->design).Type(),tree->design).Capacitance();
  else
    return 0;
}

double VanGinnekenTreeNode::GetR()
{
  if (sPoint != tree->design.SteinerPoints.Null())
    return Utils::GetDriverTimingPhisics(tree->design, (sPoint,tree->design).Pin(), SignalDirection_None).R;
  else
    return 0;
}

void VanGinnekenTreeNode::SetSteinerPoint(HSteinerPoint sp)  
{
  sPoint = sp;
}
void VanGinnekenTreeNode::SetX(double value)
{
  x = value;
}
void VanGinnekenTreeNode::SetY(double value)
{
  y = value;
}

void VanGinnekenTreeNode::SetLeft(VanGinnekenTreeNode* node)
{
  left = node;
}
void VanGinnekenTreeNode::SetRight(VanGinnekenTreeNode* node)
{
  right = node;
}
void VanGinnekenTreeNode::SetType(int t)
{
  type = t;
}

void VanGinnekenTreeNode::SetIndex(int i)
{
  index = i;
}

void VanGinnekenTreeNode::SetTree(VanGinnekenTree* t)
{
  tree = t;
}
//VanGinnekenTreeNode

//VanGinnekenTree
VanGinnekenTree::VanGinnekenTree(HDesign& hd): design(hd), DPGrid(hd)
{

}

VanGinnekenTree::VanGinnekenTree(HDesign& hd, int partitionCount): design(hd), DPGrid(hd)
{
  partitionPointCount = partitionCount;
}

VanGinnekenTree::VanGinnekenTree(HDesign& hd, int partitionCount, HSteinerPoint& source): design(hd), DPGrid(hd)
{
  partitionPointCount = partitionCount;
  CreateTree(source);
}

void VanGinnekenTree::SetEdgePartitionCount(int partitionCount)
{
  partitionPointCount = partitionCount;
}

void VanGinnekenTree::CreateTree(HSteinerPoint& source)
{
  HSteinerPointWrapper srcPoint0 = design[source];
  HSteinerPointWrapper nextPoint0 = srcPoint0;
  //считаем количество точек дерева штейнера.
  int pointCount = 0;
  TemplateTypes<HSteinerPoint>::stack points0;
  int maxPos = partitionPointCount;
  points0.push(srcPoint0);
  TypePartition = design.cfg.ValueOf("TypePartition", 0);
  
  if (TypePartition == 1)
  {
    while (!points0.empty())
    {
      srcPoint0 = points0.top();
      points0.pop();

      if (srcPoint0.HasLeft())
      {
        nextPoint0 = srcPoint0.Left();

        double rx,ry,nx,ny;
        rx = srcPoint0.X();
        ry = srcPoint0.Y();
        nx = nextPoint0.X();
        ny = nextPoint0.Y();

        int rs = DPGrid.FindRow(ry);
        int rf = DPGrid.FindRow(ny);

        int n = abs(rs - rf);
        double r = (DPGrid.RowY(rf) - DPGrid.RowY(rs)) / double(n);

        int rowInd = 0;
        int pointInRowCount = 0;
        int newPartitionPointCount = 0;
        if (((partitionPointCount - 1.0) / n) < 1)
        {
          newPartitionPointCount = n + 1;
          pointInRowCount = 1;
        }
        else
        {
          pointInRowCount = int(ceil(((partitionPointCount - 1.0) / n)));
          newPartitionPointCount = pointInRowCount * n + 1;
        }

        if (newPartitionPointCount > maxPos)
          maxPos = newPartitionPointCount;


        pointCount++;
        points0.push(nextPoint0);

        if (srcPoint0.HasRight())
        {
          nextPoint0 = srcPoint0.Right();

          double rx,ry,nx,ny;
          rx = srcPoint0.X();
          ry = srcPoint0.Y();
          nx = nextPoint0.X();
          ny = nextPoint0.Y();

          int rs = DPGrid.FindRow(ry);
          int rf = DPGrid.FindRow(ny);

          int n = abs(rs - rf);
          double r = (DPGrid.RowY(rf) - DPGrid.RowY(rs)) / double(n);

          int rowInd = 0;
          int pointInRowCount = 0;
          int newPartitionPointCount = 0;
          if (((partitionPointCount - 1.0) / n) < 1)
          {
            newPartitionPointCount = n + 1;
            pointInRowCount = 1;
          }
          else
          {
            pointInRowCount = int(ceil(((partitionPointCount - 1.0) / n)));
            newPartitionPointCount = pointInRowCount * n + 1;
          }

          if (newPartitionPointCount > maxPos)
            maxPos = newPartitionPointCount;

          pointCount++;
          points0.push(nextPoint0);
        }
      }

      else
      {
        //sink
        pointCount++;
      }
    }
    maxPos += 2;
  }
  else
  {
    while (!points0.empty())
    {
      srcPoint0 = points0.top();
      points0.pop();

      if (srcPoint0.HasLeft())
      {
        nextPoint0 = srcPoint0.Left();
        pointCount++;
        points0.push(nextPoint0);

        if (srcPoint0.HasRight())
        {
          nextPoint0 = srcPoint0.Right();
          pointCount++;
          points0.push(nextPoint0);
        }
      }

      else
      {
        //sink
        pointCount++;
      }
    }
  }

  TemplateTypes<HSteinerPoint>::stack points;
  TemplateTypes<int>::stack rootIndexs;
  //создаем дерево
  treeSize = pointCount * maxPos + pointCount + 1;
  vGTree = new VanGinnekenTreeNode [treeSize];
  int nodeIndex = 0;
  int rootIndex = 0;
  HSteinerPoint srcPoint = design[source];
  HSteinerPoint nextPoint = srcPoint;
  points.push(srcPoint);	
  rootIndexs.push(0);
  CreateNode(srcPoint, 0, nodeIndex, rootIndex, false, this);//, design[design.SteinerPoints.Null()], false, design[design.SteinerPoints.Null()]);
  while (!points.empty())
  {
    srcPoint = points.top();
    rootIndex = rootIndexs.top();
    points.pop();	
    rootIndexs.pop();
    if (design.SteinerPoints.Get<HSteinerPoint::HasLeft, bool>(srcPoint))
    {
      nextPoint = design.SteinerPoints.Get<HSteinerPoint::Left, HSteinerPoint>(srcPoint);

      CreateNode(nextPoint, 2, nodeIndex, rootIndex, false, this);
      rootIndexs.push(nodeIndex);
      points.push(nextPoint);			

      if (design.SteinerPoints.Get<HSteinerPoint::HasRight, bool>(srcPoint))
      {
        nextPoint = design.SteinerPoints.Get<HSteinerPoint::Right, HSteinerPoint>(srcPoint);

        CreateNode(nextPoint, 2, nodeIndex, rootIndex, true, this);
        rootIndexs.push(nodeIndex);
        points.push(nextPoint);
      }
    }
    else
    {
      //sink
      //CreateNode(&srcPoint, 1, nodeIndex, rootIndex);
    }
  }
  //design.Plotter.PlotVGTree(&vGTree[0], Color_Black);
  //design.Plotter.Refresh();
  if (nodeIndex >= treeSize)
    ALERT("ERROR1!!!!! + treeSize = %d\tnodeIndex = %d",treeSize, nodeIndex);
}

VanGinnekenTreeNode* VanGinnekenTree::CreateNode(HSteinerPoint node, int type, int& nodeIndex, int rootIndex, bool isRight, VanGinnekenTree* tree)
//тип звена: 0 - source; 1 - sink; 2 - точка ветвления; 3 - кандидат на вставку буфера
{

  if (type == 0)
  {
    HSteinerPointWrapper spw = design[node];
    vGTree[nodeIndex].SetSteinerPoint(node);
    vGTree[nodeIndex].SetType(type);
    vGTree[nodeIndex].SetX(spw.X());
    vGTree[nodeIndex].SetY(spw.Y());
    vGTree[nodeIndex].SetIndex(nodeIndex);
    vGTree[nodeIndex].SetTree(tree);
  }
  if (type == 2)
  {
    HSteinerPointWrapper spw = design[node];
    double rx,ry,nx,ny;
    rx = vGTree[rootIndex].GetX();
    ry = vGTree[rootIndex].GetY();
    nx = spw.X();
    ny = spw.Y();

    int rs = DPGrid.FindRow(ry);
    int rf = DPGrid.FindRow(ny);

    int n = abs(rs - rf);

    double DPGrid_RowY_rf = DPGrid.RowY(rf);
    double DPGrid_RowY_rs = DPGrid.RowY(rs);

    double r = (DPGrid.RowY(rf) - DPGrid.RowY(rs)) / double(n);

    int rowInd = 0;
    int pointInRowCount = 0;
    int newPartitionPointCount = 0;
    if (((partitionPointCount - 1.0) / n) < 1)
    {
      newPartitionPointCount = n + 1;
      pointInRowCount = 1;
    }
    else
    {
      pointInRowCount = int(ceil(((partitionPointCount - 1.0) / n)));
      newPartitionPointCount = pointInRowCount * n + 1;
    }    

    if (isRight)
      vGTree[rootIndex].SetRight(CreateNode(tree->design.SteinerPoints.Null(), 3, nodeIndex, rootIndex));
    else
      vGTree[rootIndex].SetLeft(CreateNode(tree->design.SteinerPoints.Null(), 3, nodeIndex, rootIndex));

    vGTree[nodeIndex].SetX(vGTree[rootIndex].GetX());
    vGTree[nodeIndex].SetY(vGTree[rootIndex].GetY());
    rootIndex = nodeIndex;
    if (TypePartition == 0)
    { //обычное разбиение ребра
      for (int i = 1; i < (partitionPointCount - 1); i++)
      {
        vGTree[rootIndex].SetLeft(CreateNode(tree->design.SteinerPoints.Null(), 3, nodeIndex, rootIndex));
        vGTree[nodeIndex].SetX(rx + (nx - rx) / (partitionPointCount - 1) * i);
        vGTree[nodeIndex].SetY(ry + (ny - ry) / (partitionPointCount - 1) * i);
        rootIndex = nodeIndex;
      }
    }
    else
      if (design.cfg.ValueOf("TypePartition", 0) == 1)
      {//новое разбиение

        double y = DPGrid.RowY(rs);
        for (int i = 1; i < (newPartitionPointCount - 1); i++)
        {

          vGTree[rootIndex].SetLeft(CreateNode(tree->design.SteinerPoints.Null(), 3, nodeIndex, rootIndex));

          vGTree[nodeIndex].SetY(y);
          if ((newPartitionPointCount - 1) != 0)
            vGTree[nodeIndex].SetX(DPGrid.ColumnX(DPGrid.FindColumn(rx + (nx - rx) / (newPartitionPointCount - 1) * i)));
          else
          {
            ALERT("ERROR!!!!!!!!!");
          }

          rootIndex = nodeIndex;
          if (((i) % pointInRowCount) == 0)
          {
            rowInd++;
            y = DPGrid_RowY_rs + rowInd * r;
          }
        }
      }



      vGTree[rootIndex].SetLeft(CreateNode(tree->design.SteinerPoints.Null(), 3, nodeIndex, rootIndex));
      vGTree[nodeIndex].SetX(spw.X());
      vGTree[nodeIndex].SetY(spw.Y());
      rootIndex = nodeIndex;

      nodeIndex++;
      vGTree[nodeIndex].SetSteinerPoint(node);
      vGTree[nodeIndex].SetX(spw.X());
      vGTree[nodeIndex].SetY(spw.Y());
      vGTree[rootIndex].SetLeft(&vGTree[nodeIndex]);
      vGTree[nodeIndex].SetIndex(nodeIndex);
      vGTree[nodeIndex].SetTree(tree);

      if (!spw.IsInternal())
      {
        vGTree[nodeIndex].SetType(1);
      }
      else
      {
        vGTree[nodeIndex].SetType(type);	
      }

      if (nodeIndex >= treeSize)
        ALERT("ERROR1!!!!!");

  }
  if (type == 3)
  {
    nodeIndex++;
    vGTree[nodeIndex].SetType(type);	
    vGTree[nodeIndex].SetIndex(nodeIndex);
    vGTree[nodeIndex].SetTree(tree);
  }
  return &vGTree[nodeIndex];
}


VanGinnekenTreeNode VanGinnekenTree::GetSource()
{
  return vGTree[0];
}

double VanGinnekenTree::GetR()
{
  vGTree[0];
  return vGTree[0].GetR();
}

VanGinnekenTree::~VanGinnekenTree()
{
  delete [] vGTree;
}

//VanGinnekenTree
//BufferPositions
BufferPositions::BufferPositions()
{
  index = 0;
  position = NULL;
  bufferInfo = NULL;
}

BufferPositions::BufferPositions(VanGinnekenTreeNode* pos, BufferInfo* bufInfo, int i)
{
  index = i;
  position = pos;
  bufferInfo = bufInfo;
}

bool BufferPositions::operator > (BufferPositions& element)
{
  if (position->GetIndex() > element.GetPosition()->GetIndex())
    return true;
  return false;
}

bool BufferPositions::operator < (BufferPositions& element)
{
  if (position->GetIndex() < element.GetPosition()->GetIndex())
    return true;
  return false;
}


VanGinnekenTreeNode* BufferPositions::GetPosition()
{
  return position;
}

BufferInfo* BufferPositions::GetBufferInfo()
{
  return bufferInfo;
}

int BufferPositions::GetIndex()
{
  return index;
}

void BufferPositions::SetPosition(VanGinnekenTreeNode* pos)
{
  position = pos;
}

void BufferPositions::SetBufferInfo(BufferInfo* bufInfo)
{
  bufferInfo = bufInfo;
}

void BufferPositions::SetIndex(int i)
{
  index = i;
}


//BufferPositions

//VGVariantsListElement
VGVariantsListElement::VGVariantsListElement()
{
  RAT = 0;
  c = 0;
  positionCount = 0;
  index = 0;
}
bool VGVariantsListElement::operator > (VGVariantsListElement& element)
{
  if ((c < element.c) && (RAT > element.RAT))
    return true;
  else
    return false;
}
bool VGVariantsListElement::operator < (VGVariantsListElement& element)
{
  if ((c < element.c) && (RAT > element.RAT))
    return false;
  else
    return true;
}

VGVariantsListElement& VGVariantsListElement::operator = (VGVariantsListElement& element)
{
  if(&element == this )
    return *this;
  RAT = element.GetRAT();
  c = element.GetC();
  bufferPositions = element.bufferPositions;
  positionCount = element.GetPositionCount();
  index = element.index;
  return *this;
}

void VGVariantsListElement::SortBufferPosition()
{
  bufferPositions.sort();
}

double VGVariantsListElement::GetRAT()
{
  return RAT;
}
double VGVariantsListElement::GetC()
{
  return c;
}
BufferPositions VGVariantsListElement::GetStartBufferPosition()
{
  if (positionCount > 0)
  {
    index = 0;
    return *bufferPositions.begin();
  }
  BufferPositions pos;
  return pos;
}
BufferPositions VGVariantsListElement::GetEndBufferPosition()
{
  if (positionCount > 0)
  {
    index = positionCount - 1;
    return *bufferPositions.end();
  }
  BufferPositions pos;
  return pos;
}
BufferPositions VGVariantsListElement::GetNextBufferPosition()
{
  if ((positionCount > 0) && (index < (positionCount )))
  {
    index++;
    return GetbufferPosition(index);
  }
  BufferPositions pos;
  return pos;
}
BufferPositions VGVariantsListElement::GetbufferPosition(int i)
{
  if ((positionCount > 0) && (i < (positionCount - 1)) && (i >= 0))
  {
    index = i;
    TemplateTypes<BufferPositions>::list::iterator iter = bufferPositions.begin();
    while (iter->GetIndex() != i)
      iter++;
    return *iter;
  }
  BufferPositions pos;
  return pos;
}

int VGVariantsListElement::GetPositionCount()
{
  return positionCount;
}

TemplateTypes<BufferPositions>::list* VGVariantsListElement::GetBufferPosition()
{
  return &bufferPositions;
}

void VGVariantsListElement::SetBufferPosition(BufferPositions position)
{
  bufferPositions.push_back(position);
  positionCount = int (bufferPositions.size());
}
void VGVariantsListElement::SetBufferPosition(TemplateTypes<BufferPositions>::list position)
{
  bufferPositions = position;
  positionCount = int (bufferPositions.size());
}

void VGVariantsListElement::SetRAT(double rat)
{
  RAT = rat;
}
void VGVariantsListElement::SetC(double capacity)
{
  c = capacity;
}