#ifndef __HIGH_WIREPHYSICALPARAMS_H__
#define __HIGH_WIREPHYSICALPARAMS_H__

#include "HRoutingLayer.h"

class HWirePhysicalParams
{
protected:

  double m_VertRPerDist; //kohms/nm
  double m_VertCPerDist; //pF/nm
  double m_VertEdgeCap;  //pF/nm
  double m_VertLinearC;  //pF/nm
  double m_VertWidth;    //nm
  double m_VertPitch;    //nm

  double m_HorizRPerDist; //kohms/nm
  double m_HorizCPerDist; //pF/nm
  double m_HorizEdgeCap;  //pF/nm
  double m_HorizLinearC;  //pF/nm
  double m_HorizWidth;    //nm
  double m_HorizPitch;    //nm

  double m_RPerDist; //kohms/nm
  double m_CPerDist; //pF/nm
  double m_EdgeCap;  //pF/nm
  double m_LinearC;  //pF/nm
  double m_Width;    //nm
  double m_Pitch;    //nm


public:

  //Getters
  const double& VertRPerDist;
  const double& VertCPerDist;
  const double& VertEdgeCap;
  const double& VertLinearC;
  const double& VertWidth;
  const double& VertPitch;

  const double& HorizRPerDist;
  const double& HorizCPerDist;
  const double& HorizEdgeCap;
  const double& HorizLinearC;
  const double& HorizWidth;
  const double& HorizPitch;

  const double& RPerDist;
  const double& CPerDist;
  const double& EdgeCap;
  const double& LinearC;
  const double& Width;
  const double& Pitch;


  HWirePhysicalParams():
    m_VertRPerDist(0.0),  m_VertCPerDist(0.0),  m_VertEdgeCap(0.0),
    m_VertLinearC(0.0),   m_VertWidth(0.0),     m_VertPitch(0.0),
    m_HorizRPerDist(0.0), m_HorizCPerDist(0.0), m_HorizEdgeCap(0.0),
    m_HorizLinearC(0.0),  m_HorizWidth(0.0),    m_HorizPitch(0.0),
    m_RPerDist(0.0),      m_CPerDist(0.0),      m_EdgeCap(0.0),
    m_LinearC(0.0),       m_Width(0.0),         m_Pitch(0.0),

    VertRPerDist(m_VertRPerDist),   VertCPerDist(m_VertCPerDist),
    VertEdgeCap(m_VertEdgeCap),     VertLinearC(m_VertLinearC),
    VertWidth(m_VertWidth),         VertPitch(m_VertPitch),
    HorizRPerDist(m_HorizRPerDist), HorizCPerDist(m_HorizCPerDist),
    HorizEdgeCap(m_HorizEdgeCap),   HorizLinearC(m_HorizLinearC),
    HorizWidth(m_HorizWidth),       HorizPitch(m_HorizPitch),
    RPerDist(m_RPerDist),           CPerDist(m_CPerDist),
    EdgeCap(m_EdgeCap),             LinearC(m_LinearC),
    Width(m_Width),                 Pitch(m_Pitch)
  {}

  //Setters
  void SetVertRPerDist(double value) {m_VertRPerDist = value;}
  void SetVertCPerDist(double value) {m_VertCPerDist = value;}
  void SetVertEdgeCap(double value) {m_VertEdgeCap = value;}
  void SetVertLinearC(double value) {m_VertLinearC = value;}
  void SetVertWidth(double value) {m_VertWidth = value;}
  void SetVertPitch(double value) {m_VertPitch = value;}

  void SetHorizRPerDist(double value) {m_HorizRPerDist = value;}
  void SetHorizCPerDist(double value) {m_HorizCPerDist = value;}
  void SetHorizEdgeCap(double value) {m_HorizEdgeCap = value;}
  void SetHorizLinearC(double value) {m_HorizLinearC = value;}
  void SetHorizWidth(double value) {m_HorizWidth = value;}
  void SetHorizPitch(double value) {m_HorizPitch = value;}

  void SetRPerDist(double value) {m_RPerDist = value;}
  void SetCPerDist(double value) {m_CPerDist = value;}
  void SetEdgeCap(double value) {m_EdgeCap = value;}
  void SetLinearC(double value) {m_LinearC = value;}
  void SetWidth(double value) {m_Width = value;}
  void SetPitch(double value) {m_Pitch = value;}
  
  HWirePhysicalParams & operator = (const HWirePhysicalParams& m)
  {
    if (&m == this)
      return *this;
    m_VertRPerDist = m.m_VertRPerDist; //kohms/nm
    m_VertCPerDist = m.m_VertCPerDist; //pF/nm
    m_VertEdgeCap = m.m_VertEdgeCap;  //pF/nm
    m_VertLinearC = m.m_VertLinearC;  //pF/nm
    m_VertWidth = m.m_VertWidth;    //nm
    m_VertPitch = m.m_VertPitch;    //nm

    m_HorizRPerDist = m.m_HorizRPerDist; //kohms/nm
    m_HorizCPerDist = m.m_HorizCPerDist; //pF/nm
    m_HorizEdgeCap = m.m_HorizEdgeCap;  //pF/nm
    m_HorizLinearC = m.m_HorizLinearC ;  //pF/nm
    m_HorizWidth = m.m_HorizWidth;    //nm
    m_HorizPitch = m.m_HorizPitch;    //nm

    m_RPerDist = m.m_RPerDist; //kohms/nm
    m_CPerDist = m.m_CPerDist; //pF/nm
    m_EdgeCap = m.m_EdgeCap;  //pF/nm
    m_LinearC = m.m_LinearC;  //pF/nm
    m_Width = m.m_Width;    //nm
    m_Pitch = m.m_Pitch;    //nm
    return *this;
  }
};

#endif //__HIGH_WIREPHYSICALPARAMS_H__