#include "PlacementQualityAnalyzer.h"
#include "Timing.h"
#include "Utils.h"
#include "AdaptiveRoute.h"
#include "DelayCalculation.h"
#include "Legalization.h"
#include "STA.h"
#include "TableFormatter.h"

void PlacementQualityAnalyzer::SaveCurrentPlacementAsBestAchieved()
{
  m_BestPlacement.SavePlacement(m_design);
}

void PlacementQualityAnalyzer::RestoreBestAchievedPlacement()
{
  m_BestPlacement.RestorePlacement(m_design);
  m_experiments.push_back(m_BestMetrics);
}

PlacementQualityAnalyzer::PlacementQualityAnalyzer(HDesign& design, QualityMetrics qmethod)
  : m_design(design), m_grid(0), m_experiments(), m_BestMetrics(),
  m_NumIterationsWithoutGain(0)
{
  m_grid = new HDPGrid(m_design);
  m_metric = GetMetric(GetMetric(qmethod));
}

PlacementQualityAnalyzer::PlacementQualityAnalyzer(HDesign& design, const string& qmethod)
  : m_design(design), m_grid(0), m_experiments(), m_BestMetrics(),
  m_NumIterationsWithoutGain(0)
{
  m_grid = new HDPGrid(m_design);
  m_metric = GetMetric(qmethod);
}

PlacementQualityAnalyzer::~PlacementQualityAnalyzer()
{
  delete m_grid;
}

void PlacementQualityAnalyzer::AnalyzeQuality(int id, double improvementTreshold)
{
  PlacementQuality pq;
  pq.id = id;
  pq.hpwl = Utils::CalculateHPWL(m_design, false);

  if (m_design.CanDoTiming())
  {
    //timing 1
    STA(m_design, DO_NOT_REPORT);
    pq.tns = Utils::TNS(m_design);
    pq.wns = Utils::WNS(m_design);
    pq.twl = Utils::CalculateTWL(m_design);
  }

  //legalization
  m_Placement.SavePlacement(m_design);
  AbacusLegalization(*m_grid);
  pq.hpwl_legalized = Utils::CalculateHPWL(m_design, false);

  if (m_design.CanDoTiming())
  {
    //timing 2
    STA(m_design, DO_NOT_REPORT);
    pq.tns_legalized = Utils::TNS(m_design);
    pq.wns_legalized = Utils::WNS(m_design);
    pq.twl_legalized = Utils::CalculateTWL(m_design);
  }

  m_Placement.RestorePlacement(m_design);
  m_experiments.push_back(pq);

  double improvement = GetLastIterationImprovement();

  ALERT("HPWL nonlegal  = %f", pq.hpwl);
  ALERT("HPWL Leg.      = %f", pq.hpwl_legalized);
  if (m_design.CanDoTiming())
  {
    ALERT("%-14s = %f", GetMetric(MetricTWL), pq.GetMetric(MetricTWL));
    ALERT("%-14s = %f", GetMetric(MetricTWLleg), pq.GetMetric(MetricTWLleg));
    ALERT("%-14s = %f", GetMetric(MetricTNS), pq.GetMetric(MetricTNS));
    ALERT("%-14s = %f", GetMetric(MetricTNSleg), pq.GetMetric(MetricTNSleg));
    ALERT("%-14s = %f", GetMetric(MetricWNS), pq.GetMetric(MetricWNS));
    ALERT("%-14s = %f", GetMetric(MetricWNSleg), pq.GetMetric(MetricWNSleg));
  }

  if (m_experiments.size() <= 1)
  {
    ALERT("%s best = N/A", GetMetric(m_metric));
    ALERT("%s gain over best placement is N/A", GetMetric(m_metric));
  }
  else
  {
    ALERT("%s best = %f", GetMetric(m_metric), m_BestMetrics.GetMetric(m_metric));
    ALERT("%s gain over the best placement is %.3f%%", GetMetric(m_metric), improvement * 100.0);
  }

  if (improvement >= 0.0)
    m_NumIterationsWithoutGain = 0;
  else
    m_NumIterationsWithoutGain++;

  if (improvement > improvementTreshold)
  {
    m_BestMetrics = pq;
    SaveCurrentPlacementAsBestAchieved();
  }
}

double PlacementQualityAnalyzer::GetLastIterationImprovement()
{
  if (!m_experiments.empty() && m_BestMetrics.GetMetric(m_metric) != 0.0)
    return 1.0 - m_experiments.back().GetMetric(m_metric) / m_BestMetrics.GetMetric(m_metric);
  else
    return 1.0;
}

