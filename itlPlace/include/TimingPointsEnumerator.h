#ifndef __TIMING_POINTS_ENUMERATOR_H__
#define __TIMING_POINTS_ENUMERATOR_H__

#include "HDesign.h"

namespace TOInternal
{
  class TimingPointsEnumerator: public HTimingPointWrapper
  {
  protected:
    HDesign& Design() { return m_Parent->Parent(); }
    TimingPointsEnumerator(HDesign& hd): HTimingPointWrapper(&hd.TimingPoints) {}

  public:
    ::HTimingPointWrapper& operator*() {return *this; }
    ::HTimingPointWrapper* operator->() { return (&**this); }

    HTimingPoint Current() const { return *this; }
    ::HTimingPointWrapper CurrentW() const { return *this; }

    HPin CurrentPin() const { return Pin(); }
    HPinWrapper CurrentPinW() { return Design()[Pin()]; }

    HNet CurrentNet() { return Design().Get<HPin::Net, HNet>(Pin()); }
    HNetWrapper CurrentNetW() { return Design()[CurrentNet()]; }

    HCell CurrentCell() { return Design().Get<HPin::Cell, HCell>(Pin()); }
    HCellWrapper CurrentCellW() { return Design()[CurrentCell()]; }
  };

  class RootEndPointsEnumerator: public TimingPointsEnumerator
  {
  protected:
    ::HTimingPoint GetEndPoint() { return Design().TimingPoints.Null(); }
    RootEndPointsEnumerator(HDesign& hd) : TimingPointsEnumerator(hd) {}
  };

  class FirstInternalEndPointsEnumerator: public TimingPointsEnumerator
  {
  protected:
    ::HTimingPoint GetEndPoint() { return Design().TimingPoints.FirstInternalPoint(); }
    FirstInternalEndPointsEnumerator(HDesign& hd) : TimingPointsEnumerator(hd) {}
  };

  class LastInternalEndPointsEnumerator: public TimingPointsEnumerator
  {
  protected:
    ::HTimingPoint GetEndPoint() { return Design().TimingPoints.LastInternalPoint(); }
    LastInternalEndPointsEnumerator(HDesign& hd) : TimingPointsEnumerator(hd) {}
  };

  template<class EnumeratorBase>
  class RootStartPointsEnumerator: public EnumeratorBase
  {
  protected:
    RootStartPointsEnumerator(HDesign& hd) : EnumeratorBase(hd) {}
    void Reset() { this->operator*() = Design().TimingPoints.TopologicalOrderRoot(); }
  };

  template<class EnumeratorBase>
  class FIStartPointsEnumerator: public EnumeratorBase
  {
  protected:
    FIStartPointsEnumerator(HDesign& hd) : EnumeratorBase(hd) {}
    void Reset() { this->operator*() = Design().TimingPoints.FirstInternalPoint(); }
  };

  template<class EnumeratorBase>
  class BeforeFIStartPointsEnumerator: public EnumeratorBase
  {
  protected:
    BeforeFIStartPointsEnumerator(HDesign& hd) : EnumeratorBase(hd) {}
    void Reset() { this->operator*() = Design().TimingPoints.FirstInternalPoint(); this->GoPrevious(); }
  };

  template<class EnumeratorBase>
  class LIStartPointsEnumerator: public EnumeratorBase
  {
  protected:
    LIStartPointsEnumerator(HDesign& hd) : EnumeratorBase(hd) {}
    void Reset() { this->operator*() = Design().TimingPoints.LastInternalPoint(); }
  };

  template<class EnumeratorBase>
  class AfterLIStartPointsEnumerator: public EnumeratorBase
  {
  protected:
    AfterLIStartPointsEnumerator(HDesign& hd) : EnumeratorBase(hd) {}
    void Reset() { this->operator*() = Design().TimingPoints.LastInternalPoint(); this->GoNext(); }
  };


  template<class BaseEnumeratorType, bool includeEndPoint>
  class ForvardEnumerator;

