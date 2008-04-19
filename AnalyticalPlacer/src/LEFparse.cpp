#ifdef WIN32
#pragma warning (disable : 4786)
#pragma warning (disable : 4996)
#endif

#include "..\include\data_structures.h"
#include "..\include\global.h"
#include <stdio.h>
#include <malloc.h>

#include "..\include\LEF\\lefrReader.hpp"

void checkType(lefrCallbackType_e c)
{
  if (c >= 0 && c <= lefrLibraryEndCbkType)
  {
    // OK
  }
  else
  {
    cout << "ERROR: callback type is out of bounds!" << endl;
  }
}


//char* orientStr(int orient) {
//  switch (orient) {
//      case 0: return ((char*)"N");
//      case 1: return ((char*)"W");
//      case 2: return ((char*)"S");
//      case 3: return ((char*)"E");
//      case 4: return ((char*)"FN");
//      case 5: return ((char*)"FW");
//      case 6: return ((char*)"FS");
//      case 7: return ((char*)"FE");
//  };
//  return ((char*)"BOGUS");
//}

int layerCB(lefrCallbackType_e c, lefiLayer* layer, lefiUserData ud)
{
    checkType(c);
    if(!layer->lefiLayer::hasType())
        return PARSE_OK;
    if(strcmp(layer->lefiLayer::type(),"ROUTING") != 0)
        return PARSE_OK;
    RoutingLayerInfo* this_layer = new RoutingLayerInfo();
    this_layer->Name = layer->lefiLayer::name();
    if(layer->lefiLayer::hasDirection())
    {
        if(strcmp(layer->lefiLayer::direction(),"HORIZONTAL") == 0)
            this_layer->dir = RoutingLayerInfo::horizontal;
        else if(strcmp(layer->lefiLayer::direction(),"VERTICAL") == 0)
            this_layer->dir = RoutingLayerInfo::vertical;
        else if(strcmp(layer->lefiLayer::direction(),"DIAG45") == 0)
            this_layer->dir = RoutingLayerInfo::diag45;
        else if(strcmp(layer->lefiLayer::direction(),"DIAG135") == 0)
            this_layer->dir = RoutingLayerInfo::diag135;
        else
            this_layer->dir = RoutingLayerInfo::horizontal;
    }
    else
        this_layer->dir = RoutingLayerInfo::horizontal;
    if (layer->lefiLayer::hasEdgeCap())
        this_layer->EdgeCap = layer->lefiLayer::edgeCap() / 1000.0;
    else
        this_layer->EdgeCap = 0;
    if (layer->lefiLayer::hasWidth())
        this_layer->Width = layer->lefiLayer::width() * 1000.0;
    else
    {
        if (layer->lefiLayer::hasMinwidth())
            this_layer->Width = layer->lefiLayer::minwidth() * 1000.0;
        else if (layer->lefiLayer::hasMaxwidth())
            this_layer->Width = layer->lefiLayer::maxwidth() * 1000.0;
        else
            this_layer->Width = 1;
    }
    if(layer->hasPitch())
        this_layer->Pitch = layer->pitch() * 1000.0;
    else
        this_layer->Pitch = this_layer->Width;
    if (layer->lefiLayer::hasResistance())
        this_layer->RPerDist = layer->lefiLayer::resistance() / this_layer->Width;
    else
        this_layer->RPerDist = 0;
    if (layer->lefiLayer::hasCapacitance())
        this_layer->CPerDist = layer->lefiLayer::capacitance() / 1.0e6 * this_layer->Width;
    ((LEFParserData*)ud)->Layers.push_back(this_layer);
    return PARSE_OK;
}

int macroCB(lefrCallbackType_e c, lefiMacro* macro, lefiUserData ud)
{
    checkType(c);
    MacroInfo* CurrentMacro = ((LEFParserData*)ud)->CurrentMacro;
    if (macro->lefiMacro::hasSize())
    {
        CurrentMacro->SizeX = macro->lefiMacro::sizeX() * 1000.0;
        CurrentMacro->SizeY = macro->lefiMacro::sizeY() * 1000.0;
    }
    else
    {
        CurrentMacro->SizeX = 0.0;
        CurrentMacro->SizeY = 0.0;
    }
    if (macro->lefiMacro::hasOrigin())
    {
        CurrentMacro->OriginX = macro->lefiMacro::originX() * 1000.0;
        CurrentMacro->OriginY = macro->lefiMacro::originY() * 1000.0;
    }

    for(std::map<std::string,PinInfo*>::iterator i(CurrentMacro->Pins.begin()); i != CurrentMacro->Pins.end(); ++i)
    {
        i->second->OriginX = CurrentMacro->SizeX / 2.0;
        i->second->OriginY = CurrentMacro->SizeY / 2.0;
    }
    return PARSE_OK;
}

