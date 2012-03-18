#include "Reporting.h"
#include "Utils.h"
#include "AdaptiveRoute.h"
#include "Auxiliary.h"
#include "TableFormatter.h"

void ReportNetPins(HDesign& design, HNet net)
{
	WRITELINE("Net %s details:", design.GetString<HNet::Name>(net).c_str());
	for (HNet::PinsEnumeratorW pin = design.Get<HNet::Pins, HNet::PinsEnumeratorW>(net); pin.MoveNext(); )
	{
		WRITELINE("  %s(%s) %s %s",
			design.GetString<HCell::Name>(pin.Cell()).c_str(),
			design.GetString<HMacroType::Name>(design.Get<HCell::MacroType, HMacroType>(pin.Cell())).c_str(),
			pin.Name().c_str(),
			Utils::GetPinDirectionStr(pin.Direction())
			);
	}
}

void ReportNetPinsCoordinates(HDesign& design, HNet net)
{
	WRITELINE("Net %s pins:", design.GetString<HNet::Name>(net).c_str());
	for (HNet::PinsEnumeratorW pin = design.Get<HNet::Pins, HNet::PinsEnumeratorW>(net); pin.MoveNext(); )
	{
		WRITELINE("  %s\tx = %f\ty = %f",
			pin.Name().c_str(),
			design.GetDouble<HPin::X>(pin),
			design.GetDouble<HPin::Y>(pin)
			);
	}
}

void ReportNetsInfo(HDesign& design) 
{
	int maxPinCount = 0;
	for (HNets::NetsEnumeratorW nIter = design.Nets.GetFullEnumeratorW(); nIter.MoveNext(); )
	{
		if (nIter.PinsCount() > maxPinCount)
			maxPinCount = nIter.PinsCount();
	}

	int *nNetsWithPins = new int [maxPinCount + 1];

	for (int j = 0; j < maxPinCount + 1; j++)
	{

		nNetsWithPins[j] = 0;
	}

	int index = 0;

	for (HNets::NetsEnumeratorW nIter = design.Nets.GetFullEnumeratorW(); nIter.MoveNext(); )
	{
		nNetsWithPins[nIter.PinsCount()]++;
	}

	WRITELINE("");
	WRITELINE("Reporting: Number pins in nets\n");
	for (int i = 0; i < maxPinCount + 1; i++)
		if (nNetsWithPins[i] != 0)
			WRITELINE("  %d\tnets with %d\tpins", nNetsWithPins[i], i);

	delete [] nNetsWithPins;
}

void ReportCountNetsWithTypes(HDesign& design)
{
	WRITELINE("");
	WRITELINE("Reporting: count nets with type : all, skipped, buffered, removed, Active\n");
	WRITELINE("  Nets count all = %d", design.Nets.Count());
	WRITELINE("  Nets with active type   = %d", design.Nets.Count(NetKind_Active));
	WRITELINE("  Nets with skipped type  = %d", design.Nets.Count(NetKind_Skipped));
	WRITELINE("  Nets with buffered type = %d", design.Nets.Count(NetKind_Buffered));
	WRITELINE("  Nets with removed type  = %d", design.Nets.Count(NetKind_Removed));
}

void ReportNetTiming(HDesign& design, HNet net)
{
	HNetWrapper netW = design[net]; 
	double RAT, AAT, NS;
	string pinName;
	ALERT("Reporting net %s timing", netW.Name().c_str());

	for (HNetWrapper::PinsEnumeratorW currPin = netW.GetPinsEnumeratorW(); currPin.MoveNext();)
	{
		HTimingPointWrapper tp = design[design.TimingPoints[currPin]];
		RAT = tp.RequiredTime();
		AAT = tp.ArrivalTime();
		NS = tp.NegativeSlack();

		if (currPin.IsPrimary())
			pinName = currPin.Name();
		else
			pinName = design.Cells.GetString<HCell::Name>(currPin.Cell()) + "." + currPin.Name();

		ALERT("RAT at %s\t= %.10f", pinName.c_str(), RAT);
		ALERT("AAT at %s\t= %.10f", pinName.c_str(), AAT);
		ALERT("NS at %s\t= %.10f", pinName.c_str(), NS);
	}
}


int IsCrossing(HDesign& design, HPinWrapper src, double Ai, double Aj, double Bi, double Bj, double &x, double &y, double xi1, double yi1, double xi2, double yi2, double xj1, double yj1, double xj2, double yj2)
{
	int crossing = 0; 
	x = (Bj - Bi) / (Ai - Aj);
	y = Ai * x + Bi;
	if (((((x < xi1) && (x > xi2)) || ((x < xi2) && (x > xi1))) &&
		(((x < xj1) && (x > xj2)) || ((x < xj2) && (x > xj1)))) &&
		((((y < yi1) && (y > yi2)) || ((y < yi2) && (y > yi1))) &&
		(((y < yj1) && (y > yj2)) || ((y < yj2) && (y > yj1)))))
	{
		crossing++;
	}
	return crossing;
}

