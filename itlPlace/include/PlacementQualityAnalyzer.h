#ifndef __PLACEMENT_QUALITY_ANALYZER__
#define __PLACEMENT_QUALITY_ANALYZER__

#include "HDesign.h"
#include "HDPGrid.h"
#include "PlacementStorage.h"

class PlacementQualityAnalyzer
{
public:
  PlacementQualityAnalyzer(HDesign& design);
  ~PlacementQualityAnalyzer();

  void AnalyzeQuality(int id, double improvementTreshold = 0.0);
  void Report();

  void SaveCurrentPlacementAsBestAchieved();
  void RestoreBestAchievedPlacement();

  int GetConvergeIterationsNumber();
  int GetNumIterationsWithoutGain() { return m_NumIterationsWithoutGain; }
  double GetLastIterationImprovement();

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
    double twl;
    double twl_legalized;

    PlacementQuality()
    {
      id = 0;
      hpwl = hpwl_legalized = 0.0;
      tns = tns_legalized = 0.0;
      wns = wns_legalized = 0.0;
      twl = twl_legalized = 0.0;
    }

    bool operator == (const PlacementQuality& q)
    {
      return hpwl == q.hpwl && hpwl_legalized == q.hpwl_legalized
        && twl == q.twl && twl_legalized == q.twl_legalized
        && tns == q.tns && tns_legalized == q.tns_legalized
        && wns == q.wns && wns_legalized == q.wns_legalized;
    }
    bool operator != (const PlacementQuality& q) { return !(*this == q); }
  };
  typedef TemplateTypes<PlacementQuality>::list QualityList;

  HDPGrid* m_grid;
  HDesign& m_design;
  QualityList m_experiments;

  int m_NumIterationsWithoutGain;
  PlacementQuality m_BestMetrics;
  PlacementStorage m_Placement;
  PlacementStorage m_BestPlacement;
};

#endif //__PLACEMENT_QUALITY_ANALYZER__