int PlacementQualityAnalyzer::GetConvergeIterationsNumber()
{
  if (m_experiments.size() < 2)
    return (int)m_experiments.size();
  int convIters = 0;
  PlacementQuality pq = m_experiments.back();
  for(QualityList::reverse_iterator i = m_experiments.rbegin(); i != m_experiments.rend(); ++i)
    if (pq == *i)
      ++convIters;
  return convIters;
}

void PlacementQualityAnalyzer::Report()
{
  const int col_id = 0;
  const int col_hpwl = 1;
  const int col_hpwl_leg = 2;
  const int col_twl = 3;
  const int col_twl_leg = 4;
  const int col_tns = 5;
  const int col_tns_leg = 6;
  const int col_wns = 7;
  const int col_wns_leg = 8;

  bool canDoTiming = m_design.CanDoTiming();

  TableFormatter tf("Placement Quality Analyzis Table");

  tf.NewHeaderRow();
  if (canDoTiming)
  {
    tf.SetCell(col_wns_leg, "LWNS");
    tf.SetCell(col_wns, "WNS");
    tf.SetCell(col_tns_leg, "LTNS");
    tf.SetCell(col_tns, "TNS");
    tf.SetCell(col_twl_leg, "LTWL");
    tf.SetCell(col_twl, "TWL");
    tf.SetColumnPrecision(col_wns, 4);
    tf.SetColumnPrecision(col_wns_leg, 4);
    tf.SetColumnPrecision(col_tns, 4);
    tf.SetColumnPrecision(col_tns_leg, 4);
    tf.SetColumnPrecision(col_twl, 0);
    tf.SetColumnPrecision(col_twl_leg, 0);
  }

  tf.SetCell(col_id, "ID");
  tf.SetCell(col_hpwl, "HPWL");
  tf.SetCell(col_hpwl_leg, "LHPWL");
  tf.SetColumnPrecision(col_hpwl, 0);
  tf.SetColumnPrecision(col_hpwl_leg, 0);

  tf.NewBorderRow();
  tf.SetCell(0, "-", tf.NumOfColumns(), TableFormatter::Align_Fill);

  for(QualityList::iterator i = m_experiments.begin(); i != m_experiments.end(); ++i)
  {
    tf.NewRow();
    tf.SetCell(col_id, i->id);
    tf.SetCell(col_hpwl, i->hpwl);
    tf.SetCell(col_hpwl_leg, i->hpwl_legalized);
    if (canDoTiming)
    {
      tf.SetCell(col_twl, i->twl);
      tf.SetCell(col_twl_leg, i->twl_legalized);
      tf.SetCell(col_tns, i->tns);
      tf.SetCell(col_tns_leg, i->tns_legalized);
      tf.SetCell(col_wns, i->wns);
      tf.SetCell(col_wns_leg, i->wns_legalized);
    }
  }

  WRITELINE("");
  tf.Print();
  WRITELINE("");
}

PlacementQualityAnalyzer::QualityMetrics PlacementQualityAnalyzer::GetMetric(const string& metric)
{
  for (int i = 0; i < PlacementQualityAnalyzer::__MetricLast; ++i)
    if (metric == GetMetric((PlacementQualityAnalyzer::QualityMetrics)i))
      return (PlacementQualityAnalyzer::QualityMetrics)i;
  return PlacementQualityAnalyzer::MetricHPWLleg;
}

const char* PlacementQualityAnalyzer::GetMetric(PlacementQualityAnalyzer::QualityMetrics metric)
{
  switch(metric)
  {
  case MetricHPWL:
    return "HPWL";
  case MetricHPWLleg:
    return "HPWL leg.";
  case MetricTWL:
    return "TWL";
  case MetricTWLleg:
    return "TWL leg.";
  case MetricTNS:
    return "TNS";
  case MetricTNSleg:
    return "TNS leg.";
  case MetricWNS:
    return "WNS";
  case MetricWNSleg:
    return "WNS leg.";
  default:
    return GetMetric(PlacementQualityAnalyzer::MetricHPWLleg);
  }
}

double PlacementQualityAnalyzer::PlacementQuality::GetMetric(PlacementQualityAnalyzer::QualityMetrics qm)
{
  switch(qm)
  {
  case MetricHPWL:
    return this->hpwl;
  case MetricHPWLleg:
    return this->hpwl_legalized;
  case MetricTWL:
    return this->twl;
  case MetricTWLleg:
    return this->twl_legalized;
  case MetricTNS:
    return this->tns;
  case MetricTNSleg:
    return this->tns_legalized;
  case MetricWNS:
    return this->wns;
  case MetricWNSleg:
    return this->wns_legalized;
  default:
    return this->hpwl_legalized;
  }
}