CrossingReport SteinerTreeReport(HDesign& design, HNet net, bool isReport, int intWaitTime)
{
	AdaptiveRoute(design, net);

	HNetWrapper netw = design[net];
	HPinWrapper src =  design[netw.Source()];
	HSteinerPointWrapper srcPoint = design[design.SteinerPoints[src]];
	HSteinerPointWrapper nextPoint = srcPoint;
	double xi1 = 0, yi1 = 0, xi2 = 0, yi2 = 0, xj1 = 0, yj1 = 0, xj2 = 0, yj2 = 0, Ai = 0, Aj = 0, Bi = 0, Bj = 0, x = 0, y =0;
	bool isBad = false;
	int crossing = 0, onlyRight = 0, noTree = 0;

	if (!(srcPoint.HasLeft() || srcPoint.HasRight()))
		noTree++;

	TemplateTypes<HSteinerPoint>::stack points;
	points.push(srcPoint);

	while (!points.empty())
	{
		isBad = false;
		srcPoint = points.top();
		xi1 = srcPoint.X();
		yi1 = srcPoint.Y();
		points.pop();

		if (srcPoint.HasLeft())
		{
			nextPoint = srcPoint.Left();
			xi2 = nextPoint.X();
			yi2 = nextPoint.Y();
			points.push(nextPoint);

			Ai = (yi2 - yi1)/(xi2 - xi1);
			Bi = yi1 - Ai * xi1;

			//начало1
			TemplateTypes<HSteinerPoint>::stack points2;
			HSteinerPointWrapper srcPoint2 = design[design.SteinerPoints[src]];
			HSteinerPointWrapper nextPoint2 = srcPoint2;
			points2.push(srcPoint2);
			while (!points2.empty())
			{
				srcPoint2 = points2.top();
				xj1 = srcPoint2.X();
				yj1 = srcPoint2.Y();
				points2.pop();

				if (srcPoint2.HasLeft())
				{
					nextPoint2 = srcPoint2.Left();
					xj2 = nextPoint2.X();
					yj2 = nextPoint2.Y();
					points2.push(nextPoint2);

					Aj = (yj2 - yj1)/(xj2 - xj1);
					Bj = yj1 - Aj * xj1;

					crossing += IsCrossing(design, src, Ai, Aj, Bi, Bj, x, y, xi1, yi1, xi2, yi2, xj1, yj1, xj2, yj2);

					if (srcPoint2.HasRight())
					{
						nextPoint2 = srcPoint2.Right();
						xj2 = nextPoint2.X();
						yj2 = nextPoint2.Y();
						points2.push(nextPoint2);

						Aj = (yj2 - yj1)/(xj2 - xj1);
						Bj = yj1 - Aj * xj1;

						crossing += IsCrossing(design, src, Ai, Aj, Bi, Bj, x, y, xi1, yi1, xi2, yi2, xj1, yj1, xj2, yj2);
					}
				}
			}
			//конец1

			if (srcPoint.HasRight())
			{
				nextPoint = srcPoint.Right();
				xi2 = nextPoint.X();
				yi2 = nextPoint.Y();
				points.push(nextPoint);

				Ai = (yi2 - yi1)/(xi2 - xi1);
				Bi = yi1 - Ai * xi1;

				//начало2
				TemplateTypes<HSteinerPoint>::stack points2;
				HSteinerPointWrapper srcPoint2 = design[design.SteinerPoints[src]];
				HSteinerPointWrapper nextPoint2 = srcPoint2;
				points2.push(srcPoint2);
				while (!points2.empty())
				{
					srcPoint2 = points2.top();
					xj1 = srcPoint2.X();
					yj1 = srcPoint2.Y();
					points2.pop();

					if (srcPoint2.HasLeft())
					{
						nextPoint2 = srcPoint2.Left();
						xj2 = nextPoint2.X();
						yj2 = nextPoint2.Y();
						points2.push(nextPoint2);

						Aj = (yj2 - yj1)/(xj2 - xj1);
						Bj = yj1 - Aj * xj1;

						crossing += IsCrossing(design, src, Ai, Aj, Bi, Bj, x, y, xi1, yi1, xi2, yi2, xj1, yj1, xj2, yj2);

						if (srcPoint2.HasRight())
						{
							nextPoint2 = srcPoint2.Right();
							xj2 = nextPoint2.X();
							yj2 = nextPoint2.Y();
							points2.push(nextPoint2);

							Aj = (double(yj2) - double(yj1))/(double(xj2) - double(xj1));
							Bj = yj1 - Aj * xj1;

							crossing += IsCrossing(design, src, Ai, Aj, Bi, Bj, x, y, xi1, yi1, xi2, yi2, xj1, yj1, xj2, yj2);
						}
					}
				}
				//конец2
			}
		}
		else
		{
			if (srcPoint.HasRight())
			{
				onlyRight++;
				isBad = true;
			}
		}
	}
	if ((isReport) && ((crossing > 0) || (onlyRight > 0) || (noTree > 0)))
	{
		ALERT("net = %s", netw.Name().c_str());
		ALERT("crossing\t=%d", crossing);
		ALERT("onlyRight\t=%d", onlyRight);
		ALERT("noTree\t=%d", noTree);
	}

	CrossingReport crossingType;
	crossingType.crossing = crossing;
	crossingType.name =netw.Name();
	crossingType.noTree = noTree;
	crossingType.onlyRight = onlyRight;

	if (crossing > 0)
	{
		char signature[256];

		sprintf(signature, "%s crossing = %d ", netw.Name().c_str(), crossing);

		//FIXME: use public API (Show methods) of the plotter
		design.Plotter->PlotNetSteinerTree(net, Color_Black);
		design.Plotter->Refresh(HPlotter::WaitTime(intWaitTime));
		design.Plotter->DrawText(signature);
		design.Plotter->SaveImage("",design.cfg.ValueOf("plotter.pixDirectory", ".\\") + design.Circuit.Name() + "_crossing_path\\");
		design.Plotter->Clear();
		design.Plotter->ShowPlacement();
	}
	return crossingType;
}
void SteinerWoodReport(HDesign& design, bool IsStepReport)
{
	ALERT("Steiner wood report:");
	CrossingReport crossingType;
	crossingType.name = "";
	crossingType.crossing = 0;
	crossingType.noTree = 0;
	crossingType.onlyRight = 0;
	CrossingReport sum;
	sum.name = "";
	sum.crossing = 0;
	sum.noTree = 0;
	sum.onlyRight = 0;

	for (HNets::ActiveNetsEnumeratorW net = design.Nets.GetActiveNetsEnumeratorW(); net.MoveNext(); )
	{
		crossingType = SteinerTreeReport(design, net, IsStepReport, 1);
		sum.crossing += crossingType.crossing;
		sum.noTree += crossingType.noTree;
		sum.onlyRight += crossingType.onlyRight;

	}

	ALERT("crossing in tree = %d", sum.crossing);
	ALERT("Tree heve only right  = %d", sum.onlyRight);
	ALERT("no tree in net = %d", sum.noTree);
}

