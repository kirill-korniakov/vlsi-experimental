#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include "Logging.h"
#include "Configuration.h"
#include "Auxiliary.h"
#if defined(WIN32) || defined(_WIN64)
#include <windows.h>
#endif


#define LOCALBUFSIZE 1024

static int gConsoleColors[][3] =
{
  {   0,   0,   0 },//0
  {   0,   0, 128 },//1
  {   0, 128,   0 },//2
  {   0, 128, 128 },//3
  { 128,   0,   0 },//4
  { 128,   0, 128 },//5
  { 128, 128,   0 },//6
  { 192, 192, 192 },//7
  { 128, 128, 128 },//8
  {   0,   0, 255 },//9
  {   0, 255,   0 },//a
  {   0, 255, 255 },//b
  { 255,   0,   0 },//c
  { 255,   0, 255 },//d
  { 255, 255,   0 },//e
  { 255, 255, 255 } //f
};

unsigned FindNearestConsoleColor(Color col)
{
  int r,g,b;
  ToRGB(col, r, g, b);

  unsigned best = 0;
  int distance = 195076;
  for(unsigned i = 0; i < 16; ++i)
  {
    int r1 = gConsoleColors[i][0] - r;
    int g1 = gConsoleColors[i][1] - g;
    int b1 = gConsoleColors[i][2] - b;
    int d = r1 * r1 + g1 * g1 + b1 * b1;
    if (d < distance)
    {
      best = i;
      distance = d;
    }
  }
  return best;
}

void SetConsoleTextColor(unsigned colId)
{
#if defined(WIN32) || defined(_WIN64)
  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(hStdOut, (WORD)colId);
#endif
}

void SetConsoleTextColor(Color col)
{
  SetConsoleTextColor(FindNearestConsoleColor(col));
}

void RestoreConsoleColor()
{
  SetConsoleTextColor(7);
}

unsigned Logger::ms_ErrorCounter         = 0;
unsigned Logger::ms_WarningCounter       = 0;
unsigned Logger::ms_MaxErrorsAllowed     = -1;
unsigned Logger::ms_MaxErrorsToDisplay   = -1;
unsigned Logger::ms_MaxWarningsToDisplay = -1;
bool     Logger::ms_ExitOnAssert         = true;
timetype Logger::ms_AppStartTime         = GET_TIME_METHOD();
time_t   Logger::ms_AppStartDate         = time(0);
FILE*    Logger::ms_LogFile              = stdout;
FILE*    Logger::ms_LogHtmlFile          = 0;
string   Logger::ms_LogFileName          = "stdout";
string   Logger::ms_LogHtmlFileName      = "itlOutput.htm";
string   Logger::ms_LogDuplicateFileName = "itlOutput.log";
bool     Logger::ms_WriteHtmlLog         = false;
bool     Logger::ms_DuplicateOutput      = false;
bool     Logger::ms_EmbeedCSS            = true;
string   Logger::ms_CSSFileName          = "itlPlace.css";

Logger   Logger::Global;

extern void DuplicateConsoleOutput(const string& fileName);

