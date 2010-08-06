/*  
* ConstraintsController.h
* this is a part of itlAnalyticalPlacer
* Copyright (C) 2010, ITLab, Belyakov
* email: alexander.belyakov@mail.itlab.unn.ru
*/

#ifndef __CONSTRAINTS_CONTROLLER__
#define __CONSTRAINTS_CONTROLLER__

#include "HDPGrid.h"

#define CONST_Kd 1.0 //constant based on transition of 10% to 90% VDD
                     //in the article = 0.69

struct ExtPin
{
  HPin pin;
  double deltaArrTime;
  double oldMDist; //old distance between this sink and source
  double betta;
};

struct ExtNet
{
  HNet net;
  double alpha;
  int nPins;
  ExtPin* pins; //pins[0] is source of this net
  ~ExtNet() { delete [] pins; }
};

class ConstraintsController
{
public:
  ConstraintsController(HDPGrid& _grid): grid(_grid) {}
  virtual ~ConstraintsController() {};
  virtual bool CheckCriteria(HCell* cells, int nCells, double bestDiff = 0.0) = 0;

protected:
  HDPGrid& grid;
};

//class for checking HPWL change
class HPWLChecker: public ConstraintsController
{
public:
  HPWLChecker(HDPGrid& _grid): ConstraintsController(_grid) {}
  virtual ~HPWLChecker() {}
  virtual bool CheckCriteria(HCell* cells, int nCells, double bestDiff = 0.0);
};

//class for checking Hippocrate constraints
class HippocrateChecker: public HPWLChecker
{
public:
  HippocrateChecker(HDPGrid& _grid);
  virtual ~HippocrateChecker() { delete [] nets; }
  virtual bool CheckCriteria(HCell* cells, int nCells, double bestDiff = 0.0);

private:
  bool CheckConstraintsOnNet(ExtNet& net);
  bool CheckConstraints(HCell* cells, int nCells);
  void CalculateGamma();

private:
  int nNets;
  ExtNet* nets;
  double gamma;
};

//functions for calculating parameters used in constarints
double CalculateDeltaArrivalTime(HPinWrapper sink, HDesign& hd);
double CalculateAlpha(HNet changedNet, HDesign& hd);
double CalculateBetta(HPinWrapper sink, double oldMDist, HDesign& hd);
double CalculateEstimatedTime(HPinWrapper sink, double oldMDist, HDesign& hd);

#endif