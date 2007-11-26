/*  
 * output.cpp
 * this is a part of itlDragon
 * Copyright (C) 2005, ITLab, Zhivoderov
 * email: zhivoderov.a@gmail.com
 */

#include "..\include\output.h"

void PrintCircuitInfo(Circuit& circuit)
{
  cout << "\nCircuit statistics:\n"
       << "Number of rows:\t\t"  << circuit.nRows  << "\n"
       << "Number of nets:\t\t"  << circuit.nNets  << "\n"
       << "Number of nodes:\t"   << circuit.nNodes << "\n"
       << "Number of pins:\t\t"  << circuit.nPins  << "\n"
       << "Number of terminals:\t" << circuit.nTerminals << "\n\n";
  PrintNetsInfo(circuit);
}

void PrintResultString(int argc, char* argv[], Statistics& statistics, MULTIPLACER_ERROR errorCode)
{
  FILE *resultFile, *configFile;
  char string[64], *pVar, vslString[256], resultString[1024];
  time_t ltime;
  time( &ltime );
  resultFile = fopen(resultFileName, "a");
  configFile = fopen(gOptions.configName, "r");
  if (resultFile)
  {
    sprintf( vslString, "BS duration: \t%.3f\t RNG in BS duration: \t%.5f\t"
      "CA duration: \t%.3f\t RNG in CA duration: \t%.5f\t RN count: \t%d\t"
      "LI duration: \t%.3f\t RNG in LI duration: \t%.5f\t", 
      static_cast<double>(statistics.binSwappingWT),
      static_cast<double>(rngBSTotalTime)/CLOCK_RATE,
      static_cast<double>(statistics.cellAnnealingWT),
      static_cast<double>(rngCATotalTime)/CLOCK_RATE,
      rnCATotalCount,
      static_cast<double>(statistics.localImprovementWT),
      static_cast<double>(rngLITotalTime)/CLOCK_RATE
      );
    strcat(vslString, ctime( &ltime ));
    if (errorCode == OK)
      if (errorCode == 0)
        sprintf( resultString, "benchmark: \t%s\t wirelength: \t%.0f\t worktime: \t%.3f\t GO impr:\t%f\t", 
        gOptions.benchmarkName, statistics.currentWL, statistics.totalWT, statistics.localImprovementWLI );
      else
        sprintf( resultString, "The circuit.placement isn't legal\tbenchmark: \t%s\t wirelength: \t%.0f\t worktime: \t%.3f\t GO impr:\t%f\t", 
        gOptions.benchmarkName, statistics.currentWL, statistics.totalWT, statistics.localImprovementWLI );
    for (int i = 1; i < argc; ++i)
    {
      if (!strcmp(argv[i], "-cfg"))
      {
        if (configFile)
        {
          strcat( gOptions.configName, "\t");
          strcat( resultString, gOptions.configName);
          fgets(string, 64, configFile); //do not print benchmark again
          while (!feof(configFile))
          {
            fgets(string, 64, configFile);
            pVar = strchr (string, '\n');
            if (pVar)
            {
              pVar[0] = '\t';
              strcat( resultString, string);
            }
          }
          fclose( configFile );
        }
      }
      else if (!strcmp(argv[i], "-f"))
      {
        strcat( resultString, "-f\t");
        for (int argCounter = i+1; argCounter < argc; argCounter++)
        {
          strcat( resultString, argv[argCounter] );
          strcat( resultString, "\t" );
        }
      }
    }
  }
  strcat( resultString, vslString );
  fputs( resultString, resultFile);
  fclose( resultFile );
}

