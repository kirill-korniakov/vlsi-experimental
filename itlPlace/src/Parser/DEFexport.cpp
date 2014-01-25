#include "HDesign.h"
#include "Utils.h"
#include "Auxiliary.h"
#include <stdio.h>
//#include <malloc.h>
#include <ctime>

#include "defwWriter.hpp"
#include "defwWriterCalls.hpp"

///TODO: think about list of parameters for export
struct DEFexportData
{
  HDesign* design;
  bool useOriginalNames;

  double distance_to_nm_factor;

  DEFexportData()
  {
    distance_to_nm_factor = 1e3;
  }
};


#define CHECK_STATUS(status) \
  if (status) {              \
  defwPrintError(status); \
  return(status);         \
  }

void checkType_defw(defwCallbackType_e c)
{
  if (c >= 0 && c <= defwDesignEndCbkType)
  {
    // OK
  }
  else
  {
    printf("ERROR: callback type is out of bounds!\n");
  }
}

int versionCB(defwCallbackType_e c, defiUserData ud)
{
  int status;
  checkType_defw(c);
  defwAddComment("Written by itlPlace");
  defwAddComment("NNSU 2007-2009");
  status = defwVersion(5, 6);//todo: save version somewhere and print here
  CHECK_STATUS(status);
  status = defwNewLine();
  CHECK_STATUS(status);
  return 0;
}

int designCB(defwCallbackType_e c, defiUserData ud)
{
  int status;
  checkType_defw(c);
  DEFexportData *data = (DEFexportData*)ud;

  status = defwDesignName(data->design->Circuit.Name().c_str());
  CHECK_STATUS(status);

  char buf[256];
  time_t t = time(0);
  tm* ft = gmtime(&t);
  strftime(buf, 256, "Timestamp: %Y.%m.%dT%H:%M:%S", ft);
  defwAddComment(buf);
  sprintf(buf,"Wire length: %f", Utils::CalculateHPWL(*data->design, false));
  defwAddComment(buf);

  status = defwNewLine();
  CHECK_STATUS(status);
  return 0;
}

int designendCB(defwCallbackType_e c, defiUserData ud)
{
  int status;
  checkType_defw(c);
  status = defwEnd();
  CHECK_STATUS(status);
  return 0;
}

int dieareaCB(defwCallbackType_e c, defiUserData ud)
{
  int status;
  checkType_defw(c);
  DEFexportData& def = *((DEFexportData*)ud);
  status = defwDieArea(
    (int)(def.design->Circuit.MinX()
    * def.design->Circuit.DefDistanceDBU()
    / def.distance_to_nm_factor),
    (int)(def.design->Circuit.MinY()
    * def.design->Circuit.DefDistanceDBU()
    / def.distance_to_nm_factor),
    (int)((def.design->Circuit.MinX() + def.design->Circuit.Width())
    * def.design->Circuit.DefDistanceDBU()
    / def.distance_to_nm_factor),
    (int)((def.design->Circuit.MinY() + def.design->Circuit.Height())
    * def.design->Circuit.DefDistanceDBU()
    / def.distance_to_nm_factor));
  CHECK_STATUS(status);
  status = defwNewLine();
  CHECK_STATUS(status);
  return 0;
}

int unitsCB(defwCallbackType_e c, defiUserData ud)
{
  int status;
  checkType_defw(c);
  DEFexportData *data = (DEFexportData*)ud;
  status = defwUnits((int)data->design->Circuit.DefDistanceDBU());
  CHECK_STATUS(status);
  status = defwNewLine();
  CHECK_STATUS(status);
  return 0;
}

int componentCB(defwCallbackType_e c, defiUserData ud)
{
  int status;
  checkType_defw(c);
  DEFexportData& def = *((DEFexportData*)ud);

  status = defwStartComponents(def.design->Cells.CellsCount());
  CHECK_STATUS(status);

  for(HCells::CellsEnumeratorW i = def.design->Cells.GetEnumeratorW(); i.MoveNext(); )
  {
    if (i.PlacementStatus() != PlacementStatus_Movable
       && i.PlacementStatus() != PlacementStatus_Fixed)
       continue;

    status = defwComponent(
      i.Name().c_str(),
      def.useOriginalNames
        ? def.design->GetString<HMacroType::OriginalName>(i.Type()).c_str()
        : def.design->GetString<HMacroType::Name>(i.Type()).c_str(),
      0,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      0,
      NULL,
      NULL,
      NULL,
      NULL,
      i.IsTerminal() ? "FIXED" : "PLACED",
      (int)(i.X() * def.design->Circuit.DefDistanceDBU() / def.distance_to_nm_factor),
      (int)(i.Y() * def.design->Circuit.DefDistanceDBU() / def.distance_to_nm_factor),
      (int)i.Orientation(),
      0,
      NULL,
      0,
      0,
      0,
      0);
    CHECK_STATUS(status);
  }

  status = defwEndComponents();
  CHECK_STATUS(status);
  return 0;
}

