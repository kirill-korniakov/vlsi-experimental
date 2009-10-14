#ifndef __PLACEMENTROWSBUILDER_H__
#define __PLACEMENTROWSBUILDER_H__

#include "HDesign.h"

namespace Builders
{
  class PlacementRowsBuilder
  {
  private:
    HDesign* m_hd;
    bool m_rows_started;
    bool m_rows_finished;
    bool m_row_started;
    bool m_completed;
    HPlacementRowWrapper m_CurrRow;
    HPlacementRowWrapper m_PrevRow;

  public:
    PlacementRowsBuilder(HDesign* design);
    ~PlacementRowsBuilder();

    void RowsStart(int size = 0);
    void RowsFinished();

    void RowStart();
    void RowFinished();

    HPlacementRowWrapper* operator->();

    void CompleteBuild();
  };
}

#endif //__PLACEMENTROWSBUILDER_H__