void Logger::InitializeLogging(libconfig::ConfigExt& cfg)
{
  ConfigContext ctx = cfg.OpenContext("Logger");

  ms_MaxWarningsToDisplay = (unsigned)cfg.ValueOf<int>(".maximumDisplayWarnings", -1);
  ms_MaxErrorsToDisplay =   (unsigned)cfg.ValueOf<int>(".maximumDisplayErrors", -1);
  ms_MaxErrorsAllowed =     (unsigned)cfg.ValueOf<int>(".maximumErrors", 3);
  ms_ExitOnAssert =         cfg.ValueOf(".exitOnAssert", true);
  ms_LogFileName =          cfg.ValueOf(".logFile", string("stdout"));
  ms_DuplicateOutput =      cfg.ValueOf(".duplicateConsoleOutput", true);
  ms_CSSFileName =          cfg.ValueOf(".CSSFileName", "itlPlace.css");
  ms_EmbeedCSS =            cfg.ValueOf(".EmbeedCSS", true);
  if (ms_DuplicateOutput)
    ms_LogDuplicateFileName = Aux::CreateCoolFileName("log\\", cfg.ValueOf(".DupLogFile", "itlOutput"), "log");

  if (ms_DuplicateOutput)
    DuplicateConsoleOutput(ms_LogDuplicateFileName);

  Global.Initialize(&cfg, 0);

  if (ms_LogFileName == "stdout")
    ms_LogFile = stdout;
  else if (ms_LogFileName == "stderr")
    ms_LogFile = stderr;
  else
  {
    ms_LogFile = fopen(ms_LogFileName.c_str(), "a");
    if(ms_LogFile == NULL)
    {
      ms_LogFile = stdout;
      Global.Warning(LOGINPLACE, "Unable to open log file.");
    }
  }

  ms_WriteHtmlLog =         cfg.ValueOf(".writeHTML", false);
  if (ms_WriteHtmlLog)
    ms_LogHtmlFileName =    Aux::CreateCoolFileName("log\\", cfg.ValueOf(".HTMLLogFile", "itlOutput"), "htm");

  if (ms_WriteHtmlLog)
  {
    ms_LogHtmlFile = fopen(ms_LogHtmlFileName.c_str(), "w");
    if(ms_LogHtmlFile == NULL)
    {
      ms_WriteHtmlLog = false;
      Global.Warning(LOGINPLACE, "Unable to create HTML log file.");
    }
    else
    {
      WriteHTMLHeader();
      atexit(Logger::WriteHTMLFooter);
    }
  }
}

void Logger::Initialize(libconfig::ConfigExt* cfg, Logger* parent)
{
  if (cfg == 0)
  {
    Verbose = parent == 0 ? true : parent->Verbose;
    DisplayLevel = parent == 0 ? DEFAULTLEVEL : parent->DisplayLevel;
  }
  else
  {
    Verbose = cfg->ValueOf(".Verbose", parent == 0 ? true : parent->Verbose);
    DisplayLevel = cfg->ValueOf(".LogDisplayLevel", parent == 0 ? DEFAULTLEVEL : parent->DisplayLevel);
  }
}

int IndexOfHtmlEncodingChars(const char* str, int startPos)
  {
    const char* chPtr2 = str + startPos;
    while (*chPtr2 > 0)
    {
      if (*chPtr2 <= '>')
      {
        switch (*chPtr2)
        {
        case '<':
        case '>':
        case '"':
        case '&':
          return (int)(chPtr2 - str);
        }
      }
      chPtr2++;
    }
    return -1;
  }

string HtmlEncode(const char* s)
{
  if (s == 0 || *s == 0) return "";
  int num = IndexOfHtmlEncodingChars(s, 0);
  if (num == -1) return s;


  string encoded;
  encoded.reserve(strlen(s) + 5);
  int startIndex = 0;

  while(num > 0 && s[startIndex] != 0)
  {
    if (num > startIndex)
      encoded.append(s + startIndex, num - startIndex);
    char ch = s[num];
    if (ch > '>')
    {
      char buf[10];
      int buflen = 0;
      sprintf(buf, "&#%d;%n", (int)ch, &buflen);
      encoded.append(buf, buflen);
    }
    else
    {
      switch (ch)
      {
      case '<':
        encoded.append("&lt;");
        break;
      case '>':
        encoded.append("&gt;");
        break;
      case '&':
        encoded.append("&amp;");
        break;
      case '"':
        encoded.append("&quot;");
        break;
      }
    }

    startIndex = num + 1;
    if (s[startIndex] != 0)
    {
      num = IndexOfHtmlEncodingChars(s, startIndex);
      if (num == -1)
        encoded.append(s + startIndex);
    }
  }
  return encoded;
}

