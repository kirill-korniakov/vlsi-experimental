#include "PlacementQualityAnalyzer.h"
#include "Timing.h"
#include "Utils.h"
#include "AdaptiveRoute.h"
#include "DelayCalculation.h"
#include "Legalization.h"
#include "STA.h"

void PlacementQualityAnalyzer::SavePlacement()
{
  if(m_rows_size < m_design.Cells.CellsCount())
  {
    delete[] m_xrow;
    delete[] m_yrow;
    m_rows_size = m_design.Cells.CellsCount();
    m_xrow = new double[m_rows_size];
    m_yrow = new double[m_rows_size];
  }
  int i = 0;
  for (HCells::CellsEnumeratorW cell = m_design.Cells.GetEnumeratorW(); cell.MoveNext(); ++i)
  {
    m_xrow[i] = cell.X();
    m_yrow[i] = cell.Y();
  }
}

void PlacementQualityAnalyzer::RestorePlacement()
{
  int i = 0;
  for (HCells::CellsEnumeratorW cell = m_design.Cells.GetEnumeratorW(); cell.MoveNext(); ++i)
  {
    cell.SetX(m_xrow[i]);
    cell.SetY(m_yrow[i]);
  }
}

void PlacementQualityAnalyzer::SaveCurrentPlacementAsBestAchieved()
{
  if(m_BestPlacementCellsCount < m_design.Cells.CellsCount())
  {
    delete[] m_BestPlacementCellsX;
    delete[] m_BestPlacementCellsY;
    m_BestPlacementCellsCount = m_design.Cells.CellsCount();
    m_BestPlacementCellsX = new double[m_BestPlacementCellsCount];
    m_BestPlacementCellsY = new double[m_BestPlacementCellsCount];
  }
  int i = 0;
  for (HCells::CellsEnumeratorW cell = m_design.Cells.GetEnumeratorW(); cell.MoveNext(); ++i)
  {
    m_BestPlacementCellsX[i] = cell.X();
    m_BestPlacementCellsY[i] = cell.Y();
  }
}

void PlacementQualityAnalyzer::RestoreBestAchievedPlacement()
{
  int i = 0;
  for (HCells::CellsEnumeratorW cell = m_design.Cells.GetEnumeratorW(); cell.MoveNext(); ++i)
  {
    cell.SetX(m_BestPlacementCellsX[i]);
    cell.SetY(m_BestPlacementCellsY[i]);
  }
}

PlacementQualityAnalyzer::PlacementQualityAnalyzer(HDesign& design)
  : m_design(design), m_grid(0), m_rows_size(0), m_xrow(0), m_yrow(0), m_experiments(),
  m_BestPlacementHPWLLegalized(0.0), m_BestPlacementCellsCount(0), 
  m_BestPlacementCellsX(0), m_BestPlacementCellsY(0),
  nIterationsWithoutGain(0)
{
  m_grid = new HDPGrid(m_design);
}

PlacementQualityAnalyzer::~PlacementQualityAnalyzer()
{
  delete m_grid;
  delete m_xrow;
  delete m_yrow;
  delete m_BestPlacementCellsX;
  delete m_BestPlacementCellsY;
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
  SavePlacement();
  AbacusLegalization(*m_grid);
  pq.hpwl_legalized = Utils::CalculateHPWL(m_design, false);

  ALERTFORMAT(("HPWL nonlegal  = %f", pq.hpwl));
  ALERTFORMAT(("HPWL Leg.      = %f", pq.hpwl_legalized));
  ALERTFORMAT(("HPWL Leg. best = %f", m_BestPlacementHPWLLegalized));

  //timing 2
  STA(m_design, DO_NOT_REPORT);
  pq.tns_legalized = Utils::TNS(m_design);
  pq.wns_legalized = Utils::WNS(m_design);

  RestorePlacement();

  m_experiments.push_back(pq);
}

double PlacementQualityAnalyzer::LastIterImprovement()
{
  if (!m_experiments.empty() && m_BestPlacementHPWLLegalized > 0.0)
    return 1.0 - m_experiments.back().hpwl_legalized/m_BestPlacementHPWLLegalized;
  else
    return 1.0;
}

bool PlacementQualityAnalyzer::IsAcceptableImprovementAchieved()
{
  double improvement = LastIterImprovement();
  ALERTFORMAT(("HPWL Leg. gain over best placement is %.3f%%", improvement*100.0));

  if (improvement > m_design.cfg.ValueOf(".improvementTreshold", 0.0))
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
    static int maxIterationsWithoutGain = m_design.cfg.ValueOf(".nTolerantIterations", 3);
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