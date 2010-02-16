#include "Reporting.h"
#include "Utils.h"
#include "AdaptiveRoute.h"

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
			WRITELINE("  Number of nets with\t%d\tpin =\t%d", i, nNetsWithPins[i]);

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
	ALERTFORMAT(("Reporting net %s timing", netW.Name().c_str()));

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

		ALERTFORMAT(("RAT at %s\t= %.10f", pinName.c_str(), RAT));
		ALERTFORMAT(("AAT at %s\t= %.10f", pinName.c_str(), AAT));
		ALERTFORMAT(("NS at %s\t= %.10f", pinName.c_str(), NS));
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

			//������1
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
			//�����1

			if (srcPoint.HasRight())
			{
				nextPoint = srcPoint.Right();
				xi2 = nextPoint.X();
				yi2 = nextPoint.Y();
				points.push(nextPoint);

				Ai = (yi2 - yi1)/(xi2 - xi1);
				Bi = yi1 - Ai * xi1;

				//������2
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
				//�����2
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
		ALERTFORMAT(("net = %s", netw.Name().c_str()));
		ALERTFORMAT(("crossing\t=%d", crossing));
		ALERTFORMAT(("onlyRight\t=%d", onlyRight));
		ALERTFORMAT(("noTree\t=%d", noTree));
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

		design.Plotter.PlotNetSteinerTree(net, Color_Black);
		design.Plotter.Refresh(HPlotter::WaitTime(intWaitTime));
		design.Plotter.PlotText(signature);
		design.Plotter.SaveImage("",design.cfg.ValueOf("plotter.pixDirectory", ".\\") + design.Circuit.Name() + "_crossing_path\\");
		design.Plotter.Clear();
		design.Plotter.ShowPlacement();

	}
	return crossingType;
}
void SteinerWoodReport(HDesign& design, bool IsStepReport)
{
	ALERTFORMAT(("Steiner wood report:"));
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

	ALERTFORMAT(("crossing in tree = %d", sum.crossing));
	ALERTFORMAT(("Tree heve only right  = %d", sum.onlyRight));
	ALERTFORMAT(("no tree in net = %d", sum.noTree));
}