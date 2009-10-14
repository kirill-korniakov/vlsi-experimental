#include "HDesign.h"
#include <stdio.h>
#include <malloc.h>
#include "RoutingLayersBuilder.h"
#include "MacroTypesBuilder.h"
#include "SitesBuilder.h"
#include "MacroTypeUtils.h"

#include "LEF\\lefrReader.hpp"

struct LEFParserData
{
  HDesign *Design;
  Builders::RoutingLayersBuilder rlBuilder;
  Builders::MacroTypesBuilder    mtBuilder;
  Builders::SitesBuilder         stBuilder;

  double distance_to_nm_factor;
  double capacitance_to_pf_factor;
  double resistance_to_kohm_factor;

  LEFParserData(HDesign *hd)
    : Design(hd),
    rlBuilder(hd),
    mtBuilder(hd),
    stBuilder(hd)
  {
    distance_to_nm_factor = 1e3;
    capacitance_to_pf_factor = 1.0;
    resistance_to_kohm_factor = 1e-3;
  }
};

void checkType(lefrCallbackType_e c)
{
  if (c >= 0 && c <= lefrLibraryEndCbkType)
  {
    // OK
  }
  else
  {
    LOGERROR("Callback type is out of bounds!");
  }
}

int layerCB(lefrCallbackType_e c, lefiLayer* layer, lefiUserData ud)
{
  checkType(c);
  if(!layer->lefiLayer::hasType())
    return PARSE_OK;
  if(strcmp(layer->lefiLayer::type(),"ROUTING") != 0)
    return PARSE_OK;

  LEFParserData& lef = *((LEFParserData*)ud);
  Builders::RoutingLayersBuilder& lb = lef.rlBuilder;
  lb.LayerStart();

  lb->SetName(layer->lefiLayer::name());
  if(layer->lefiLayer::hasDirection())
  {
    if(strcmp(layer->lefiLayer::direction(),"HORIZONTAL") == 0)
      lb->SetDirection(LayerDirection_Horizontal);
    else if(strcmp(layer->lefiLayer::direction(),"VERTICAL") == 0)
      lb->SetDirection(LayerDirection_Vertical);
    else if(strcmp(layer->lefiLayer::direction(),"DIAG45") == 0)
      lb->SetDirection(LayerDirection_Diag45);
    else if(strcmp(layer->lefiLayer::direction(),"DIAG135") == 0)
      lb->SetDirection(LayerDirection_Diag135);
    else
      lb->SetDirection(LayerDirection_Default);
  }
  else
    lb->SetDirection(LayerDirection_Default);
/*
  EDGECAPACITANCE value

    Specifies a floating-point value of peripheral capacitance, in
    picofarads per micron. The place-and-route tool uses this value
    in two situations:
       Estimating capacitance before routing
       Calculating segment capacitance after routing
    For the second calculation, the tool uses value only if you set
    layer thickness, or layer height, to 0. In this situation, the
    peripheral capacitance is used in the following formula:
      segment capacitance = (layer capacitance per square x
      segment width x segment length) + (peripheral capacitance x 2
      (segment width + segment length))
*/
  if (layer->lefiLayer::hasEdgeCap())
    lb->SetEdgeCap(layer->lefiLayer::edgeCap() / lef.distance_to_nm_factor * lef.capacitance_to_pf_factor);
  else
    lb->SetEdgeCap(0.0);

  if (layer->lefiLayer::hasWidth())
    lb->SetWidth(layer->lefiLayer::width() * lef.distance_to_nm_factor);
  else
  {
    if (layer->lefiLayer::hasMinwidth())
      lb->SetWidth(layer->lefiLayer::minwidth() * lef.distance_to_nm_factor);
    else if (layer->lefiLayer::hasMaxwidth())
      lb->SetWidth(layer->lefiLayer::maxwidth() * lef.distance_to_nm_factor);
    else
      lb->SetWidth(1.0);
  }

  if(layer->hasPitch())
    lb->SetPitch(layer->pitch() * lef.distance_to_nm_factor);
  else
    lb->SetPitch(lb->Width());

/*
  RESISTANCE RPERSQ value

    Specifies the resistance for a square of wire, in ohms per square.
    The resistance of a wire can be defined as
    RPERSQU x wire length/wire width
*/
  if (layer->lefiLayer::hasResistance())
    lb->SetRPerDist(layer->lefiLayer::resistance() / lb->Width() * lef.resistance_to_kohm_factor);
  else
    lb->SetRPerDist(0.0);

/*
  CAPACITANCE CPERSQDIST value
    Specifies the capacitance for each square unit, in picofarads per
    square micron. This is used to model wire-to-ground
    capacitance.
*/
  if (layer->lefiLayer::hasCapacitance())
    lb->SetCPerDist(layer->lefiLayer::capacitance()
      * lb->Width()
      / lef.distance_to_nm_factor
      / lef.distance_to_nm_factor
      * lef.capacitance_to_pf_factor);

  lb.LayerFinished();
  return PARSE_OK;
}