void GetLinebreaksCount(const char* s, int& startbreaks, int& endbreaks, int& strlen, bool& hasinnerbreaks)
{
  //s must have non-zero length
  const char* str = s;

  startbreaks = 0;
  endbreaks = 0;

  while(*str != 0)
  {
    if (*str == '\n')
      startbreaks++;
    else if (*str >= ' ' && *str != '\t')
      break;
    ++str;
  }

  int breaksNum = 0;
  while(*str != 0)
  {
    if (*str == '\n')
    {
      ++breaksNum;
      ++endbreaks;
    }
    else if (*str >= ' ' && *str != '\t')
      endbreaks = 0;
    ++str;
  }

  strlen = (int)(str - s);
  hasinnerbreaks = breaksNum > endbreaks;
}

string HtmlEncode2(const char* s)
{
  if (s == 0 || *s == 0) return "";

  int strlen = 0;
  int startbreaks = 0;
  int endbreaks = 0;
  bool hasinnerbreaks = false;
  GetLinebreaksCount(s, startbreaks, endbreaks, strlen, hasinnerbreaks);

  int num = IndexOfHtmlEncodingChars(s, 0);
  if (num == -1 && !hasinnerbreaks && endbreaks == 0 && startbreaks == 0) return s;

  string encoded;
  encoded.reserve(strlen + 5 + (hasinnerbreaks ? 9 : (startbreaks + endbreaks) * 6));

  if (hasinnerbreaks)
    encoded.append("<pre>");
  else
    for(int i1 = 0; i1 < startbreaks; ++i1)
      encoded.append("<br />");

  int startIndex = 0;
  while(num > 0 && s[startIndex] != 0)
  {
    if (num > startIndex)
      encoded.append(s + startIndex, num - startIndex);
    char ch = s[num];
    if (ch > '>')
    {
      char buf[10];
      int buflen = 0;
      sprintf(buf, "&#%d;%n", (int)ch, &buflen);
      encoded.append(buf, buflen);
    }
    else
    {
      switch (ch)
      {
      case '<':
        encoded.append("&lt;");
        break;
      case '>':
        encoded.append("&gt;");
        break;
      case '&':
        encoded.append("&amp;");
        break;
      case '"':
        encoded.append("&quot;");
        break;
      }
    }

    startIndex = num + 1;
    if (s[startIndex] != 0)
    {
      num = IndexOfHtmlEncodingChars(s, startIndex);
      if (num == -1)
        encoded.append(s + startIndex);
    }
  }

  if (hasinnerbreaks)
    encoded.append("</pre>");
  else
    for(int i2 = 0; i2 < endbreaks; ++i2)
      encoded.append("<br />");

  return encoded;
}

static void WriteToHTMLStream(bool encode, const char* pattern, FILE* file, va_list argList)
{
  if (!encode)
    vfprintf(file, pattern, argList);
  else
  {
    char cbuffer[LOCALBUFSIZE];
    int len = vsnprintf(cbuffer, LOCALBUFSIZE, pattern, argList);
    if (len < 0) return;//error occured
    if (len >= LOCALBUFSIZE)
    {
      char* dbuffer = 0;
      try
      {
        dbuffer = new char[len + 1];
        vsnprintf(dbuffer, len + 1, pattern, argList);
        dbuffer[len] = 0;
        string encoded = HtmlEncode2(dbuffer);
        fwrite(encoded.c_str(), 1, encoded.size(), file);
      }
      catch(...)
      {
        delete[] dbuffer;
        throw;
      }
      delete[] dbuffer;
    }
    else
    {
      string encoded = HtmlEncode2(cbuffer);
      fwrite(encoded.c_str(), 1, encoded.size(), file);
    }
  }
}

void Logger::WriteToHTMLStream(bool encode, const char* pattern, ...)
{
  if (!ms_WriteHtmlLog || ms_LogHtmlFile == 0) return;

  va_list  argList;
  va_start(argList, pattern);

  ::WriteToHTMLStream(encode, pattern, ms_LogHtmlFile, argList);

  va_end(argList);
}

void Logger::WriteToHTMLStream(const char* text)
{
  if (ms_LogHtmlFile != 0)
    fwrite(text, 1, strlen(text), ms_LogHtmlFile);
}

