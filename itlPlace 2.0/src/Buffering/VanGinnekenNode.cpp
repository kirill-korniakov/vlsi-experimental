#include "VanGinnekenNode.h"
#include <math.h>

VGNode::VGNode(HSteinerPoint start, HSteinerPoint lEnd, HSteinerPoint rEnd, double capacitance, 
               double requiredArrivalTime, int steps, int step_num, int typePoint, 
               HDesign& hd, int act, int index):      
m_SubtreeRoot(start), m_LeftEnd(lEnd),m_RightEnd(rEnd), m_hd(hd)
{
  left = NULL;
  right = NULL;
  m_steps = steps;

  HPinWrapper _start = m_hd[m_hd[start].Pin()];
  HPinWrapper _lEnd = m_hd[m_hd[lEnd].Pin()];
  HPinWrapper _rEnd = m_hd[m_hd[rEnd].Pin()];

  m_xs = _start.X();
  m_xle = _lEnd.X();
  m_xre = _rEnd.X();
  m_ys = _start.Y();
  m_yle = _lEnd.Y();
  m_yre = _rEnd.Y();

  m_Capacitance = capacitance;
  m_RAT = requiredArrivalTime;
  m_typePoint = typePoint;
  m_nLeftSteps = step_num; 
  m_nRightSteps = step_num;

  m_RelativePosition = 2;
  isTransitionL = false;
  isTransitionR = false;
  m_indexStart = index;
  m_leftCount = 0;
}

VGNode::~VGNode()
{
  Destroy();
}

void VGNode::UpdatingTree1()
{ 
  int step = 2;

  m_SubtreeRoot = nullSP;
  m_LeftEnd = nullSP;
  m_RightEnd = nullSP;
  m_steps = step;
  m_nLeftSteps = 0; 
  m_nRightSteps = 0;
  m_xs = 2615590.088;
  m_xle = 5220000.000;
  m_xre = 0;
  m_ys = 2958229.004;
  m_yle = 5560000.000;
  m_yre = 0;
  m_Capacitance = 0.5;
  m_RAT = 0;
  m_typePoint = 0;
  m_RelativePosition = 2;
  m_indexStart = 0;

  left = new VGNode(nullSP, nullSP, nullSP,0 , 0, 0, 0, 0, m_hd, 0, 0);
  right = NULL;
  m_leftCount = (step + 1) * 4;
  isRightNodeExist = false;

  left->m_SubtreeRoot = nullSP;
  left->m_LeftEnd = nullSP;
  left->m_RightEnd = nullSP;
  left->m_steps = step;
  left->m_nLeftSteps = 0; 
  left->m_nRightSteps = 0;
  left->m_xs = 5220000.000;
  left->m_xle = 3380000.000;
  left->m_xre = 4880000.000;
  left->m_ys = 5560000.000;
  left->m_yle = 270000.000;
  left->m_yre = 5560000.000;
  left->m_Capacitance = 0.5;
  left->m_RAT = 0;
  left->m_typePoint = 1;
  left->m_RelativePosition = 0;
  left->m_indexStart = (step + 1) * 1;;
  left->left = new VGNode(nullSP, nullSP, nullSP,0 , 0, 0, 0, 0, m_hd, 0, 0);
  left->m_leftCount = (step + 1) * 2;;
  left->isRightNodeExist = true;
  

  left->left->m_SubtreeRoot = nullSP;
  left->left->m_LeftEnd = nullSP;
  left->left->m_RightEnd = nullSP;
  left->left->m_steps = step;
  left->left->m_nLeftSteps = 0; 
  left->left->m_nRightSteps = 0;
  left->left->m_xs = 3380000.000;
  left->left->m_xle = 0;
  left->left->m_xre = 0;
  left->left->m_ys = 270000.000;
  left->left->m_yle = 0;
  left->left->m_yre = 0;
  left->left->m_Capacitance = 0.5;
  left->left->m_RAT = 0;
  left->left->m_typePoint = 2;
  left->left->m_RelativePosition = 0;
  left->left->m_indexStart = (step + 1) * 2;
  left->left->left = NULL;
  left->left->right = NULL;
  left->left->isRightNodeExist = false;
  

  left->right = new VGNode(nullSP, nullSP, nullSP,0 , 0, 0, 0, 0, m_hd, 0, 0);
  left->right->m_SubtreeRoot = nullSP;
  left->right->m_LeftEnd = nullSP;
  left->right->m_RightEnd = nullSP;
  left->right->m_steps = step;
  left->right->m_nLeftSteps = 0; 
  left->right->m_nRightSteps = 0;
  left->right->m_xs = 4880000.000;
  left->right->m_xle = 3020000.000;
  left->right->m_xre = 0;
  left->right->m_ys = 5560000.000;
  left->right->m_yle = 270000.000;
  left->right->m_yre = 0;
  left->right->m_Capacitance = 0.5;
  left->right->m_RAT = 0;
  left->right->m_typePoint = 1;
  left->right->m_RelativePosition = 1;
  left->right->m_indexStart = (step + 1) * 3;
  left->right->left = new VGNode(nullSP, nullSP, nullSP,0 , 0, 0, 0, 0, m_hd, 0, 0);
  left->right->right = NULL;
  left->right->m_leftCount = (step + 1) * 4;
  left->right->isRightNodeExist = false;
  

  left->right->left->m_SubtreeRoot = nullSP;
  left->right->left->m_LeftEnd = nullSP;
  left->right->left->m_RightEnd = nullSP;
  left->right->left->m_steps = step;
  left->right->left->m_nLeftSteps = 0; 
  left->right->left->m_nRightSteps = 0;
  left->right->left->m_xs = 3020000.000;
  left->right->left->m_xle = 0;
  left->right->left->m_xre = 0;
  left->right->left->m_ys = 270000.000;
  left->right->left->m_yle = 0;
  left->right->left->m_yre = 0;
  left->right->left->m_Capacitance = 0.5;
  left->right->left->m_RAT = 0;
  left->right->left->m_typePoint = 2;
  left->right->left->m_RelativePosition = 0;
  left->right->left->m_indexStart = (step + 1) * 4;;
  left->right->left->left = NULL;
  left->right->left->right = NULL;
  left->right->left->isRightNodeExist = false;

}

