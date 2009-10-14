#ifndef __HIGH_SITE_H__
#define __HIGH_SITE_H__

#include "HBaseDesignItem.h"

BEGINITEM(HSite, HSiteWrapper)

  enum Properties
  {
    Name,
    Width,
    Height
  };
ENDITEM(HSite)

BEGINHCOLLECTION(HSites, HSite)

  int Count() const {return m_ld->Tech->nSites - 1;}
  HSite operator[](const string& name) const;

  ///Enumerator
  typedef HConstEnumerator<HSite, 1> Enumerator;
  typedef HEnumeratorW<HSiteWrapper, HSites> EnumeratorW;
  Enumerator GetEnumerator() const {return Enumerator(m_ld->Tech->nSites);}
  EnumeratorW GetEnumeratorW();

  ///Events
  Event<2, int, int> OnGrow; ///int nSitesLimit_before, int nSitesLimit_after
  Event<1, int> OnInitialize; ///int nSitesLimit
  Event<1, HSite> OnSiteAllocation;
  
  ///Operations
  void Initialize(int sitesLimit);
  void Grow(int sitesLimit);
  HSite AllocateSite();

  //Getters & Setters
  GETTERS_SETTERS_DEFINITION()

  //properties specializations
  PROPERTYA(string, HSite::Name, m_ld->Tech->siteName)
  PROPERTYA(double, HSite::Width, m_ld->Tech->siteWidth)
  PROPERTYA(double, HSite::Height, m_ld->Tech->siteHeight)

ENDHCOLLECTION(HSites)

BEGINWRAPPER(HSiteWrapper, HSites)

  //properties specializations
  PROPERTYWD(string, Name)
  PROPERTYWD(double, Width)
  PROPERTYWD(double, Height)

ENDWRAPPER(HSiteWrapper)

inline HSites::EnumeratorW HSites::GetEnumeratorW()
  { return EnumeratorW(this, 1, m_ld->Tech->nSites); }

#endif //__HIGH_SITE_H__