#ifndef __LOGGING_H__
#define __LOGGING_H__

#include "TimeTools.h"

typedef usec_t timetype;
#define GET_TIME_METHOD get_usec
#define GETSECONDSFROMTIME(time) ((double)((time) / 1000000.0))

void InitializeLogging();
void PrintRevisionNumber();

#ifndef LOGGGINGLEVEL
#define LOGGGINGLEVEL 1
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

//internal methods
void __log_print_fn__(int level, const char* file, int line, const char* func, const char* text);
void __alert_print_fn__(int level, const char* file, int line, const char* func, const char* text);
char* __cdecl __log__format_fn__(const char* expr, ...);
void __alert_print_fn__(const char* text);
void __cdecl __log_write_fn__(const char* expr, ...);
void __cdecl __log_writeline_fn__(const char* expr, ...);

#define LOGTEXTONLEVEL(level, text) \
  __log_print_fn__(level, __FILE__, __LINE__, __FUNCTION__, text);
#define LOGFORMATONLEVEL(level, printf_format) LOGTEXTONLEVEL(level, __log__format_fn__ printf_format)
#define ASSERTONLEVEL(level, expression) \
  if(!(expression)) __alert_print_fn__(level, __FILE__, __LINE__, __FUNCTION__, #expression);

#if defined(_DEBUG) || LOGGGINGLEVEL <= DEFAULTLEVEL
#define LOG(text) LOGTEXTONLEVEL(DEFAULTLEVEL, text)
#define LOGFORMAT(printfargs) LOGFORMATONLEVEL(DEFAULTLEVEL, printfargs)
#define ASSERT(expression) ASSERTONLEVEL(DEFAULTLEVEL, expression)
#else
#define LOG(text) ;
#define LOGFORMAT(printfargs) ;
#define ASSERT(expression) ;
#endif

#if defined(_DEBUG) || LOGGGINGLEVEL < DEBUGLEVEL
#define LOGDEBUG(text) LOGTEXTONLEVEL(DEBUGLEVEL, text)
#define LOGDEBUGFORMAT(printfargs) LOGFORMATONLEVEL(DEBUGLEVEL, printfargs)
#define DEBUG_ASSERT(expression) ASSERTONLEVEL(DEBUGLEVEL, expression)
#else
#define LOGDEBUG(text) ;
#define LOGDEBUGFORMAT(printfargs) ;
#define DEBUG_ASSERT(expression) ;
#endif

#if LOGGGINGLEVEL < INFOLEVEL
#define LOGINFO(text) LOGTEXTONLEVEL(INFOLEVEL, text)
#define LOGINFOFORMAT(printfargs) LOGFORMATONLEVEL(INFOLEVEL, printfargs)
#define INFO_ASSERT(expression) ASSERTONLEVEL(INFOLEVEL, expression)
#else
#define LOGINFO(text) ;
#define LOGINFOFORMAT(printfargs) ;
#define INFO_ASSERT(expression) ;
#endif

#if LOGGGINGLEVEL < WARNINGLEVEL
#define LOGWARNING(text) LOGTEXTONLEVEL(WARNINGLEVEL, text)
#define LOGWARNINGFORMAT(printfargs) LOGFORMATONLEVEL(WARNINGLEVEL, printfargs)
#define WARNING_ASSERT(expression) ASSERTONLEVEL(WARNINGLEVEL, expression)
#else
#define LOGWARNING(text) ;
#define LOGWARNINGFORMAT(printfargs) ;
#define WARNING_ASSERT(expression) ;
#endif

#if LOGGGINGLEVEL < ERRORLEVEL
#define LOGERROR(text) LOGTEXTONLEVEL(ERRORLEVEL, text)
#define LOGERRORFORMAT(printfargs) LOGFORMATONLEVEL(ERRORLEVEL, printfargs)
#define ERROR_ASSERT(expression) ASSERTONLEVEL(ERRORLEVEL, expression)
#else
#define LOGERROR(text) ;
#define LOGERRORFORMAT(printfargs) ;
#define ERROR_ASSERT(expression) ;
#endif

#define LOGCRITICAL(text) LOGTEXTONLEVEL(FATALERRORLEVEL, text)
#define LOGCRITICALFORMAT(printfargs) LOGFORMATONLEVEL(FATALERRORLEVEL, printfargs)
#define CRITICAL_ASSERT(expression) ASSERTONLEVEL(FATALERRORLEVEL, expression)

#define ALERT(text) __alert_print_fn__(text);
#define ALERTFORMAT(printfargs) __alert_print_fn__(__log__format_fn__ printfargs);

#define WRITE __log_write_fn__
#define WRITELINE __log_writeline_fn__

#define LOGWRITE(text) __log_write_fn__(text);
#define LOGWRITEFORMAT(printfargs) __log_write_fn__ printfargs;

#define LOGWRITELINE(text) __log_writeline_fn__(text);
#define LOGWRITELINEFORMAT(printfargs) __log_writeline_fn__ printfargs;

#endif //__LOGGING_H__