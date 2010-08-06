#include "PlacementRowsBuilder.h"
#include "PlacementRowUtils.h"

namespace Builders
{
  PlacementRowsBuilder::PlacementRowsBuilder(HDesign* design)
    : m_hd(design),
    m_CurrRow(&design->PlacementRows),
    m_PrevRow(&design->PlacementRows)
  {
    m_rows_started = false;
    m_rows_finished = false;
    m_row_started = false;
    m_completed = false;
  }

  PlacementRowsBuilder::~PlacementRowsBuilder()
  {
    if(!m_completed)
      CompleteBuild();
  }

  void PlacementRowsBuilder::RowsStart(int size)
  {
    ASSERT(!m_rows_started);

    m_hd->PlacementRows.Initialize(size + 1);

    m_rows_started = true;
  }
  
  void PlacementRowsBuilder::RowsFinished()
  {
    ASSERT(!m_row_started && m_rows_started && !m_rows_finished);

    m_rows_finished = true;
  }

  void PlacementRowsBuilder::RowStart()
  {
    ASSERT(!m_row_started && m_rows_started && !m_rows_finished);

    m_CurrRow = m_hd->PlacementRows.AllocateRow();

    m_row_started = true;
  }

  void PlacementRowsBuilder::RowFinished()
  {
    ASSERT(m_row_started && m_rows_started && !m_rows_finished);

    if (!::IsNull(m_PrevRow) && m_CurrRow.Site() == m_PrevRow.Site())
    {
      if (m_CurrRow.IsHorizontal() && m_PrevRow.IsHorizontal())
      {
        if (m_CurrRow.Y() > m_PrevRow.Y())
        {
          m_CurrRow.SetPreviousRow(m_PrevRow);
          m_PrevRow.SetNextRow(m_CurrRow);
        }
        else
        {
          m_CurrRow.SetNextRow(m_PrevRow);
          m_PrevRow.SetPreviousRow(m_CurrRow);
        }
      }
      else if (m_CurrRow.IsVertical() && m_PrevRow.IsVertical())
      {
        if (m_CurrRow.X() > m_PrevRow.X())
        {
          m_CurrRow.SetPreviousRow(m_PrevRow);
          m_PrevRow.SetNextRow(m_CurrRow);
        }
        else
        {
          m_CurrRow.SetNextRow(m_PrevRow);
          m_PrevRow.SetPreviousRow(m_CurrRow);
        }
      }
    }
    m_PrevRow = m_CurrRow;

    m_row_started = false;
  }

  HPlacementRowWrapper* PlacementRowsBuilder::operator->()
  {
    ASSERT(m_row_started);
    return &m_CurrRow;
  }

  void PlacementRowsBuilder::CompleteBuild()
  {
    if(!m_rows_started)
      RowsStart(0);
    if(!m_rows_finished)
      RowsFinished();

    if(m_hd->PlacementRows.Count() == 0)
      Utils::GeneratePlacementRows(*m_hd);

    Utils::CalcPlacementRectangle(*m_hd);

    m_hd = 0;
    m_completed = true;
  }
}