int VGNode::InitializeTree(HSteinerPoint start, double capacitance, double requiredArrivalTime, 
                           int steps, int typePoint, int act, int index)
{
  m_SubtreeRoot = start;
  m_LeftEnd = m_hd.SteinerPoints.Get<HSteinerPoint::Left, HSteinerPoint>( start);
  m_RightEnd = m_hd.SteinerPoints.Get<HSteinerPoint::Right, HSteinerPoint>( start);
  m_steps = steps;
  m_nLeftSteps = 0; 
  m_nRightSteps = 0;
  m_xs = m_hd.SteinerPoints.GetDouble<HSteinerPoint::X>(m_SubtreeRoot);
  m_xle = m_hd.SteinerPoints.GetDouble<HSteinerPoint::X>(m_LeftEnd);
  m_xre = m_hd.SteinerPoints.GetDouble<HSteinerPoint::X>(m_RightEnd);
  m_ys = m_hd.SteinerPoints.GetDouble<HSteinerPoint::Y>(m_SubtreeRoot);
  m_yle = m_hd.SteinerPoints.GetDouble<HSteinerPoint::Y>(m_LeftEnd);
  m_yre = m_hd.SteinerPoints.GetDouble<HSteinerPoint::Y>(m_RightEnd);
  m_Capacitance = 0.0;//capacitance;
  m_RAT = requiredArrivalTime;
  m_typePoint = typePoint;
  m_RelativePosition = act;
  m_indexStart = index;
  left = NULL;
  right = NULL;
  isRightNodeExist = false;
  int nLeft = 0, nRight = 0;//NOTE:номер последнего в левом(правом) поддереве 
  int res = index;
  bool isLeft = false;
  bool isInter = m_hd.SteinerPoints.GetBool<HSteinerPoint::IsInternal>(m_SubtreeRoot);

  if (m_LeftEnd != nullSP )
  {
    left = new VGNode(nullSP, nullSP, nullSP,0 , 0, 0, 0, 0, m_hd, 0, 0);
    nLeft = left->InitializeTree(m_LeftEnd, capacitance, 0, steps, 1, 2, index + steps + 1);
    if (nLeft < 0)
      return nLeft;
    res = nLeft;
    m_leftCount = nLeft;
    isLeft = true;
  }
  else
  {
    m_xle = 0;
    m_yle = 0;
  }

  if (m_RightEnd != nullSP )
  {
    if (isLeft)
    {
      right = new VGNode(nullSP, nullSP, nullSP,0 , 0, 0, 0, 0, m_hd, 0, 0);
      isRightNodeExist = true;
      nRight = right->InitializeTree(m_RightEnd, capacitance, 0, steps, 1, 2, nLeft +  steps + 1);
      if (nRight < 0)
        return nRight;
      res = nRight;
    }
    else
    {
      WARNING_ASSERT(isLeft != 0);
      return -1;
    }
  }
  else
  {
    m_xre = 0;
    m_yre = 0;
  }

  //NOTE:если нет ни правого ни левого значит синк
  if ((left == NULL) && (right == NULL)) 
  {
    WARNING_ASSERT(m_typePoint != 0);
    if (!m_hd.SteinerPoints.GetBool<HSteinerPoint::IsInternal>(m_SubtreeRoot))
    {
      HTimingPointWrapper timPointForSubtreeRoot = 
        m_hd[m_hd.TimingPoints[m_hd.SteinerPoints.Get<HSteinerPoint::Pin, HPin>(m_SubtreeRoot)]];
      m_RAT = timPointForSubtreeRoot.RequiredTime() * FBI_TIME_SCALING;
      m_Capacitance = m_hd.GetDouble<HPinType::Capacitance>(m_hd.Get<HPin::Type, HPinType>(m_hd.Get<HSteinerPoint::Pin, HPin>(m_SubtreeRoot)));
    }

    //NOTE:если изначально определен как корень то ошибка и выход
    if (m_typePoint == 0) 
    {

      return -1;//NOTE:дописать обработку падения
    }
    else
    {
      m_typePoint = 2;
      m_steps = 0;
    }
  }
  return res;
}