void PrintToPL(const char* fileName, Circuit& circuit, Statistics& statistics,
     double shiftX, double shiftY)
{
  if (fileName == 0)   
  {
    printf("Error: null pointer of pl file\n");
    return;
  }
  time_t ltime;
  time( &ltime );
  FILE *f;
  char buffer[256];
  char newFileName[256];
  char *pVal;
  strcpy(newFileName, fileName);
  if (circuit.nNodes && circuit.nTerminals && circuit.placement)
  {
    ReshiftCoords(circuit);
    /*for (int i = 0; i < circuit.nNodes; ++i)
    {
      circuit.placement[i].xCoord -= 0.5*nodes[i].width  - shiftX;
      circuit.placement[i].yCoord -= 0.5*nodes[i].height - shiftY;
    }
    for (int i = 0; i < circuit.nTerminals; ++i)
    {
      circuit.placement[i + circuit.nNodes].xCoord -= 0.5*terminals[i].width  - shiftX;
      circuit.placement[i + circuit.nNodes].yCoord -= 0.5*terminals[i].height - shiftY;
    }*/
    pVal = strrchr(newFileName, '\\');
    if (pVal) strcpy(newFileName, ++pVal);
    pVal = strrchr(newFileName, '.');
    if (pVal) pVal[0] = '\0';
    strcat(newFileName, " itlDragon ");
    strcat(newFileName, ctime( &ltime ));
    pVal = strrchr(newFileName, '\n');
    if (pVal) pVal[0] = '\0';
    strcat(newFileName, ".pl");
    while (1)
    {
      pVal = strchr(newFileName, ' ');
      if (pVal) pVal[0] = '_';
      else break;
    }
    while (1)
    {
      pVal = strchr(newFileName, ':');
      if (pVal) pVal[0] = '_';
      else break;
    }
    f = fopen( newFileName , "w");
    sprintf(buffer, "UCLA pl 1.0\n");
    fputs( buffer, f);
    sprintf(buffer, "# ITLab\n# Created : %s", ctime( &ltime ));
    fputs( buffer, f);
    sprintf(buffer, "# Work time: %.3f\n", statistics.totalWT);
    fputs( buffer, f);
    sprintf(buffer, "# Wire length: %.3f\n\n", statistics.currentWL);
    fputs( buffer, f);
    for (int i = 0; i < circuit.nNodes + circuit.nTerminals; ++i)
    {
      sprintf(buffer, "%8s %10.3f %10.3f : %s\n", circuit.tableOfNames[i].name, circuit.placement[i].xCoord, circuit.placement[i].yCoord, circuit.placement[i].orient);
      fputs( buffer, f);
    }
    fclose(f);
    ShiftCoords(circuit);
    /*for (int i = 0; i < circuit.nNodes; ++i)
    {
      circuit.placement[i].xCoord += 0.5*nodes[i].width  - shiftX;
      circuit.placement[i].yCoord += 0.5*nodes[i].height - shiftY;
    }
    for (int i = 0; i < circuit.nTerminals; ++i)
    {
      circuit.placement[i + circuit.nNodes].xCoord += 0.5*terminals[i].width  - shiftX;
      circuit.placement[i + circuit.nNodes].yCoord += 0.5*terminals[i].height - shiftY;
    }*/
  }
}

void PrintToPL(const char* fileName, Circuit& circuit,
               double shiftX, double shiftY)
{
  if (fileName == 0)   
  {
    printf("Error: null pointer of pl file\n");
    return;
  }
  time_t ltime;
  time( &ltime );
  FILE *f;
  char buffer[256];
  char newFileName[256];
  char *pVal;
  strcpy(newFileName, fileName);
  if (circuit.nNodes && circuit.nTerminals && circuit.placement)
  {
    ReshiftCoords(circuit);
    pVal = strrchr(newFileName, '\\');
    if (pVal) strcpy(newFileName, ++pVal);
    pVal = strrchr(newFileName, '.');
    if (pVal) pVal[0] = '\0';
    strcat(newFileName, " itlDragon ");
    strcat(newFileName, ctime( &ltime ));
    pVal = strrchr(newFileName, '\n');
    if (pVal) pVal[0] = '\0';
    strcat(newFileName, ".pl");
    while (1)
    {
      pVal = strchr(newFileName, ' ');
      if (pVal) pVal[0] = '_';
      else break;
    }
    while (1)
    {
      pVal = strchr(newFileName, ':');
      if (pVal) pVal[0] = '_';
      else break;
    }
    f = fopen( newFileName , "w");
    sprintf(buffer, "UCLA pl 1.0\n");
    fputs( buffer, f);
    sprintf(buffer, "# ITLab\n# Created : %s", ctime( &ltime ));
    fputs( buffer, f);
    for (int i = 0; i < circuit.nNodes + circuit.nTerminals; ++i)
    {
      sprintf(buffer, "%8s %10.3f %10.3f : %s\n", circuit.tableOfNames[i].name, circuit.placement[i].xCoord, circuit.placement[i].yCoord, circuit.placement[i].orient);
      fputs( buffer, f);
    }
    fclose(f);
    ShiftCoords(circuit);
  }
}

