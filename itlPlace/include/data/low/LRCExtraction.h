#ifndef __LOW_RCEXTRACTION_H__
#define __LOW_RCEXTRACTION_H__

struct LRCExtraction
{
  //all data binded to routing points
  double *rceExtractedC; // pf
  double *rceExtractedR; // kohms
  double *rceRisePointC; // pf
  double *rceFallPointC; // pf
  double *rceFallObservedC; //pf
  double *rceRiseObservedC; //pf
  double *rceFallPathDelay; //ns
  double *rceRisePathDelay; //ns

  LRCExtraction()
  {
    this->rceExtractedC = 0;
    this->rceExtractedR = 0;
    this->rceFallObservedC = 0;
    this->rceRiseObservedC = 0;
    this->rceRisePointC = 0;
    this->rceFallPointC = 0;
    this->rceFallPathDelay = 0;
    this->rceRisePathDelay = 0;
  }
};

#endif //__LOW_RCEXTRACTION_H__