  template<class BaseEnumeratorType>
  class ForvardEnumerator<BaseEnumeratorType, true> : public BaseEnumeratorType
  {
  public:
    ForvardEnumerator(HDesign& hd) : BaseEnumeratorType(hd) {Reset();}

    bool MoveNext()
    {
      if (*this == GetEndPoint())
        return false;
      this->GoNext();
      return true;
    }
  };

  template<class BaseEnumeratorType>
  class ForvardEnumerator<BaseEnumeratorType, false> : public BaseEnumeratorType
  {
    //typedef typename BaseEnumeratorType BaseEnumeratorType;
  public:
    ForvardEnumerator(HDesign& hd) : BaseEnumeratorType(hd) {Reset();}

    bool MoveNext()
    {
      return this->GoNext() != GetEndPoint();
    }
  };


  template<class BaseEnumeratorType, bool includeEndPoint>
  class BackvardEnumerator;

  template<class BaseEnumeratorType>
  class BackvardEnumerator<BaseEnumeratorType, true> : public BaseEnumeratorType
  {
  public:
    BackvardEnumerator(HDesign& hd) : BaseEnumeratorType(hd) {Reset();}

    bool MoveNext()
    {
      if (*this == GetEndPoint())
        return false;
      this->GoPrevious();
      return true;
    }
  };

  template<class BaseEnumeratorType>
  class BackvardEnumerator<BaseEnumeratorType, false> : public BaseEnumeratorType
  {
  public:
    BackvardEnumerator(HDesign& hd) : BaseEnumeratorType(hd) {Reset();}

    bool MoveNext()
    {
      return this->GoPrevious() != GetEndPoint();
    }
  };

}//namespace TOInternal

namespace TopologicalOrder
{
  typedef TOInternal::ForvardEnumerator<TOInternal::RootStartPointsEnumerator<TOInternal::FirstInternalEndPointsEnumerator>, false>
    TspEnumerator;
  typedef TOInternal::ForvardEnumerator<TOInternal::LIStartPointsEnumerator<TOInternal::RootEndPointsEnumerator>, false>
    TepEnumerator;
  typedef TOInternal::ForvardEnumerator<TOInternal::BeforeFIStartPointsEnumerator<TOInternal::LastInternalEndPointsEnumerator>, true>
    IntEnumerator;
  typedef TOInternal::ForvardEnumerator<TOInternal::RootStartPointsEnumerator<TOInternal::LastInternalEndPointsEnumerator>, true>
    TspIntEnumerator;
  typedef TOInternal::ForvardEnumerator<TOInternal::BeforeFIStartPointsEnumerator<TOInternal::RootEndPointsEnumerator>, false>
    IntTepEnumerator;
  typedef TOInternal::ForvardEnumerator<TOInternal::RootStartPointsEnumerator<TOInternal::RootEndPointsEnumerator>, false>
    AllPointsEnumerator;
}

namespace ReverseTopologicalOrder
{
  typedef TOInternal::BackvardEnumerator<TOInternal::FIStartPointsEnumerator<TOInternal::RootEndPointsEnumerator>, false>
    TspEnumerator;
  typedef TOInternal::BackvardEnumerator<TOInternal::RootStartPointsEnumerator<TOInternal::LastInternalEndPointsEnumerator>, false>
    TepEnumerator;
  typedef TOInternal::BackvardEnumerator<TOInternal::AfterLIStartPointsEnumerator<TOInternal::FirstInternalEndPointsEnumerator>, true>
    IntEnumerator;
  typedef TOInternal::BackvardEnumerator<TOInternal::RootStartPointsEnumerator<TOInternal::FirstInternalEndPointsEnumerator>, true>
    TepIntEnumerator;
  typedef TOInternal::BackvardEnumerator<TOInternal::AfterLIStartPointsEnumerator<TOInternal::RootEndPointsEnumerator>, false>
    IntTspEnumerator;
  typedef TOInternal::BackvardEnumerator<TOInternal::RootStartPointsEnumerator<TOInternal::RootEndPointsEnumerator>, false>
    AllPointsEnumerator;
}

#endif //__TIMING_POINTS_ENUMERATOR_H__ 