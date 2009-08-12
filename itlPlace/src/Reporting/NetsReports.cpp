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
    nNetsWithPins[nIter.PinsCount()]++ ;
  }

  WRITELINE("");
  WRITELINE("Reporting: Number pins in nets\n");
  for (int i = 0; i < maxPinCount + 1; i++)
    if (nNetsWithPins[i] != 0)
      WRITELINE("  Number of nets with\t%d\tpin =\t%d", i, nNetsWithPins[i]);

   delete []nNetsWithPins ;

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

void PrintToFastRouterFormat(HDesign& hd, char* fileName)
{
    FILE* routerInputFile;

    if (!(routerInputFile = fopen(fileName, "w")))
    {
        printf("cannot open file %s\n!", fileName);
        return;
    }

    int nHorTiles  = 324; //TODO: add this parameters to config
    int nVertTiles = 324;
    fprintf(routerInputFile, "grid %d %d %d\n", nHorTiles, nVertTiles, 2);
    fprintf(routerInputFile, "vertical capacity %d %d\n", 70, 0);
    fprintf(routerInputFile, "horizontal capacity %d %d\n", 0, 70);
    fprintf(routerInputFile, "minimum spacing %d %d\n", 1, 1);
    fprintf(routerInputFile, "via spacing %d %d\n", 1, 1);

    double xMin  = 0;
    double yMin  = 0;
    double xMax  = 0;
    double yMax  = 0;
    int    nNets = 0;

    for (HNets::NetsEnumeratorW curNet = hd.Nets.GetNetsEnumeratorW(); curNet.MoveNext();)
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

    fprintf(routerInputFile, "%f %f %f %f\n\n", xMin, yMin, ((xMax - xMin) / nHorTiles),
            ((yMax - yMin) / nVertTiles));
    fprintf(routerInputFile, "num net %d\n", nNets);
    int netId = 0;

    for (HNets::NetsEnumeratorW nIter = hd.Nets.GetNetsEnumeratorW(); nIter.MoveNext(); )
    {
        fprintf(routerInputFile, "n%s %d %d %d\n", nIter.Name().c_str(), netId++, nIter.PinsCount(), 1);

        for (HNetWrapper::PinsEnumeratorW currPin = nIter.GetPinsEnumeratorW(); currPin.MoveNext();)
        {
            fprintf(routerInputFile, "%f %f %d\n", currPin.X(), currPin.Y(), 1);
        }
    }
}

void PrintToBoxRouterFormat(HDesign& hd, char* fileName)
{
    FILE* routerInputFile;

    if (!(routerInputFile = fopen(fileName, "w")))
    {
        printf("cannot open file %s\n!", fileName);
        return;
    }

    int nHorTiles  = 324; //TODO: add this parameters to config
    int nVertTiles = 324;
    fprintf(routerInputFile, "grid %d %d\n", nHorTiles, nVertTiles);
    fprintf(routerInputFile, "vertical capacity %d\n", 70);
    fprintf(routerInputFile, "horizontal capacity %d\n", 70);

    double xMin  = 0;
    double yMin  = 0;
    double xMax  = 0;
    double yMax  = 0;
    int    nNets = 0;

    for (HNets::NetsEnumeratorW curNet = hd.Nets.GetNetsEnumeratorW(); curNet.MoveNext();)
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

    fprintf(routerInputFile, "num net %d\n", nNets);
    int netId = 0;
    double tileWidth  = (xMax - xMin) / nHorTiles;
    double tileHeight = (yMax - yMin) / nVertTiles;

    for (HNets::NetsEnumeratorW nIter = hd.Nets.GetNetsEnumeratorW(); nIter.MoveNext(); )
    {
        fprintf(routerInputFile, "n%s %d %d\n", nIter.Name().c_str(), netId++, nIter.PinsCount());

        for (HNetWrapper::PinsEnumeratorW currPin = nIter.GetPinsEnumeratorW(); currPin.MoveNext();)
        {
            int horTileInd  = (currPin.X() - xMin) / tileWidth;
            int vertTileInd = (currPin.Y() - yMin) / tileHeight;
            fprintf(routerInputFile, "%d %d\n", horTileInd, vertTileInd);
        }
    }
}