void PrintToTmpPL(Circuit& circuit, Statistics& statistics, double shiftX, double shiftY)
{
  time_t ltime;
  time( &ltime );
  FILE *f;
  char buffer[256];
  char newFileName[256];
  //strcpy(newFileName, "\"");
  strcpy(newFileName, "temp");
  if (circuit.nNodes && circuit.nTerminals && circuit.placement)
  {  
    ReshiftCoords(circuit);

    strcat(newFileName, ".pl");
    f = fopen( newFileName , "w");
    sprintf(buffer, "UCLA pl 1.0\n");
    fputs( buffer, f);
    sprintf(buffer, "# ITLab\n# Created : %s", ctime( &ltime ));
    fputs( buffer, f);
    sprintf(buffer, "# Work time: %.3f\n", statistics.totalWT);
    fputs( buffer, f);
    sprintf(buffer, "# Wire length: %.3f\n\n", statistics.currentWL);
    fputs( buffer, f);
    for (int i = 0; i < circuit.nNodes + circuit.nTerminals; ++i)
    {
      sprintf(buffer, "%8s %10.3f %10.3f : %s\n", circuit.tableOfNames[i].name, circuit.placement[i].xCoord,
                                                  circuit.placement[i].yCoord,  circuit.placement[i].orient);
      fputs( buffer, f);
    }
    fclose(f);

    ShiftCoords(circuit);
  } 
}

void PrintToTmpPL(Circuit& circuit, double shiftX, double shiftY)
{
  time_t ltime;
  time( &ltime );
  FILE *f;
  char buffer[256];
  char newFileName[256];
  //strcpy(newFileName, "\"");
  strcpy(newFileName, "temp");
  if (circuit.nNodes && circuit.nTerminals && circuit.placement)
  {  
    ReshiftCoords(circuit);

    strcat(newFileName, ".pl");
    f = fopen( newFileName , "w");
    sprintf(buffer, "UCLA pl 1.0\n");
    fputs( buffer, f);
    sprintf(buffer, "# ITLab\n# Created : %s", ctime( &ltime ));
    fputs( buffer, f);
    sprintf(buffer, "# Work time: \n");
    fputs( buffer, f);
    sprintf(buffer, "# Wire length: \n\n");
    fputs( buffer, f);
    for (int i = 0; i < circuit.nNodes + circuit.nTerminals; ++i)
    {
      sprintf(buffer, "%8s %10.3f %10.3f : %s\n", circuit.tableOfNames[i].name, circuit.placement[i].xCoord, 
                                                  circuit.placement[i].yCoord,  circuit.placement[i].orient);
      fputs( buffer, f);
    }
    fclose(f);

    ShiftCoords(circuit);
  } 
}

void PrintPLT(const char* fileName, Circuit& circuit, Statistics& statistics)
{
  if (fileName == 0)   
  {
  printf("Error: null pointer of plt file\n");
  return ;
  }
  FILE *f;
  char buffer[280];
  if ( (circuit.nNodes) && circuit.placement)
  {
  f = fopen( fileName , "w");
  sprintf(buffer, "set title 'CELLS'\n\nplot[:][:] '-' w l 3, '-' w l 5\n\n");
  fputs( buffer, f);
  for (int i = 0; i < circuit.nNodes; ++i)
  {
    sprintf(buffer, "%15f %15f\n", circuit.placement[i].xCoord, circuit.placement[i].yCoord);
    fputs( buffer, f);
    sprintf(buffer, "%15f %15f\n", circuit.placement[i].xCoord, circuit.placement[i].yCoord + circuit.nodes[i].height);
    fputs( buffer, f);
    sprintf(buffer, "%15f %15f\n", circuit.placement[i].xCoord + circuit.nodes[i].width, circuit.placement[i].yCoord + circuit.nodes[i].height);
    fputs( buffer, f);
    sprintf(buffer, "%15f %15f\n", circuit.placement[i].xCoord + circuit.nodes[i].width, circuit.placement[i].yCoord);
    fputs( buffer, f);
    sprintf(buffer, "%15f %15f\n", circuit.placement[i].xCoord, circuit.placement[i].yCoord);
    fputs( buffer, f);
    fputs( "\n", f);
  }
  fputs( "EOF\n", f);
  for (int i = 0; i < circuit.nTerminals; ++i)
  {
    sprintf(buffer, "%15f %15f\n", circuit.placement[circuit.nNodes + i].xCoord, circuit.placement[circuit.nNodes + i].yCoord);
    fputs( buffer, f);
    sprintf(buffer, "%15f %15f\n", circuit.placement[circuit.nNodes + i].xCoord, circuit.placement[circuit.nNodes + i].yCoord + circuit.terminals[i].height);
    fputs( buffer, f);
    sprintf(buffer, "%15f %15f\n", circuit.placement[circuit.nNodes + i].xCoord + circuit.terminals[i].width, circuit.placement[circuit.nNodes + i].yCoord + circuit.terminals[i].height);
    fputs( buffer, f);
    sprintf(buffer, "%15f %15f\n", circuit.placement[circuit.nNodes + i].xCoord + circuit.terminals[i].width, circuit.placement[circuit.nNodes + i].yCoord);
    fputs( buffer, f);
    sprintf(buffer, "%15f %15f\n", circuit.placement[circuit.nNodes + i].xCoord, circuit.placement[circuit.nNodes + i].yCoord);
    fputs( buffer, f);
    fputs( "\n", f);
  }
  fputs( "EOF", f );
  fclose(f);
  }
}

