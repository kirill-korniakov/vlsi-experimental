#ifdef WIN32
#pragma warning (disable : 4786)
#pragma warning (disable : 4996)
#endif

#include "..\include\data_structures.h"
#include "..\include\global.h"
#include <stdio.h>
#include <malloc.h>

#include "..\include\DEF\defrReader.hpp"

struct DEFParserData
{
    double UnitsFactor;
    int current_index;
    int *indexes;
    int nskipped;
    double area;
    Circuit* circuit;
};

std::map<std::string,PinInfo*> __netd_nets;

int DEFParserSearchNode(const char* id, DEFParserData* data)
{
    int first = 0;
	int last = data->circuit->nNodes+data->circuit->nTerminals;
    while (first <= last)
    {
       int mid = (first + last) / 2;  // compute mid point.
       int cmp = strcmp(id,data->circuit->tableOfNames[data->indexes[mid]].name);
       if (cmp > 0)
           first = mid + 1;  // repeat search in top half.
       else if (cmp < 0) 
           last = mid - 1; // repeat search in bottom half.
       else
           return data->indexes[mid];     // found it. return position /////
    }
    return -1;
}

void checkType_def(defrCallbackType_e c)
{
    if (c >= 0 && c <= defrDesignEndCbkType)
    {
        // OK
    }
    else
    {
        cout << "ERROR: callback type is out of bounds!" << endl;
    }
}

int dnameCB(defrCallbackType_e c, const char* string, defiUserData ud)
{
    checkType_def(c);
    DEFParserData* data = (DEFParserData*)ud;
    data->circuit->designName.~str();
    new ((void*)&(data->circuit->designName)) str(string);
    return PARSE_OK;
} 

int unitsCB(defrCallbackType_e c, double number, defiUserData ud)
{
    checkType_def(c);
    ((DEFParserData*)ud)->UnitsFactor = (1.0 / number) * 1000.0;
    ((DEFParserData*)ud)->circuit->defConvertFactor = (int)number;
    return PARSE_OK;
}

int dieAreaCB(defrCallbackType_e c, defiBox *box, defiUserData ud)
{
    checkType_def(c);
    DEFParserData* data = (DEFParserData*)ud;
    data->circuit->shiftX = box->xl() * data->UnitsFactor;
    data->circuit->shiftY = box->yl() * data->UnitsFactor;
    data->circuit->width = (box->xh() - box->xl()) * data->UnitsFactor;
    data->circuit->height = (box->yh() - box->yl()) * data->UnitsFactor;
    return PARSE_OK;
}

int componentStartCB(defrCallbackType_e c, int number, defiUserData ud)
{
    checkType_def(c);
    cout << "components parsing started..." << endl;
    DEFParserData* data = (DEFParserData*)ud;
    data->circuit->nNodes = number;
    data->circuit->nTerminals = 0;
    data->current_index = 0;
    data->nskipped = 0;
    data->circuit->nodes = new Node[number];
    data->circuit->terminals = data->circuit->nodes + number;
    data->circuit->placement = new Place[number];
    data->circuit->tableOfNames = (str*) new char[number * sizeof(str)];
    return PARSE_OK;
}

int componentCB(defrCallbackType_e c, defiComponent *comp, defiUserData ud)
{
    checkType_def(c);
    DEFParserData* data = (DEFParserData*)ud;
    std::map<std::string,MacroInfo*>::iterator minfo
        = data->circuit->tech->CellTypes->find(comp->name());
    if(minfo == data->circuit->tech->CellTypes->end())
    {
        data->nskipped++;
        cout << "Unknown macro type detected: " << comp->id() << "   "
            << comp->name() << endl;
        return PARSE_OK;
    }
    int index;
    if(comp->isFixed())
    {
        data->circuit->nTerminals++;
        index = data->circuit->nNodes - data->circuit->nTerminals;
    }
    else
    {
        index = data->current_index;
        data->current_index++;
    }
    data->circuit->nodes[index].type = minfo->second;
    data->circuit->nodes[index].width = (int)data->circuit->nodes[index].type->SizeX;
    data->circuit->nodes[index].height = (int)data->circuit->nodes[index].type->SizeY;

    data->area += data->circuit->nodes[index].width * data->circuit->nodes[index].height;

    data->circuit->placement[index].xCoord = comp->placementX() * data->UnitsFactor;
    data->circuit->placement[index].yCoord = comp->placementY() * data->UnitsFactor;
    strcpy(data->circuit->placement[index].orient,comp->placementOrientStr());
    new ((void*)(data->circuit->tableOfNames + index)) str(comp->id());
    return PARSE_OK;
}

