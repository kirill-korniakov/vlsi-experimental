#ifndef __LOW_TIMING_H__
#define __LOW_TIMING_H__

#include "stdTypes.h"

enum KSignalDirection
{
  SignalDirection_Rise = 0,
  SignalDirection_Fall = 1,
  SignalDirection_None = 2,
  SignalDirection_Average = 3,
  SignalDirection_Default = 0
};

enum PathExtractionType
{
  PathExtractionType_Arrival,
  PathExtractionType_Required
};

struct LTiming
{
  //binded to pins
  double *tpFallArrivalTime;
  double *tpRiseArrivalTime;
  double *tpFallRequiredTime;
  double *tpRiseRequiredTime;
  IDType *tpArrivalPropagationOrder; //each element is ID of next pin in propagation order
  IDType *tpRequiredPropagationOrder;

  IDType *tpFallRequiredAncestorPoint;
  IDType *tpFallArrivalAncestorPoint;
  IDType *tpRiseRequiredAncestorPoint;
  IDType *tpRiseArrivalAncestorPoint;

  int nTimingStartPoints;
  int nTimingEndPoints;
  int tpFirstInternalPoint;
  int tpLastInternalPoint;

  KSignalDirection* cppSigDirection;
  IDType* cppTimingPointID;
  int ncppLimit;
  int ncppsEnd;

  int* cpFirstPointIdx;
  int* cpAfterLastPointIdx;
  PathExtractionType* cpExtractionType;
  int ncpEnd;
  int ncpLimit;

  LTiming()
  {
    this->tpFallArrivalTime = 0;
    this->tpRiseArrivalTime = 0;
    this->tpFallRequiredTime = 0;
    this->tpRiseRequiredTime = 0;
    this->tpArrivalPropagationOrder = 0;
    this->tpRequiredPropagationOrder = 0;
    this->nTimingStartPoints = 0;
    this->nTimingEndPoints = 0;
    this->tpFallRequiredAncestorPoint = 0;
    this->tpFallArrivalAncestorPoint = 0;
    this->tpRiseRequiredAncestorPoint = 0;
    this->tpRiseArrivalAncestorPoint = 0;
    this->tpFirstInternalPoint = 0;
    this->tpLastInternalPoint = 0;
    this->ncppsEnd = 0;
    this->ncppLimit = 0;
    this->cppTimingPointID = 0;
    this->cppSigDirection = 0;
    this->cpFirstPointIdx = 0;
    this->cpAfterLastPointIdx = 0;
    this->cpExtractionType = 0;
    this->ncpEnd = 0;
    this->ncpLimit = 0;
  }
};

#endif //__LOW_TIMING_H__