MULTIPLACER_ERROR ShiftCoords(Circuit& circuit)
{
  if (circuit.nNodes && circuit.nTerminals && circuit.placement)
  {
    int shiftX = circuit.shiftX;
    int shiftY = circuit.shiftY;
    
    for (int i = 0; i < circuit.nNodes; ++i)
    {
      circuit.placement[i].xCoord += 0.5*circuit.nodes[i].width  - shiftX;
      circuit.placement[i].yCoord += 0.5*circuit.nodes[i].height - shiftY;
    }
    for (int i = 0; i < circuit.nTerminals; ++i)
    {
      circuit.placement[i + circuit.nNodes].xCoord += 0.5*circuit.terminals[i].width  - shiftX;
      circuit.placement[i + circuit.nNodes].yCoord += 0.5*circuit.terminals[i].height - shiftY;
    }
    
    for (int i = 0; i < circuit.nRows; ++i)
    {
      circuit.rows[i].subrowOrigin -= shiftX;
      circuit.rows[i].coordinate   -= shiftY;
    }
    return OK;
  }
  return EMPTY_DATA_STRUCTURES;
}

MULTIPLACER_ERROR ReshiftCoords(Circuit& circuit)
{
  if (circuit.nNodes && circuit.nTerminals && circuit.placement)
  {
    int shiftX = circuit.shiftX;
    int shiftY = circuit.shiftY;
    
    for (int i = 0; i < circuit.nNodes; ++i)
    {
      circuit.placement[i].xCoord -= 0.5*circuit.nodes[i].width  - shiftX;
      circuit.placement[i].yCoord -= 0.5*circuit.nodes[i].height - shiftY;
    }
    for (int i = 0; i < circuit.nTerminals; ++i)
    {
      circuit.placement[i + circuit.nNodes].xCoord -= 0.5*circuit.terminals[i].width  - shiftX;
      circuit.placement[i + circuit.nNodes].yCoord -= 0.5*circuit.terminals[i].height - shiftY;
    }  
    for (int i = 0; i < circuit.nRows; ++i)
    {
      circuit.rows[i].subrowOrigin += shiftX;
      circuit.rows[i].coordinate   += shiftY;
    }
    return OK;
  }
  return EMPTY_DATA_STRUCTURES; 
}