void Logger::Write(const char* pattern, ...)
{
  if (!Verbose) return;
  va_list  argList;
  va_start(argList, pattern);

  vfprintf(ms_LogFile, pattern, argList);

  if (ms_WriteHtmlLog)
  {
    WriteToHTMLStream("<span class=\"text\">");
    ::WriteToHTMLStream(true, pattern, ms_LogHtmlFile, argList);
    WriteToHTMLStream("</span>");
  }

  va_end(argList);
}

void Logger::WriteLine(const char* pattern, ...)
{
  if (!Verbose) return;
  va_list  argList;
  va_start(argList, pattern);

  vfprintf(ms_LogFile, pattern, argList);
  fwrite("\n", 1, 1, ms_LogFile);

  if (ms_WriteHtmlLog)
  {
    WriteToHTMLStream("<span class=\"text\">");
    ::WriteToHTMLStream(true, pattern, ms_LogHtmlFile, argList);
    WriteToHTMLStream("</span><br />\n");
  }

  va_end(argList);
}

void Logger::WriteLine()
{
  if (!Verbose) return;
  fwrite("\n", 1, 1, ms_LogFile);
  //to put <br /> into html use WriteLine("");
}

void Logger::WriteIgnoringHTML(const char* pattern, ...)
{
  if (!Verbose) return;
  va_list  argList;
  va_start(argList, pattern);

  vfprintf(ms_LogFile, pattern, argList);

  va_end(argList);
}

void Logger::__PrintAssert(const LogCortege& lcort, const char* text, bool forceExit)
{
  double secsElapsed = GetUptime();
  fprintf(ms_LogFile, "[%10.3f] ASSERT in %s (%s:%d)\n             %s\n",
    secsElapsed, lcort.func, lcort.file, lcort.line, text);
  if (ms_WriteHtmlLog)
  {
    WriteToHTMLStream("<div class=\"assert\">");
    WriteToHTMLStream(false, "<span class=\"timestamp\">%.3f</span>", secsElapsed);
    WriteToHTMLStream(true, "ASSERT(%s)", text);
    WriteToHTMLStream(false, "<dl class=\"location\">\n<dt>Function Name:</dt> <dd>%s</dd>\n", lcort.func);
    WriteToHTMLStream(false, "<dt>File Name:</dt> <dd>%s</dd>\n", lcort.file);
    WriteToHTMLStream(false, "<dt>Source Line:</dt> <dd>%d</dd>\n</dl>", lcort.line);
    WriteToHTMLStream("</div>\n");
  }

  if (ms_ExitOnAssert || forceExit)
  {
    fprintf(ms_LogFile, "Aborted...");
    if (ms_WriteHtmlLog)
      WriteToHTMLStream("<span class=\"aborted\">Aborted...</span>\n");

    exit(1);
  }
}

void Logger::vAlert(const char* pattern, va_list argList)
{
    if (!Verbose) return;

    double secsElapsed = GetUptime();

    fprintf(ms_LogFile, "[%10.3f] ", secsElapsed);
    vfprintf(ms_LogFile, pattern, argList);
    fwrite("\n", 1, 1, ms_LogFile);

    if (ms_WriteHtmlLog)
    {
        WriteToHTMLStream("<div class=\"alert\">");
        WriteToHTMLStream(false, "<span class=\"timestamp\">%.3f</span>", secsElapsed);
        ::WriteToHTMLStream(true, pattern, ms_LogHtmlFile, argList);
        WriteToHTMLStream("</div>\n");
    }
}

void Logger::Alert(const char* pattern, ...)
{
    va_list  argList;
    va_start(argList, pattern);
    vAlert(pattern, argList);
    va_end(argList);
}

void Logger::Alert(std::string pattern, ...)
{
    va_list argList;
    va_start(argList, pattern);
    vAlert(pattern.c_str(), argList);
    va_end(argList);
}

