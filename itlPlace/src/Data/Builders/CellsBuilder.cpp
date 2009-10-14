#include "CellsBuilder.h"
#include "CellUtils.h"

namespace Builders
{
  CellsBuilder::CellsBuilder(HDesign* design): m_hd(design), m_CurrCell(&design->Cells)
  {
    m_cells_started = false;
    m_cells_finished = false;
    m_cell_started = false;
    m_completed = false;
  }

  CellsBuilder::~CellsBuilder()
  {
    if(!m_completed)
      CompleteBuild();
  }

  void CellsBuilder::CellsStart(int size)
  {
    ERROR_ASSERT(!m_cells_started);

    m_hd->Cells.Initialize(size + 1);

    m_cells_started = true;
  }
  
  void CellsBuilder::CellsFinished()
  {
    ERROR_ASSERT(!m_cell_started && m_cells_started && !m_cells_finished);

    Utils::NormalizeCellsOrder(*m_hd);
    m_hd->Cells.BuildTerminalsIndex();

    m_cells_finished = true;
  }

  void CellsBuilder::CellStart()
  {
    ERROR_ASSERT(!m_cell_started && m_cells_started && !m_cells_finished);

    if(::IsNull(m_CurrCell) || !::IsNull(m_CurrCell.Type()))
      m_CurrCell = m_hd->Cells.AllocateCell();

    m_cell_started = true;
  }

  void CellsBuilder::CellFinished()
  {
    ERROR_ASSERT(m_cell_started && m_cells_started && !m_cells_finished);

    m_cell_started = false;
  }

  HCellWrapper* CellsBuilder::operator->()
  {
    ERROR_ASSERT(m_cell_started);
    return &m_CurrCell;
  }

  void CellsBuilder::CompleteBuild()
  {
    if(!m_cells_started)
      CellsStart(0);
    if(!m_cells_finished)
      CellsFinished();
    m_hd = 0;
    m_completed = true;
  }
}