void CreateHTMLReport(double wireLength, double workTime, MULTIPLACER_ERROR errorCode)
{
  FILE *testResFileTXT;
  FILE *testResFileHTM;
  char string[256];
  char *pVar = NULL;
  int fileLen = 0;
  time_t ltime;
  time( &ltime );
  /*testResFile = fopen("testReport.txt", "a");
  if (testResFile)
  {
    string[0] = '\0';
    
    fputs(ctime(&ltime), testResFile);
    
    sprintf(string, "Benchmark:\t%s\n", gOptions.benchmarkName);
    fputs(string, testResFile);
    
    sprintf(string, "Wirelength:\t%.0f\n", wireLength);
    fputs(string, testResFile);
    
    sprintf(string, "Work time\t%.3f\n", workTime);
    fputs(string, testResFile);
    
    PrintErrorMessage(string, errorCode);
    fputs(string, testResFile);
    fputs("\n", testResFile);
    
    fclose(testResFile);
  }*/
  //testResFile = fopen("testReport.html", "a");
  testResFileTXT = fopen("currTestReport.txt", "a"); 
  if (testResFileTXT)
  {
    string[0] = '\0';
    
    fputs(ctime(&ltime), testResFileTXT);
    
    PrintErrorMessage(string, errorCode);
    fputs(string, testResFileTXT);
    
    sprintf(string, "%s\n", (char *)GetCommandLine());
    fputs(string, testResFileTXT);
    
    sprintf(string, "%s\n", gOptions.benchmarkName);
    fputs(string, testResFileTXT);
    
    sprintf(string, "%.0f\n", wireLength);
    fputs(string, testResFileTXT);
    
    sprintf(string, "%.3f\n", workTime);
    fputs(string, testResFileTXT);
    
    string[0] = ' ';
    string[1] = '\0';
    fputs(string, testResFileTXT);
    fputs("\n", testResFileTXT);
    
    fclose(testResFileTXT);
  }
  
  testResFileTXT = fopen("currTestReport.txt", "r"); 
  if (testResFileTXT)
  {
    testResFileHTM = fopen("testReport.html", "w"); 
    if (testResFileHTM)
    {
      fseek(testResFileHTM, 0L, 0);
      fputs("<html>\n", testResFileHTM);
      fputs("    <head>\n", testResFileHTM);
      fputs("        <title>\n", testResFileHTM);
      fputs("        itlDragon test report\n", testResFileHTM);
      fputs("        </title>\n", testResFileHTM);
      fputs("    </head>\n", testResFileHTM);
      fputs("    <body>\n", testResFileHTM);
      fputs("    <h1>\n", testResFileHTM);
      fputs("    itlDragon test report\n", testResFileHTM);
      fputs("    </h1>\n", testResFileHTM);
      fputs("        <p>\n", testResFileHTM);
      
      while (!feof(testResFileTXT))
      {
        fgets(string, 256, testResFileTXT);
        if (string[0] == ' ')
          continue;
        //fputs("            <table style=\"width: 100%; border-right: silver thin double; border-top: silver thin double; border-left: silver thin double; border-bottom: silver thin double;\">\n", testResFileHTM);
        
        pVar = NULL;
        pVar = strstr(string, ":");
        if (pVar)
        {
          fputs("            <table style=\"width: 100%; border-right: silver thin double; border-top: silver thin double; border-left: silver thin double; border-bottom: silver thin double;\">\n", testResFileHTM);
          fputs("                <tr>\n", testResFileHTM);
          fputs("                    <td style=\"width: 25px; border-right: silver thin double; border-top: silver thin double; border-left: silver thin double; border-bottom: silver thin double;\">\n", testResFileHTM);
          fputs("                        Date</td>\n", testResFileHTM);
          fputs("                    <td style=\"border-right: silver thin double; border-top: silver thin double; border-left: silver thin double;\n", testResFileHTM);
          fputs("                        width: 432px; border-bottom: silver thin double\">\n", testResFileHTM);
          fputs(string, testResFileHTM);
          fputs("</td>\n", testResFileHTM);
        }
        else 
          continue;
        
        fgets(string, 256, testResFileTXT);  
        
        fputs("                    <td align=\"center\" rowspan=\"5\" style=\"width: 100px; border-right: silver thin double; border-top: silver thin double; border-left: silver thin double; border-bottom: silver thin double;\">\n", testResFileHTM);
        if (stricmp(string, "OK\n") == 0)
        {
          fputs("                        <span style=\"color: #00cc00; font-family: Verdana\"><strong>\n", testResFileHTM);
          fputs("[OK]", testResFileHTM);
        }
        else
        {
          fputs("                        <span style=\"color: #ff0000; font-family: Verdana\"><strong>\n", testResFileHTM);
          fputs("[FAILED]", testResFileHTM);
        }
        fputs("</strong></span></td>\n", testResFileHTM);
        
        fputs("                </tr>\n", testResFileHTM);
        
        fgets(string, 256, testResFileTXT);
        
        fputs("                <tr>\n", testResFileHTM);
        fputs("                    <td style=\"border-right: silver thin double; border-top: silver thin double; border-left: silver thin double;\n", testResFileHTM);
        fputs("                        width: 25px; border-bottom: silver thin double; height: 25px; text-align: left;\">\n", testResFileHTM);
        fputs("                        Command line</td>\n", testResFileHTM);
        fputs("                    <td style=\"border-right: silver thin double; border-top: silver thin double; border-left: silver thin double;\n", testResFileHTM);
        fputs("                        width: 432px; border-bottom: silver thin double\">\n", testResFileHTM);
        fputs(string, testResFileHTM);
        fputs("</td>\n", testResFileHTM);
        
        fgets(string, 256, testResFileTXT);
        
        //fputs("                <tr>\n", testResFileHTM);
        
        fputs("                <tr>\n", testResFileHTM);
        fputs("                    <td style=\"width: 25px; border-right: silver thin double; border-top: silver thin double; border-left: silver thin double; border-bottom: silver thin double; height: 25px;\">\n", testResFileHTM);
        fputs("                        Benchmark</td>\n", testResFileHTM);
        fputs("                    <td style=\"border-right: silver thin double; border-top: silver thin double; border-left: silver thin double;\n", testResFileHTM);
        fputs("                        width: 432px; border-bottom: silver thin double\">\n", testResFileHTM);
        fputs(string, testResFileHTM);
        fputs("</td>\n", testResFileHTM);
        
        fgets(string, 256, testResFileTXT);
        
        fputs("                <tr>\n", testResFileHTM);
        fputs("                    <td style=\"width: 25px; border-right: silver thin double; border-top: silver thin double; border-left: silver thin double; border-bottom: silver thin double\">\n", testResFileHTM);
        fputs("                        Wirelength</td>\n", testResFileHTM);
        fputs("                    <td style=\"border-right: silver thin double; border-top: silver thin double; border-left: silver thin double;\n", testResFileHTM);
        fputs("                        width: 432px; border-bottom: silver thin double\">\n", testResFileHTM);
        fputs(string, testResFileHTM);
        fputs("</td>\n", testResFileHTM);
        
        fgets(string, 256, testResFileTXT);
        
        fputs("                <tr>\n", testResFileHTM);
        fputs("                    <td style=\"width: 25px; border-right: silver thin double; border-top: silver thin double; border-left: silver thin double; border-bottom: silver thin double;\" align=\"left\">\n", testResFileHTM);
        fputs("                        Work time</td>\n", testResFileHTM);
        fputs("                    <td style=\"border-right: silver thin double; border-top: silver thin double; border-left: silver thin double;\n", testResFileHTM);
        fputs("                        width: 432px; border-bottom: silver thin double\">\n", testResFileHTM);
        fputs(string, testResFileHTM);
        fputs("</td>\n", testResFileHTM);
        
        fgets(string, 256, testResFileTXT);
        
        fputs("            </table>\n", testResFileHTM);
        fputs("<br />\n", testResFileHTM);
        fputs("<br />\n", testResFileHTM);
      }
  
      fputs("        </p>\n", testResFileHTM);  
      fputs("    </body>\n", testResFileHTM);
      fputs("</html>\n", testResFileHTM);
      
      fclose(testResFileHTM);
    }
    fclose(testResFileTXT);
  } 
}

