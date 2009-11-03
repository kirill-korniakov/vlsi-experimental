#ifndef __PLACEMENT_STORAGE__
#define __PLACEMENT_STORAGE__

#include "HDesign.h"

class PlacementStorage
{
private:
  double* m_xrow;
  double* m_yrow;
  int m_data_size;
  int m_allocated_space;
public:
  PlacementStorage():m_allocated_space(0), m_data_size(0), m_yrow(0), m_xrow(0) {}
  PlacementStorage(HDesign& design):m_allocated_space(0), m_data_size(0), m_yrow(0), m_xrow(0) 
  {
    SavePlacement(design);
  }

  void SavePlacement(HDesign& design);
  void RestorePlacement(HDesign& design) const;

  int Size() const {return m_data_size;}

  ~PlacementStorage();
};

#endif //__PLACEMENT_STORAGE__
