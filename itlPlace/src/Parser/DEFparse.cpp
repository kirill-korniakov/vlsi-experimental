#include "HDesign.h"
#include <stdio.h>
//#include <malloc.h>
#include <map>
#include "Utils.h"
#include "CellsBuilder.h"
#include "PinsBuilder.h"
#include "PlacementRowsBuilder.h"
#include "NetsBuilder.h"

#include "defrReader.hpp"

struct DEFParserData
{
  HDesign *Design;
  Builders::CellsBuilder         cBuilder;
  Builders::PinsBuilder          pBuilder;
  Builders::PlacementRowsBuilder prBuilder;
  Builders::NetsBuilder          nBuilder;

  std::map<string, HPin> nets_primaries;

  double distance_to_nm_factor;

  DEFParserData(HDesign *hd)
    : Design(hd),
    pBuilder(hd, hd->cfg.ValueOf(".pinsReservePercent", 1.25)),
    cBuilder(hd),
    prBuilder(hd),
    nBuilder(hd)
  {
    distance_to_nm_factor = 1e3;
  }
};

void checkType_def(defrCallbackType_e c)
{
  if (c >= 0 && c <= defrDesignEndCbkType)
  {
    // OK
  }
  else
  {
    LOGCRITICAL("Callback type is out of bounds!");
  }
}

int dnameCB(defrCallbackType_e c, const char* string, defiUserData ud)
{
  checkType_def(c);

  HDesign& design = *((DEFParserData*)ud)->Design;
  design.Circuit.SetName(string);

  return PARSE_OK;
} 

int unitsCB(defrCallbackType_e c, double number, defiUserData ud)
{
  checkType_def(c);

  HDesign& design = *((DEFParserData*)ud)->Design;
  design.Circuit.SetDefDistanceDBU(number);

  return PARSE_OK;
}

int dieAreaCB(defrCallbackType_e c, defiBox *box, defiUserData ud)
{
  checkType_def(c);

  DEFParserData& def = *((DEFParserData*)ud);
  HDesign& design = *def.Design;
  design.Circuit.SetMinX(
    design.Circuit.XScaleFactor()
    * box->xl()
    * def.distance_to_nm_factor
    / design.Circuit.DefDistanceDBU());
  design.Circuit.SetMinY(
    design.Circuit.YScaleFactor()
    * box->yl()
    * def.distance_to_nm_factor 
    / design.Circuit.DefDistanceDBU());
  design.Circuit.SetWidth(
    design.Circuit.XScaleFactor()
    * (box->xh() - box->xl())
    * def.distance_to_nm_factor
    / design.Circuit.DefDistanceDBU());
  design.Circuit.SetHeight(
    design.Circuit.YScaleFactor()
    * (box->yh() - box->yl())
    * def.distance_to_nm_factor
    / design.Circuit.DefDistanceDBU());

  design.Circuit.SetPlacementMinX(design.Circuit.MinX());
  design.Circuit.SetPlacementMinY(design.Circuit.MinY());
  design.Circuit.SetPlacementMaxX(design.Circuit.MinX() + design.Circuit.Width());
  design.Circuit.SetPlacementMaxY(design.Circuit.MinY() + design.Circuit.Height());

  return PARSE_OK;
}

int componentStartCB(defrCallbackType_e c, int number, defiUserData ud)
{
  checkType_def(c);
  ALERT("\tCells parsing started...");

  ((DEFParserData*)ud)->cBuilder.CellsStart(
    (int)(number * 
    ((DEFParserData*)ud)->Design->cfg.ValueOf(".cellsReservePercent", 1.25)));

  return PARSE_OK;
}

int componentEndCB(defrCallbackType_e c, void* v, defiUserData ud)
{
  checkType_def(c);

  ((DEFParserData*)ud)->cBuilder.CellsFinished();

  ALERT("\tCells parsing finished.");
  return PARSE_OK;
}

