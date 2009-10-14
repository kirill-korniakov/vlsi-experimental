#ifndef __HIGH_NET_H__
#define __HIGH_NET_H__

#include "HBaseDesignItem.h"
#include "HPin.h"

class HNets;
class HWire;

BEGINITEM(HNet, HNetWrapper)

  enum Properties
  {
    Name,
    Kind,
    Source,
    Pins,
    Sinks,
    PinsCount,
    SinksCount,
    LastSink,
    HPWL,
    Weight,
    LNet
  };

  class PinsEnumeratorW;
  typedef PinsEnumeratorW SinksEnumeratorW;

ENDITEM(HNet)

class HNet::PinsEnumeratorW: public HEnumeratorW<HPinWrapper, HPins>
{
protected:
  IDType m_netPinIdx;
public:
  typedef HEnumeratorW<HPinWrapper, HPins> BaseType;
  
  PinsEnumeratorW(HPins *aPins, IDType aStart, IDType aEnd) 
    : BaseType(aPins, aStart, aEnd), m_netPinIdx(aStart - 1)
  {}

  void Reset()
  {
    m_netPinIdx = m_start - 1;
  }

  bool MoveNext()
  {
    if(++m_netPinIdx < m_end)
    {
      ::FromID(*this, ::ToLow(m_Parent)->NetList.netPins[m_netPinIdx]);
      return true;
    }
    return false;
  }

  IDType NetPinID() const { return m_netPinIdx; }
};

BEGINHCOLLECTION(HNets, HNet)

  ///Enumerator
  typedef HConstEnumerator<HNet, 1> NetsEnumerator;
  typedef HEnumeratorW<HNetWrapper, HNets> NetsEnumeratorW;
  class ActiveNetsEnumeratorW;

  NetsEnumerator GetFullEnumerator() const 
    { return NetsEnumerator(m_ld->NetList.nNetsEnd); }
  NetsEnumeratorW GetFullEnumeratorW();
  ActiveNetsEnumeratorW GetNetsEnumeratorW();
  
  int Count() const
    { return m_ld->NetList.nNetsEnd - 1; }
  int Count(NetKind kind) const 
    { return m_ld->NetList.nNetsByKind[kind]; }

  ///Events
  Event<1, int> OnInitialize; //int nNetsLimit
  Event<2, int, int> OnGrow; //int nNetsLimit_before, int nNetsLimit_after
  Event<1, HNet> OnNetAllocation;
  Event<2, HNet, HNet> BeforeSwap; ///HNet2 first, HNet2 second

  ///Operations
  void Initialize(int netsLimit);
  void Grow(int netsLimit);
  void Swap(HNet& first, HNet& second);
  HNet AllocateNet(bool isBase = false);
  void AllocatePins(HNet net, int count);
  void AssignPin(HNet net, HNet::PinsEnumeratorW& netPin, HPin pin);
  void AssignPin(HNet net, IDType netPinID, HPin pin);

  //Getters & Setters
  GETTERS_SETTERS_DEFINITION()

  //properties specializations
  PROPERTYA(string, HNet::Name, m_ld->NetList.netName)
  PROPERTYA(double, HNet::HPWL, m_ld->NetList.netHPWL)
  PROPERTYA(double, HNet::Weight, m_ld->NetList.netWeight)
  PROPERTYA(double, HNet::LNet, m_ld->NetList.netLnet)
  
  //getters specializations
  GETTERA(NetKind, HNet::Kind, m_ld->NetList.netKind)

  GETTER(HPin, HNet::Source)
    { return ::__ConstructPtr<HPin>(m_ld->NetList.netPins[m_ld->NetList.netPinStart[ARGID]]); }

  GETTER(HPin, HNet::LastSink)
    { return ::__ConstructPtr<HPin>(m_ld->NetList.netPins[m_ld->NetList.netPinEnd[ARGID] - 1]); }

  GETTER(int, HNet::PinsCount)
    { return m_ld->NetList.netPinEnd[ARGID]
           - m_ld->NetList.netPinStart[ARGID]; }

  GETTER(int, HNet::SinksCount)
    { return m_ld->NetList.netPinEnd[ARGID]
           - m_ld->NetList.netPinStart[ARGID] - 1; }
  
  GETTER(HNet::PinsEnumeratorW, HNet::Pins);
  GETTER(HNet::SinksEnumeratorW, HNet::Sinks);

  //setters specializations
  SETTER(HPin, HNet::Source)
    { AssignPin(arg, m_ld->NetList.netPinStart[ARGID], value); }

  SETTER(NetKind, HNet::Kind)
  {
#pragma warning( push )
#pragma warning(disable:6385)
    if (m_ld->NetList.netKind[ARGID] != value)
    {
      m_ld->NetList.nNetsByKind[value] += 1;
      m_ld->NetList.nNetsByKind[m_ld->NetList.netKind[ARGID]] -= 1;
      m_ld->NetList.netKind[ARGID] = value;
    }
#pragma warning( pop )
  }

  private: void PinsGrowEventHandler(int pinsFrom, int pinsTo);
ENDHCOLLECTION(HNets)

BEGINWRAPPER(HNetWrapper, HNets)

  //properties
  PROPERTYWD(string, Name)
  PROPERTYWD(double, HPWL)
  PROPERTYWD(double, Weight)
  PROPERTYWD(double, LNet)
  PROPERTYWD(NetKind, Kind)
  PROPERTYWD(HPin, Source)

  //Getters
  GETTERWD(HPin, LastSink)
  GETTERWD(int, PinsCount)
  GETTERWD(int, SinksCount)
  GETTERW(PinsEnumeratorW, Pins, GetPinsEnumeratorW)
  GETTERW(SinksEnumeratorW, Sinks, GetSinksEnumeratorW)

  //Setters
  //SETTERWD(double, Weight)

ENDWRAPPER(HNetWrapper)

class HNets::ActiveNetsEnumeratorW : public HNets::NetsEnumeratorW
{
public:
  ActiveNetsEnumeratorW(HNets* nt, IDType start, IDType end)
    : NetsEnumeratorW(nt, start, end)
  {
  }

  bool MoveNext()
  {
    while(NetsEnumeratorW::MoveNext())
      if(this->Kind() == NetKind_Active)
        return true;
    return false;
  }
};

inline HNets::NetsEnumeratorW HNets::GetFullEnumeratorW()
  { return NetsEnumeratorW(this, 1, m_ld->NetList.nNetsEnd); }

inline HNets::ActiveNetsEnumeratorW HNets::GetNetsEnumeratorW()
  { return ActiveNetsEnumeratorW(this, 1, m_ld->NetList.nNetsEnd); }

#endif //__HIGH_NET_H__