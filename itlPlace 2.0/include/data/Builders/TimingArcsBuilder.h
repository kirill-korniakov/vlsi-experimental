#ifndef __TIMINGARCSBUILDER_H__
#define __TIMINGARCSBUILDER_H__

#include "HDesign.h"

namespace Builders
{
  class TimingArcsBuilder
  {
  private:
    HDesign* m_hd;
    bool m_completed;
    bool m_timingarcs_started;
    bool m_timingarcs_finished;
    bool m_macrotype_opened;
    bool m_pintype_opened;
    bool m_arc_started;
    
    HMacroType m_mtype;
    HPinType   m_ptype;

    HTimingArcTypeWrapper m_arc;

  public:
    TimingArcsBuilder(HDesign* design);
    ~TimingArcsBuilder();

    void TimingArcsStart(int size = 0);
    void TimingArcsFinished();

    void OpenMacroType(const string& name);
    void CloseMacroType();

    void OpenPinType(const string& name);
    void ClosePinType();

    void AddNewArc(const string& relatedPinName);
    void ArcFinished();

    HTimingArcTypeWrapper* operator->();

    void CompleteBuild();

    HMacroType MacroType() const
    {
      ERROR_ASSERT(m_macrotype_opened);
      return m_mtype;
    }

    HPinType PinType() const
    {
      ERROR_ASSERT(m_pintype_opened);
      return m_ptype;
    }
  };
}

#endif //__TIMINGARCSBUILDER_H__