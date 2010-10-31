#include "BufferPositions.h"

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
    if (::ToID(position->GetNet()) >  ::ToID(element.GetPosition()->GetNet()))
        return true;
    else 
        if (::ToID(position->GetNet()) <  ::ToID(element.GetPosition()->GetNet()))
            return false;
        else
        {
            if (position->index > element.GetPosition()->index)
                return true;
            return false;
        }
        return false;
}

bool BufferPositions::operator < (BufferPositions& element)
{
    if (::ToID(position->GetNet()) <  ::ToID(element.GetPosition()->GetNet()))
        return true;
    else 
        if (::ToID(position->GetNet()) >  ::ToID(element.GetPosition()->GetNet()))
            return false;
        else
        {
            if (position->index < element.GetPosition()->index)
                return true;
            return false;
        }
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