#include "PlacementQualityAnalyzer.h"
#include "Timing.h"
#include "Utils.h"
#include "AdaptiveRoute.h"
#include "DelayCalculation.h"
#include "Legalization.h"
#include "STA.h"
#include "TableFormatter.h"
#include "OptimizationContext.h"

PlacementQualityAnalyzer::MetricInfo PlacementQualityAnalyzer::metricsInfo[] = 
{
    MetricInfo(MetricObjective, "Objective", 2), 
    MetricInfo(MetricObHPWL,    "ObHPWL",    2), 
    MetricInfo(MetricObLR,      "ObLR",      2), 
    MetricInfo(MetricObSOD,     "ObSOD",     2), 
    MetricInfo(MetricObSpr,     "ObSpr",     2), 
    MetricInfo(MetricGHPWL,     "gHPWL",     8), 
    MetricInfo(MetricGLR,       "gLR",       8), 
    MetricInfo(MetricGSpr,      "gSpr",      8), 
    MetricInfo(MetricHPWL,      "HPWL",      0), 
    MetricInfo(MetricHPWLleg,   "LHPWL",     0), 
    MetricInfo(MetricTWL,       "TWL",       0), 
    MetricInfo(MetricTWLleg,    "LTWL",      0), 
    MetricInfo(MetricTNS,       "TNS",       4), 
    MetricInfo(MetricTNSleg,    "LTNS",      4), 
    MetricInfo(MetricWNS,       "WNS",       4), 
    MetricInfo(MetricWNSleg,    "LWNS",      4) 
};

void PlacementQualityAnalyzer::ReorderColumns()
{
    metricsInfo[MetricObSOD].column = -1;
    metricsInfo[MetricTWL].column = -1;
    metricsInfo[MetricTWLleg].column = -1;
}

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

      pq.metrics[MetricObHPWL] = criteriaValues->lse;
      pq.metrics[MetricObLR] = criteriaValues->lr;
      pq.metrics[MetricObSOD] = criteriaValues->sod;
      pq.metrics[MetricObSpr] = criteriaValues->spr;

      pq.metrics[MetricGHPWL] = criteriaValues->gLSENorm;
      pq.metrics[MetricGLR] = criteriaValues->gLRNorm;
      pq.metrics[MetricGSpr] = criteriaValues->gSprNorm;
  }
  else
  {
      pq.metrics[MetricObjective] = -1.0;

      pq.metrics[MetricObHPWL] = -1.0;
      pq.metrics[MetricObLR] = -1.0;
      pq.metrics[MetricObSOD] = -1.0;
      pq.metrics[MetricObSpr] = -1.0;

      pq.metrics[MetricGHPWL] = -1.0;
      pq.metrics[MetricGLR] = -1.0;
      pq.metrics[MetricGSpr] = -1.0;
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

  if (improvement >= 0.0)
    m_NumIterationsWithoutGain = 0;
  else
    m_NumIterationsWithoutGain++;

  if (improvement > improvementTreshold)
  {
    m_BestMetrics = pq;
    SaveCurrentPlacementAsBestAchieved();
  }

  //PrintAllCriterias(pq);
  PrintMajorCriteria(improvement);
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

void PlacementQualityAnalyzer::PrintMajorCriteria( double improvement ) 
{
    if (m_experiments.size() <= 1)
    {
        ALERT(Color_White, "%-9s best  = N/A", GetMetric(m_metric));
        ALERT(Color_White, "%s gain over best placement is N/A", GetMetric(m_metric));
    }
    else
    {
        ALERT(Color_White, "%-9s best = %f", GetMetric(m_metric), m_BestMetrics.GetMetric(m_metric));
        ALERT(Color_White, "%s gain over the best placement is %.3f%%", GetMetric(m_metric), improvement * 100.0);
    }
}

void PlacementQualityAnalyzer::PrintAllCriterias( PlacementQualityAnalyzer::PlacementQuality &pq ) 
{
    ALERT("%-14s = %f", "HPWL", pq.metrics[MetricHPWL]);
    ALERT("%-14s = %f", "HPWL Leg.", pq.metrics[MetricHPWLleg]);
    if (m_design.CanDoTiming())
    {
        ALERT("%-14s = %f", GetMetric(MetricTWL), pq.GetMetric(MetricTWL));
        ALERT("%-14s = %f", GetMetric(MetricTWLleg), pq.GetMetric(MetricTWLleg));
        ALERT("%-14s = %f", GetMetric(MetricTNS), pq.GetMetric(MetricTNS));
        ALERT("%-14s = %f", GetMetric(MetricTNSleg), pq.GetMetric(MetricTNSleg));
        ALERT("%-14s = %f", GetMetric(MetricWNS), pq.GetMetric(MetricWNS));
        ALERT("%-14s = %f", GetMetric(MetricWNSleg), pq.GetMetric(MetricWNSleg));
    }
}

void PlacementQualityAnalyzer::Report()
{
  TableFormatter tf("Placement Quality Analysis Table");
  ReorderColumns();

  tf.NewHeaderRow();
  tf.SetCell(0, "ID");

  for (int i = 0; i < __MetricsNum; i++)
  {
      tf.SetCell(metricsInfo[i].column, metricsInfo[i].Name.c_str());
      tf.SetColumnPrecision(metricsInfo[i].column, metricsInfo[i].precision);
  }

  tf.NewBorderRow();
  tf.SetCell(0, "-", tf.NumOfColumns(), TableFormatter::Align_Fill);

  for(QualityList::iterator i = m_experiments.begin(); i != m_experiments.end(); ++i)
  {
    tf.NewRow();
    tf.SetCell(0, i->id);

    for (int idx = 0; idx < __MetricsNum; idx++)
        tf.SetCell(metricsInfo[idx].column, i->metrics[idx]);
  }

  tf.NewBorderRow();
  tf.SetCell(0, "-", tf.NumOfColumns(), TableFormatter::Align_Fill);

  //TODO: implement disabling
  QualityList::iterator initial = m_experiments.begin();
  for(QualityList::iterator i = m_experiments.begin(); i != m_experiments.end(); ++i)
  {
	  tf.NewRow();
	  tf.SetCell(0, i->id);

      for (int idx = 0; idx < __MetricsNum; idx++)
          tf.SetCell(metricsInfo[idx].column, i->metrics[idx] / initial->metrics[idx] * 100);
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
    return metricsInfo[(int)metric].Name.c_str();
}

double PlacementQualityAnalyzer::PlacementQuality::GetMetric(QualityMetrics qm)
{
  if (qm == __MetricsNum)
    return -1.0;
  else
    return metrics[qm];
}