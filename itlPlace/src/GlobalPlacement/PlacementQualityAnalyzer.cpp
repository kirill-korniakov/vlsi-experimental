#include "PlacementQualityAnalyzer.h"
#include "Timing.h"
#include "Utils.h"
#include "AdaptiveRoute.h"
#include "DelayCalculation.h"
#include "Legalization.h"
#include "STA.h"

void PlacementQualityAnalyzer::SaveCurrentPlacementAsBestAchieved()
{
  m_BestPlacement.SavePlacement(m_design);
}

void PlacementQualityAnalyzer::RestoreBestAchievedPlacement()
{
  m_BestPlacement.RestorePlacement(m_design);
}

PlacementQualityAnalyzer::PlacementQualityAnalyzer(HDesign& design)
  : m_design(design), m_grid(0), m_experiments(),
  m_BestPlacementHPWLLegalized(0.0),
  nIterationsWithoutGain(0)
{
  m_grid = new HDPGrid(m_design);
  m_Placement.SavePlacement(m_design);
  AbacusLegalization(*m_grid);
  m_BestPlacementHPWLLegalized = Utils::CalculateHPWL(m_design, false);
  m_Placement.RestorePlacement(m_design);
}

PlacementQualityAnalyzer::~PlacementQualityAnalyzer()
{
  delete m_grid;
}

void PlacementQualityAnalyzer::AnalyzeQuality(int id)
{
  PlacementQuality pq;
  pq.id = id;
  pq.hpwl = Utils::CalculateHPWL(m_design, false);

  //timing 1
  STA(m_design, DO_NOT_REPORT);
  pq.tns = Utils::TNS(m_design);
  pq.wns = Utils::WNS(m_design);

  //legalization
  m_Placement.SavePlacement(m_design);
  AbacusLegalization(*m_grid);
  pq.hpwl_legalized = Utils::CalculateHPWL(m_design, false);

  ALERTFORMAT(("HPWL nonlegal  = %f", pq.hpwl));
  ALERTFORMAT(("HPWL Leg.      = %f", pq.hpwl_legalized));
  ALERTFORMAT(("HPWL Leg. best = %f", m_BestPlacementHPWLLegalized));

  //timing 2
  STA(m_design, DO_NOT_REPORT);
  pq.tns_legalized = Utils::TNS(m_design);
  pq.wns_legalized = Utils::WNS(m_design);

  m_Placement.RestorePlacement(m_design);

  m_experiments.push_back(pq);
}

double PlacementQualityAnalyzer::LastIterImprovement()
{
  if (!m_experiments.empty())
    return 1.0 - m_experiments.back().hpwl_legalized/m_BestPlacementHPWLLegalized;
  else
    return 1.0;
}

bool PlacementQualityAnalyzer::IsAcceptableImprovementAchieved()
{
  double improvement = LastIterImprovement();
  ALERTFORMAT(("HPWL Leg. gain over best placement is %.3f%%", improvement*100.0));

  if (improvement > m_design.cfg.ValueOf("GlobalPlacement.improvementTreshold", 0.0))
  {
    m_BestPlacementHPWLLegalized = m_experiments.back().hpwl_legalized; //TODO: move from here
    return true;
  }
  else
  {
    return false;
  }
}

bool PlacementQualityAnalyzer::IsNextIterationApproved()
{
  if (IsAcceptableImprovementAchieved())
  {
    SaveCurrentPlacementAsBestAchieved();
    nIterationsWithoutGain = 0;
    return true;
  }
  else
  {
    nIterationsWithoutGain++;
    static int maxIterationsWithoutGain = m_design.cfg.ValueOf("GlobalPlacement.nTolerantIterations", 3);
    if (nIterationsWithoutGain > maxIterationsWithoutGain)
    {
      nIterationsWithoutGain = 0;
      return false;
    }
    else
    {
      return true;
    }
  }
}

void PlacementQualityAnalyzer::Report()
{
  WRITELINE("");
  WRITELINE("Placement Quality Analysis Table:");
  WRITELINE("%3s %20s %20s %15s %15s %15s %15s", "ID", "HPWL", "HPWL Leg.", "TNS", "TNS Leg.", "WNS", "WNS Leg.");
  for(QualityList::iterator i = m_experiments.begin(); i != m_experiments.end(); ++i)
  {
    WRITELINE("%3d %20f %20f %15f %15f %15f %15f",
      i->id,
      i->hpwl,
      i->hpwl_legalized,
      i->tns,
      i->tns_legalized,
      i->wns,
      i->wns_legalized);
  }

  WRITELINE("");
}