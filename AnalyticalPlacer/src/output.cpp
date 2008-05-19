/*  
 * output.cpp
 * this is a part of itlAnalyticalPlacer
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
               double shiftX, double shiftY,
               int nBinRows, int nBinCols)
{
  time_t ltime;
  time( &ltime );
  FILE *f;
  char buffer[256];
  char newFileName[256];
  char *pVal;

  strcpy(newFileName, fileName);
  pVal = strrchr(newFileName, '\\');
  if (pVal) strcpy(newFileName, ++pVal);
  pVal = strrchr(newFileName, '.');
  if (pVal) pVal[0] = '\0';
  strcat(newFileName, " itlAPlacer ");
  strcat(newFileName, ctime( &ltime ));
  pVal = strrchr(newFileName, '\n');
  if (pVal) pVal[0] = '\0';
  if (gOptions.isLEFDEFinput)
  {
    strcat(newFileName, ".def");
  } 
  else
  {
    strcat(newFileName, ".pl");
  }
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

  if (gOptions.isLEFDEFinput)
  {
    if (fileName == 0)   
    {
      printf("Error: null pointer of def file\n");
      return;
    }

    ExportDEF(newFileName, circuit);
  }
  else
  {
    if (fileName == 0)   
    {
      printf("Error: null pointer of pl file\n");
      return;
    }

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
      cout << "Dumping placement to \"" << newFileName << "\"" << endl;
      f = fopen(newFileName, "w");
      sprintf(buffer, "UCLA pl 1.0\n");
      fputs( buffer, f);
      sprintf(buffer, "# ITLab\n# Created : %s", ctime( &ltime ));
      fputs( buffer, f);
      sprintf(buffer, "# Work time: %.3f\n", statistics.totalWT);
      fputs( buffer, f);
      sprintf(buffer, "# Wire length: %.3f\n", statistics.currentWL);
      fputs( buffer, f);
      sprintf(buffer, "#_Bin grid: %d x %d\n\n", nBinRows, nBinCols);
      fputs( buffer, f);
      int shift_ = (int)(circuit.terminals - circuit.nodes);
      for (int i = 0; i < shift_ + circuit.nTerminals; ++i)
      {
        if(i == circuit.nNodes) i = shift_;
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
}

void PrintToPL(const char* fileName, Circuit& circuit,
               double shiftX, double shiftY, int nBinRows, int nBinCols)
{
  time_t ltime;
  time( &ltime );
  FILE *f;
  char buffer[256];
  char newFileName[256];
  char *pVal;

  strcpy(newFileName, fileName);
  pVal = strrchr(newFileName, '\\');
  if (pVal) strcpy(newFileName, ++pVal);
  pVal = strrchr(newFileName, '.');
  if (pVal) pVal[0] = '\0';
  strcat(newFileName, " itlAPlacer ");
  strcat(newFileName, ctime( &ltime ));
  pVal = strrchr(newFileName, '\n');
  if (pVal) pVal[0] = '\0';
  if (gOptions.isLEFDEFinput)
  {
    strcat(newFileName, ".def");
  } 
  else
  {
    strcat(newFileName, ".pl");
  }
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

  if (gOptions.isLEFDEFinput)
  {
    if (fileName == 0)   
    {
      printf("Error: null pointer of def file\n");
      return;
    }

    ExportDEF(newFileName, circuit);
  }
  else
  {
    if (fileName == 0)   
    {
      printf("Error: null pointer of pl file\n");
      return;
    }

    if (circuit.nNodes && circuit.nTerminals && circuit.placement)
    {
      ReshiftCoords(circuit);
      
      cout << "Dumping placement to \"" << newFileName << "\"" << endl;
      f = fopen(newFileName, "w");
      sprintf(buffer, "UCLA pl 1.0\n");
      fputs( buffer, f);
      sprintf(buffer, "# ITLab\n# Created : %s", ctime( &ltime ));
      fputs( buffer, f);
      sprintf(buffer, "#_Bin grid: %d x %d\n\n", nBinRows, nBinCols);
      fputs( buffer, f);
      int shift_ = (int)(circuit.terminals - circuit.nodes);
      for (int i = 0; i < shift_ + circuit.nTerminals; ++i)
      {
        if(i == circuit.nNodes) i = shift_;
        sprintf(buffer, "%8s %10.3f %10.3f : %s\n", circuit.tableOfNames[i].name, circuit.placement[i].xCoord, circuit.placement[i].yCoord, circuit.placement[i].orient);
        fputs( buffer, f);
      }
      fclose(f);
      ShiftCoords(circuit);
    }
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
    int shift_ = (int)(circuit.terminals - circuit.nodes);
    for (int i = 0; i < shift_ + circuit.nTerminals; ++i)
    {
      if(i == circuit.nNodes) i = shift_;
      sprintf(buffer, "%8s %10.3f %10.3f : %s\n", circuit.tableOfNames[i].name, circuit.placement[i].xCoord,
                                                  circuit.placement[i].yCoord,  circuit.placement[i].orient);
      fputs(buffer, f);
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
    int shift_ = (int)(circuit.terminals - circuit.nodes);
    for (int i = 0; i < shift_ + circuit.nTerminals; ++i)
    {
      if (i == circuit.nNodes) i = shift_;
      sprintf(buffer, "%8s %10.3f %10.3f : %s\n", circuit.tableOfNames[i].name, circuit.placement[i].xCoord, 
                                                  circuit.placement[i].yCoord,  circuit.placement[i].orient);
      fputs( buffer, f);
    }
    fclose(f);

    ShiftCoords(circuit);
  } 
}

void PrintPLT(const char* fileName, Circuit& circuit)
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
  int shift_ = (int)(circuit.terminals - circuit.nodes);
  for (int i = 0; i < circuit.nTerminals; ++i)
  {
    sprintf(buffer, "%15f %15f\n", circuit.placement[shift_ + i].xCoord, circuit.placement[shift_ + i].yCoord);
    fputs( buffer, f);
    sprintf(buffer, "%15f %15f\n", circuit.placement[shift_ + i].xCoord, circuit.placement[shift_ + i].yCoord + circuit.terminals[i].height);
    fputs( buffer, f);
    sprintf(buffer, "%15f %15f\n", circuit.placement[shift_ + i].xCoord + circuit.terminals[i].width, circuit.placement[shift_ + i].yCoord + circuit.terminals[i].height);
    fputs( buffer, f);
    sprintf(buffer, "%15f %15f\n", circuit.placement[shift_ + i].xCoord + circuit.terminals[i].width, circuit.placement[shift_ + i].yCoord);
    fputs( buffer, f);
    sprintf(buffer, "%15f %15f\n", circuit.placement[shift_ + i].xCoord, circuit.placement[shift_ + i].yCoord);
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
    double shiftX = circuit.shiftX;
    double shiftY = circuit.shiftY;
    
    for (int i = 0; i < circuit.nNodes; ++i)
    {
      circuit.placement[i].xCoord += 0.5*circuit.nodes[i].width  - shiftX;
      circuit.placement[i].yCoord += 0.5*circuit.nodes[i].height - shiftY;
    }
    int shift_ = (int)(circuit.terminals - circuit.nodes);
    for (int i = 0; i < circuit.nTerminals; ++i)
    {
      circuit.placement[i + shift_].xCoord += 0.5*circuit.terminals[i].width  - shiftX;
      circuit.placement[i + shift_].yCoord += 0.5*circuit.terminals[i].height - shiftY;
    }
    
    for (int i = 0; i < circuit.nRows; ++i)
    {
      circuit.rows[i].subrowOrigin -= shiftX;
      circuit.rows[i].coordinate   -= (int)shiftY;
    }
    return OK;
  }
  return EMPTY_DATA_STRUCTURES;
}

MULTIPLACER_ERROR ReshiftCoords(Circuit& circuit)
{
  if (circuit.nNodes && circuit.nTerminals && circuit.placement)
  {
    double shiftX = circuit.shiftX;
    double shiftY = circuit.shiftY;
    
    for (int i = 0; i < circuit.nNodes; ++i)
    {
      circuit.placement[i].xCoord -= 0.5*circuit.nodes[i].width  - shiftX;
      circuit.placement[i].yCoord -= 0.5*circuit.nodes[i].height - shiftY;
    }
    int shift_ = (int)(circuit.terminals - circuit.nodes);
    for (int i = 0; i < circuit.nTerminals; ++i)
    {
      circuit.placement[i + shift_].xCoord -= 0.5*circuit.terminals[i].width  - shiftX;
      circuit.placement[i + shift_].yCoord -= 0.5*circuit.terminals[i].height - shiftY;
    }  
    for (int i = 0; i < circuit.nRows; ++i)
    {
      circuit.rows[i].subrowOrigin += shiftX;
      circuit.rows[i].coordinate   += (int)shiftY;
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
      fputs("        itlAnalyticalPlacer test report\n", testResFileHTM);
      fputs("        </title>\n", testResFileHTM);
      fputs("    </head>\n", testResFileHTM);
      fputs("    <body>\n", testResFileHTM);
      fputs("    <h1>\n", testResFileHTM);
      fputs("    itlAnalyticalPlacer test report\n", testResFileHTM);
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

void DumpNetWeights(char* fileName, Circuit& circuit)
{
  if (fileName == 0)
  {
    cout << "null pointer on dumping net-weights file. Skipping net-weights dump\n";
    return;
  }

  if (!gOptions.isLEFDEFinput)
  {
    cout << "Skipping dumping net-weights step\n";
    return;
  }

  FILE *netWeightsFile;
  char currString[32];

  netWeightsFile = fopen(fileName, "w");
  if (netWeightsFile && circuit.netWeights != NULL)
  {
    cout << "Dumping net-weights to " << fileName << endl;

    for (int i = 0; i < circuit.nNets; ++i)
    {
      sprintf(currString, "%f\n", circuit.netWeights[i]);
      fputs(currString, netWeightsFile);
    }

    fclose(netWeightsFile);
  }
}

void LEFDEF2Bookshelf(char* baseName, Circuit& circuit)
{
  if (gOptions.isLEFDEFinput)
  {
    size_t len = strlen(baseName);
    char* newAux = new char[len + 5];
    char* newPl = new char[len + 4];
    char* newNodes = new char[len + 7];
    char* newScl = new char[len + 5];
    char* newNets = new char[len + 6];
    char* newWts = new char[len + 5];

    strcpy(newAux, baseName);
    strcat(newAux, ".aux");
    strcpy(newPl, baseName);
    strcat(newPl, ".pl");
    strcpy(newNodes, baseName);
    strcat(newNodes, ".nodes");
    strcpy(newScl, baseName);
    strcat(newScl, ".scl");
    strcpy(newNets, baseName);
    strcat(newNets, ".nets");
    strcpy(newWts, baseName);
    strcat(newWts, ".wts");

    CreateAux(newAux, newPl, newNodes, newScl, newNets, newWts, circuit);
    CreatePl(newPl, circuit);
    CreateNodes(newNodes, circuit);
    CreateScl(newScl, circuit);
    CreateNets(newNets, circuit);
    CreateWts(newWts, circuit);

    delete[] newAux;
    delete[] newPl;
    delete[] newNodes;
    delete[] newScl;
    delete[] newNets;
    delete[] newWts;
  }
}

void CreateAux(char* newAux, char* newPl, char* newNodes, char* newScl, char* newNets, char* newWts, Circuit& circuit)
{
  FILE *auxFile;
  char string[128];

  auxFile = fopen(newAux, "w");
  
  if (auxFile)
  {
    sprintf(string, "RowBasedPlacement : %s %s %s %s %s", newPl, newNodes, newScl, newNets, newWts);
    fputs(string, auxFile);
    
    fclose(auxFile);
  }
  else
    cout << "Couldn't open aux file\n";
}

void CreatePl(char* fileName, Circuit& circuit)
{
  FILE *plFile;
  char string[128];
  time_t ltime;
  time(&ltime);

  plFile = fopen(fileName, "w");

  if (plFile)
  {
    ReshiftCoords(circuit);

    sprintf(string, "UCLA pl 1.0\n");
    fputs(string, plFile);
    sprintf(string, "# ITLab\n# Created : %s\n", ctime(&ltime));
    fputs(string, plFile);
    
    int shift_ = (int)(circuit.terminals - circuit.nodes);
    
    for (int i = 0; i < circuit.nNodes; ++i)
    {
      sprintf(string, "%8s %10.3f %10.3f : %s\n", circuit.tableOfNames[i].name, circuit.placement[i].xCoord, 
              circuit.placement[i].yCoord,  circuit.placement[i].orient);
      fputs(string, plFile);
    }

    for (int i = circuit.Shift_; i < circuit.Shift_ + circuit.nTerminals; ++i)
    {
      sprintf(string, "%8s %10.3f %10.3f : %s\n", circuit.tableOfNames[i].name, circuit.placement[i].xCoord, 
        circuit.placement[i].yCoord,  circuit.placement[i].orient);
      fputs(string, plFile);
    }
    
    ShiftCoords(circuit);

    fclose(plFile);
  }
}

void CreateNodes(char* fileName, Circuit& circuit)
{
  FILE *nodesFile;
  char string[128];
  time_t ltime;
  time(&ltime);

  nodesFile = fopen(fileName, "w");

  if (nodesFile)
  {
    sprintf(string, "UCLA nodes 1.0\n");
    fputs(string, nodesFile);
    sprintf(string, "# ITLab\n# Created : %s\n", ctime(&ltime));
    fputs(string, nodesFile);
    sprintf(string, "NumNodes : %d\n", circuit.nNodes + circuit.nTerminals);
    fputs(string, nodesFile);
    sprintf(string, "NumTerminals : %d\n\n", circuit.nTerminals);
    fputs(string, nodesFile);

    for (int i = 0; i < circuit.nNodes; ++i)
    {
      sprintf(string, "%8s %d %d\n", circuit.tableOfNames[i].name, circuit.nodes[i].width, circuit.nodes[i].height);
      fputs(string, nodesFile);
    }

    for (int i = circuit.Shift_; i < circuit.Shift_ + circuit.nTerminals; ++i)
    {
      sprintf(string, "%8s %d %d terminal\n", circuit.tableOfNames[i].name, 
              max(circuit.nodes[i].width, 1), max(circuit.nodes[i].height, 1));
      fputs(string, nodesFile);
    }

    fclose(nodesFile);
  }
}

void CreateScl(char* fileName, Circuit& circuit)
{
  FILE *sclFile;
  char string[128];
  time_t ltime;
  time(&ltime);

  sclFile = fopen(fileName, "w");

  if (sclFile)
  {
    sprintf(string, "UCLA scl 1.0\n");
    fputs(string, sclFile);
    sprintf(string, "# ITLab\n# Created : %s\n", ctime(&ltime));
    fputs(string, sclFile);
    sprintf(string, "NumRows : %d\n\n", circuit.nRows);
    fputs(string, sclFile);

    for (int i = 0; i < circuit.nRows; ++i)
    {
      fputs("CoreRow Horizontal\n", sclFile);
      
      sprintf(string, " Coordinate    : %d\n", circuit.rows[i].coordinate + (int)circuit.shiftY);
      fputs(string, sclFile);
      sprintf(string, " Height        : %d\n", circuit.rows[i].height);
      fputs(string, sclFile);
      sprintf(string, " Sitewidth     : %d\n", circuit.rows[i].siteWidth);
      fputs(string, sclFile);
      sprintf(string, " Sitespacing   : %d\n", circuit.rows[i].siteSpacing);
      fputs(string, sclFile);
      sprintf(string, " Siteorient    : %s\n", circuit.rows[i].siteorient);
      fputs(string, sclFile);
      sprintf(string, " Sitesymmetry  : %s\n", circuit.rows[i].sitesymm);
      fputs(string, sclFile);
      sprintf(string, " SubrowOrigin  : %f", circuit.rows[i].subrowOrigin + circuit.shiftX);
      fputs(string, sclFile);
      sprintf(string, " Numsites  : %d\n", circuit.rows[i].numSites);
      fputs(string, sclFile);

      fputs("End\n", sclFile);
    }

    fclose(sclFile);
  }
}

void CreateNets(char* fileName, Circuit& circuit)
{
  FILE *netsFile;
  char string[128];
  time_t ltime;
  time(&ltime);
  int cellIdx;

  netsFile = fopen(fileName, "w");

  if (netsFile)
  {
    sprintf(string, "UCLA nets 1.0\n");
    fputs(string, netsFile);
    sprintf(string, "# ITLab\n# Created : %s\n", ctime(&ltime));
    fputs(string, netsFile);
    sprintf(string, "NumNets  : %d\n", circuit.nNets);
    fputs(string, netsFile);
    sprintf(string, "NumPins : %d\n\n", circuit.nPins);
    fputs(string, netsFile);

    for (int i = 0; i < circuit.nNets; ++i)
    {
      sprintf(string, "NetDegree : %d\n", circuit.nets[i].numOfPins);
      fputs(string, netsFile);
      
      for (int j = 0; j < circuit.nets[i].numOfPins; ++j)
      {
        cellIdx = circuit.nets[i].arrPins[j].cellIdx;
        /*if (cellIdx >= circuit.nNodes)
        {
          cellIdx -= circuit.Shift_ - circuit.nNodes;
        }*/
        if (circuit.nets[i].arrPins[j].xOffset == 0 && circuit.nets[i].arrPins[j].yOffset == 0)
        {
          sprintf(string, "\t%15s\t%c\n", circuit.tableOfNames[cellIdx].name, circuit.nets[i].arrPins[j].chtype);
        } 
        else
        {
          sprintf(string, "\t%15s\t%c : %f %f\n", circuit.tableOfNames[cellIdx].name, circuit.nets[i].arrPins[j].chtype,
                  circuit.nets[i].arrPins[j].xOffset, circuit.nets[i].arrPins[j].yOffset);
        }
        fputs(string, netsFile);
      }
    }

    fclose(netsFile);
  }
}

