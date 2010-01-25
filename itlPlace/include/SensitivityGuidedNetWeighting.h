/*  
* SensitivityGuidedNetWeighting.h
* this is a part of itlAnalyticalPlacer
* Copyright (C) 2009, ITLab, Belyakov
* email: alexander.belyakov@mail.itlab.unn.ru
*/

#ifndef __SENSITIVITY_GUIDED_NET_WEIGHTING_H__
#define __SENSITIVITY_GUIDED_NET_WEIGHTING_H__

#include "HDesign.h"
#include "Utils.h"

HPin FindNextSrc(HPin src, const HDesign& hd);

struct NetSensitivity
{
  HNetWrapper net;
  double slackByWeight;
  double FOMByWeight;

  NetSensitivity(HNetWrapper _net, double slByW, double FOMByW):
                 net(_net), slackByWeight(slByW), FOMByWeight(FOMByW) {}
};

typedef std::list<NetSensitivity> NetSensitivities;

void SensitivityGuidedNetWeighting(HDesign& hd);

#endif