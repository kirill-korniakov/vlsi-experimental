#ifndef __SPREADING_DATA__
#define __SPREADING_DATA__

struct Bin
{
  double xCoord;             /// x coordinates of the bin.
  double yCoord;             /// y coordinates of the bin.
  double sumPotential;

  Bin()
  {
    xCoord = 0.0; yCoord = 0.0;
    sumPotential = 0.0;
  }
};

struct BinGrid
{
  Bin**   bins;
  double  binHeight;
  double  binWidth;
  int     nBinRows;
  int     nBinCols;
  int     nBins;
};

struct SpreadingData
{
  BinGrid   binGrid;

  double    totalCellArea;
  double    desiredCellsAreaAtEveryBin;
  double**  clusterPotentialOverBins;
  double    potentialRadiusX;
  double    potentialRadiusY;
  double    invPSX;
  double    invPSY;

  double    muInitial;
  double    muSpreading;
  double*   muBinsPen;

  double*   binsPenaltyValues;
  double*   individualBinsDiscrepancy;

  SpreadingData()
  {
    binGrid.bins = 0;
    clusterPotentialOverBins = 0;
  }
};

#endif