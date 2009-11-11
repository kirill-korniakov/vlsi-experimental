#include "ExportToRouters.h"

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

    int nHorTiles  = grid.Design().cfg.ValueOf("FGRRouting.nHorTiles", 0);
    int nVertTiles = grid.Design().cfg.ValueOf("FGRRouting.nVertTiles", 0);

    if (nHorTiles * nVertTiles == 0)
    {
      nHorTiles  = grid.NumRows() / 2;
      nVertTiles = grid.NumRows() / 2;
    }

    int verticalCapacity   = grid.Design().cfg.ValueOf("FGRRouting.verticalCapacity", 20);
    int horizontalCapacity = grid.Design().cfg.ValueOf("FGRRouting.horizontalCapacity", 20);
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

    int nHorTiles  = grid.Design().cfg.ValueOf("FGRRouting.nHorTiles", 0);
    int nVertTiles = grid.Design().cfg.ValueOf("FGRRouting.nVertTiles", 0);

    if (nHorTiles * nVertTiles == 0)
    {
      nHorTiles  = grid.NumRows() / 2;
      nVertTiles = grid.NumRows() / 2;
    }

    int verticalCapacity   = grid.Design().cfg.ValueOf("FGRRouting.verticalCapacity", 20);
    int horizontalCapacity = grid.Design().cfg.ValueOf("FGRRouting.horizontalCapacity", 20);
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