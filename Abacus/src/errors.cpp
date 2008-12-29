#include <iostream>
#include "..\include\errors.h"

using namespace std;

#if 0
bool doLog = true;
#else
bool doLog = false;
#endif

void Trace(std::string message, bool isVisible)
{
  if (doLog && isVisible)
    cout << "-- " << message.c_str() << endl;
}

void LogEnter(string message, bool isVisible)
{
  if (doLog && isVisible)
    cout << ">> " << message.c_str() << endl;
}

void LogExit(string message, bool isVisible)
{
  if (doLog && isVisible)
    cout << "<< " << message.c_str() << endl;
}


//void CheckCode(MULTIPLACER_ERROR errorCode)
//{
//  if (errorCode == OK) return;
//  
//  char errorMessage[256];
//  PrintErrorMessage( errorMessage, errorCode );
//  cout << errorMessage;
//  
//  char errorString[128];
//  if (errorCode)
//  {
//    FILE *resultFile;
//    resultFile = fopen(resultFileName, "a");
//    strcpy( errorString, "benchmark: \t%s\t " );
//    strcat( errorString, errorMessage);
//    if(resultFile)
//    {
//      fprintf( resultFile, errorString, gOptions.benchmarkName);
//    }
//    fclose (resultFile);
//    Exit();
//  }
//}

//void PrintErrorMessage(char* errorMsg, int errorCode)
//{
//  if (errorCode != 0)
//  {
//    char errorCodeStr[256];
//    int firstDigit   = static_cast<int>( errorCode / 100 );
//    int errorSubCode = errorCode % 100;
//    char stageName[128];
//    switch (firstDigit)
//    {
//      case 0: strcpy( stageName, "parsing"); break;
//      case 1: strcpy( stageName, "global placement"); break;
//      case 2: strcpy( stageName, "bin-swapping"); break;
//      case 3: strcpy( stageName, "adjustment step"); break;
//      case 4: strcpy( stageName, "directed bin-swapping"); break;
//      case 5: strcpy( stageName, "cell annealing"); break;
//      case 6: strcpy( stageName, "overlap_removing"); break;
//      case 7: strcpy( stageName, "detailed placement"); break;
//      case 8: strcpy( stageName, "legality cheking"); break;
//      case 9: strcpy( stageName, "output"); break;
//    }
//    strcpy( errorMsg, "Program aborted because of error during ");
//    strcat( errorMsg, stageName );
//    strcat( errorMsg, ". Error code: " );
//    itoa( errorCode, errorCodeStr, 10 );
//    strcat( errorCodeStr, "\n" );
//    strcat( errorMsg, errorCodeStr );
//  }
//  else
//    strcpy( errorMsg, "OK\n" );
//    //strcpy( errorMsg, "\nNo errors detected while programm execution\nDeleting memory...\n" );
//}