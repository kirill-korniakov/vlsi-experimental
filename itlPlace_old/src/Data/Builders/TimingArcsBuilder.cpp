#include "TimingArcsBuilder.h"
#include "Utils.h"

namespace Builders
{
  TimingArcsBuilder::TimingArcsBuilder(HDesign* design)
    : m_hd(design),
    m_mtype(),
    m_ptype(),
    m_arc(&design->TimingArcTypes)
  {
    m_timingarcs_started = false;
    m_timingarcs_finished = false;
    m_completed = false;
    m_macrotype_opened = false;
    m_pintype_opened = false;
    m_arc_started = false;
  }

  TimingArcsBuilder::~TimingArcsBuilder()
  {
    if(!m_completed)
      CompleteBuild();
  }

  void TimingArcsBuilder::TimingArcsStart(int size)
  {
    ASSERT(!m_timingarcs_started);

    m_hd->TimingArcTypes.Initialize(size + 1);

    m_timingarcs_started = true;
  }
  
  void TimingArcsBuilder::TimingArcsFinished()
  {
    ASSERT(!m_arc_started
      && !m_pintype_opened
      && !m_macrotype_opened
      && m_timingarcs_started
      && !m_timingarcs_finished);

    m_timingarcs_finished = true;
  }

  HTimingArcTypeWrapper* TimingArcsBuilder::operator->()
  {
    ASSERT(m_arc_started);
    return &m_arc;
  }

  void TimingArcsBuilder::CompleteBuild()
  {
    if(!m_timingarcs_started)
      TimingArcsStart(0);
    if(!m_timingarcs_finished)
      TimingArcsFinished();
    m_hd = 0;
    m_completed = true;
  }


  void TimingArcsBuilder::OpenMacroType(const string& name)
  {
    ASSERT(!m_macrotype_opened && m_timingarcs_started);

    m_mtype = Utils::FindMacroTypeByName(*m_hd, name);
    ASSERT(!::IsNull(m_mtype));

    m_macrotype_opened = true;
  }
  
  void TimingArcsBuilder::CloseMacroType()
  {
    ASSERT(m_macrotype_opened && !m_pintype_opened);
    
    m_mtype = m_hd->MacroTypes.Null();
    
    m_macrotype_opened = false;
  }

  void TimingArcsBuilder::OpenPinType(const string& name)
  {
    ASSERT(m_macrotype_opened && !m_pintype_opened);

    m_ptype = Utils::FindMacroTypePinByName(*m_hd, m_mtype, name);
    ASSERT(!::IsNull(m_ptype));

    m_pintype_opened = true;
  }

  void TimingArcsBuilder::ClosePinType()
  {
    ASSERT(m_pintype_opened && !m_arc_started);
    
    m_ptype = m_hd->PinTypes.Null();
    
    m_pintype_opened = false;
  }

  void TimingArcsBuilder::AddNewArc(const string& relatedPinName)
  {
    ASSERT(m_pintype_opened && !m_arc_started);

    HPinType start_pin = Utils::FindMacroTypePinByName(*m_hd, m_mtype, relatedPinName);
    ASSERT(!::IsNull(start_pin));

    m_arc = m_hd->TimingArcTypes.AddNextArcToPinType(start_pin, m_ptype);
    ASSERT(!::IsNull(m_arc));

    m_arc_started = true;
  }
  
  void TimingArcsBuilder::ArcFinished()
  {
    ASSERT(m_arc_started);

    m_arc = m_hd->TimingArcTypes.Null();

    m_arc_started = false;
  }
}