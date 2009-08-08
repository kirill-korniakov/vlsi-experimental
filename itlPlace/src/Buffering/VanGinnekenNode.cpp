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
  m_Capacitance = capacitance;
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
  if ((isRightNodeExist == true) || (m_nLeftSteps != 0))
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