#include "PinsBuilder.h"
#include "PinUtils.h"

namespace Builders
{
  PinsBuilder::PinsBuilder(HDesign* design, double percentToReserve): m_hd(design), m_CurrPin(&design->Pins)
  {
    m_pins_started = false;
    m_pins_finished = false;
    m_pp_started = false;
    m_completed = false;
    m_PercentToReserve = percentToReserve;
  }

  PinsBuilder::~PinsBuilder()
  {
    if(!m_completed)
      CompleteBuild();
  }

  void PinsBuilder::PinsStart(int size)
  {
    ERROR_ASSERT(!m_pins_started);

    m_hd->Pins.Initialize(size, 2 * size + 1);

    m_pins_started = true;
  }
  
  void PinsBuilder::PinsFinished()
  {
    ERROR_ASSERT(!m_pp_started && m_pins_started && !m_pins_finished);

    Utils::AllocatePinsForAllCells(*m_hd, m_PercentToReserve);

    m_pins_finished = true;
  }

  void PinsBuilder::PrimaryPinStart()
  {
    ERROR_ASSERT(!m_pp_started && m_pins_started && !m_pins_finished);

    m_CurrPin = m_hd->Pins.AllocatePrimaryPin();

    m_pp_started = true;
  }

  void PinsBuilder::PinFinished()
  {
    ERROR_ASSERT(m_pp_started && m_pins_started && !m_pins_finished);

    m_pp_started = false;
  }

  HPinWrapper* PinsBuilder::operator->()
  {
    ERROR_ASSERT(m_pp_started);
    return &m_CurrPin;
  }

  void PinsBuilder::CompleteBuild()
  {
    if(!m_pins_started)
      PinsStart(0);
    if(!m_pins_finished)
      PinsFinished();
    m_hd = 0;
    m_completed = true;
  }
}