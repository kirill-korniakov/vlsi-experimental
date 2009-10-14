#ifndef __PINSBUILDER_H__
#define __PINSBUILDER_H__

#include "HDesign.h"

namespace Builders
{
  class PinsBuilder
  {
  private:
    HDesign* m_hd;
    bool m_pins_started;
    bool m_pins_finished;
    bool m_pp_started;
    bool m_completed;
    HPinWrapper m_CurrPin;
    double m_PercentToReserve;

  public:
    PinsBuilder(HDesign* design, double percentToReserve = 1.25);
    ~PinsBuilder();

    void PinsStart(int primariesCount = 0);
    void PinsFinished();

    void PrimaryPinStart();

    void PinFinished();

    HPinWrapper* operator->();

    void CompleteBuild();
  };
}

#endif //__PINSBUILDER_H__