int pinCB(defwCallbackType_e c, defiUserData ud)
{
  int status;
  checkType_defw(c);
  DEFexportData& def = *((DEFexportData*)ud);

  status = defwStartPins(def.design->Pins.PrimariesCount());
  CHECK_STATUS(status);

  for (HPins::PrimariesEnumeratorW i = def.design->Pins.GetEnumeratorW(); i.MoveNext(); )
  {
    status = defwPin(
      i.Name().c_str(),
      i.Name().c_str(),
      0, ///TODO: probably we need to work with this field
      i.IsPrimaryInput() ? "INPUT" : "OUTPUT",
      NULL,
      "COVER",
      (int)(i.OffsetX() * def.design->Circuit.DefDistanceDBU() / def.distance_to_nm_factor),
      (int)(i.OffsetY() * def.design->Circuit.DefDistanceDBU() / def.distance_to_nm_factor),
      0,
      NULL,
      0,
      0,
      0,
      0);
    CHECK_STATUS(status);
  }

  status = defwEndPins();
  CHECK_STATUS(status);
  return 0;
}

int netCB(defwCallbackType_e c, defiUserData ud)
{
  int status;
  checkType_defw(c);
  DEFexportData& def = *((DEFexportData*)ud);

  status = defwStartNets(def.design->Nets.Count(NetKind_Active) + def.design->Nets.Count(NetKind_Skipped));
  CHECK_STATUS(status);

  for (HNets::NetsEnumeratorW i = def.design->Nets.GetFullEnumeratorW(); i.MoveNext(); )
  {
    if(i.Kind() != NetKind_Active && i.Kind() != NetKind_Skipped) continue;

    status = defwNet(i.Name().c_str());
    CHECK_STATUS(status);

    for (HNet::PinsEnumeratorW piter = i.GetPinsEnumeratorW(); piter.MoveNext(); )
    {
      if (piter.IsPrimary())
      {
        status = defwNetConnection(
          "PIN",
          piter.Name().c_str(),
          0);
      }
      else
      {
        ASSERT((def.design->Get<HCell::PlacementStatus, PlacementStatus>(piter.Cell()) != PlacementStatus_Fictive));
        status = defwNetConnection(
          def.design->GetString<HCell::Name>(piter.Cell()).c_str(),
          def.useOriginalNames
            ? def.design->GetString<HPinType::OriginalName>(piter.Type()).c_str()
            : def.design->GetString<HPinType::Name>(piter.Type()).c_str(),
          0);
      }     
      CHECK_STATUS(status);
    }

    status = defwNetEndOneNet();
    CHECK_STATUS(status);
  }

  status = defwEndNets();
  CHECK_STATUS(status);
  return 0;
}

int rowCB( defwCallbackType_e c, defiUserData ud)
{
  int status;
  checkType_defw(c);
  DEFexportData& def = *((DEFexportData*)ud);
  HDesign& design = *def.design;

  for(HPlacementRows::Enumerator i = design.PlacementRows.GetEnumerator(); i.MoveNext(); )
  {
    HPlacementRowWrapper row = design[i];
    status = defwRow(row.Name().c_str(),
      design.GetString<HSite::Name>(row.Site()).c_str(),
      (int)(row.X() * design.Circuit.DefDistanceDBU() / def.distance_to_nm_factor),
      (int)(row.Y() * design.Circuit.DefDistanceDBU() / def.distance_to_nm_factor),
      (int)row.Orientation(),
      row.HorizSitesCount(),
      row.VertSitesCount(),
      (int)(row.SiteWidth() * design.Circuit.DefDistanceDBU() / def.distance_to_nm_factor),
      (int)(row.SiteHeight() * design.Circuit.DefDistanceDBU() / def.distance_to_nm_factor)
      );
    CHECK_STATUS(status);
  }

  return 0;
}

string GetExportDEFFileName(HDesign& hd, const string& defName)
{
  string fileName = defName;

  if (fileName == "")
    fileName = Aux::CreateCoolFileName("./ExportDEF/",  hd.Circuit.Name(), ".def");
  else
    fileName = "./ExportDEF/" + defName;

  return fileName;
}

void ExportDEF(HDesign& design, const string& defName, bool useOriginalNames)
{
  ConfigContext ctx(design.cfg.OpenContext("DEFExport"));

  WRITELINE("");
  //ALERT("DEF export");

  FILE * fout;
  if ((fout = fopen(defName.c_str(), "w")) == 0)
  {
    LOGERROR(("Could not open output def file: " + defName).c_str());
    return;
  }

  DEFexportData userData;
  userData.design = &design;
  userData.useOriginalNames = useOriginalNames;

  int status = defwInitCbk(fout);
  ASSERT(status == DEFW_OK);

  defwSetVersionCbk(versionCB);
  defwSetDesignCbk(designCB);
  defwSetUnitsCbk(unitsCB);
  defwSetDieAreaCbk(dieareaCB);
  defwSetComponentCbk(componentCB);
  defwSetPinCbk(pinCB);
  defwSetNetCbk(netCB);    
  defwSetRowCbk(rowCB);
  defwSetDesignEndCbk(designendCB);

  status = defwWrite(fout, defName.c_str(), (void*)(&userData));
  ASSERT(status == DEFW_OK);

  fclose(fout);
  
  ALERT("DEF is written to %s", defName.c_str());
}

void ExportDEF(HDesign& design, const string& defName)
{
  ExportDEF(design, GetExportDEFFileName(design, defName), design.cfg.ValueOf("tech_mapping.enabled", false));
}
