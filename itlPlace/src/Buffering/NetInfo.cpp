#include "NetInfo.h"
#include "Utils.h"
#include "Timing.h"
#include <math.h>

double fnOttenDelay(double k, double r, double c, double lbuf, double lnet, double lext, double dbuf, double dnet)
{
  double lk = lext + (k + 1.0) * lbuf;
  return r * c * 0.5 * (lk * lk / (k + 1.0) - k * (lbuf * lbuf - dbuf * dbuf) - (lnet * lnet - dnet * dnet));
}

bool isOneMoreBetter(double k, double lext, double dbuf)
{
  return lext * lext - dbuf * dbuf * (k + 1.0) * (k + 2.0) > 0;
}

void NetInfo::SetNetInfoComputedFields(BufferInfo& buf, NetInfo& n, double r, double c)
{
  n.m_Lnet = n.Rd() / r + n.Cs() / c;
  n.m_Dnet = sqrt(2.0 * n.Rd() * n.Cs() / r / c);
  n.m_Lext = n.L() + n.Lnet() - buf.Lbuf();
  n.m_KoptNaked = n.Lext()/buf.Dbuf() - 1.0;
  n.m_X2opt = 0.5 * n.Lext() + (buf.Rb() - n.Rd()) / r;

  double discr = n.Lext() * n.Lext() - 2.0 * buf.Dbuf()*buf.Dbuf();

  if (discr < 0)
  {
    n.m_Xmax = n.m_Xmin = 0.0;
    n.m_IsOttenApplicable = false;
    n.m_IsModifiedOttenApplicable = false;
  }
  else
  {
   //   if (n.X2opt() >= 0 && n.X2opt() <= n.L())
    //n.m_X2opt = n.X2opt();

    double discr2 = sqrt(discr) * 0.5;
    n.m_Xmin = n.X2opt() - discr2;
    n.m_Xmax = n.X2opt() + discr2;

    if(n.X2opt() < 0 && n.Xmax() < 0 || n.X2opt() > n.L() && n.Xmin() > n.L())
    {
      n.m_IsModifiedOttenApplicable = false;
      n.m_IsOttenApplicable = false;
    }
    else
    {
      if (n.X2opt() >= 0 && n.X2opt() <= n.L())
      {
        n.m_IsOttenApplicable = true;
        n.m_IsModifiedOttenApplicable = false;
      }
      else
      {
        n.m_IsOttenApplicable = false;
        n.m_IsModifiedOttenApplicable = true;
      }
    }
  }

  {
    double k = (int)n.KoptNaked();
    n.m_IsOttenApplicableExact = (n.Lext() >= 0.0)
      && (n.Lext() / (k + 1) + (buf.Rb() - n.Rd()) / r >= 0)
      && (n.Lext() / (k + 2) + (buf.Rb() - n.Rd()) / r >= 0)
      && (n.Lext() / (k + 1) + (buf.Cb() - n.Cs()) / c >= 0)
      && (n.Lext() / (k + 2) + (buf.Cb() - n.Cs()) / c >= 0);
  }

  n.m_OttenDelay = fnOttenDelay(0, r, c, buf.Lbuf(), n.Lnet(), n.Lext(), buf.Dbuf(), n.Dnet());
  //n.m_OttenDelay = //n.Rd() * (n.Cs() + n.L() * c) + 
    //c * 0.5*
    //n.L()// *  (n.Cs() + n.L() * c * 0.5)
    //;

  if (n.IsOttenApplicable() || n.IsModifiedOttenApplicable())
  {
    if (n.IsOttenApplicable())
    {
      n.m_Kopt = n.KoptNaked();
      n.m_KoptInt = (int)n.KoptNaked();

      if (isOneMoreBetter(n.KoptInt(), n.Lext(), buf.Dbuf()))
        n.m_KoptInt++;

      n.m_KoptDelay = fnOttenDelay(n.KoptInt(), r, c, buf.Lbuf(), n.Lnet(), n.Lext(), buf.Dbuf(), n.Dnet());
      n.m_OptimalDelay = fnOttenDelay(n.Kopt(), r, c, buf.Lbuf(), n.Lnet(), n.Lext(), buf.Dbuf(), n.Dnet());
    }
    else if (n.X2opt() < 0)
    {
      NetInfo nf;
      nf.m_Rd = buf.Rb();//n.Rd();
      nf.m_Cs = n.Cs();
      nf.m_HPWL = n.HPWL();
      nf.m_WL = n.WL();
      SetNetInfoComputedFields(buf, nf, r, c);

      n.m_Kopt = 1.0 + nf.Kopt();
      n.m_KoptInt = 1 + nf.KoptInt();
      n.m_KoptDelay = n.Rd() * buf.Cb() + buf.Tb() + nf.KoptDelay();
      n.m_OptimalDelay = n.Rd() * buf.Cb() + buf.Tb() + nf.OptimalDelay();
    }
    else if (n.X2opt() > n.L())
    {
      NetInfo nf;
      nf.m_Rd = n.Rd();
      nf.m_Cs = buf.Cb();//n.Cs();
      nf.m_HPWL = n.HPWL();
      nf.m_WL = n.WL();
      SetNetInfoComputedFields(buf, nf, r, c);

      n.m_Kopt = 1.0 + nf.Kopt();
      n.m_KoptInt = 1 + nf.KoptInt();
      n.m_KoptDelay = n.Cs() * buf.Rb() + buf.Tb() + nf.KoptDelay();
      n.m_OptimalDelay = n.Cs() * buf.Rb() + buf.Tb() + nf.OptimalDelay();
    }
  }
  else //if (discr < 0 || n.Xmax() < 0 || n.Xmin() > n.L())
  {
    n.m_Kopt = 0.0;
    n.m_KoptInt = 0;
    n.m_KoptDelay = n.OttenDelay();
    n.m_OptimalDelay = n.OttenDelay();
  }
}