void Logger::vAlert(Color col, const char* pattern, va_list argList)
{
    if (!Verbose) return;

    double secsElapsed = GetUptime();

    SetConsoleTextColor(col);
    fprintf(ms_LogFile, "[%10.3f] ", secsElapsed);
    vfprintf(ms_LogFile, pattern, argList);
    fwrite("\n", 1, 1, ms_LogFile);
    RestoreConsoleColor();

    if (ms_WriteHtmlLog)
    {
        int r,g,b;
        ToRGB(col, r, g, b);
        WriteToHTMLStream(false, "<div class=\"alert\" style=\"color:#%02x%02x%02x;\">", r, g, b);
        WriteToHTMLStream(false, "<span class=\"timestamp\">%.3f</span>", secsElapsed);
        ::WriteToHTMLStream(true, pattern, ms_LogHtmlFile, argList);
        WriteToHTMLStream("</div>\n");
    }
}

void Logger::Alert(Color col, const char* pattern, ...)
{
  va_list  argList;
  va_start(argList, pattern);
  vAlert(col, pattern, argList);
  va_end(argList);
}

void Logger::Alert(Color col, std::string pattern, ...)
{
    va_list argList;
    va_start(argList, pattern);
    vAlert(col, pattern.c_str(), argList);
    va_end(argList);
}

static void WriteNotification(bool writeHtml,
                         const char* notificationName,
                         const char* notificationClassName,
                         const LogCortege& lcort,
                         FILE* logfile,
                         FILE* htmlLogFile,
                         Logger* logger,
                         double secsElapsed,
                         const char* pattern,
                         va_list argList)
{
  fprintf(logfile, "[%10.3f] %s PRINT in %s (%s:%d)\n             ", secsElapsed, notificationName, lcort.func, lcort.file, lcort.line);
  vfprintf(logfile, pattern, argList);
  fwrite("\n", 1, 1, logfile);

  if (writeHtml)
  {
    logger->WriteToHTMLStream(false, "<div class=\"%s\">", notificationClassName);
    logger->WriteToHTMLStream(false, "<span class=\"timestamp\">%.3f</span> %s ", secsElapsed, notificationName);
    ::WriteToHTMLStream(true, pattern, htmlLogFile, argList);
    logger->WriteToHTMLStream(false, "<dl class=\"location\">\n<dt>Function Name:</dt> <dd>%s</dd>\n", lcort.func);
    logger->WriteToHTMLStream(false, "<dt>File Name:</dt> <dd>%s</dd>\n", lcort.file);
    logger->WriteToHTMLStream(false, "<dt>Source Line:</dt> <dd>%d</dd>\n</dl>\n</div>\n", lcort.line);
  }
}

#ifdef LOGGER_REQUIRES_DEBUG_LEVEL_PRINT
void Logger::Debug(const LogCortege& lcort, const char* pattern, ...)
{
  if (DisplayLevel > DEBUGLEVEL) return;
  va_list  argList;
  va_start(argList, pattern);
  double secsElapsed = GetUptime();

  ::WriteNotification(ms_WriteHtmlLog, "DEBUG", "debug", lcort, ms_LogFile, ms_LogHtmlFile, this, secsElapsed, pattern, argList);

  va_end(argList);
}
#endif

#ifdef LOGGER_REQUIRES_INFO_LEVEL_PRINT
void Logger::Info(const LogCortege& lcort, const char* pattern, ...)
{
  if (DisplayLevel > INFOLEVEL) return;
  va_list  argList;
  va_start(argList, pattern);
  double secsElapsed = GetUptime();

  ::WriteNotification(ms_WriteHtmlLog, "INFO", "info", lcort, ms_LogFile, ms_LogHtmlFile, this, secsElapsed, pattern, argList);

  va_end(argList);
}
#endif

