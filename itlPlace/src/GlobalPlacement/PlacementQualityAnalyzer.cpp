#include "PlacementQualityAnalyzer.h"
#include "Timing.h"
#include "Utils.h"
#include "AdaptiveRoute.h"
#include "DelayCalculation.h"
#include "Legalization.h"
#include "STA.h"
#include "TableFormatter.h"
#include "OptimizationContext.h"

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

void PlacementQualityAnalyzer::AnalyzeQuality(int id, CriteriaValues* criteriaValues, double improvementTreshold)
{
  PlacementQuality pq;
  pq.id = id;

  if (criteriaValues)
  {
      pq.metrics[MetricObjective] = criteriaValues->objective;
      pq.metrics[MetricObHPWL] = criteriaValues->hpwl;
      pq.metrics[MetricObLR] = criteriaValues->lr;
      pq.metrics[MetricObSOD] = criteriaValues->sod;
      pq.metrics[MetricObSpreading] = criteriaValues->spreading;
  }
  else
  {
      pq.metrics[MetricObjective] = -1.0;
      pq.metrics[MetricObHPWL] = -1.0;
      pq.metrics[MetricObLR] = -1.0;
      pq.metrics[MetricObSOD] = -1.0;
      pq.metrics[MetricObSpreading] = -1.0;
  }

  pq.metrics[MetricHPWL] = Utils::CalculateHPWL(m_design, false);

  if (m_design.CanDoTiming())
  {
    //timing 1
    STA(m_design, DO_NOT_REPORT);
    pq.metrics[MetricTNS] = Utils::TNS(m_design);
    pq.metrics[MetricWNS] = Utils::WNS(m_design);
    pq.metrics[MetricTWL] = Utils::CalculateTWL(m_design);
  }

  //legalization
  m_Placement.SavePlacement(m_design);
  AbacusLegalization(*m_grid);
  pq.metrics[MetricHPWLleg] = Utils::CalculateHPWL(m_design, false);

  if (m_design.CanDoTiming())
  {
    //timing 2
    STA(m_design, DO_NOT_REPORT);
    pq.metrics[MetricTNSleg] = Utils::TNS(m_design);
    pq.metrics[MetricWNSleg] = Utils::WNS(m_design);
    pq.metrics[MetricTWLleg] = Utils::CalculateTWL(m_design);
  }

  m_Placement.RestorePlacement(m_design);
  m_experiments.push_back(pq);

  double improvement = GetLastIterationImprovement();

  ALERT("HPWL nonlegal  = %f", pq.metrics[MetricHPWL]);
  ALERT("HPWL Leg.      = %f", pq.metrics[MetricHPWLleg]);
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
  const int col_id        = 0;
  const int col_objective = 1;
  const int col_ob_hpwl   = 2;
  const int col_ob_lr     = 3;
  const int col_ob_sod    = 4;
  const int col_ob_spr    = 5;
  const int col_hpwl     = 6;
  const int col_hpwl_leg = 7;
  const int col_twl      = 8;
  const int col_twl_leg  = 9;
  const int col_tns      = 10;
  const int col_tns_leg  = 11;
  const int col_wns      = 12;
  const int col_wns_leg  = 13;

  bool canDoTiming = m_design.CanDoTiming();

  TableFormatter tf("Placement Quality Analysis Table");

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
  tf.SetCell(col_objective, "Objective");
  tf.SetCell(col_ob_hpwl, "ObHPWL");
  tf.SetCell(col_ob_lr, "ObLR");
  tf.SetCell(col_ob_sod, "ObSOD");
  tf.SetCell(col_ob_spr, "ObSpreading");
  tf.SetCell(col_hpwl, "HPWL");
  tf.SetCell(col_hpwl_leg, "LHPWL");
  tf.SetColumnPrecision(col_objective, 2);
  tf.SetColumnPrecision(col_hpwl, 0);
  tf.SetColumnPrecision(col_hpwl_leg, 0);

  tf.NewBorderRow();
  tf.SetCell(0, "-", tf.NumOfColumns(), TableFormatter::Align_Fill);

  for(QualityList::iterator i = m_experiments.begin(); i != m_experiments.end(); ++i)
  {
    tf.NewRow();
    tf.SetCell(col_id, i->id);
    tf.SetCell(col_objective, i->metrics[MetricObjective]);
    tf.SetCell(col_ob_hpwl, i->metrics[MetricObHPWL]);
    tf.SetCell(col_ob_lr, i->metrics[MetricObLR]);
    tf.SetCell(col_ob_sod, i->metrics[MetricObSOD]);
    tf.SetCell(col_ob_spr, i->metrics[MetricObSpreading]);
    tf.SetCell(col_hpwl, i->metrics[MetricHPWL]);
    tf.SetCell(col_hpwl_leg, i->metrics[MetricHPWLleg]);
    if (canDoTiming)
    {
      tf.SetCell(col_twl, i->metrics[MetricTWL]);
      tf.SetCell(col_twl_leg, i->metrics[MetricTWLleg]);
      tf.SetCell(col_tns, i->metrics[MetricTNS]);
      tf.SetCell(col_tns_leg, i->metrics[MetricTNSleg]);
      tf.SetCell(col_wns, i->metrics[MetricWNS]);
      tf.SetCell(col_wns_leg, i->metrics[MetricWNSleg]);
    }
  }

  //TODO: embed properly
  //This code shows improvement in percents, but right now there are problems with formatting.
  QualityList::iterator initial = m_experiments.begin();

  for(QualityList::iterator i = m_experiments.begin(); i != m_experiments.end(); ++i)
  {
	  tf.NewRow();
	  tf.SetCell(col_id, i->id);
	  tf.SetCell(col_objective, i->metrics[MetricObjective] / initial->metrics[MetricObjective] * 100);
      tf.SetCell(col_ob_hpwl, i->metrics[MetricObHPWL] / initial->metrics[MetricObHPWL] * 100);
      tf.SetCell(col_ob_lr, i->metrics[MetricObLR] / initial->metrics[MetricObLR] * 100);
      tf.SetCell(col_ob_sod, i->metrics[MetricObSOD] / initial->metrics[MetricObSOD] * 100);
      tf.SetCell(col_ob_spr, i->metrics[MetricObSpreading] / initial->metrics[MetricObSpreading] * 100);
	  tf.SetCell(col_hpwl, i->metrics[MetricHPWL] / initial->metrics[MetricHPWL] * 100);
	  tf.SetCell(col_hpwl_leg, i->metrics[MetricHPWLleg] / initial->metrics[MetricHPWLleg] * 100);
	  if (canDoTiming)
	  {
		  tf.SetCell(col_twl, i->metrics[MetricTWL] / initial->metrics[MetricTWL] * 100);
		  tf.SetCell(col_twl_leg, i->metrics[MetricTWLleg] / initial->metrics[MetricTWLleg] * 100);
		  tf.SetCell(col_tns, i->metrics[MetricTNS] / initial->metrics[MetricTNS] * 100);
		  tf.SetCell(col_tns_leg, i->metrics[MetricTNSleg] / initial->metrics[MetricTNSleg] * 100);
		  tf.SetCell(col_wns, i->metrics[MetricWNS] / initial->metrics[MetricWNS] * 100);
		  tf.SetCell(col_wns_leg, i->metrics[MetricWNSleg] / initial->metrics[MetricWNSleg] * 100);
	  }
  }

  WRITELINE("");
  tf.Print();
  WRITELINE("");
}

PlacementQualityAnalyzer::QualityMetrics PlacementQualityAnalyzer::GetMetric(const string& metric)
{
  for (int i = 0; i < __MetricsNum; ++i)
    if (metric == GetMetric((QualityMetrics)i))
      return (QualityMetrics)i;
  return MetricHPWLleg;
}

const char* PlacementQualityAnalyzer::GetMetric(QualityMetrics metric)
{
  switch(metric)
  {
  case MetricObjective:
    return "Objective";
  case MetricObHPWL:
    return "ObHPWL";
  case MetricObLR:
      return "ObLR";
  case MetricObSOD:
      return "ObSOD";
  case MetricObSpreading:
      return "ObSpr";
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
    return GetMetric(MetricHPWLleg);
  }
}

double PlacementQualityAnalyzer::PlacementQuality::GetMetric(QualityMetrics qm)
{
  if (qm == __MetricsNum)
    return -1.0;
  else
    return metrics[qm];
}