double VGNode::X(int isNew) const 
{ 
  //NOTE: находимся в начале
  if ((m_nLeftSteps == 0) && (m_nRightSteps == 0)) 
  {
    return m_xs; 
  }
  else
    //NOTE:если левый 
    if((m_nLeftSteps != 0) && (m_nRightSteps == 0))
    {
      //NOTE:старый и не перешли
      if ((isTransitionL == false) && (isNew == 0)) 
        return m_xs + (m_xle - m_xs) * (m_nLeftSteps - 1) / (m_steps + 1);
      else
        //NOTE:старый и перешли
        if ((isTransitionL == true) && (isNew == 0))
          return m_xs + (m_xle - m_xs) * (m_nLeftSteps) / (m_steps + 1);
        else
          //NOTE:новый
          if (isNew == 1) 
            return m_xs + (m_xle - m_xs) * (m_nLeftSteps) / (m_steps + 1);
          else 
            return m_xs;

    }
    else
      //NOTE:если правый 
      if((m_nLeftSteps == 0) && (m_nRightSteps != 0))
      {
        //NOTE:старый и не перешли
        if ((isTransitionR == false) && (isNew == 0)) 
          return m_xs + (m_xre - m_xs) * (m_nRightSteps - 1) / (m_steps + 1);
        else
          //NOTE:старый и перешли
          if ((isTransitionR == true) && (isNew == 0))
            return m_xs + (m_xre - m_xs) * (m_nRightSteps) / (m_steps + 1);
          else
            //NOTE:новый
            if (isNew == 1) 
              return m_xs + (m_xre - m_xs) * (m_nRightSteps) / (m_steps + 1);
            else 
              return m_xs;
      }
      //NOTE:переходили и в право и в лево
      else
      {
        //NOTE:левый
        if(m_RelativePosition == 0)
        {
          //NOTE:старый и не перешли
          if ((isTransitionL == false) && (isNew == 0)) 
            return m_xs + (m_xle - m_xs) * (m_nLeftSteps - 1) / (m_steps + 1);
          else
            //NOTE:старый и перешли
            if ((isTransitionL == true) && (isNew == 0))
              return m_xs + (m_xle - m_xs) * (m_nLeftSteps) / (m_steps + 1);
            else
              //NOTE:новый
              if (isNew == 1) 
                return m_xs + (m_xle - m_xs) * (m_nLeftSteps) / (m_steps + 1);
              else 
                return m_xs;
        }
        //NOTE:правый
        else
        {
          //NOTE:старый и не перешли
          if ((isTransitionR == false) && (isNew == 0)) 
            return m_xs + (m_xre - m_xs) * (m_nRightSteps - 1) / (m_steps + 1);
          else
            //NOTE:старый и перешли
            if ((isTransitionR == true) && (isNew == 0))
              return m_xs + (m_xre - m_xs) * (m_nRightSteps) / (m_steps + 1);
            else
              //NOTE:новый
              if (isNew == 1) 
                return m_xs + (m_xre - m_xs) * (m_nRightSteps) / (m_steps + 1);
              else 
                return m_xs;
        }
      }
} 