int macroBeginCB(lefrCallbackType_e c, const char* macroName, lefiUserData ud)
{
    checkType(c);
    ((LEFParserData*)ud)->CurrentMacro = new MacroInfo();
    ((LEFParserData*)ud)->CurrentMacro->Name = macroName;
    return PARSE_OK;
}

int macroEndCB(lefrCallbackType_e c, const char* macroName, lefiUserData ud)
{
    checkType(c);
    (*((LEFParserData*)ud)->CellTypes)[((LEFParserData*)ud)->CurrentMacro->Name]
            = ((LEFParserData*)ud)->CurrentMacro;
    return PARSE_OK;
}

int pinCB(lefrCallbackType_e c, lefiPin* pin, lefiUserData ud)
{
    checkType(c);
    PinInfo* pinfo = new PinInfo();
    pinfo->Name = pin->lefiPin::name();
    if (pin->lefiPin::hasDirection())
    {
        if(strcmp(pin->lefiPin::direction(),"INPUT") == 0)
            pinfo->dir = PinInfo::INPUT;
        else if(strcmp(pin->lefiPin::direction(),"OUTPUT") == 0)
            pinfo->dir = PinInfo::OUTPUT;
        else if(strcmp(pin->lefiPin::direction(),"INOUT") == 0)
            pinfo->dir = PinInfo::INOUT;
        else
            pinfo->dir = PinInfo::INPUT;
    }
    else
        pinfo->dir = PinInfo::INPUT;
    if (pin->lefiPin::hasUse())
        pinfo->isSpecial = true;
    else
        pinfo->isSpecial = false;
    if (pin->lefiPin::hasCapacitance())
        pinfo->Capacitance = pin->lefiPin::capacitance();
    else
        pinfo->Capacitance = 0.0;
    if (pin->lefiPin::hasOutResistance())
        pinfo->Resistance = (pin->lefiPin::outResistanceHigh() + pin->lefiPin::outResistanceLow()) / 2.0;
    else
    {
        if (pin->lefiPin::hasResistance())
            pinfo->Resistance = pin->lefiPin::resistance();
        else
            pinfo->Resistance = 0.0;
    }
    pinfo->OriginX = 0.0;
    pinfo->OriginY = 0.0;
    ((LEFParserData*)ud)->CurrentMacro->Pins[pinfo->Name] = pinfo;
    return PARSE_OK;
}

int unitsCB(lefrCallbackType_e c, lefiUnits* unit, lefiUserData ud)
{
    checkType(c);
    DataBaseUnitsMapping* gUnits = ((LEFParserData*)ud)->gUnits;
    if (unit->lefiUnits::hasDatabase())
        gUnits->Distance = (1.0 / unit->databaseNumber()) * 1000.0;
    if (unit->lefiUnits::hasCapacitance())
        gUnits->Capacitance = 1.0 / unit->lefiUnits::capacitance();
    if (unit->lefiUnits::hasResistance())
        gUnits->Resistance = 1.0 / unit->lefiUnits::resistance();
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

LEFParserData* ParseLEF(const char* filename)
{
    LEFParserData* userData = new LEFParserData();
    userData->CellTypes = new std::map<std::string,MacroInfo*>();
    userData->gUnits = new DataBaseUnitsMapping();
    userData->CurrentMacro = 0;
    

    lefrSetMacroBeginCbk(macroBeginCB);
    lefrSetMacroCbk(macroCB);
    lefrSetMacroEndCbk(macroEndCB);
    lefrSetPinCbk(pinCB);
    lefrSetUnitsCbk(unitsCB);
    lefrSetLayerCbk(layerCB);
    //lefrSetRegisterUnusedCallbacks();

    lefrSetMallocFunction(mallocCB);
    lefrSetReallocFunction(reallocCB);
    lefrSetFreeFunction(freeCB);

    lefrSetCaseSensitivity(false);
    lefrSetRelaxMode();
    lefrSetShiftCase();  // will shift name to uppercase if caseinsensitive
                         // is set to off or not set

    static int __init_reader_code = lefrInit();

    lefrReset();
    FILE* f;
    if ((f = fopen(filename,"r")) == 0)
    {
        cout << "Couldn't open input file " << filename << endl;
        return 0;
    }

    if (lefrRead(f, filename, (void*)userData))
    {
        cout << "LEF Reader returns bad status." << endl;
        Exit();
    }

    lefrReleaseNResetMemory();
    userData->CurrentMacro = 0;
    userData->SinglePins = new MacroInfo();
    userData->SinglePins->Name = "PIN";
    userData->SinglePins->OriginX = 0;
    userData->SinglePins->OriginY = 0;
    userData->SinglePins->SizeX = 0;
    userData->SinglePins->SizeY = 0;
    (*(userData->CellTypes))["PIN"] = userData->SinglePins;

    return userData;
}
