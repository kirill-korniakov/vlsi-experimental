#ifndef __NETSBUILDER_H__
#define __NETSBUILDER_H__

#include "HDesign.h"

namespace Builders
{
  class NetsBuilder
  {
  private:
    HDesign* m_hd;
    bool m_nets_started;
    bool m_nets_finished;
    bool m_net_started;
    bool m_source_added;
    int m_sinks_expected;
    bool m_completed;
    HNetWrapper m_CurrNet;
    HNet::SinksEnumeratorW m_SEn;

  public:
    NetsBuilder(HDesign* design);
    ~NetsBuilder();

    void NetsStart(int primariesCount = 0);
    void NetsFinished();

    void NetStart(int pinsCount);

    void AddSink(HPin pin);
    void SetSource(HPin pin);

    void NetFinished();

    HNetWrapper* operator->();

    void CompleteBuild();

    bool IsSourceAdded() const 
    { 
      WARNING_ASSERT(m_net_started);
      return m_source_added; 
    }

    bool CanAddSink() const
    {
      return m_net_started && m_sinks_expected > 0;
    }
  };
}

#endif //__NETSBUILDER_H__