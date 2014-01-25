#ifndef __EVENT_H__
#define __EVENT_H__

#include "stdTypes.h"
#include "FastDelegate.h"

template<int nArgs, class T1 = int, class T2 = int, class T3 = int, class T4 = int>
class Event;

template<>
class Event<0>
{
public:
  typedef fastdelegate::FastDelegate0<> EventDelegate;
  typedef Event<0> SelfType;

private:
  typedef TemplateTypes<EventDelegate>::list ListenersList;
  ListenersList _lst;

public:

  SelfType& operator+= (EventDelegate aDelegate)
  {
    _lst.push_back(aDelegate);
    return *this;
  }

  SelfType& operator-= (EventDelegate aDelegate)
  {
    _lst.remove(aDelegate);
    return *this;
  }

  void operator() () const
  {
    for(ListenersList::const_iterator i = _lst.begin(); i != _lst.end(); ++i)
      (*i)();
  }
};

template<class T1>
class Event<1, T1>
{
public:
  typedef fastdelegate::FastDelegate1<T1> EventDelegate;
  typedef Event<1, T1> SelfType;

private:
  typedef typename TemplateTypes<EventDelegate>::list ListenersList;
  ListenersList _lst;

public:

  SelfType& operator+= (EventDelegate aDelegate)
  {
    _lst.push_back(aDelegate);
    return *this;
  }

  SelfType& operator-= (EventDelegate aDelegate)
  {
    _lst.remove(aDelegate);
    return *this;
  }

  void operator() (const T1& arg1) const
  {
    for(typename ListenersList::const_iterator i = _lst.begin(); i != _lst.end(); ++i)
      (*i)(arg1);
  }
};

template<class T1, class T2>
class Event<2, T1, T2>
{
public:
  typedef fastdelegate::FastDelegate2<T1, T2> EventDelegate;
  typedef Event<2, T1, T2> SelfType;

private:
  typedef typename TemplateTypes<EventDelegate>::list ListenersList;
  ListenersList _lst;

public:

  SelfType& operator+= (EventDelegate aDelegate)
  {
    _lst.push_back(aDelegate);
    return *this;
  }

  SelfType& operator-= (EventDelegate aDelegate)
  {
    _lst.remove(aDelegate);
    return *this;
  }

  void operator() (const T1& arg1, const T2& arg2) const
  {
    for(typename ListenersList::const_iterator i = _lst.begin(); i != _lst.end(); ++i)
      (*i)(arg1, arg2);
  }
};

template<class T1, class T2, class T3>
class Event<3, T1, T2, T3>
{
public:
  typedef fastdelegate::FastDelegate3<T1, T2, T3> EventDelegate;
  typedef Event<3, T1, T2, T3> SelfType;

private:
  typedef typename TemplateTypes<EventDelegate>::list ListenersList;
  ListenersList _lst;

public:

  SelfType& operator+= (EventDelegate aDelegate)
  {
    _lst.push_back(aDelegate);
    return *this;
  }

  SelfType& operator-= (EventDelegate aDelegate)
  {
    _lst.remove(aDelegate);
    return *this;
  }

  void operator() (const T1& arg1, const T2& arg2, const T3& arg3) const
  {
    for(typename ListenersList::const_iterator i = _lst.begin(); i != _lst.end(); ++i)
      (*i)(arg1, arg2, arg3);
  }
};

template<class T1, class T2, class T3, class T4>
class Event<4, T1, T2, T3, T4>
{
public:
  typedef fastdelegate::FastDelegate4<T1, T2, T3, T4> EventDelegate;
  typedef Event<4, T1, T2, T3, T4> SelfType;

private:
  typedef typename TemplateTypes<EventDelegate>::list ListenersList;
  ListenersList _lst;

public:

  SelfType& operator+= (EventDelegate aDelegate)
  {
    _lst.push_back(aDelegate);
    return *this;
  }

  SelfType& operator-= (EventDelegate aDelegate)
  {
    _lst.remove(aDelegate);
    return *this;
  }

  void operator() (const T1& arg1, const T2& arg2, const T3& arg3, const T4& arg4) const
  {
    for(typename ListenersList::const_iterator i = _lst.begin(); i != _lst.end(); ++i)
      (*i)(arg1, arg2, arg3, arg4);
  }
};


#endif //__EVENT_H__