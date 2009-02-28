/* 
* main.cpp
* this is a part of itlAnalyticalPlacer
* Copyright (C) 2005-2006, ITLab, Kornyakov, Kurina, Zhivoderov
* email: kirillkornyakov@yandex.ru
* email: zhivoderov.a@gmail.com
* email: nina.kurina@gmail.com
*/

#include <iostream>
#include <conio.h>
#include <algorithm>
#include <string>
#include <vector>
#include <math.h>
#include <windows.h>
#include <direct.h>
#include "..\include\parser.h"
#include "..\include\output.h"
#include "..\include\check_legality.h"
#include "..\include\errors.h"
#include "..\include\global.h"
//#include "..\include\abs_detailed_placement.h"
#include "..\include\SA_Window.h"

#define numOfCells 20
#define numOfSteps 4000
#define numOfRows 3

SAWindow *slidingWindow;
double *x_array = NULL;
Circuit  circuit;


int ReadBounds(char fileName[], double *&boundsArray, char *&auxName, int &firstRow )
{
    FILE *f = fopen(fileName, "r");
    int stringSize = 100;
    int k1 = 0;
    int k2 = 0;

    if (f == NULL) {
        cout<<"File with bounds not found!\n";
        return 0;
    }

    char *str = new char [stringSize];
    auxName = new char [stringSize];
    char *num = new char [stringSize];

    for (int i = 0; i < stringSize; i++) {
        str[i] = '\0';
        num[i] = '\0';
        auxName[i] = '\0';
    }

    fgets(str, stringSize - 1, f); //getting aux file name
    for(int i = 0; str[i] != '\n'; i++)
        auxName[i] = str[i];

    fgets(str, stringSize - 1, f); //getting number of rows

    while((str[k1] != ':') && (k1 < stringSize))
        k1++;

    k1 += 2;

    while ((k1 < stringSize) && (isdigit(str[k1]) ))
        num[k2++] = str[k1++];

    int rowsNum = atoi(num);
    //cout<<"num: "<<rowsNum<<endl; ///
    boundsArray = new double [rowsNum * 2];

    fgets(str, stringSize - 1, f); //getting index of first row
    for(int j = 0; j < k2 + 1; j++)
        num[j] = '\0';

    k1 = 0;
    k2 = 0;

    while((str[k1] != ':') && (k1 < stringSize))
        k1++;

    k1 += 2;

    while ((k1 < stringSize) && (isdigit(str[k1]) ))
        num[k2++] = str[k1++];

    firstRow = atoi(num);
    cout<<"firstRow: "<<firstRow<<endl;

    for(int i = 0; i < rowsNum * 2;) { //getting bounds of rows
        for(int j = 0; j < k2 + 1; j++)
            num[j] = '\0';

        k1 = 0;
        k2 = 0;
        fgets(str, stringSize - 1, f);

        if(str[0] == 'e') {
            cout<<"incorrect file\n";
            delete [] str;
            delete [] num;
            return 0;
        }

        k1 = 0;
        k2 = 0;

        while((k1 < stringSize) && (str[k1] != ':'))
            k1++;

        k1 += 3;

        while ((k1 < stringSize) && (isdigit(str[k1]) ))
            num[k2++] = str[k1++];

        //cout<<num<<endl;
        boundsArray[i++] = atof(num); //first bound for this row
        //cout<<boundsArray[i - 1]<<endl; ///

        for(int j = 0; j < k2 + 1; j++)
            num[j] = '\0';

        k2 = 0;
        k1 += 2;

        while ((k1 < stringSize) && (isdigit(str[k1]) ))
            num[k2++] = str[k1++];

        boundsArray[i++] = atoi(num); //second bound for this row        
    }

    delete [] str;
    delete [] num;
    fclose(f);
    return rowsNum;
}

int Initialize() {

    Statistics statistics;
    int _argc = 3;
    char **_argv = new char* [_argc];
    _argv[0] = "detailed_placement";
    _argv[1] = "-f";
    int firstRow = 0;
    _argv[2] = "ibm01.aux";

    char abtBoundsFile[] = "benchmark_info.txt";	
    //ReadBounds(abtBoundsFile, x_array, _argv[2], firstRow);
    MULTIPLACER_ERROR errorCode = OK;
    CMDParse(_argc, _argv);

    if (gOptions.calcTimingFileName[0] != '\0')
    {
        //if (gOptions.isLEFDEFinput)
        //ParseDEF(gOptions.calcTimingFileName, circuit);
        //else
        if (gOptions.defName[0] != '\0')
        {
            //ParseLEFDEF(gOptions.lefName, gOptions.defName, circuit);
            ParseAux(gOptions.benchmarkName, circuit);
            ParsePl(gOptions.calcTimingFileName, circuit);
            MakeTableOfConnections(circuit);
        }

        exit(1);
    }

    // initialize all data structures, parse benchmark and so on
    errorCode = Initialization(circuit, statistics);
    CheckCode(errorCode);

    // we shift point of origin to the bottom left corner of placement area
    ShiftCoords(circuit);
    statistics.currentWL = cf_recalc_all(UPDATE_NETS_WLS, circuit.nNets, circuit.nets, circuit.placement);
    cout<<"WL: "<<statistics.currentWL<<endl;

    PrintCircuitInfo(circuit);

    if (gOptions.doCheckLegality)
    {// if itlAnalyticalPlacer is run with -check key
        cout << "Pin-to-Pin HPWL: " << statistics.currentWL << "\n\n";  
        errorCode = CheckLegalityOfPlacement(circuit);
        CheckCode(errorCode);
        Exit();
    }

    if (gOptions.doConvertToRouter == true)
    {
        cout << "Save nets to router format..." << endl;
        PrintNetsToRouterFormat(circuit);
        cout << "File " << gOptions.GRFileName << " written successful" << endl;
        Exit();
    }

    if (gOptions.isLEFDEFinput && gOptions.convert2BookshelfName[0] != '\0')
    {
        LEFDEF2Bookshelf(gOptions.convert2BookshelfName, circuit);
        cout << "Writing bookshelf from LEFDEF to " << gOptions.convert2BookshelfName << endl;
        Exit();
    }

    PrintToTmpPL(circuit, statistics);

    //*************************** O U T P U T ************************//
    cout<<"WL before window: "<<cf_recalc_all(UPDATE_NETS_WLS, circuit.nNets, circuit.nets, circuit.placement)<<endl;
    MakeWindows(circuit, numOfSteps, numOfCells, numOfRows);
    cout<<"WL after window: "<<cf_recalc_all(UPDATE_NETS_WLS, circuit.nNets, circuit.nets, circuit.placement)<<endl;
    CheckLegalityOfPlacement(circuit);
    return 0;
}

int main() {
    Initialize();
    system("PAUSE");    
    return 0;
}

// ---------------------------------------------------------------------------
