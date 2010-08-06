#ifndef __LOGGING_H__
#define __LOGGING_H__

#include "TimeTools.h"
#include "stdTypes.h"
#include "Color.h"

typedef usec_t timetype;
#define GET_TIME_METHOD get_usec
#define GETSECONDSFROMTIME(time) ((double)((time) / 1000000.0))

#ifndef LOGGGINGLEVEL
#define LOGGGINGLEVEL 0
#endif

//display levels
#define DEFAULTLEVEL       0
#define DEBUGLEVEL         1
#define INFOLEVEL          2
#define WARNINGLEVEL       3
#define ERRORLEVEL         4
#define FATALERRORLEVEL    5
#define CRITICALERRORLEVEL 5
#define ALERTLEVEL         6

#if defined(_DEBUG) || LOGGGINGLEVEL <= DEBUGLEVEL
#define LOGGER_REQUIRES_DEBUG_LEVEL_PRINT
#endif

#if LOGGGINGLEVEL <= INFOLEVEL
#define LOGGER_REQUIRES_INFO_LEVEL_PRINT
#endif

#if LOGGGINGLEVEL <= WARNINGLEVEL
#define LOGGER_REQUIRES_WARNING_LEVEL_PRINT
#endif

#if LOGGGINGLEVEL <= ERRORLEVEL
#define LOGGER_REQUIRES_ERROR_LEVEL_PRINT
#endif

struct LogCortege
{
  const char* file;
  int line;
  const char* func;

  LogCortege(const char* afile, int aline, const char* afunc)
    : file(afile), line(aline), func(afunc)
  {}
};

#define LOGINPLACE LogCortege(__FILE__, __LINE__, __FUNCTION__)

#define ASSERT(expression) {if(!(expression)) Logger::Global.__PrintAssert(LOGINPLACE, #expression);}
#define CRITICAL_ASSERT(expression) {if(!(expression)) Logger::Global.__PrintAssert(LOGINPLACE, #expression, true);}
#if defined(_DEBUG)
#define DASSERT(expression) {if(!(expression)) Logger::Global.__PrintAssert(LOGINPLACE, #expression);}
#else
#define DASSERT(expression) {}
#endif

#define LOGDEBUG(text) Logger::Global.Debug(LOGINPLACE, text)
#define LOGINFO(text) Logger::Global.Info(LOGINPLACE, text)
#define LOGWARNING(text) Logger::Global.Warning(LOGINPLACE, text)
#define LOGERROR(text) Logger::Global.Error(LOGINPLACE, text)
#define LOGCRITICAL(text) Logger::Global.FatalError(LOGINPLACE, text)

#define GLOGDEBUG Logger::Global.Debug
#define GLOGINFO Logger::Global.Info
#define GLOGWARNING Logger::Global.Warning
#define GLOGERROR Logger::Global.Error
#define GLOGCRITICAL Logger::Global.FatalError

#define ALERT Logger::Global.Alert
#define WRITE Logger::Global.Write
#define WRITELINE Logger::Global.WriteLine

//config class definition
namespace libconfig
{
  class ConfigExt;
}

class Logger
{
private:
  //static
  static unsigned ms_ErrorCounter;
  static unsigned ms_WarningCounter;

  static unsigned ms_MaxErrorsAllowed;
  static unsigned ms_MaxErrorsToDisplay;
  static unsigned ms_MaxWarningsToDisplay;

  static bool ms_ExitOnAssert;

  static timetype ms_AppStartTime;
  static time_t ms_AppStartDate;

  static FILE* ms_LogFile;
  static FILE* ms_LogHtmlFile;

  static string ms_LogFileName;
  static string ms_LogHtmlFileName;
  static string ms_LogDuplicateFileName;
  static string ms_CSSFileName;

  static bool ms_WriteHtmlLog;
  static bool ms_DuplicateOutput;
  static bool ms_EmbeedCSS;

  //static functions
  static void WriteToHTMLStream(const char* text);//+
  static void WriteHTMLHeader();
  static void WriteHTMLFooter();

  void vAlert(const char* pattern, va_list argList);
  void vAlert(Color col, const char* pattern, va_list argList);

  //member functions
  void Initialize(libconfig::ConfigExt* cfg, Logger* parent);//+-

public:
  static Logger Global;

  static void InitializeLogging(libconfig::ConfigExt& cfg);
  static void RenameDuplicatedFiles(const string& newName);//+
  static double GetUptime() { return (GETSECONDSFROMTIME(GET_TIME_METHOD() - Logger::ms_AppStartTime)); }
  static time_t GetStartTime() {return ms_AppStartDate; }

  bool HasHTMLStream() const {return ms_WriteHtmlLog; }

  //public members
  bool Verbose;
  int DisplayLevel;

  //public functions
  void Write(const char* pattern, ...);//+
  void WriteLine(const char* pattern, ...);//+
  void WriteLine();//+

  void WriteIgnoringHTML(const char* pattern, ...);//+

  void WriteToHTMLStream(bool encode, const char* pattern, ...);//+

  void __PrintAssert(const LogCortege& lcort, const char* text, bool forceExit = false);//+

  void Alert(const char* pattern, ...);//+
  void Alert(std::string pattern, ...);//+
  void Alert(Color col, const char* pattern, ...);//+
  void Alert(Color col, std::string pattern, ...);//+

  void PrintRevisionNumber();//+

#ifdef LOGGER_REQUIRES_DEBUG_LEVEL_PRINT
  void Debug(const LogCortege& lcort, const char* pattern, ...);
#else
  void Debug(const LogCortege& lcort, const char* pattern, ...) {}
#endif

#ifdef LOGGER_REQUIRES_INFO_LEVEL_PRINT
  void Info(const LogCortege& lcort, const char* pattern, ...);
#else
  void Info(const LogCortege& lcort, const char* pattern, ...) {}
#endif

#ifdef LOGGER_REQUIRES_WARNING_LEVEL_PRINT
  void Warning(const LogCortege& lcort, const char* pattern, ...);
#else
  void Warning(const LogCortege& lcort, const char* pattern, ...) {ms_WarningCounter++;}
#endif

#ifdef LOGGER_REQUIRES_ERROR_LEVEL_PRINT
  void Error(const LogCortege& lcort, const char* pattern, ...);
#else
  void Error(const LogCortege& lcort, const char* pattern, ...)
  {
    ms_ErrorCounter++;
    if (ms_ErrorCounter > ms_MaxErrorsAllowed)
    {
      fprintf(ms_LogFile, "Reached maximum errors count.\nApplication has to be terminated.\n");
      if (ms_WriteHtmlLog)
        WriteToHTMLStream("<span class=\"maximumErrors\">Reached maximum errors count.<br />Application has to be terminated.</span><br />\n");
      exit(1);
    }
  }
#endif

  //fatal error works always
  void FatalError(const LogCortege& lcort, const char* pattern, ...);
};

#endif //__LOGGING_H__