//NOTE:0-старый, 1-новый
double VGNode::Y(int isNew) const  
{ 
  //NOTE: находимся в начале
  if ((m_nLeftSteps == 0) && (m_nRightSteps == 0))
  {
    //NOTE:if ((isTransition == true) && ())
    return m_ys;  
  }
  else
    //NOTE:если левый
    if((m_nLeftSteps != 0) && (m_nRightSteps == 0)) 
    {
      //NOTE:старый и не перешли
      if ((isTransitionL == false) && (isNew == 0)) 
        return m_ys + (m_yle - m_ys) * (m_nLeftSteps - 1) / (m_steps + 1);
      else
        //NOTE:старый и перешли
        if ((isTransitionL == true) && (isNew == 0))
          return m_ys + (m_yle - m_ys) * (m_nLeftSteps) / (m_steps + 1);
        else
          //NOTE:новый
          if (isNew == 1) 
            return m_ys + (m_yle - m_ys) * (m_nLeftSteps) / (m_steps + 1);
          else 
            return m_ys;
    }
    else
      //NOTE:если правый
      if((m_nLeftSteps == 0) && (m_nRightSteps != 0)) 
      {
        //NOTE:старый и не перешли
        if ((isTransitionR == false) && (isNew == 0)) 
          return m_ys + (m_yre - m_ys) * (m_nRightSteps - 1) / (m_steps + 1);
        else
          //NOTE:старый и перешли
          if ((isTransitionR == true) && (isNew == 0))
            return m_ys + (m_yre - m_ys) * (m_nRightSteps) / (m_steps + 1);
          else
            //NOTE:новый
            if (isNew == 1) 
              return m_ys + (m_yre - m_ys) * (m_nRightSteps) / (m_steps + 1);
            else 
              return m_ys;
      }
      //NOTE:переходили и в право и в лево
      else
      {
        //NOTE:левый
        if(m_RelativePosition == 0)
        {
          //NOTE:старый и не перешли
          if ((isTransitionL == false) && (isNew == 0)) 
            return m_ys + (m_yle - m_ys) * (m_nLeftSteps - 1) / (m_steps + 1);
          else
            //NOTE:старый и перешли
            if ((isTransitionL == true) && (isNew == 0))
              return m_ys + (m_yle - m_ys) * (m_nLeftSteps) / (m_steps + 1);
            else
              //NOTE:новый
              if (isNew == 1) 
                return m_ys + (m_yle - m_ys) * (m_nLeftSteps) / (m_steps + 1);
              else 
                return m_ys;
        }
        //NOTE:правый
        else
        {
          //NOTE:старый и не перешли
          if ((isTransitionR == false) && (isNew == 0)) 
            return m_ys + (m_yre - m_ys) * (m_nRightSteps - 1) / (m_steps + 1);
          else
            //NOTE:старый и перешли
            if ((isTransitionR == true) && (isNew == 0))
              return m_ys + (m_yre - m_ys) * (m_nRightSteps) / (m_steps + 1);
            else
              //NOTE:новый
              if (isNew == 1) 
                return m_ys + (m_yre - m_ys) * (m_nRightSteps) / (m_steps + 1);
              else 
                return m_ys;
        }
      }
} 

double  VGNode::Cap() const 
{
  return m_Capacitance; 
} 

double  VGNode::Time() const 
{
  return m_RAT; 
} 

bool VGNode::IsSource() const 
{
  if ((m_nLeftSteps != 0) || (m_nRightSteps != 0))
    return false;
  return (m_typePoint == 0);
}

bool VGNode::IsInternal() const 
{
  if ((m_nLeftSteps != 0) || (m_nRightSteps != 0))
    return true;
  return (m_typePoint == 1);
}

bool VGNode::IsSink() const 
{
  if ((m_nLeftSteps != 0) || (m_nRightSteps != 0))
    return false;
  return (m_typePoint == 2);
}

VGNode* VGNode::LeftStep() 
{ 
  if (m_nRightSteps != 0)
  {
    m_RelativePosition = 0;
    if (m_nLeftSteps < (m_steps - 1)) 
    {
      m_nLeftSteps++;
      return this;
    } 
    else
    {
      m_RelativePosition = 1;
      isTransitionR = true;
      return right;
    }
  }
  m_RelativePosition = 0;
  if (m_nLeftSteps < m_steps)
  {
    m_nLeftSteps++;
    return this;
  } 
  else
  {
    isTransitionL = true;
    return left;
  }
}