NetInfo NetInfo::Create(HDesign& hd, HNet net, BufferInfo& buf)
{
  NetInfo n = NetInfo();
  n.m_Name = hd.GetString<HNet::Name>(net);

  HPin source = hd.Get<HNet::Source, HPin>(net);
  ERROR_ASSERT(!::IsNull(source));

  //get worst timing arc
  HTimingPoint sourceTP = hd.TimingPoints[source];
  HTimingPoint sa = hd.Get<HTimingPoint::ArrivalAncestor, HTimingPoint>(sourceTP);

  HTimingArcType arc;
  double arcTime;
  bool isInversed;

  GetArrivalArc(hd, sa, sourceTP, &arc, &arcTime, &isInversed);

  double driverDelay = arcTime;

  //calc Rd
  n.m_Rd = hd.GetDouble<HTimingArcType::ResistanceRise>(arc);
  driverDelay -= hd.GetDouble<HTimingArcType::TIntrinsicRise>(arc);

  //calc Cs and Fanout, and real delays
  n.m_Cs = 0.0;
  n.m_Fanout = 0;
  n.m_MaxRealDelay = 0.0;
  n.m_SumRealDelay = 0.0;
  for (HNet::SinksEnumeratorW sink = hd.Get<HNet::Sinks, HNet::SinksEnumeratorW>(net);  sink.MoveNext(); )
  {
    n.m_Fanout++;
    n.m_Cs += hd.GetDouble<HPinType::Capacitance>(sink.Type());
    double delay = hd.GetDouble<HSteinerPoint::PathDelay>(hd.SteinerPoints[sink]);
    if (delay > n.m_MaxRealDelay)
      n.m_MaxRealDelay = delay;
    n.m_SumRealDelay += delay;
  }

  n.m_MaxRealDelay += driverDelay;
  n.m_SumRealDelay += driverDelay;

  //wire length
  n.m_HPWL = Utils::CalcNetHPWL(hd, net);
  n.m_WL = hd.GetDouble<HWire::Length>(hd.Wires[net]);

  SetNetInfoComputedFields(buf, n, hd.RoutingLayers.Physics.RPerDist, hd.RoutingLayers.Physics.LinearC);

  return n;
}