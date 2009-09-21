#ifndef __NETINFO_H__
#define __NETINFO_H__

#include "HDesign.h"
#include "BufferInfo.h"

class NetInfo
{
private:
  NetInfo() 
  {
    m_Rd = m_Cs = m_HPWL = m_WL = m_Lnet = m_Dnet = m_Lext = m_X2opt = m_Xmin = 0.0;
    m_Xmax = m_Kopt = m_KoptNaked = m_MaxRealDelay = m_SumRealDelay = m_OttenDelay = 0.0;
    m_OptimalDelay = m_KoptDelay = 0.0;
    m_KoptInt = m_Fanout = 0;
    m_IsOttenApplicable = m_IsOttenApplicableExact = m_IsModifiedOttenApplicable = false;
  }

  double m_Rd;
  double m_Cs;//sum of sinks capacitances
  double m_HPWL;
  double m_WL;
  double m_Lnet;
  double m_Dnet;
  double m_Lext;
  double m_X2opt;
  double m_Xmin;
  double m_Xmax;
  double m_Kopt;
  double m_KoptNaked;//formula applied without any checks
  int m_KoptInt;
  bool m_IsOttenApplicable;
  bool m_IsModifiedOttenApplicable;
  bool m_IsOttenApplicableExact;

  double m_MaxRealDelay; //from timing
  double m_SumRealDelay; //from timing
  double m_OttenDelay;  //with 0 buffers
  double m_OptimalDelay; //with m_Kopt buffers
  double m_KoptDelay; //with m_KoptInt buffers
  int m_Fanout;
  string m_Name;
  //double m_Slack;

  static void SetNetInfoComputedFields(BufferInfo& buf, NetInfo& n, double r, double c);
public:
  static NetInfo Create(HDesign& hd, HNet net, BufferInfo& buf);

  double Rd() const {return m_Rd;}
  double Cs() const {return m_Cs;}
  double HPWL() const {return m_HPWL;}
  double WL() const {return m_WL;}
  double L() const {return m_HPWL;}
  double Lnet() const {return m_Lnet;}
  double Dnet() const {return m_Dnet;}
  double Lext() const {return m_Lext;}
  double X2opt() const {return m_X2opt;}
  double Xmin() const {return m_Xmin;}
  double Xmax() const {return m_Xmax;}
  double Kopt() const {return m_Kopt;}
  double KoptNaked() const {return m_KoptNaked;}
  int KoptInt() const {return m_KoptInt;}
  bool IsOttenApplicable() const {return m_IsOttenApplicable;}
  bool IsOttenApplicableExact() const {return m_IsOttenApplicableExact;}
  bool IsModifiedOttenApplicable() const {return m_IsModifiedOttenApplicable;}
  double MaxRealDelay() const {return m_MaxRealDelay;}
  double SumRealDelay() const {return m_SumRealDelay;}
  double OttenDelay() const {return m_OttenDelay;}
  double OptimalDelay() const {return m_OptimalDelay;}
  double KoptDelay() const {return m_KoptDelay;}
  int Fanout() const {return m_Fanout;}
  string Name() const {return m_Name;}
};

#endif //__NETINFO_H__