#ifndef __PLACEMENT_QUALITY_ANALYZER__
#define __PLACEMENT_QUALITY_ANALYZER__

#include "HDesign.h"
#include "HDPGrid.h"
#include "PlacementStorage.h"

struct CriteriaValues;

class PlacementQualityAnalyzer
{
public:
  enum QualityMetrics
  {
    MetricObjective = 0,
    MetricObHPWL    = 1,
    MetricObLR,
    MetricObSOD,
    MetricObSpreading,
    MetricHPWL,
    MetricHPWLleg,
    MetricTWL,
    MetricTWLleg,
    MetricTNS,
    MetricTNSleg,
    MetricWNS,
    MetricWNSleg,
    __MetricsNum
  };

  static QualityMetrics GetMetric(const string& metric);
  static const char* GetMetric(QualityMetrics metric);

  PlacementQualityAnalyzer(HDesign& design, QualityMetrics qmethod);
  PlacementQualityAnalyzer(HDesign& design, const string& qmethod);
  ~PlacementQualityAnalyzer();

  void AnalyzeQuality(int id, CriteriaValues* criteriaValues = 0, double improvementTreshold = 0.0);
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
    double metrics[__MetricsNum];

    PlacementQuality()
    {
      id = 0;
      for (int i = 0; i < __MetricsNum; i++)
      {
        metrics[i] = 0.0;
      }
    }

    bool operator == (const PlacementQuality& q)
    {
      bool isEqual = true;
      //NOTE: we start from MetricHPWL because we do not interesting in objective value
      //TODO: actually we have to compare with epsilon tolerance
      for (int i = MetricHPWL; i < __MetricsNum; i++)
      {
        if (metrics[i] != q.metrics[i])
        {
          isEqual = false;
          break;
        }
      }
      return isEqual;
    }
    bool operator != (const PlacementQuality& q) { return !(*this == q); }

    double GetMetric(PlacementQualityAnalyzer::QualityMetrics qm);
  };
  typedef TemplateTypes<PlacementQuality>::list QualityList;

  HDPGrid* m_grid;
  HDesign& m_design;
  QualityList m_experiments;
  QualityMetrics m_metric;

  int m_NumIterationsWithoutGain;
  PlacementQuality m_BestMetrics;
  PlacementStorage m_Placement;
  PlacementStorage m_BestPlacement;
};

#endif //__PLACEMENT_QUALITY_ANALYZER__