int netStartCB(defrCallbackType_e c, int number, defiUserData ud)
{
    checkType_def(c);
    cout << "nets parsing started..." << endl;
    DEFParserData* data = (DEFParserData*)ud;
    data->circuit->nNets = number;
    data->current_index = 0;
    data->circuit->nets = new Net[number * 2 + BUF_COUNT_RESERVE];
    data->circuit->nPins = 0;

    int pins = (int)data->circuit->tech->SinglePins->Pins.size();
    int allnodes = data->circuit->nNodes + pins - data->nskipped;
    int plnodes = data->circuit->nNodes - data->circuit->nTerminals - data->nskipped;

	  int node_space = allnodes * 2 + BUF_COUNT_RESERVE+1;
	  int first_terminal = node_space - data->circuit->nTerminals - pins;
    data->circuit->Shift_ = first_terminal;

    Node* nodes = new Node[node_space];
    {
        memcpy(nodes, data->circuit->nodes, sizeof(Node) * plnodes);
        memcpy(nodes + first_terminal, data->circuit->nodes + plnodes + data->nskipped, 
            sizeof(Node) * data->circuit->nTerminals);
        delete[] data->circuit->nodes;
        data->circuit->nodes = nodes;
        data->circuit->terminals = nodes + first_terminal;
    }
    Place* place = new Place[node_space];
    {
        memcpy(place, data->circuit->placement, sizeof(Place) * plnodes);
        memcpy(place + first_terminal, data->circuit->placement + plnodes + data->nskipped,
            sizeof(Place) * data->circuit->nTerminals);
        delete[] data->circuit->placement;
        data->circuit->placement = place;
    }
    str* names = (str*) new char[sizeof(str) * node_space];
    {
        memcpy(names, data->circuit->tableOfNames, sizeof(str) * plnodes);
        memcpy(names + first_terminal, data->circuit->tableOfNames + plnodes + data->nskipped,
            sizeof(str) * data->circuit->nTerminals);
        delete[] (char*)data->circuit->tableOfNames;
        data->circuit->tableOfNames = names;
    }
    //data->circuit->nNodes -= data->nskipped;
	int terminal_index = first_terminal + data->circuit->nTerminals;
    for(std::map<std::string,PinInfo*>::iterator iter = data->circuit->tech->SinglePins->Pins.begin();
		iter != data->circuit->tech->SinglePins->Pins.end(); ++iter, terminal_index++)
    {
        nodes[terminal_index].height = 0;
        nodes[terminal_index].width = 0;
        nodes[terminal_index].type = data->circuit->tech->SinglePins;
        place[terminal_index].xCoord = iter->second->OriginX;
        place[terminal_index].yCoord = iter->second->OriginY;
        strcpy(place[terminal_index].orient, "N");
        new ((void*)(names + terminal_index)) str(iter->first.c_str());
    }//data->circuit->nNodes == allnodes;
    data->circuit->nTerminals += pins;
	data->circuit->nNodes = plnodes;
    data->indexes = new int[allnodes];
	int i = 0;
    for(; i < plnodes; i++)
        data->indexes[i] = i;
	for(int j = first_terminal; j < node_space; j++,i++)
        data->indexes[i] = j;

    int Index,Index1, ContrIndex,ContrIndex1,N;
    int Y;
    long int Stack[32][2];// = ((0,0));
    for (int i = 1; i < 32; i++) Stack[i][0] = Stack[i][1] = -1;
    Stack[0][0] = 0; Stack[0][1] = allnodes -1;
    N = 0;
    while (N >= 0)
    {
        Index1 = Index = Stack[N][0];
        ContrIndex1 = ContrIndex = Stack[N][1];
        Y = data->indexes[Index];
        while (Index != ContrIndex)
        {
            if (strcmp(data->circuit->tableOfNames[Y].name,
                data->circuit->tableOfNames[data->indexes[ContrIndex]].name) > 0)//(Y>data[ContrIndex])
            {
                data->indexes[Index] = data->indexes[ContrIndex];
                data->indexes[ContrIndex] = Y;
                goto Sort7L1;
            }
Sort7L2:    ContrIndex--;
        }
        while (Index != ContrIndex)
        {
            if (strcmp(data->circuit->tableOfNames[data->indexes[Index]].name,
                data->circuit->tableOfNames[Y].name) > 0)//(data[Index]>Y)
            {
                data->indexes[ContrIndex] = data->indexes[Index];
                data->indexes[Index] = Y;
                goto Sort7L2;
            }
Sort7L1:    Index++;
        }
        if ((2 * Index - Index1 - ContrIndex1 - 2) > 0)
        {
            if (Index1 < Index - 1)
            {
                Stack[N][1] = Index - 1;
                N++;
            }
            if (Index + 1 < ContrIndex1)
            {
                Stack[N][0] = Index + 1;
                Stack[N][1] = ContrIndex1;
                N++;
            }
        }
        else
        {
            if (Index + 1 < ContrIndex1)
            {
                Stack[N][0] = Index + 1;
                Stack[N][1] = ContrIndex1;
                N++;
            }
            if (Index1 < Index - 1)
            {
                Stack[N][0] = Index1;
                Stack[N][1] = Index - 1;
                N++;
            }
        }
        N--;
    }

    return PARSE_OK;
}