int componentCB(defrCallbackType_e c, defiComponent *comp, defiUserData ud)
{
  checkType_def(c);

  DEFParserData& def = *((DEFParserData*)ud);
  Builders::CellsBuilder& cb = def.cBuilder;
  HDesign& design = *def.Design;

  cb.CellStart();

  HMacroType type = Utils::FindMacroTypeByName(design, comp->name());
  if(::IsNull(type))
    GLOGWARNING(LOGINPLACE, "Macro %s has unknown type: %s", comp->id(), comp->name());

  cb->SetType(type);
  if(comp->isFixed())
    cb->SetPlacementStatus(PlacementStatus_Fixed);
  else
    cb->SetPlacementStatus(PlacementStatus_Movable);

  cb->SetHeight(design.GetDouble<HMacroType::SizeY>(type));
  cb->SetWidth(design.GetDouble<HMacroType::SizeX>(type));
  cb->SetName(comp->id());
  cb->SetX(
    design.Circuit.XScaleFactor()
    * comp->placementX()
    * def.distance_to_nm_factor
    / design.Circuit.DefDistanceDBU());
  cb->SetY(
    design.Circuit.YScaleFactor()
    * comp->placementY()
    * def.distance_to_nm_factor
    / design.Circuit.DefDistanceDBU());
  cb->SetOrientation(Utils::GetCellOrient(comp->placementOrientStr()));

  cb.CellFinished();
  return PARSE_OK;
}

int pinStartCB(defrCallbackType_e c, int number, defiUserData ud)
{
  checkType_def(c);
  ALERT("\tPins parsing started...");

  ((DEFParserData*)ud)->pBuilder.PinsStart(number);

  return PARSE_OK;
}

int pinEndCB(defrCallbackType_e c, void* v, defiUserData ud)
{
  checkType_def(c);

  ((DEFParserData*)ud)->pBuilder.PinsFinished();

  ALERT("\tPins parsing finished.");
  return PARSE_OK;
}

int pinCB(defrCallbackType_e c, defiPin *pin, defiUserData ud)
{
  checkType_def(c);

  DEFParserData& def = *((DEFParserData*)ud);
  Builders::PinsBuilder& pb = def.pBuilder;
  HDesign& design = *def.Design;

  pb.PrimaryPinStart();

  PinDirection dir = Utils::GetPinDirection(pin->direction());
  if(dir == PinDirection_INPUT)
    pb->SetDirection(PinDirection_OUTPUT);
  else if(dir == PinDirection_OUTPUT)
    pb->SetDirection(PinDirection_INPUT);
  else
    pb->SetDirection(PinDirection_INOUT);

  pb->SetOffsetX(
    design.Circuit.XScaleFactor()
    * pin->placementX()
    * def.distance_to_nm_factor
    / design.Circuit.DefDistanceDBU());
  pb->SetOffsetY(
    design.Circuit.YScaleFactor()
    * pin->placementY()
    * def.distance_to_nm_factor
    / design.Circuit.DefDistanceDBU());
  pb->SetName(pin->pinName());

  def.nets_primaries[pin->netName()] = *(pb.operator->());

  pb.PinFinished();
  return PARSE_OK;
}

int netStartCB(defrCallbackType_e c, int number, defiUserData ud)
{
  checkType_def(c);
  ALERT("\tNets parsing started...");

  DEFParserData& def = *((DEFParserData*)ud);
  def.cBuilder.CompleteBuild();
  def.pBuilder.CompleteBuild();

  def.nBuilder.NetsStart(
    (int)(number 
    * def.Design->cfg.ValueOf(".netsReservePercent", 1.25)));

  return PARSE_OK;
}

int netEndCB(defrCallbackType_e c, void* v, defiUserData ud)
{
  checkType_def(c);

  ((DEFParserData*)ud)->nBuilder.NetsFinished();
  
  ALERT("\tNets parsing finished.");
  return PARSE_OK;
}


