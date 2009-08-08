#include "HDesign.h"

COLLECTIONCONSTRUCTOR(HSites)

void HSites::Initialize(int sitesLimit)
{
  CHECKIFINITIALIZED();

  Grow(sitesLimit);
  m_ld->Tech->siteName[0] = "NULL";
  m_ld->Tech->siteWidth[0] = 0;
  m_ld->Tech->siteHeight[0] = 0;

  m_ld->Tech->nSites = 1;
  OnInitialize(sitesLimit);
}

void HSites::Grow(int sitesLimit)
{
  int old_size = m_ld->Tech->nSitesLimit;
  ::Grow(&m_ld->Tech->siteName, old_size, sitesLimit);
  ::Grow(&m_ld->Tech->siteHeight, old_size, sitesLimit);
  ::Grow(&m_ld->Tech->siteWidth, old_size, sitesLimit);
  m_ld->Tech->nSitesLimit = sitesLimit;

  OnGrow(old_size, sitesLimit);
}

HSite HSites::AllocateSite()
{
  while (m_ld->Tech->nSites >= m_ld->Tech->nSitesLimit)
    Grow(GetNewSize(m_hd->cfg, m_ld->Tech->nSitesLimit, "data.sitesLimitGrow", 1, 0, 1.0));

  HSite res = ::__ConstructPtr<HSite>(m_ld->Tech->nSites);
  m_ld->Tech->nSites++;

  OnSiteAllocation(res);

  return res;
}

HSite HSites::operator[](const string& name) const
{
  for(int i = m_ld->Tech->nSites - 1; i >= 0; i--)
    if(m_ld->Tech->siteName[i] == name)
      return ::__ConstructPtr<HSite>(i);
  return Null();
}
