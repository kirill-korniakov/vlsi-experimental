#ifndef __BUFFERINFO_H__
#define __BUFFERINFO_H__

#include "HDesign.h"

class BufferInfo
{
private:
  BufferInfo()
  {
    m_R = m_C = m_T = m_Lbuf = m_Dbuf = 0.0;
  }

  static BufferInfo Create(HDesign& hd, string macro, string inputPin, string outputPin);

  double m_R;
  double m_C;
  double m_T;
  double m_Lbuf;
  double m_Dbuf;
  HMacroType m_BufType;
public:

  double Rb() const {return m_R;}
  double Cb() const {return m_C;}
  double Tb() const {return m_T;}
  double Lbuf() const {return m_Lbuf;}
  double Dbuf() const {return m_Dbuf;}

  HMacroType Type() const {return m_BufType;}

  static BufferInfo Create(HDesign& hd);
};

#endif //__BUFFERINFO_H__