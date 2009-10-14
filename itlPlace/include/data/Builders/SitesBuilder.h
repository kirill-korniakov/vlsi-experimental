#ifndef __SITESBUILDER_H__
#define __SITESBUILDER_H__

#include "HDesign.h"

namespace Builders
{
  class SitesBuilder
  {
  private:
    HDesign* m_hd;
    bool m_sites_started;
    bool m_sites_finished;
    bool m_site_started;
    bool m_completed;
    HSiteWrapper m_CurrSite;

  public:
    SitesBuilder(HDesign* design);
    ~SitesBuilder();

    void SitesStart(int size = 0);
    void SitesFinished();

    void SiteStart();
    void SiteFinished();

    HSiteWrapper* operator->();

    void CompleteBuild();
  };
}

#endif //__SITESBUILDER_H__