#ifndef __SPREADING_DATA__
#define __SPREADING_DATA__

struct Bin
{
  double xCoord;             /// x coordinates of the bin.
  double yCoord;             /// y coordinates of the bin.
  double sumPotential;
  double sumBufPotential;

  Bin()
  {
    xCoord = 0.0; yCoord = 0.0;
    sumPotential = 0.0;
    sumBufPotential = 0.0;
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

struct SprData
{
  BinGrid   binGrid;

  double    totalCellArea;
  double    desiredCellsAreaAtEveryBin;
  double**  clusterPotentialOverBins;
  double**  bufferPotentialOverBins;
  double    potentialRadiusX;
  double    potentialRadiusY;
  double    invPSX;
  double    invPSY;

  double    sprWInitial;
  double*   muBinsPen;

  double*   binsPenaltyValues;
  double*   individualBinsDiscrepancy;

  bool useUnidirectSpreading;

  SprData()
  {
    binGrid.bins = 0;
    clusterPotentialOverBins = 0;
    bufferPotentialOverBins = 0;
  }
};

#endif