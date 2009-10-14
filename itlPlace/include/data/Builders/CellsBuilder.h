#ifndef __CELLSBUILDER_H__
#define __CELLSBUILDER_H__

#include "HDesign.h"

namespace Builders
{
  class CellsBuilder
  {
  private:
    HDesign* m_hd;
    bool m_cells_started;
    bool m_cells_finished;
    bool m_cell_started;
    bool m_completed;
    HCellWrapper m_CurrCell;

  public:
    CellsBuilder(HDesign* design);
    ~CellsBuilder();

    void CellsStart(int size = 0);
    void CellsFinished();

    void CellStart();
    void CellFinished();

    HCellWrapper* operator->();

    void CompleteBuild();
  };
}

#endif //__CELLSBUILDER_H__