void PrintNetsToRouterFormat(Circuit& circuit)
{
  double minx = 0;
  double miny = 0;

  for(int i = 0; i < circuit.nNets; i++)
    for(int j = 0; j < circuit.nets[i].numOfPins; j++)
    {
      int ind = circuit.nets[i].arrPins[j].cellIdx;
      double x = circuit.placement[ind].xCoord + circuit.nets[i].arrPins[j].xOffset;
      double y = circuit.placement[ind].yCoord + circuit.nets[i].arrPins[j].yOffset;
      if(x < minx) minx = x;
      if(y < miny) miny = y;
    }

  ofstream out(gOptions.GRFileName);
  
  out << "##shift " << -minx << " " << -miny << endl;
  out << "grid " << ((int)(circuit.width - minx))/gOptions.GRTileSize+1 << " " 
                 << ((int)(circuit.height - miny))/gOptions.GRTileSize+1 << endl;
  out << "vertical capacity " << gOptions.GRVertCapacity << endl;
  out << "horizontal capacity " << gOptions.GRHorizCapacity << endl;
  out << "num net " << circuit.nNets << endl;

  for(int i = 0; i < circuit.nNets; i++)
  {
    out << "net" << i << " " << i << " " << circuit.nets[i].numOfPins << endl;
    for(int j = 0; j < circuit.nets[i].numOfPins; j++)
    {
      int ind = circuit.nets[i].arrPins[j].cellIdx;
      int x = ((int)(circuit.placement[ind].xCoord + circuit.nets[i].arrPins[j].xOffset - minx))/gOptions.GRTileSize;
      int y = ((int)(circuit.placement[ind].yCoord + circuit.nets[i].arrPins[j].yOffset - miny))/gOptions.GRTileSize;
      out << x << " " << y << endl;
    }
  }
  out.close();
}
