#include "Reporting.h"
#include "Utils.h"

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

void PrintToFastRouterFormat(HDPGrid& grid, const char* fileName)
{
    FILE* routerInputFile;
    char* outputFileName = new char [strlen(fileName)];
    int pos = 0;
    
    while (fileName[pos] != '.')
    {
      outputFileName[pos] = fileName[pos];
      pos++;
    }

    outputFileName[pos]     = '.';
    outputFileName[pos + 1] = 'f';
    outputFileName[pos + 2] = 'r';
    outputFileName[pos + 3] = '\0';
    
    if (!(routerInputFile = fopen(outputFileName, "w")))
    {
        printf("cannot open file %s\n!", outputFileName);
        return;
    }

    int nHorTiles  = grid.Design().cfg.ValueOf("PrintingToRoutersFormats.nHorTiles", 0);
    int nVertTiles = grid.Design().cfg.ValueOf("PrintingToRoutersFormats.nVertTiles", 0);

    if (nHorTiles * nVertTiles == 0)
    {
      nHorTiles  = grid.NumRows();
      nVertTiles = grid.NumRows();
    }

    int verticalCapacity   = grid.Design().cfg.ValueOf("PrintingToRoutersFormats.verticalCapacity", 7);
    int horizontalCapacity = grid.Design().cfg.ValueOf("PrintingToRoutersFormats.horizontalCapacity", 7);
    fprintf(routerInputFile, "grid %d %d %d\n", nHorTiles, nVertTiles, 2);
    fprintf(routerInputFile, "vertical capacity %d %d\n", 0, verticalCapacity);
    fprintf(routerInputFile, "horizontal capacity %d %d\n", horizontalCapacity, 0);
    fprintf(routerInputFile, "minimum width %d %d\n", 1, 1);
    fprintf(routerInputFile, "minimum spacing %d %d\n", 1, 1);
    fprintf(routerInputFile, "via spacing %d %d\n", 1, 1);

    double xMin  = grid.ColumnX(grid.NumCols());
    double yMin  = grid.RowY(grid.NumRows());
    double xMax  = grid.ColumnX(0);
    double yMax  = grid.RowY(0);
    int    nNets = 0;

    for (HNets::NetsEnumeratorW curNet = grid.Design().Nets.GetNetsEnumeratorW(); curNet.MoveNext();)
    {
        for (HNetWrapper::PinsEnumeratorW currPin = curNet.GetPinsEnumeratorW(); currPin.MoveNext();)
        {
            if (xMin > currPin.X())
                xMin = currPin.X();

            if (xMax < currPin.X())
                xMax = currPin.X();

            if (yMin > currPin.Y())
                yMin = currPin.Y();

            if (yMax < currPin.Y())
                yMax = currPin.Y();
        }
        nNets++;
    }

    xMax += grid.SiteWidth();  //in order to place pins with xNax (or yMax)
    yMax += grid.SiteHeight(); //to the last tiles

    fprintf(routerInputFile, "%f %f %f %f\n\n", xMin, yMin, ((xMax - xMin) / nHorTiles),
            ((yMax - yMin) / nVertTiles));
    fprintf(routerInputFile, "num net %d\n", nNets);
    int netId = 0;

    for (HNets::NetsEnumeratorW nIter = grid.Design().Nets.GetNetsEnumeratorW(); nIter.MoveNext(); )
    {
        fprintf(routerInputFile, "n%s_%d %d %d %d\n", nIter.Name().c_str(), netId, netId++, nIter.PinsCount(), 1);

        for (HNetWrapper::PinsEnumeratorW currPin = nIter.GetPinsEnumeratorW(); currPin.MoveNext();)
        {
            fprintf(routerInputFile, "%f %f %d\n", currPin.X(), currPin.Y(), 1);
        }
    }

    delete [] outputFileName;
    fclose(routerInputFile);
}