int macroCB(lefrCallbackType_e c, lefiMacro* macro, lefiUserData ud)
{
  checkType(c);

  LEFParserData& lef = *((LEFParserData*)ud);
  Builders::MacroTypesBuilder& mb = lef.mtBuilder;

  mb->SetName(macro->name());
  mb->SetOriginalName(macro->name());

  if (macro->lefiMacro::hasSize())
  {
    mb->SetSizeX(macro->lefiMacro::sizeX() * lef.distance_to_nm_factor);
    mb->SetSizeY(macro->lefiMacro::sizeY() * lef.distance_to_nm_factor);
  }
  else
  {
    mb->SetSizeX(0.0);
    mb->SetSizeY(0.0);
  }

  if (macro->lefiMacro::hasOrigin())
  {
    mb->SetOriginX(macro->lefiMacro::originX() * lef.distance_to_nm_factor);
    mb->SetOriginX(macro->lefiMacro::originY() * lef.distance_to_nm_factor);
  }
  else
  {
    mb->SetOriginX(0.0);
    mb->SetOriginX(0.0);
  }

  Utils::TryDetectMacroTypeByName(*mb);

  return PARSE_OK;
}

int macroBeginCB(lefrCallbackType_e c, const char* macroName, lefiUserData ud)
{
  checkType(c);

  ((LEFParserData*)ud)->mtBuilder.MacroTypeStart();

  return PARSE_OK;
}

int macroEndCB(lefrCallbackType_e c, const char* macroName, lefiUserData ud)
{
  checkType(c);

  ((LEFParserData*)ud)->mtBuilder.MacroTypeFinished();

  return PARSE_OK;
}

int pinCB(lefrCallbackType_e c, lefiPin* pin, lefiUserData ud)
{
  checkType(c);

  Builders::MacroTypesBuilder& mb = ((LEFParserData*)ud)->mtBuilder;

  mb.AllocatePinType();

  mb.LastPin().SetName(pin->lefiPin::name());
  mb.LastPin().SetOriginalName(pin->lefiPin::name());

  if (pin->lefiPin::hasDirection())
  {
    if(strcmp(pin->lefiPin::direction(),"INPUT") == 0)
      mb.LastPin().SetDirection(PinDirection_INPUT);
    else if(strcmp(pin->lefiPin::direction(),"OUTPUT") == 0)
      mb.LastPin().SetDirection(PinDirection_OUTPUT);
    else if(strcmp(pin->lefiPin::direction(),"INOUT") == 0)
      mb.LastPin().SetDirection(PinDirection_INOUT);
    else
      mb.LastPin().SetDirection(PinDirection_Default);
  }
  else
    mb.LastPin().SetDirection(PinDirection_Default);

  if (pin->lefiPin::hasUse())
  {
    if(strcmp(pin->lefiPin::use(),"ANALOG") == 0)
      mb.LastPin().SetFunction(PinFunction_Analog);
    else if(strcmp(pin->lefiPin::use(),"CLOCK") == 0)
      mb.LastPin().SetFunction(PinFunction_Clock);
    else if(strcmp(pin->lefiPin::use(),"GROUND") == 0)
      mb.LastPin().SetFunction(PinFunction_Ground);
    else if(strcmp(pin->lefiPin::use(),"POWER") == 0)
      mb.LastPin().SetFunction(PinFunction_Power);
    else if(strcmp(pin->lefiPin::use(),"SIGNAL") == 0)
      mb.LastPin().SetFunction(PinFunction_Signal);
    else
      mb.LastPin().SetFunction(PinFunction_Default);
  }
  else
    mb.LastPin().SetFunction(PinFunction_Default);

  mb.LastPin().SetFallCapacitance(0.0);
  mb.LastPin().SetRiseCapacitance(0.0);
  mb.LastPin().SetCapacitance(0.0);

  ///TODO: determine right origin
  mb.LastPin().SetOriginX(0);
  mb.LastPin().SetOriginY(0);

  return PARSE_OK;
}

