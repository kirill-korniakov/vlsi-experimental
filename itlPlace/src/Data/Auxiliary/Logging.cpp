#include "Logging.h"
#include <stdio.h>
#include <stdlib.h>
#include "Configuration.h"
#include "stdTypes.h"
#include <stdarg.h>

extern long RevisionNumber;

void PrintRevisionNumber()
{
  if (RevisionNumber != 0)
    printf("Running itlPlace revision: %d\n", RevisionNumber);
  else
    printf("Running unknown revision of itlPlace\n");
}

namespace logging
{

const char* gcNames[] = {
  "NULL",
  "DEBUG",
  "INFO",
  "WARNING",
  "ERROR",
  "FATAL ERROR",
  "ALERT"
};

int gDisplayLevel = 2;
int gDefaultLogLevel = 2;
int gDefaultAssertLevel = 1;
unsigned int gMaximumDisplayWarnings = -1;
unsigned int gDisplayedWarnings = 0;
unsigned int gMaximumDisplayErrors = -1;
unsigned int gDisplayedErrors = 0;
unsigned int gMaximumErrors = 3;
bool gExitOnAssert = false;
FILE* gLogFile = stdout;
timetype gAppStartTime = 0;
__declspec(thread) char gLogBuffer[8096] = {0};

}

void InitializeLogging()
{
  logging::gAppStartTime = GET_TIME_METHOD();
  logging::gDisplayLevel = gCfg.ValueOf("logger.displayLevel", 2);
  logging::gDefaultLogLevel = gCfg.ValueOf("logger.defaultLogLevel", 2);
  logging::gDefaultAssertLevel = gCfg.ValueOf("logger.defaultAssertLevel", 1);
  logging::gMaximumDisplayWarnings = gCfg.ValueOf("logger.maximumDisplayWarnings", -1);
  logging::gMaximumDisplayErrors = gCfg.ValueOf("logger.maximumDisplayErrors", -1);
  logging::gMaximumErrors = gCfg.ValueOf("logger.maximumErrors", 3);
  logging::gExitOnAssert = gCfg.ValueOf("logger.exitOnAssert", false);
  string logfile = gCfg.ValueOf("logger.logFile", string("stdout"));
  if (logfile == "stdout")
    logging::gLogFile = stdout;
  else if (logfile == "stderr")
    logging::gLogFile = stderr;
  else
  {
    logging::gLogFile = fopen(logfile.c_str(), "a+");
    if(logging::gLogFile == NULL)
    {
      logging::gLogFile = stdout;
      LOGWARNING("Unable to open log file.")
    }
  }
}

#define SECONDSELAPSED (GETSECONDSFROMTIME(GET_TIME_METHOD() - logging::gAppStartTime))
#define LOGTEMPLATE "[%10.3f] %s in %s: %s\n"
#define ASSERTTEMPLATE "[%10.3f] %s ASSERT in %s (%s)\n\t %s(line:%d)\n"
#define ALERTTEMPLATE "[%10.3f] %s\n"

#define PRINTLOG fprintf(logging::gLogFile, LOGTEMPLATE, SECONDSELAPSED, logging::gcNames[level], func, text, file, line)
#define PRINTASSERT fprintf(logging::gLogFile, ASSERTTEMPLATE, SECONDSELAPSED, logging::gcNames[level], func, text, file, line)
#define PRINTALERT fprintf(logging::gLogFile, ALERTTEMPLATE, SECONDSELAPSED, text)
#define ERRORSMAXIMUMCOUNTREACHED fprintf(logging::gLogFile, "Reached maximum errors count.\nApplication has to be terminated.\n")

void __log_print_fn__(int level, const char* file, int line, const char* func, const char* text)
{
  if(level == 0) level = logging::gDefaultLogLevel;
  switch(level)
  {
  case WARNINGLEVEL:
    if(logging::gDisplayedWarnings++ > logging::gMaximumDisplayWarnings)
      return;
    break;
  case ERRORLEVEL:
    if(logging::gMaximumErrors <= logging::gDisplayedErrors)
    {
      PRINTLOG;
      ERRORSMAXIMUMCOUNTREACHED;
      exit(1);
    }
    if(logging::gDisplayedErrors++ > logging::gMaximumDisplayErrors)
      return;
    break;
  case FATALERRORLEVEL:
    PRINTLOG;
    exit(1);
    break;
  case ALERTLEVEL:
    __alert_print_fn__(text);
    return;
  }
  if(level >= logging::gDisplayLevel)
    PRINTLOG;
}

void __alert_print_fn__(int level, const char* file, int line, const char* func, const char* text)
{
  if(level == 0) level = logging::gDefaultAssertLevel;
  if(logging::gExitOnAssert || level == FATALERRORLEVEL)
  {
    PRINTASSERT;
    exit(1);
  }
  switch(level)
  {
  case WARNINGLEVEL:
    if(logging::gDisplayedWarnings++ > logging::gMaximumDisplayWarnings)
      return;
    break;
  case ERRORLEVEL:
    if(logging::gMaximumErrors <= logging::gDisplayedErrors)
    {
      PRINTASSERT;
      ERRORSMAXIMUMCOUNTREACHED;
      exit(1);
    }
    if(logging::gDisplayedErrors++ > logging::gMaximumDisplayErrors)
      return;
    break;
  }
  if(level >= logging::gDisplayLevel)
    PRINTASSERT;
}

char* __cdecl __log__format_fn__(const char* expr, ...)
{
  va_list argptr;
  va_start(argptr, expr);
  vsprintf(logging::gLogBuffer, expr, argptr);
  va_end(argptr);
  return logging::gLogBuffer;
}

void __cdecl __log_write_fn__(const char* expr, ...)
{
  va_list argptr;
  va_start(argptr, expr);
  vfprintf(logging::gLogFile, expr, argptr);
  va_end(argptr);
}

void __cdecl __log_writeline_fn__(const char* expr, ...)
{
  va_list argptr;
  va_start(argptr, expr);
  vfprintf(logging::gLogFile, expr, argptr);
  fprintf(logging::gLogFile, "\n");
  va_end(argptr);
}

void __alert_print_fn__(const char* text)
{
  PRINTALERT;
}

double GetUptime()
{
  return SECONDSELAPSED;
}