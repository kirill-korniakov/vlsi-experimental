#include "NetsBuilder.h"
#include "Utils.h"

namespace Builders
{
  NetsBuilder::NetsBuilder(HDesign* design)
    : m_hd(design),
    m_CurrNet(&design->Nets),
    m_SEn(&design->Pins, 0, 0)
  {
    m_nets_started = false;
    m_nets_finished = false;
    m_net_started = false;
    m_completed = false;
    m_source_added = false;
    m_sinks_expected = 0;
  }

  NetsBuilder::~NetsBuilder()
  {
    if(!m_completed)
      CompleteBuild();
  }

  void NetsBuilder::NetsStart(int size)
  {
    ASSERT(!m_nets_started);

    m_hd->Nets.Initialize(size + 1);

    m_nets_started = true;
  }
  
  void NetsBuilder::NetsFinished()
  {
    ASSERT(!m_net_started && m_nets_started && !m_nets_finished);

    if (m_hd->CanDoTiming())
      Utils::CalculateLNets(*m_hd);

    m_nets_finished = true;
  }

  void NetsBuilder::NetStart(int pinsCount)
  {
    ASSERT(!m_net_started && m_nets_started && !m_nets_finished);

    m_CurrNet = m_hd->Nets.AllocateNet(true);
    m_hd->Nets.AllocatePins(m_CurrNet, pinsCount);
    m_sinks_expected = pinsCount - 1;

    if(m_sinks_expected > 0)
      m_SEn = m_CurrNet.GetSinksEnumeratorW();

    m_source_added = false;
    m_net_started = true;
  }

  void NetsBuilder::AddSink(HPin pin)
  {
    ASSERT(m_net_started && m_nets_started && !m_nets_finished);
    CRITICAL_ASSERT(m_sinks_expected > 0);

    bool res = m_SEn.MoveNext();
    ASSERT(res);

    m_hd->Nets.AssignPin(m_CurrNet, m_SEn, pin);

    m_sinks_expected--;
  }

  void NetsBuilder::SetSource(HPin pin)
  {
    ASSERT(m_net_started && m_nets_started && !m_nets_finished);
    CRITICAL_ASSERT(m_sinks_expected >= 0);

    m_CurrNet.SetSource(pin);

    m_source_added = true;
  }

  void NetsBuilder::NetFinished()
  {
    ASSERT(m_net_started && m_nets_started && !m_nets_finished);
    ASSERT(m_sinks_expected == 0);

    ASSERT(m_source_added || !::IsNull(m_CurrNet.Source()));

    m_net_started = false;
  }

  HNetWrapper* NetsBuilder::operator->()
  {
    ASSERT(m_net_started);
    return &m_CurrNet;
  }

  void NetsBuilder::CompleteBuild()
  {
    if(!m_nets_started)
      NetsStart(0);
    if(!m_nets_finished)
      NetsFinished();
    m_hd = 0;
    m_completed = true;
  }
}