#ifdef LOGGER_REQUIRES_WARNING_LEVEL_PRINT
void Logger::Warning(const LogCortege& lcort, const char* pattern, ...)
{
  if (ms_WarningCounter < ms_MaxWarningsToDisplay && DisplayLevel <= WARNINGLEVEL)
  {
    va_list  argList;
    va_start(argList, pattern);
    double secsElapsed = GetUptime();

    ::WriteNotification(ms_WriteHtmlLog, "WARNING", "warning", lcort, ms_LogFile, ms_LogHtmlFile, this, secsElapsed, pattern, argList);

    va_end(argList);
  }
  ms_WarningCounter++;
}
#endif

#ifdef LOGGER_REQUIRES_ERROR_LEVEL_PRINT
void Logger::Error(const LogCortege& lcort, const char* pattern, ...)
{
  if (ms_ErrorCounter < ms_MaxErrorsToDisplay && DisplayLevel <= ERRORLEVEL)
  {
    va_list  argList;
    va_start(argList, pattern);
    double secsElapsed = GetUptime();

    ::WriteNotification(ms_WriteHtmlLog, "ERROR", "error", lcort, ms_LogFile, ms_LogHtmlFile, this, secsElapsed, pattern, argList);

    va_end(argList);
  }
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

void Logger::FatalError(const LogCortege& lcort, const char* pattern, ...)
{
  va_list  argList;
  va_start(argList, pattern);
  double secsElapsed = GetUptime();

  ::WriteNotification(ms_WriteHtmlLog, "FATAL ERROR", "fatal_error", lcort, ms_LogFile, ms_LogHtmlFile, this, secsElapsed, pattern, argList);

  va_end(argList);

  fprintf(ms_LogFile, "Fatal error occured.\nApplication has to be terminated.\n");
  if (ms_WriteHtmlLog)
    WriteToHTMLStream("<span class=\"fatal_error_msg\">Fatal error occured.<br />Application has to be terminated.</span><br />\n");
  exit(1);
}

namespace Revision
{
extern long        RevisionNumber;
extern const char* RevisionDate;
extern const char* RevisionRange;
extern const char* RepositoryURL;
extern bool        HasLocalModifications;
extern bool        HasMixedRevisions;
extern bool        HasVersionControl;
}

void Logger::PrintRevisionNumber()
{
  if (!Verbose) return;
  WriteToHTMLStream("<div class=\"revision\">");

  if (!Revision::HasVersionControl || Revision::RevisionNumber == 0)
  {//1. No version control
    fprintf(ms_LogFile, "Running UNVERSIONED itlPlace\n", Revision::RepositoryURL, Revision::RevisionRange);
    WriteToHTMLStream("<span class=\"norev\">Running UNVERSIONED itlPlace</span>");
  }
  else if (Revision::HasMixedRevisions)
  {//2. Mixed revision range
    fprintf(ms_LogFile, "Running mixed revisions %s of itlPlace [%s]\n", Revision::RevisionRange, Revision::RepositoryURL);
    WriteToHTMLStream(false, "Running <span class=\"revMixed\">mixed revisions %s</span> of <a href=\"%s\">itlPlace</a>",
      Revision::RevisionRange, Revision::RepositoryURL);
  }
  else if (Revision::HasLocalModifications)
  {//3. With local modifications
    fprintf(ms_LogFile, "Running locally modified itlPlace [%s] Revision %d (%s)\n", Revision::RepositoryURL, Revision::RevisionNumber, Revision::RevisionDate);
    WriteToHTMLStream(false, "Running <span class=\"revLocalMod\">locally modified</span> <a href=\"%s\">itlPlace</a> <a href=\"http://redmine.software.unn.ru/projects/vlsi/repository/revisions/%d\">Revision %d (%s)</a>",
       Revision::RepositoryURL, Revision::RevisionNumber, Revision::RevisionNumber, Revision::RevisionDate);
  }
  else
  {//4. Clear
    fprintf(ms_LogFile, "Running itlPlace [%s] Revision %d (%s)\n", Revision::RepositoryURL, Revision::RevisionNumber, Revision::RevisionDate);
    WriteToHTMLStream(false, "Running <a href=\"%s\">itlPlace</a> <a href=\"http://redmine.software.unn.ru/projects/vlsi/repository/revisions/%d\">Revision %d (%s)</a>",
       Revision::RepositoryURL, Revision::RevisionNumber, Revision::RevisionNumber, Revision::RevisionDate);
  }

  WriteToHTMLStream("</div>\n");
}

void Logger::RenameDuplicatedFiles(const string& newName)
{
  if (ms_WriteHtmlLog)
  {
    string htmName = Aux::ChangeExtention(newName, ".htm");
    fclose(ms_LogHtmlFile);
    ms_LogHtmlFile = 0;
    if (0 == rename(ms_LogHtmlFileName.c_str(), htmName.c_str()))
      ms_LogHtmlFileName = htmName;
    ms_LogHtmlFile = fopen(ms_LogHtmlFileName.c_str(), "a");
    if (ms_LogHtmlFile == 0)
      ms_WriteHtmlLog = false;
  }
  if (ms_DuplicateOutput)
  {
    string logName = Aux::ChangeExtention(newName, ".log");
    if (0 == rename(ms_LogDuplicateFileName.c_str(), logName.c_str()))
      ms_LogDuplicateFileName = logName;
  }
}

void Logger::WriteHTMLHeader()
{
  WriteToHTMLStream("<!DOCTYPE HTML>\n");
  WriteToHTMLStream("<html>\n<head>\n");
  WriteToHTMLStream("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=us-ascii\">\n");
  if (Revision::HasVersionControl)
    if(Revision::HasMixedRevisions)
      Global.WriteToHTMLStream(false, "<title>itlPlace Log - Revisions %s</title>\n", Revision::RevisionRange);
    else
      Global.WriteToHTMLStream(false, "<title>itlPlace Log - Revision %d</title>\n", Revision::RevisionNumber);
  else
    WriteToHTMLStream("<title>itlPlace Log - UNKNOWN Revision</title>\n");
  if (!ms_EmbeedCSS || !Aux::FileExists(ms_CSSFileName))
  {
    Global.WriteToHTMLStream(false, "<link rel=\"stylesheet\" type=\"text/css\" href=\"%s\">\n", ms_CSSFileName.c_str());
    Global.WriteToHTMLStream(false, "<link rel=\"stylesheet\" type=\"text/css\" href=\"../%s\">\n", ms_CSSFileName.c_str());
  }
  else
  {
    WriteToHTMLStream("<style type=\"text/css\">\n");
    FILE* cssFile = fopen(ms_CSSFileName.c_str(), "r");
    if (cssFile != 0)
    {
#define CSSCOPYBUFFERSIZE 4096
      char buffer[CSSCOPYBUFFERSIZE];
      while(true)
      {
        size_t bytesRead = fread(buffer, 1, CSSCOPYBUFFERSIZE, cssFile);
        fwrite(buffer, 1, bytesRead, ms_LogHtmlFile);
        if (bytesRead < CSSCOPYBUFFERSIZE)
          break;
      }
      fclose(cssFile);
    }
    WriteToHTMLStream("</style>\n");
  }
  if (true)//add js includes
  {
    WriteToHTMLStream(
" <!--[if IE]><script language=\"javascript\" type=\"text/javascript\" src=\"http://bugzilla.software.unn.ru/itlPlace/excanvas.min.js\"></script><![endif]-->\n"
" <script type=\"text/javascript\" src=\"http://bugzilla.software.unn.ru/itlPlace/jquery.js\"></script>\n"
" <script type=\"text/javascript\" src=\"http://bugzilla.software.unn.ru/itlPlace/jquery.flot.js\"></script>\n"
" <script type=\"text/javascript\" src=\"http://bugzilla.software.unn.ru/itlPlace/jquery.graphTable-0.2.js\"></script>\n"
" <script type=\"text/javascript\" src=\"http://bugzilla.software.unn.ru/itlPlace/plotQAgraphs.js\"></script>\n");
  }
  WriteToHTMLStream("</head>\n<body>\n");
}

void Logger::WriteHTMLFooter()
{
  WriteToHTMLStream("</body>\n</html>");
}