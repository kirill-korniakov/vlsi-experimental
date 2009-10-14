#ifndef __PLACEMENT_QUALITY_ANALYZER__
#define __PLACEMENT_QUALITY_ANALYZER__

#include "HDesign.h"
#include "HDPGrid.h"

class PlacementQualityAnalyzer
{
private:
  struct PlacementQuality
  {
    int id;
    double hpwl;
    double hpwl_legalized;
    double tns;
    double tns_legalized;
    double wns;
    double wns_legalized;
  };
  typedef TemplateTypes<PlacementQuality>::list QualityList;

  double* m_xrow;
  double* m_yrow;
  int m_rows_size;
  HDPGrid* m_grid;
  HDesign& m_design;
  QualityList m_experiments;
  int nIterationsWithoutGain;

  double* m_BestPlacementCellsX;
  double* m_BestPlacementCellsY;
  int     m_BestPlacementCellsCount;
  double m_BestPlacementHPWLLegalized;

  void SavePlacement();
  void RestorePlacement();
  double LastIterImprovement();

public:
  PlacementQualityAnalyzer(HDesign& design);
  ~PlacementQualityAnalyzer();

  void AnalyzeQuality(int id);
  void Report();

  void SaveCurrentPlacementAsBestAchieved();
  void RestoreBestAchievedPlacement();
  
  bool IsAcceptableImprovementAchieved();
  bool IsNextIterationApproved();
};

#endif //__PLACEMENT_QUALITY_ANALYZER__