int netCB(defrCallbackType_e c, defiNet *net, defiUserData ud)
{
  checkType_def(c);
  TemplateTypes<HPin>::queue net_pins;

  DEFParserData& def = *((DEFParserData*)ud);
  HDesign& design = *((DEFParserData*)ud)->Design;

  std::map<string, HPin>::iterator net_pin = def.nets_primaries.find(net->name());
  string _pin_name = "";
  if(net_pin != def.nets_primaries.end())
  {
    HPin p = (*net_pin).second;
    _pin_name = design.GetString<HPin::Name>(p);
    net_pins.push(p);
    def.nets_primaries.erase(net_pin);    
  }
  for(int i = 0; i < net->numConnections(); i++)
  {
    if(strcmp("PIN", net->instance(i)) != 0)
    {
      HPin pin = Utils::FindCellPinByName(design, Utils::FindCellByName(design, net->instance(i)), net->pin(i));
      if(!::IsNull(pin))
        net_pins.push(pin);
      else
        GLOGWARNING(LOGINPLACE, "Pin \"%s %s\" not found.", net->instance(i), net->pin(i));
    }
    else
    {
      if(_pin_name == net->pin(i)) continue;
      
      HPin primary = Utils::FindPrimaryPinByName(design, net->pin(i));

      if(::IsNull(primary))
      {
        GLOGWARNING(LOGINPLACE, "Primary Pin \"%s %s\" not found.", net->instance(i), net->pin(i));
      }
      else
        net_pins.push(primary);
    }
  }

  if(net_pins.size() < 2)
  {
    GLOGWARNING(LOGINPLACE, "Net %s has less then 2 pins. Net is skipped.", net->name());
    return PARSE_OK;
  }

  Builders::NetsBuilder& nb = def.nBuilder;

  nb.NetStart((int)net_pins.size());

  nb->SetName(net->name());
  nb->SetKind(NetKind_Default);

  while(!net_pins.empty())
  {
    if(design.Get<HPin::Direction, PinDirection>(net_pins.front()) != PinDirection_INPUT
      && !nb.IsSourceAdded())
    {
      nb.SetSource(net_pins.front());
      net_pins.pop();
    }
    else
    {
      nb.AddSink(net_pins.front());
      net_pins.pop();
      
      if(!nb.CanAddSink() && !nb.IsSourceAdded())
        if(design.Get<HPin::Direction, PinDirection>(net_pins.front()) == PinDirection_INPUT)
        {
          GLOGERROR(LOGINPLACE, "Net %s has no source.", net->name());
          design.Set<HPin::Direction>(net_pins.front(), PinDirection_OUTPUT);
        }
    }
  }

  nb.NetFinished();

  return PARSE_OK;
}

int rowCB(defrCallbackType_e c, defiRow *row, defiUserData ud)
{//TODO: scale sites count if needed
  checkType_def(c);

  DEFParserData& def = *((DEFParserData*)ud);
  Builders::PlacementRowsBuilder& prb = def.prBuilder;
  HDesign& design = *def.Design;

  prb.RowStart();

  prb->SetName(row->name());
  HSite site = design.Sites[row->macro()];
  if (::IsNull(site))
    site = design.Sites[design.cfg.ValueOf("tech_mapping.defaultSiteName",(const char*)"")];
  prb->SetSite(site);
  prb->SetOrientation((CellOrientation) row->orient());
  prb->SetX(
    design.Circuit.XScaleFactor()
    * row->x()
    * def.distance_to_nm_factor
    / design.Circuit.DefDistanceDBU());
  prb->SetY(
    design.Circuit.YScaleFactor()
    * row->y()
    * def.distance_to_nm_factor
    / design.Circuit.DefDistanceDBU());
  if(row->hasDo())
  {
    prb->SetHorizSitesCount((int)row->xNum());
    prb->SetVertSitesCount((int)row->yNum());
    if(row->hasDoStep())
    {
      prb->SetSiteWidth(
        design.Circuit.XScaleFactor()
        * row->xStep()
        * def.distance_to_nm_factor
        / design.Circuit.DefDistanceDBU());
      prb->SetSiteHeight(
        design.Circuit.YScaleFactor()
        * row->yStep()
        * def.distance_to_nm_factor
        / design.Circuit.DefDistanceDBU());
    }
    else
    {
      prb->SetSiteWidth(design.GetDouble<HSite::Width>(prb->Site()));
      prb->SetSiteHeight(design.GetDouble<HSite::Height>(prb->Site()));
    }
  }
  else
  {
    prb->SetHorizSitesCount(1);
    prb->SetVertSitesCount(1);
  }

  prb.RowFinished();
  return PARSE_OK;
}

