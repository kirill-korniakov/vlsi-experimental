#ifndef __MACROTYPESBUILDER_H__
#define __MACROTYPESBUILDER_H__

#include "HDesign.h"

namespace Builders
{
  class MacroTypesBuilder
  {
  private:
    HDesign* m_hd;
    bool m_macrotypes_started;
    bool m_macrotypes_finished;
    bool m_macro_started;
    bool m_completed;
    bool m_was_pins_added;
    HMacroTypeWrapper m_CurrMacroType;
    HPinType m_first_pin;
    HPinTypeWrapper m_last_pin;

  public:
    MacroTypesBuilder(HDesign* design);
    ~MacroTypesBuilder();

    void MacroTypesStart(int size = 0, int pins_initial_count = 0);
    void MacroTypesFinished();

    void MacroTypeStart();

    void AllocatePinType();

    void MacroTypeFinished();

    HMacroTypeWrapper* operator->();
    HMacroTypeWrapper& operator*();

    HPinTypeWrapper& LastPin();

    void CompleteBuild();

    bool IsMacroFinished() const {return !m_macro_started; }
  };
}

#endif //__MACROTYPESBUILDER_H__