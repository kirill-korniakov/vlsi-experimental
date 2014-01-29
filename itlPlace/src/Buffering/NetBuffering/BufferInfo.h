#ifndef __BUFFERINFO_H__
#define __BUFFERINFO_H__

#include "HBaseDesignItem.h"
#include "HMacroType.h"
#include "HPinType.h"

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
  HPinType m_InPin;
  HPinType m_OutPin;
public:

  static const KSignalDirection SignalModel = SignalDirection_None;

  double Rb() const {return m_R;}
  double Cb() const {return m_C;}
  double Tb() const {return m_T;}
  double Lbuf() const {return m_Lbuf;}
  double Dbuf() const {return m_Dbuf;}

  double TIntrinsic() {return m_T;}
  double Capacitance() {return m_C;}
  double Resistance() {return m_R;}

  HMacroType Type() const {return m_BufType;}
  HPinType InPin() const {return m_InPin; }
  HPinType OutPin() const {return m_OutPin; }

  static BufferInfo Create(HDesign& hd);
  static TemplateTypes<BufferInfo>::vector CreateVector(HDesign& hd, string* bufferList = NULL);
};

#endif //__BUFFERINFO_H__