VGNode* VGNode::GetLeft() 
{ 
  return left;
}

VGNode* VGNode::RightStep() 
{ 
  if (m_nRightSteps < 1)
    if ((isRightNodeExist == true) || (m_nLeftSteps == 0))
    {
      m_RelativePosition = 1;
      if ((m_nRightSteps < m_steps) && (right != 0))
      {
        m_nRightSteps++;
        return this;
      } 
      else
      {
        isTransitionR = true;
        return right;
      }
    }
    else
      return NULL;
  else
  {
    return LeftStep();
  }
}

VGNode* VGNode::GetRight() 
{ 
  return right;
}

void VGNode::BackStep()
{
  if ((m_RelativePosition == 0) && (m_nLeftSteps != 0))
  {
    if (isTransitionL == true)
      isTransitionL = false;
    else
      m_nLeftSteps--;
  }
  else
    if ((m_nRightSteps != 0) && (m_RelativePosition == 0))
      m_nRightSteps--;
  if (m_RelativePosition == 1)
    if (isTransitionR == true)
    {
      isTransitionR = false;
      m_RelativePosition = 0;
    }
    else
      m_nRightSteps--;
  if ((m_nLeftSteps == 0) && (m_nRightSteps == 0))
    m_RelativePosition = 2;
}

int VGNode::Index() const 
{
  if (m_RelativePosition == 0) 
  {
    if (m_nRightSteps == 0)
      return m_indexStart + m_nLeftSteps;
    else
      if (m_nRightSteps !=0)
        return m_leftCount + m_nLeftSteps + m_nRightSteps;
  }
  else
  {
    if (m_RelativePosition == 1)
      return m_leftCount + m_nRightSteps;
    else  
      return m_indexStart + m_nLeftSteps + m_nRightSteps;
  }
  return m_indexStart;
}

VGNode& VGNode::GetSteinerPoint(int index, VGNode& node, VGItem& result, bool doIndexesClear)
{
  if (doIndexesClear)
    IndexesClear();

  if (index < node.Index())
  {
    ALERTFORMAT(("ERROR buffering: buffer index is inadmissible "));
    index = node.Index();
    return node;
  }

  if (node.Index() == index)
  {
    result.x = node.X(1);
    result.y = node.Y(1);
    if (index == node.m_indexStart)
    {
      result.steinerPoint = node.m_SubtreeRoot;
    }
    else
    {
      result.steinerPoint = nullSP;
    }
    return node;
  }


  if (index <= node.m_leftCount)
  {
    return GetSteinerPoint(index, *node.LeftStep(), result); 
    node.BackStep();
  }
  else
  {
    return GetSteinerPoint(index, *node.RightStep(), result);
    node.BackStep();
  }
}

void VGNode::IndexesClear()
{
  m_nLeftSteps = 0;
  m_nRightSteps = 0;
  isTransitionL = false;
  isTransitionR = false;
  m_RelativePosition = 2;
  if (GetLeft() != NULL)  
    GetLeft()->IndexesClear();
  if (GetRight() != NULL)
    GetRight()->IndexesClear();
}

bool VGNode::IsRightNodeExist() 
{ 
  if ((m_nLeftSteps != 0) || (m_nRightSteps != 0))
    return false;

  return isRightNodeExist; 
}

bool VGNode::IsLeftNodeExist()
{
  if (LeftStep() != NULL)
  {
    BackStep();
    return true;
  }
  else
  {
    BackStep();
    return false;
  }
}

void VGNode::Destroy()
{
  if (GetLeft() != NULL) 
    GetLeft()->Destroy();
  if (GetRight() != NULL)
    GetRight()->Destroy();
  delete left;
  left = NULL;
  delete right;
  right = NULL;
}

void VGNode::NULLPointer()
{
  left = NULL;
  right = NULL;
}  

bool VGNode::IsReal()
{
  if ((m_nLeftSteps == 0) && (m_nRightSteps == 0))
    return true;
  else
    return false;
}

double VGNode::LengthTree(bool doIndexesClear)
{
  if (doIndexesClear)
    IndexesClear();
  double lengthTree = 0;
  if (left != NULL)
  {
    lengthTree += left->LengthTree();
    lengthTree += (fabs(m_xs - m_xle)  + fabs(m_ys - m_yle));
  }
  if (right != NULL)
  {
    lengthTree += right->LengthTree();
    lengthTree += (fabs(m_xs - m_xre)  + fabs(m_ys - m_yre));
  }

  return lengthTree;
}