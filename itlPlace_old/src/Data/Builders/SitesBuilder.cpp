#include "SitesBuilder.h"

namespace Builders
{
  SitesBuilder::SitesBuilder(HDesign* design): m_hd(design), m_CurrSite(&design->Sites)
  {
    m_sites_started = false;
    m_sites_finished = false;
    m_site_started = false;
    m_completed = false;
  }

  SitesBuilder::~SitesBuilder()
  {
    if(!m_completed)
      CompleteBuild();
  }

  void SitesBuilder::SitesStart(int size)
  {
    ASSERT(!m_sites_started);

    m_hd->Sites.Initialize(size + 1);

    m_sites_started = true;
  }
  
  void SitesBuilder::SitesFinished()
  {
    ASSERT(!m_site_started && m_sites_started && !m_sites_finished);

    m_sites_finished = true;
  }

  void SitesBuilder::SiteStart()
  {
    ASSERT(!m_site_started && m_sites_started && !m_sites_finished);

    m_CurrSite = m_hd->Sites.AllocateSite();

    m_site_started = true;
  }

  void SitesBuilder::SiteFinished()
  {
    ASSERT(m_site_started && m_sites_started && !m_sites_finished);

    m_site_started = false;
  }

  HSiteWrapper* SitesBuilder::operator->()
  {
    ASSERT(m_site_started);
    return &m_CurrSite;
  }

  void SitesBuilder::CompleteBuild()
  {
    if(!m_sites_started)
      SitesStart(0);
    if(!m_sites_finished)
      SitesFinished();
    m_hd = 0;
    m_completed = true;
  }
}