void* mallocCB_def(int size)
{
  return malloc(size);
}

void* reallocCB_def(void* name, int size)
{
  return realloc(name, size);
}

void freeCB_def(void* name)
{
  free(name);
}

int siteCB(defrCallbackType_e c, defiSite *site, defiUserData ud)
{//TODO: scale sites count if needed
  checkType_def(c);

  DEFParserData& def = *((DEFParserData*)ud);
  Builders::PlacementRowsBuilder& prb = def.prBuilder;
  HDesign& design = *def.Design;

  prb.RowStart();

  prb->SetName("SITE_ROW");
  HSite hsite = design.Sites[site->name()];
  if (::IsNull(hsite))
    hsite = design.Sites[design.cfg.ValueOf("tech_mapping.defaultSiteName",(const char*)"")];
  prb->SetSite(hsite);
  prb->SetOrientation((CellOrientation) site->orient());
  prb->SetX(
    design.Circuit.XScaleFactor()
    * site->x_orig()
    * def.distance_to_nm_factor
    / design.Circuit.DefDistanceDBU());
  prb->SetY(
    design.Circuit.YScaleFactor()
    * site->y_orig()
    * def.distance_to_nm_factor
    / design.Circuit.DefDistanceDBU());
  prb->SetHorizSitesCount((int)site->x_num());
  prb->SetVertSitesCount((int)site->y_num());
  prb->SetSiteWidth(
    design.Circuit.XScaleFactor()
    * site->x_step()
    * def.distance_to_nm_factor
    / design.Circuit.DefDistanceDBU());
  prb->SetSiteHeight(
    design.Circuit.YScaleFactor()
    * site->y_step()
    * def.distance_to_nm_factor
    / design.Circuit.DefDistanceDBU());

  prb.RowFinished();
  return PARSE_OK;
}

void ParseDEF(HDesign& design)
{
  ConfigContext ctx(design.cfg.OpenContext("DEFParser"));

  DEFParserData userData(&design);

  //dlobal design parameters
  defrSetDesignCbk(dnameCB);
  defrSetUnitsCbk(unitsCB);
  defrSetDieAreaCbk(dieAreaCB);
  
  //placement rows
  defrSetRowCbk(rowCB);
  defrSetSiteCbk(siteCB);

  //nets
  defrSetNetStartCbk(netStartCB);
  defrSetNetCbk(netCB);
  defrSetNetEndCbk(netEndCB);
  
  //primary pads
  defrSetStartPinsCbk(pinStartCB);
  defrSetPinCbk(pinCB);
  defrSetPinEndCbk(pinEndCB);

  //components (=cells)
  defrSetComponentStartCbk(componentStartCB);
  defrSetComponentCbk(componentCB);
  defrSetComponentEndCbk(componentEndCB);

  //parser configuration
  defrSetMallocFunction(mallocCB_def);
  defrSetReallocFunction(reallocCB_def);
  defrSetFreeFunction(freeCB_def);
  defrSetCaseSensitivity(false);

  userData.prBuilder.RowsStart(
    design.cfg.ValueOf(".placementRowsStartCount", 100));

  static int __init_reader_code = defrInit();

  defrReset();

  string defFileName = design.cfg.ValueOf("benchmark.def");
  ALERT("Going to read DEF file: %s", defFileName.c_str());

  FILE* defFile = fopen(defFileName.c_str(), "r");
  CRITICAL_ASSERT(defFile != NULL);

  ALERT("DEF file %s parsing started...", defFileName.c_str());
  
  int defReaderStatus = defrRead(defFile, defFileName.c_str(), (void*)(&userData), 0);
  ASSERT(defReaderStatus == PARSE_OK);

  fclose(defFile);

  defrReleaseNResetMemory();

  ALERT("DEF parsing finished.");
}
