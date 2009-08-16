#include "Buffering.h"
#include "VanGinneken.h"
#include "Utils.h"
#include "Auxiliary.h"
#include "STA.h"
#include "Reporting.h"
#include "Parser.h"

void NetlistBuffering(HDesign& design)
{ 
  ConfigContext ctx = design.cfg.OpenContext("Buffering");
  WRITELINE("");
  ALERT("BUFFERING STARTED");

  //report
  ALERTFORMAT(("HPWL before buffering: %f", Utils::CalculateHPWL(design, true)));
  ALERT("STA before buffering:");
  STA(design);

  //buffering
  VanGinneken vg(design);
  HDPGrid DPGrid(design);
  for (HNets::NetsEnumeratorW nIter = design.Nets.GetNetsEnumeratorW(); nIter.MoveNext(); ) 
  {
    vg.NetBuffering(nIter);
    FindTopologicalOrder(design);
    ALERT("not legalized");
    STA(design);
    ALERT("legalized");
    Legalization(DPGrid);
    STA(design);
  }
  
  //report
  WRITELINE("");
  ALERTFORMAT(("HPWL after buffering (not legalized): %f", Utils::CalculateHPWL(design, true)));
  ALERT("STA after buffering (not legalized):");
  STA(design);
  design.Plotter.ShowPlacement();
  design.Plotter.SaveMilestoneImage("BI");


  //legalization
  WRITELINE("");
  ALERT("Legalization after buffering:");
  Legalization(DPGrid);
  
  //report
  WRITELINE("");
  ALERTFORMAT(("HPWL after buffering and legalization: %f", Utils::CalculateHPWL(design, true)));
  ALERT("STA after buffering and legalization:");
  STA(design);
  design.Plotter.ShowPlacement();
  design.Plotter.SaveMilestoneImage("BI+LEG");

  ALERT("BUFFERING FINISHED");
}

void TestBuffering(HDesign& design)
{
  ConfigContext ctx = design.cfg.OpenContext("Buffering");
  WRITELINE("");
  ALERT("BUFFERING STARTED");
  ALERTFORMAT(("HPWL before buffering: %f", Utils::CalculateHPWL(design, true)));
  ALERT("STA before buffering:");
  STA(design);
      
  //buffering
  VanGinneken vg(design);
  HDPGrid DPGrid(design);
  string name = Aux::CreateCoolFileName("", design.Circuit.Name() + "_Buffered" ,".def");

  //FIXME: debugging
  //ALERTFORMAT(("Buffer inside = %d", vg.BufferingOfMostCriticalPaths()));
  
  for (HNets::NetsEnumeratorW nIter = design.Nets.GetNetsEnumeratorW(); nIter.MoveNext(); ) 
  {
    if (nIter.PinsCount() != 2) 
    	continue;
    	
    //if (nIter.Name() != "overflw")
    //	continue;

    //bool isAnyPinPrimary = false;
    //for (HNetWrapper::PinsEnumeratorW currPin = nIter.GetPinsEnumeratorW(); currPin.MoveNext();)
    //{
    //  if (currPin.IsPrimary())
    //  {
    //    isAnyPinPrimary = true;
    //    break;
    //  }
    //}
    //if (isAnyPinPrimary)
    //{
    //  continue;
    //}

    WRITELINE("");
    ALERTFORMAT(("Buffer insertion into net %s", nIter.Name().c_str()));
    ReportNetPinsCoordinates(design, nIter);
    STA(design);
    ReportNetTiming(design, nIter);
    ExportDEF(design, "not_buffered");
    vg.NetBuffering(nIter);
    //ReportBufferingPhysics(vg);
    ExportDEF(design, "buffered");
    FindTopologicalOrder(design);
    //ALERT("After buffer insertion:");
    STA(design);
    ReportNetTiming(design, nIter);
    //ALERT("After legalization:");
    //Legalization(DPGrid);
    //STA(design);
    //break;
  }

  WRITELINE("");
  FindTopologicalOrder(design);
  ALERTFORMAT(("HPWL after buffering (not legalized): %f", Utils::CalculateHPWL(design, true)));
  ALERT("STA after buffering (not legalized):");
  STA(design);
  design.Plotter.ShowPlacement();
  design.Plotter.SaveMilestoneImage("BI");

  //legalization
  WRITELINE("");
  ALERT("Legalization after buffering:");
  Legalization(DPGrid);
  design.Plotter.ShowPlacement();
  design.Plotter.SaveMilestoneImage("BI+LEG");

  WRITELINE("");
  FindTopologicalOrder(design);
  ALERTFORMAT(("HPWL after buffering and legalization: %f", Utils::CalculateHPWL(design, true)));
  ALERT("STA after buffering and legalization:");
  STA(design);

  ALERT("BUFFERING FINISHED");
}