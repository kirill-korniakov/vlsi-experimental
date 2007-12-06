/* 
 * global.h
 * this is a part of itlDragon
 * Copyright (C) 2005-2006, ITLab, Kornyakov, Kurina, Zhivoderov
 * email: kirillkornyakov@yandex.ru
 * email: woody-quaker@yandex.ru
 * email: nina.kurina@gmail.com
 */
 
#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "..\include\data_structures.h"
#include "..\include\errors.h"
#include "..\include\cost_function.h"

#define RB_IDX  0
#define BS_IDX  1
#define DBS_IDX 2
#define AS_IDX  3
#define CA_IDX  4
#define OR_IDX  5
#define DP_IDX  6

#define NUM_STAGES     7
#define MAX_NUM_PARAMS 15

struct Options
{
  bool doRecursiveBisection;
  bool doBinSwapping;
  bool doDirectedBinSwapping;  
  bool doCellAnnealing;
  bool doOverlapRemoving;
  bool doDetailedPlacement;
  bool doCheckLegality;
  
  char benchmarkName[256];
  char plName[256];
  char configName[256];

  bool doDumpGP;
  bool onlyGP;
  
  bool doLog;
  bool doTest;

  double innerParameters[NUM_STAGES][MAX_NUM_PARAMS];
};
extern Options gOptions;

void SetDefaultKeysValues();

inline double dtoi(double x)
{
  char tmp[64];
  sprintf(tmp, "%.0f", x);
  return atof(tmp);
}

inline double round8(double x)
{
  char tmp[64];
  sprintf(tmp, "%.8f", x);
  return atof(tmp);
}

struct Statistics
{
  char   statisticsFileName[256];

  double currentWL;

  // WL improvement of each phase (in percents)
  double directedBinSwappingWLI;
  double cellAnnealingWLI;
  double localImprovementWLI;

  // work time of each phase (in seconds)
  double totalWT;
  double recursiveBisectionWT;
  double binSwappingWT;
  double adjustmentStepWT;
  double directedBinSwappingWT;
  double cellAnnealingWT;
  double localImprovementWT;
};

void InitializeStatistics(Statistics& statistics);

extern const char* resultFileName;

// for time measurement
extern const double CLOCK_RATE;  // clock rate of our processors
extern __int64 rngBSTotalTime;
extern __int64 rngCATotalTime;
extern __int64 rngLITotalTime;

extern int rnCATotalCount;

ITLDRAGON_ERROR Initialization(Circuit& circuit, Statistics& statistics);
void MakeTableOfConnections(Circuit& circuit);
void FreeMemory();
void Exit();

void CheckCode(ITLDRAGON_ERROR errorCode);
void PrintErrorMessage(char* errorMsg, ITLDRAGON_ERROR errorCode);

class Tccout
{
  public:
    FILE *logFile;
    void Start()
    {
      logFile = fopen( "itlDragon.log", "a+" );
      char cmdString[512];
      char buffer[256];
      time_t ltime;
      time(&ltime);
      strcpy(cmdString, (char *)GetCommandLine());
      
      /*
---------------------------------------------
# Log file name:    dragon.log
# Command line:        ./dragon3 -f ./ibm01.aux 
# Date :        Sat Sep 16 14:05:57 2006

---------------------------------------------*/
      if (gOptions.doLog)
      {
        logFile = fopen("itlDragon.log", "a+");
        fputs("\n\n---------------------------------------------\n", logFile);
        fputs("# Log file name:  itlDragon.log\n", logFile);
        fputs("# Command line:   ", logFile);
        fputs(cmdString, logFile);
        sprintf(buffer, "\n# Date:           %s", ctime(&ltime));
        fputs(buffer, logFile );
        fputs("\n---------------------------------------------\n\n", logFile);
        fclose(logFile );
      }  
    }
    Tccout()
    {
      
    }
    ~Tccout(){ /*fclose( logFile );*/ }
    Tccout& operator<< (const char *s)
    {
      std::cout << s;
      //printf( s );
      if (gOptions.doLog)
      {
        logFile = fopen("itlDragon.log", "a+");
        fputs(s, logFile);
        fclose(logFile);
      }
      return *this;
    }
    Tccout& operator<< (const int a)
    {
      //std::cout << a;
      printf("%d", a);
      if (gOptions.doLog)
      {
        char s[32];
        logFile = fopen("itlDragon.log", "a+");
        sprintf(s, "%d", a);
        fputs(s, logFile);
        fclose(logFile);
      }
      return *this;
    }
    Tccout& operator<< (const double a)
    {
      //std::cout << a;
      printf("%.3f", a);
      if (gOptions.doLog)
      {
        char s[32];
        logFile = fopen("itlDragon.log", "a+");
        sprintf(s, "%f", a);
        fputs(s, logFile);
        fclose(logFile);
      }
      return *this;
    }
};
#define cout ccout
#define endl "\n"

extern Tccout ccout;

#endif