#ifdef WIN32
#pragma warning (disable : 4786)
#pragma warning (disable : 4996)
#endif

#include "../include/data_structures.h"
#include "../include/global.h"
#include <stdio.h>
#include <malloc.h>
#include <ctime>
#include "../include/DEF/defwWriter.hpp"
#include "../include/DEF/defwWriterCalls.hpp"

struct DEFexportData
{
    Circuit* circuit;
    int PinsStart;
    double Factor;
};


#define CHECK_STATUS(status) \
  if (status) {              \
     defwPrintError(status); \
     return(status);         \
  }

int orientStatus(const char *orient)
{
    if(orient[0] == 0) return 0;
    if(strcmp(orient,"N") == 0) return 0;
    else if(strcmp(orient,"W") == 0) return 1;
    else if(strcmp(orient,"S") == 0) return 2;
    else if(strcmp(orient,"E") == 0) return 3;
    else if(strcmp(orient,"FN") == 0) return 4;
    else if(strcmp(orient,"FW") == 0) return 5;
    else if(strcmp(orient,"FS") == 0) return 6;
    else if(strcmp(orient,"FE") == 0) return 7;
    else return -1;

/*      case 0: return ((char*)"N");
      case 1: return ((char*)"W");
      case 2: return ((char*)"S");
      case 3: return ((char*)"E");
      case 4: return ((char*)"FN");
      case 5: return ((char*)"FW");
      case 6: return ((char*)"FS");
      case 7: return ((char*)"FE");*/

}

void checkType_defw(defwCallbackType_e c)
{
    if (c >= 0 && c <= defwDesignEndCbkType)
    {
        // OK
    }
    else
    {
        cout << "ERROR: callback type is out of bounds!" << endl;
    }
}

int designCB(defwCallbackType_e c, defiUserData ud)
{
    int status;
    checkType_defw(c);
    DEFexportData *data = (DEFexportData*)ud;
    char name[256];
    strcpy(name, data->circuit->designName.name);
    time_t t = time(0);
    tm* ft = gmtime(&t);
    strftime(name+strlen(name),256,"%Y%m%d%H%M%S",ft);
    status = defwDesignName(name);
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
    DEFexportData *data = (DEFexportData*)ud;
    status = defwDieArea((int)(data->circuit->shiftX * data->Factor),
        (int)(data->circuit->shiftY * data->Factor),
        (int)((data->circuit->shiftX + data->circuit->width) * data->Factor),
        (int)((data->circuit->shiftY + data->circuit->height) * data->Factor));
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
    status = defwUnits(data->circuit->defConvertFactor);
    CHECK_STATUS(status);
    status = defwNewLine();
    CHECK_STATUS(status);
    return 0;
}

int componentCB(defwCallbackType_e c, defiUserData ud)
{
    int status;
    checkType_defw(c);
    DEFexportData *data = (DEFexportData*)ud;

    status = defwStartComponents(data->circuit->nNodes + data->PinsStart);
    CHECK_STATUS(status);

    int i = 0;
    for(; i < data->circuit->nNodes; i++)
    {
        status = defwComponent(
            data->circuit->tableOfNames[i].name,
            data->circuit->nodes[i].type->Name.c_str(),
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
            "PLACED",
            (int)(data->circuit->placement[i].xCoord * data->Factor),
            (int)(data->circuit->placement[i].yCoord * data->Factor),
            orientStatus(data->circuit->placement[i].orient),
            0,
            NULL,
            0,
            0,
            0,
            0);
        CHECK_STATUS(status);
    }
	i = (int)(data->circuit->terminals - data->circuit->nodes);
    int j = 0;
    for(; j < data->PinsStart; j++)
    {
        status = defwComponent(
            data->circuit->tableOfNames[i+j].name,
            data->circuit->terminals[j].type->Name.c_str(),
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
            "FIXED",
            (int)(data->circuit->placement[i+j].xCoord * data->Factor),
            (int)(data->circuit->placement[i+j].yCoord * data->Factor),
            orientStatus(data->circuit->placement[i+j].orient),
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
    DEFexportData *data = (DEFexportData*)ud;
    
    status = defwStartPins(data->circuit->nTerminals - data->PinsStart);
    CHECK_STATUS(status);

	int offset = (int)(data->circuit->terminals - data->circuit->nodes);
    for(int j = data->PinsStart; j < data->circuit->nTerminals; j++)
    {
        PinInfo * info = data->circuit->tech->SinglePins->Pins[data->circuit->tableOfNames[offset + j].name];
        const char *dir = info->dir == PinInfo::INPUT ? "INPUT" 
            : (info->dir == PinInfo::OUTPUT ? "OUTPUT" : "INOUT");
        status = defwPin(
            info->Name.c_str(),
            info->Name.c_str(),
            (int)info->isSpecial,
            dir,
            NULL,
            "COVER",
            (int)(info->OriginX * data->Factor),
            (int)(info->OriginY * data->Factor),
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
    DEFexportData *data = (DEFexportData*)ud;
	int offset = (int)(data->circuit->terminals - data->circuit->nodes);

    status = defwStartNets(data->circuit->nNets);
    CHECK_STATUS(status);

    for(int i = 0; i < data->circuit->nNets; i++)
    {
        
        status = defwNet(data->circuit->nets[i].name);
        CHECK_STATUS(status);
        
        for(int j = 0; j < data->circuit->nets[i].numOfPins; j++)
        {
            int idx = data->circuit->nets[i].arrPins[j].cellIdx;
            const char *cellname = idx > offset + data->PinsStart
                ? "PIN" : data->circuit->tableOfNames[idx].name;
            status = defwNetConnection(
                cellname,
                data->circuit->nets[i].arrPins[j].type->Name.c_str(),
                0);
            CHECK_STATUS(status);
        }

        status = defwNetEndOneNet();
        CHECK_STATUS(status);
    }

    status = defwEndNets();
    CHECK_STATUS(status);
    return 0;
}

void ShiftCoordinates(Circuit& circuit, bool tocenter);

int ExportDEF(const char* defname, Circuit& circuit)
{
    FILE * fout;
    if ((fout = fopen(defname, "w")) == 0)
    {
        cout << "ERROR: could not open output file" << endl;
        return 2;
    }

    DEFexportData userData;
    userData.circuit = &circuit;
    for(userData.PinsStart = 0; 
        circuit.terminals[userData.PinsStart].type != circuit.tech->SinglePins
            && userData.PinsStart < circuit.nTerminals;
        userData.PinsStart++);
    userData.Factor = circuit.defConvertFactor / 1000.0;

    int status = defwInitCbk(fout);
    CHECK_STATUS(status);

    defwSetComponentCbk(componentCB);
    defwSetDesignCbk(designCB);
    defwSetDesignEndCbk(designendCB);
    defwSetDieAreaCbk(dieareaCB);
    defwSetNetCbk(netCB);
    defwSetPinCbk(pinCB);
    defwSetUnitsCbk(unitsCB);

    ShiftCoordinates(circuit,false);
    status = defwWrite(fout, defname, (void*)(&userData));
    ShiftCoordinates(circuit,true);
    CHECK_STATUS(status);

    fclose(fout);
    return 0;
}