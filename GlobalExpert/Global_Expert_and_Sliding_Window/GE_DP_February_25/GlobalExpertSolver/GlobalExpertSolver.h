/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//                    NIZHNY NOVGOROD STATE UNIVERSITY                     //
//                                                                         //
//                    Copyright (c) 1999-2008 by NNSU.                     //
//                          All Rights Reserved.                           //
//                                                                         //
//  File:      GlobalExpert.h                                              //
//                                                                         //
//  Purpose:   Header for Global Expert global optimization solver         //
//                                                                         //
//  Author(s): V. Ryabov                                                   //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#ifndef __GLOBAL_EXPERT_H__
#define __GLOBAL_EXPERT_H__

namespace GlobalExpert
{

//#define GLOBAL_EXPERT_EXPORTS

#ifdef GLOBAL_EXPERT_EXPORTS
#define GLOBAL_EXPERT_API __declspec(dllexport)
#else
#define GLOBAL_EXPERT_API __declspec(dllimport)
#endif

#define MAX_PATH_LENGTH 512

typedef double (__cdecl *tMultiFunction)(const double*, int);
typedef double (__cdecl *tFunction)(const double*);

const int GE_OK = 0;

class OptimProcess;
class Logger;



// ---------------------------------------------------------------------------
struct TGlobExpResults
{
  // basic result data
  double* foundY;
  double foundF;

  // additional measures
  int maximumIndex;
  int* indexesCounts;

  // for internal comparisons
  double* knownY;
  double knownF;
  int* indexesCountsOpCharY;
  int* indexesCountsOpCharF;
  int evolveIndexCountY;
  int evolveIndexCountF;

  TGlobExpResults();
  ~TGlobExpResults();
};

// ---------------------------------------------------------------------------
class TGlobExpSolverParams
{
public:
  bool NeedToLoad;
  char PathToLoad[MAX_PATH_LENGTH];
  bool NeedToSaveAfterStop;
  char PathToSave[MAX_PATH_LENGTH];

  bool NeedToSendLoadedPointsToGUI;
  int SocketPort;
  char ServerIP[MAX_PATH_LENGTH];

  int N;

  int NumOfCriteria;
  int* OrderOfCriteria;
  int NumOfConstraints;
  int* OrderOfConstraints;

  tMultiFunction targetFunctions;
  tFunction* constraints;
  tFunction* criteria;

  double* a;
  double* b;

  int StorageType;
  int LocalQueueSize;
  int BlockSize;
  int MaxNumOfBlocks;

  int MethodID;
  int m;
  int L;
  double r;
  int IterMax;
  double eps;
  double qReservParam;
  int NumOfModification;
  double* firstPoint;

  char resultsFile[MAX_PATH_LENGTH];
  char progressFile[MAX_PATH_LENGTH];
  char cacheDir[MAX_PATH_LENGTH];

  bool performanceLogging;
  bool processLogging;
  bool sendRecvLogging;

  TGlobExpSolverParams();
  TGlobExpSolverParams(const tMultiFunction& objectiveFunction);
  TGlobExpSolverParams(const tFunction& objectiveFunction);
  ~TGlobExpSolverParams();

  void EnableOptionalLoggers();
  void SetDomain(const double* _a, const double* _b, const int _N);
  void SetDomain(const double* _a, const double* _b);
  void SetFirstPoint(const double* _firstY);
  void SetFirstPoint(const double* _firstY, const int _N);

  void SetConstraints(const tFunction* functions, const int numOfConstraints);
  void SetCriteria(const tFunction* functions, const int numOfCriteria);
  void SetMultiFunction(const tMultiFunction& objectiveFunction);//, const int numOfConstraints);

  bool IsTaskCorrect(char* errorMsg);
  bool Solve(TGlobExpResults& searchResults, char* errorMsg);

  // optional loggers
  Logger* performanceLogger;
  Logger* processLogger;
  Logger* sendRecvLogger;

  // mandatory loggers
  Logger* resultsFileLogger;
  Logger* progressFileLogger;

private:
  int mpi_np, mpi_rank;
  double duration;
  char exePlacementFolder[MAX_PATH_LENGTH];

  OptimProcess* Process;
  int ActiveJob;

  //double* foundY;
  //double* foundF;
  TGlobExpResults m_searchResults;

  void Run();
  void ReRun();
  void ShowParameters();
  void ShowOptimum(int criterion = -1, bool isWithoutException = false);

  //void ShowIterResults();
};

/*
class GlobalExpertSolver
{
private:
  bool NeedToLoad;
  char* PathToLoad;
  bool NeedToSendLoadedPointsToGUI;
  bool NeedToSaveAfterStop;
  char* PathToSave;
  int SocketPort;
  char* ServerIP;

  //char dllName[MAX_PATH_LENGTH];
  int N;

  tfunction* functions;

  int NumOfCriteria;
  int* OrderOfCriteria;
  int NumOfConstraints;
  int* OrderOfConstraints;

  int StorageType;
  int LocalQueueSize;
  int BlockSize;
  int MaxNumOfBlocks;

  int MethodID;
  int m;
  int L;
  double r;
  int IterMax;
  double eps;
  double qReservParam;
  int NumOfModification;
  double* firstPoint;

  char* resultsFile;
  char* progressFile;

  bool performanceLogging;
  bool processLogging;
  bool sendRecvLogging;

  void InitDefault();

public:
  GlobalExpertSolver(tfunction& objectiveFunction);
  ~GlobalExpertSolver();
};
*/
} // namespace GlobalExpert

#endif // __GLOBAL_EXPERT_H__
// - end of file --------------------------------------------------------------