int netCB(defrCallbackType_e c, defiNet *net, defiUserData ud)
{
    checkType_def(c);
    DEFParserData* data = (DEFParserData*)ud;
 /*   if(net->numConnections() < 2)
    {
        data->circuit->nNets--;
        cout << "Net " << net->name() << " skipped." << endl;
        return PARSE_OK;
    }*/
    data->circuit->nets[data->current_index].numOfPins = net->numConnections();
    data->circuit->nets[data->current_index].name = new char[strlen(net->name())+1];
    strcpy(data->circuit->nets[data->current_index].name, net->name());
    data->circuit->nPins += net->numConnections();

    std::map<std::string,PinInfo*>::iterator __net = __netd_nets.find(net->name());
    PinInfo* __net_pin = 0;
    if(__net != __netd_nets.end())
      __net_pin = __net->second;
    int numberOfSources = 0;
    Pin* arr
        = data->circuit->nets[data->current_index].arrPins
        = new Pin[net->numConnections()+1];
    for(int i = 0; i < net->numConnections(); i++)
    {
        int nodeindex;
        bool _is_PIN = false;
        if(strcmp("PIN", net->instance(i)) != 0)
        {
            nodeindex = DEFParserSearchNode(net->instance(i), data);
            _is_PIN = true;
        }
        else
            nodeindex = DEFParserSearchNode(net->pin(i), data);
        if(nodeindex < 0)
        {
            cout << "Bad node detected: " << net->instance(i) << "   "
                << net->pin(i) << endl;
            data->circuit->nets[data->current_index].numOfPins--;
            data->circuit->nPins--;
            arr--;
            continue;
        }
        arr[i].cellIdx = nodeindex;
        std::map<std::string, PinInfo*>::iterator pin_iter
            = data->circuit->nodes[nodeindex].type->Pins.find(net->pin(i));
        if(pin_iter == data->circuit->nodes[nodeindex].type->Pins.end())
        {
            cout << "Bad pin detected: " << net->instance(i) << "   "
                << net->pin(i) << endl;
            data->circuit->nets[data->current_index].numOfPins--;
            data->circuit->nPins--;
            arr--;
            continue;
        }
        arr[i].type = pin_iter->second;
        if(pin_iter->second == __net_pin)
          __net_pin = 0;

        //filter pins on same element
        Pin* _p = data->circuit->nets[data->current_index].arrPins;
        for(; _p < arr+i; _p++)
        {
            if(_p->cellIdx == nodeindex)
                break;
        }
        if(_p < arr+i)
        {
            cout << "Duplicated pin detected: " << net->instance(i) << "   "
                << net->pin(i) << endl;
            if(arr[i].type->dir == PinInfo::OUTPUT)
                _p->type = arr[i].type;//! prevent output deleting
            data->circuit->nets[data->current_index].numOfPins--;
            data->circuit->nPins--;
            arr--;
            continue;
        }//end filter

        if(arr[i].type->dir == PinInfo::OUTPUT)
            numberOfSources++;
        if(numberOfSources > 1)
            cout << "WARNING: net " << net->name() << " has more than one source (" << numberOfSources << ")" << endl;

        arr[i].xOffset = _is_PIN ? 0 : arr[i].type->OriginX;
        arr[i].yOffset = _is_PIN ? 0 : arr[i].type->OriginY;
        switch(arr[i].type->dir)
        {
        case PinInfo::INPUT:
            arr[i].chtype = 'I';
            break;
        case PinInfo::OUTPUT:
            arr[i].chtype = 'O';
            break;
        case PinInfo::INOUT:
            cout << "Warning: net " << net->name() << " has INOUT pin." << endl;
            arr[i].chtype = 'B';
            break;
        default:
            arr[i].chtype = 'I';
        }
    }
    
    if(__net_pin != 0)
    {//insert PIN
      data->circuit->nPins++;
      data->circuit->nets[data->current_index].numOfPins++;
      int pos = net->numConnections();
      int nodeindex = DEFParserSearchNode(__net_pin->Name.c_str(), data);
      arr[pos].cellIdx = nodeindex;
      arr[pos].type = __net_pin;
      if(arr[pos].type->dir == PinInfo::OUTPUT)
        numberOfSources++;
      if(numberOfSources > 1)
        cout << "WARNING: net " << net->name() << " has more than one source (" << numberOfSources << ")" << endl;
      arr[pos].xOffset = 0;
      arr[pos].yOffset = 0;
      switch(arr[pos].type->dir)
      {
      case PinInfo::INPUT:
          arr[pos].chtype = 'I';
          break;
      case PinInfo::OUTPUT:
          arr[pos].chtype = 'O';
          break;
      case PinInfo::INOUT:
          cout << "Warning: net " << net->name() << " has INOUT pin." << endl;
          arr[pos].chtype = 'B';
          break;
      default:
          arr[pos].chtype = 'I';
      }
    }

    if(data->circuit->nets[data->current_index].numOfPins < 2)
    {
        data->circuit->nNets--;
        data->circuit->nPins -= data->circuit->nets[data->current_index].numOfPins;
        delete[] data->circuit->nets[data->current_index].arrPins;
        delete[] data->circuit->nets[data->current_index].name;
        cout << "Net " << net->name() << " skipped." << endl;
        return PARSE_OK;
    }
    data->current_index++;
    return PARSE_OK;
}