void CreateWts(char* fileName, Circuit& circuit)
{
  FILE *wtsFile;
  char string[128];
  time_t ltime;
  time(&ltime);

  wtsFile = fopen(fileName, "w");

  if (wtsFile)
  {
    sprintf(string, "UCLA wts 1.0\n");
    fputs(string, wtsFile);
    sprintf(string, "# ITLab\n# Created : %s\n", ctime(&ltime));
    fputs(string, wtsFile);

    if (circuit.weights)
    {
      for (int i = 0; i < circuit.nNodes; ++i)
      {
        sprintf(string, "%8s %f\n", circuit.tableOfNames[i].name, circuit.weights[i].wts);
        fputs(string, wtsFile);
      }

      for (int i = circuit.Shift_; i < circuit.Shift_ + circuit.nTerminals; ++i)
      {
        sprintf(string, "%8s %f\n", circuit.tableOfNames[i].name, circuit.weights[i].wts);
        fputs(string, wtsFile);
      }
    }
    else
    {
      for (int i = 0; i < circuit.nNodes; ++i)
      {
        sprintf(string, "%8s %f\n", circuit.tableOfNames[i].name, 1.0);
        fputs(string, wtsFile);
      }

      for (int i = circuit.Shift_; i < circuit.Shift_ + circuit.nTerminals; ++i)
      {
        sprintf(string, "%8s %f\n", circuit.tableOfNames[i].name, 1.0);
        fputs(string, wtsFile);
      }
    }

    fclose(wtsFile);
  }
}