int siteCB(lefrCallbackType_e c, lefiSite* l, lefiUserData ud)
{
  checkType(c);

  LEFParserData& lef = *((LEFParserData*)ud);
  Builders::SitesBuilder& sb = lef.stBuilder;

  sb.SiteStart();

  sb->SetName(l->name());
  if(l->hasSize())
  {
    sb->SetHeight(l->sizeY() * lef.distance_to_nm_factor);
    sb->SetWidth(l->sizeX() * lef.distance_to_nm_factor);
  }
  else
  {
    sb->SetHeight(0.0);
    sb->SetWidth(0.0);
  }

  sb.SiteFinished();

  return PARSE_OK;
}

void* mallocCB(int size)
{
  return malloc(size);
}

void* reallocCB(void* name, int size)
{
  return realloc(name, size);
}

void freeCB(void* name)
{
  free(name);
}

void ParseLEF(HDesign& design)
{
  ConfigContext ctx = design.cfg.OpenContext("LEFParser");

  if(!design.HasTechInfo())
    design.SetTechInfo();

  LEFParserData userData(&design);
  
  userData.rlBuilder.LayersStart(
    design.cfg.ValueOf(".startLayersCount", 6));
  userData.mtBuilder.MacroTypesStart(
    design.cfg.ValueOf(".startMacrosCount", 42),
    design.cfg.ValueOf(".startPinsCount", 224));
  userData.stBuilder.SitesStart(
    design.cfg.ValueOf(".startSitesCount", 1));
  
  lefrSetMacroBeginCbk(macroBeginCB);
  lefrSetMacroCbk(macroCB);
  lefrSetMacroEndCbk(macroEndCB);
  lefrSetPinCbk(pinCB);
  lefrSetLayerCbk(layerCB);
  lefrSetSiteCbk(siteCB);

  lefrSetMallocFunction(mallocCB);
  lefrSetReallocFunction(reallocCB);
  lefrSetFreeFunction(freeCB);

  lefrSetCaseSensitivity(false);
  lefrSetRelaxMode();
  lefrSetShiftCase();  // will shift name to uppercase if caseinsensitive
                       // is set to off or not set

  static int __init_reader_code = lefrInit();

  lefrReset();
  FILE* lefFile = fopen(design.cfg.ValueOf("benchmark.lef"),"r");
  CRITICAL_ASSERT(lefFile != NULL);

  ALERTFORMAT(("LEF file %s parsing started...", (const char*)design.cfg.ValueOf("benchmark.lef")));

  int lefReaderStatus = lefrRead(lefFile, design.cfg.ValueOf("benchmark.lef"), (void*)(&userData));
  ERROR_ASSERT(lefReaderStatus == PARSE_OK);

  fclose(lefFile);

  lefrReleaseNResetMemory();

  ALERT("LEF parsing finished.");
}