int pinCB(defrCallbackType_e c, defiPin *pin, defiUserData ud)
{
    checkType_def(c);
    DEFParserData* data = (DEFParserData*)ud;
    PinInfo *res = new PinInfo();
    res->Capacitance = 0;
    res->Resistance = 0;
    res->isSpecial = 0;
    res->OriginX = pin->placementX() * data->UnitsFactor;
    res->OriginY = pin->placementY() * data->UnitsFactor;
    __netd_nets[pin->netName()] = res;
    if(pin->hasDirection())
    {
        if(strcmp(pin->direction(),"INPUT") == 0)
        {
            res->dir = PinInfo::OUTPUT;//PinInfo::INPUT;
            data->circuit->nPrimaryInputs++;
        }
        else if(strcmp(pin->direction(),"OUTPUT") == 0)
        {
            res->dir = PinInfo::INPUT;//PinInfo::OUTPUT;
            data->circuit->nPrimaryOutputs++;
        }
        else if(strcmp(pin->direction(),"INOUT") == 0)
        {
            cout << "Warning: INOUT primary - " << pin->pinName() << endl;
            res->dir = PinInfo::INOUT;
            data->circuit->nPrimaryInputs++;
        }
        else
            res->dir = PinInfo::OUTPUT;
    }
    else
        res->dir = PinInfo::INPUT;
    res->Name = pin->pinName();
    data->circuit->tech->SinglePins->Pins[res->Name] = res;
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

void FindSources(Circuit& c)
{
    for(int i = 0; i < c.nNets; i++)
    {
        c.nets[i].sourceIdx = NO_SOURCE;
        for(int j = 0; j < c.nets[i].numOfPins; j++)
            if(c.nets[i].arrPins[j].type->dir == PinInfo::OUTPUT)
            {
                c.nets[i].sourceIdx = j;
                break;
            }
    }
}

void IndexPrimaries(Circuit& circuit)
{
//    circuit.nPrimaryInputs = 
  //      circuit.nPrimaryOutputs = 0;
    circuit.primaryInputs = new int[circuit.nPrimaryInputs];
    circuit.primaryOutputs = new int[circuit.nPrimaryOutputs];

    int pi = 0;
    int po = 0;
	int offset = (int)(circuit.terminals - circuit.nodes);
    //find primary inputs and outputs
    int i = circuit.nTerminals - circuit.nPrimaryInputs - circuit.nPrimaryOutputs;
    for(std::map<std::string,PinInfo*>::iterator iter = circuit.tech->SinglePins->Pins.begin();
        i < circuit.nTerminals; ++iter, i++)
    {
        switch(iter->second->dir)
        {
        case PinInfo::OUTPUT:
            circuit.primaryInputs[pi++] = offset + i;
            break;
        case PinInfo::INPUT:
            circuit.primaryOutputs[po++] = offset + i;
            break;
        case PinInfo::INOUT:
          circuit.primaryInputs[pi++] = offset + i;
          break;
        default:
          break;
        }
        circuit.placement[offset + i].arrivalTime = 
            circuit.placement[offset + i].requiredTime = 0.0;
    }
}

int ParseDEF(const char* filename, Circuit& circuit)
{
    DEFParserData userData;
    userData.circuit = &circuit;
    userData.UnitsFactor = (1.0 / 100.0) * 1000.0;
    userData.area = 0.0;

    defrSetDesignCbk(dnameCB);
    defrSetUnitsCbk(unitsCB);
    defrSetDieAreaCbk(dieAreaCB);
    defrSetNetCbk(netCB);
    defrSetNetStartCbk(netStartCB);
    //defrSetNetEndCbk(netEndCB);
    defrSetPinCbk(pinCB);
    defrSetComponentCbk(componentCB);
    defrSetComponentStartCbk(componentStartCB);
    //defrSetComponentEndCbk(componentEndCB);

    defrSetMallocFunction(mallocCB_def);
    defrSetReallocFunction(reallocCB_def);
    defrSetFreeFunction(freeCB_def);

    static int __init_reader_code = defrInit();

    defrReset();

    FILE* f;
    if ((f = fopen(filename,"r")) == 0)
    {
        cout << "Couldn't open input file " << filename << endl;
        return 0;
    }

    if (defrRead(f, filename, (void*)(&userData), 0))
       cout << "Reader returns bad status." << endl;

    defrReleaseNResetMemory();
    __netd_nets.clear();

    //circuit.nNodes -= circuit.nTerminals;
    delete[] userData.indexes;
    FindSources(circuit);
    IndexPrimaries(circuit);
    return 0;
}