void PrintToBoxRouterFormat(HDPGrid& grid, const char* fileName)
{
    FILE* routerInputFile;
    char* outputFileName = new char [strlen(fileName)];
    int pos = 0;
    
    while (fileName[pos] != '.')
    {
      outputFileName[pos] = fileName[pos];
      pos++;
    }

    outputFileName[pos]     = '.';
    outputFileName[pos + 1] = 'b';
    outputFileName[pos + 2] = 'r';
    outputFileName[pos + 3] = '\0';
    
    if (!(routerInputFile = fopen(outputFileName, "w")))
    {
        printf("cannot open file %s\n!", outputFileName);
        return;
    }

    int nHorTiles  = grid.Design().cfg.ValueOf("PrintingToRoutersFormats.nHorTiles", 0);
    int nVertTiles = grid.Design().cfg.ValueOf("PrintingToRoutersFormats.nVertTiles", 0);

    if (nHorTiles * nVertTiles == 0)
    {
      nHorTiles  = grid.NumRows();
      nVertTiles = grid.NumRows();
    }

    int verticalCapacity   = grid.Design().cfg.ValueOf("PrintingToRoutersFormats.verticalCapacity", 7);
    int horizontalCapacity = grid.Design().cfg.ValueOf("PrintingToRoutersFormats.horizontalCapacity", 7);
    fprintf(routerInputFile, "grid %d %d\n", nHorTiles, nVertTiles);
    fprintf(routerInputFile, "vertical capacity %d\n", verticalCapacity);
    fprintf(routerInputFile, "horizontal capacity %d\n", horizontalCapacity);

    double xMin  = grid.ColumnX(grid.NumCols());
    double yMin  = grid.RowY(grid.NumRows());
    double xMax  = grid.ColumnX(0);
    double yMax  = grid.RowY(0);
    int    nNets = 0;

    for (HNets::NetsEnumeratorW curNet = grid.Design().Nets.GetNetsEnumeratorW(); curNet.MoveNext();)
    {
        for (HNetWrapper::PinsEnumeratorW currPin = curNet.GetPinsEnumeratorW(); currPin.MoveNext();)
        {
            if (xMin > currPin.X())
                xMin = currPin.X();

            if (xMax < currPin.X())
                xMax = currPin.X();

            if (yMin > currPin.Y())
                yMin = currPin.Y();

            if (yMax < currPin.Y())
                yMax = currPin.Y();
        }
        nNets++;
    }

    xMax += grid.SiteWidth();  //in order to place pins with xNax (or yMax)
    yMax += grid.SiteHeight(); //to the last tiles

    fprintf(routerInputFile, "num net %d\n", nNets);
    int netId = 0;
    double tileWidth  = (xMax - xMin) / nHorTiles;
    double tileHeight = (yMax - yMin) / nVertTiles;

    for (HNets::NetsEnumeratorW nIter = grid.Design().Nets.GetNetsEnumeratorW(); nIter.MoveNext(); )
    {
        fprintf(routerInputFile, "n%s_%d %d %d\n", nIter.Name().c_str(), netId, netId++, nIter.PinsCount());

        for (HNetWrapper::PinsEnumeratorW currPin = nIter.GetPinsEnumeratorW(); currPin.MoveNext();)
        {
            int horTileInd  = (currPin.X() - xMin) / tileWidth;
            int vertTileInd = (currPin.Y() - yMin) / tileHeight;
            fprintf(routerInputFile, "%d %d\n", horTileInd, vertTileInd);
        }
    }

    delete [] outputFileName;
    fclose(routerInputFile);
}

void ReportNetTiming(HDesign& design, HNet net)
{
  HNetWrapper netW = design[net]; 
  double RAT, AAT;
  string pinName;
  ALERTFORMAT(("Reporting net %s timing", netW.Name().c_str()));
  
  for (HNetWrapper::PinsEnumeratorW currPin = netW.GetPinsEnumeratorW(); currPin.MoveNext();)
  {
    HTimingPointWrapper tp = design[design.TimingPoints[currPin]];
    RAT = tp.RequiredTime();
    AAT = tp.ArrivalTime();

    if (currPin.IsPrimary())
      pinName = currPin.Name();
    else
      pinName = design.Cells.GetString<HCell::Name>(currPin.Cell()) + "." + currPin.Name();

    ALERTFORMAT(("RAT at %s\t= %.10f", pinName.c_str(), RAT));
    ALERTFORMAT(("AAT at %s\t= %.10f", pinName.c_str(), AAT));
  }
}