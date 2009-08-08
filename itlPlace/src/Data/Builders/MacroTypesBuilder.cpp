#include "MacroTypesBuilder.h"

namespace Builders
{
  MacroTypesBuilder::MacroTypesBuilder(HDesign* design)
    : m_hd(design),
    m_CurrMacroType(&design->MacroTypes),
    m_first_pin(),
    m_last_pin(&design->PinTypes)
  {
    m_macrotypes_started = false;
    m_macrotypes_finished = false;
    m_macro_started = false;
    m_completed = false;
    m_was_pins_added = false;
  }

  MacroTypesBuilder::~MacroTypesBuilder()
  {
    if(!m_completed)
      CompleteBuild();
  }

  void MacroTypesBuilder::MacroTypesStart(int size, int pins_initial_count)
  {
    ERROR_ASSERT(!m_macrotypes_started);

    m_hd->MacroTypes.Initialize(size + 1);
    m_hd->PinTypes.Initialize(pins_initial_count + 1);

    m_macrotypes_started = true;
  }
  
  void MacroTypesBuilder::MacroTypesFinished()
  {
    ERROR_ASSERT(!m_macro_started && m_macrotypes_started && !m_macrotypes_finished);

    m_macrotypes_finished = true;
  }

  void MacroTypesBuilder::MacroTypeStart()
  {
    ERROR_ASSERT(!m_macro_started && m_macrotypes_started && !m_macrotypes_finished);

    m_CurrMacroType = m_hd->MacroTypes.AllocateMacroType();

    m_was_pins_added = false;
    m_macro_started = true;
  }

  void MacroTypesBuilder::MacroTypeFinished()
  {
    ERROR_ASSERT(m_macro_started && m_macrotypes_started && !m_macrotypes_finished);
    
    if(m_was_pins_added)
      m_hd->PinTypes.SetPinTypes(
        m_CurrMacroType,
        m_first_pin,
        m_last_pin
      );

    ///FIXME: determine right origin and remove this
    for(HMacroType::PinsEnumeratorW pin = (*this)->GetEnumeratorW(); pin.MoveNext(); )
    {
      pin.SetOriginX(0.5 * (*this)->SizeX());
      pin.SetOriginY(0.5 * (*this)->SizeY());
    }

    m_macro_started = false;
  }

  void MacroTypesBuilder::AllocatePinType()
  {
    ERROR_ASSERT(m_macro_started && m_macrotypes_started && !m_macrotypes_finished);

    if(m_was_pins_added)
      m_last_pin = m_hd->PinTypes.AllocatePinType();
    else
    {
      m_was_pins_added = true;
      m_first_pin = m_last_pin = m_hd->PinTypes.AllocatePinType();
    }
  }

  HMacroTypeWrapper* MacroTypesBuilder::operator->()
  {
    ERROR_ASSERT(m_macro_started);
    return &m_CurrMacroType;
  }

  HMacroTypeWrapper& MacroTypesBuilder::operator*()
  {
    ERROR_ASSERT(m_macro_started);
    return m_CurrMacroType;
  }

  HPinTypeWrapper& MacroTypesBuilder::LastPin()
  {
    ERROR_ASSERT(m_was_pins_added);
    return m_last_pin;
  }

  void MacroTypesBuilder::CompleteBuild()
  {
    if(!m_macrotypes_started)
      MacroTypesStart(0, 0);
    if(!m_macrotypes_finished)
      MacroTypesFinished();
    m_hd = 0;
    m_completed = true;
  }
}