string GetSTPointName(HSteinerPoint pt, HDesign& hd)
{
  if ((hd,pt).IsInternal())
    return Aux::Format("pt%d",-::ToID(pt));
  else
    return Utils::MakePinFullName(hd, (hd,pt).Pin());
}

void PrintSteinerTree(HDesign& design, HNet net)
{
  int numCols = 0;
  int colName = numCols++;
  int colLeft = numCols++;
  int colRight = numCols++;
  int colX = numCols++;
  int colY = numCols++;
  int colExtractedR = numCols++;
  int colExtractedC = numCols++;
  int colObservedC = numCols++;
  int colPathDelay = numCols++;

  //create header
  TableFormatter tf(Aux::Format("Steiner tree for net %s",(net,design).Name().c_str()), numCols);
  //columns
  tf.SetColumnAlign(colName, TableFormatter::Align_Left);
  tf.SetColumnAlign(colLeft, TableFormatter::Align_Left);
  tf.SetColumnAlign(colRight, TableFormatter::Align_Left);
  
  //header row
  tf.NewHeaderRow();
  tf.SetCell(colName, "Point Name");
  tf.SetCell(colLeft, "Left child");
  tf.SetCell(colRight, "Right child");
  tf.SetCell(colX, "X (nm)");
  tf.SetColumnPrecision(colX, 2);
  tf.SetCell(colY, "Y (nm)");
  tf.SetColumnPrecision(colY, 2);
  tf.SetCell(colExtractedR, "Extracted R (kohms)");
  tf.SetCell(colExtractedC, "Extracted C (pF)");
  tf.SetCell(colObservedC, "Observed C (pF)");
  tf.SetCell(colPathDelay, "Path Delay (ns)");
  tf.NewBorderRow();

  std::stack<HSteinerPoint> trackBack;
  HSteinerPointWrapper wpt = design[design.SteinerPoints[(design,net).Source()]];

  while(true)
  {
    tf.NewRow();
    tf.SetCell(colName, GetSTPointName(wpt, design));
    if (wpt.HasLeft())
      tf.SetCell(colLeft, GetSTPointName(wpt.Left(), design));
    if (wpt.HasRight())
    {
      tf.SetCell(colRight, GetSTPointName(wpt.Right(), design));
      trackBack.push(wpt.Right());
    }
    tf.SetCell(colX, wpt.X());
    tf.SetCell(colY, wpt.Y());
    tf.SetCell(colExtractedR, wpt.ExtractedR());
    tf.SetCell(colExtractedC, wpt.ExtractedC());
    tf.SetCell(colObservedC, wpt.ObservedC());
    tf.SetCell(colPathDelay, wpt.PathDelay());

    if (wpt.HasLeft())
      wpt.GoLeft();
    else if (trackBack.empty())
      break;
    else
    {
      wpt = trackBack.top();
      trackBack.pop();
    }
  }

  tf.Print();
}

void ReportRouting(HDesign& design)
{
  for (HNets::ActiveNetsEnumeratorW net = design.Nets.GetActiveNetsEnumeratorW(); net.MoveNext(); )
    PrintSteinerTree(design, net);
}