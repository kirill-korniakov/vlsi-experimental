#ifdef WIN32

#pragma warning(disable:4311 4312 4018)

#include <windows.h>
#include "psapi.h"
#include <tlhelp32.h>
#include <list>

#include "Auxiliary.h"


static DWORD adr_WriteFile;
static HANDLE logFile;

BOOL
WINAPI
_WriteFile(
           __in        HANDLE hFile,
           __in_bcount(nNumberOfBytesToWrite) LPCVOID lpBuffer,
           __in        DWORD nNumberOfBytesToWrite,
           __out_opt   LPDWORD lpNumberOfBytesWritten,
           __inout_opt LPOVERLAPPED lpOverlapped
           )
{
  if(hFile == GetStdHandle(STD_OUTPUT_HANDLE) || hFile == GetStdHandle(STD_ERROR_HANDLE))
  {
    BOOL result = ((BOOL (__stdcall*)(HANDLE, LPCVOID, DWORD, LPDWORD, LPOVERLAPPED ))adr_WriteFile)
      (hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);

    DWORD written;
    ((BOOL (__stdcall*)(HANDLE, LPCVOID, DWORD, LPDWORD, LPOVERLAPPED ))adr_WriteFile)
      (logFile, lpBuffer, nNumberOfBytesToWrite, &written, lpOverlapped);

    return result;
  }
  return ((BOOL (__stdcall*)(HANDLE, LPCVOID, DWORD, LPDWORD, LPOVERLAPPED ))adr_WriteFile)
    (hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
}

// Смещение + База = Виртуальный Адрес
#define RVATOVA( base, offset )(((DWORD)(base) + (DWORD)(offset))) 

static int OverrideFunction(DWORD oldFunc, DWORD newFunc)
{
  HANDLE hProcess = GetCurrentProcess();
  HMODULE hMods[1024];
  DWORD cbNeeded;

  if( EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
  {
    for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++ )
    {
      BYTE *pimage = (BYTE*)hMods[i]; 
      // Получаем указатели на стандартные структуры данных PE заголовка
      IMAGE_DOS_HEADER *idh
        = (IMAGE_DOS_HEADER*)pimage;
      IMAGE_OPTIONAL_HEADER *ioh
        = (IMAGE_OPTIONAL_HEADER*)(pimage + idh->e_lfanew + 4 + sizeof(IMAGE_FILE_HEADER));
      IMAGE_SECTION_HEADER *ish
        = (IMAGE_SECTION_HEADER*)((BYTE*)ioh + sizeof(IMAGE_OPTIONAL_HEADER));

      if (idh->e_magic != IMAGE_DOS_SIGNATURE) 
        continue;

      if(ioh->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size == 0)
      {
        //printf("Empty import section\n");
        continue;
      }
      // Получаем адрес секции .idata(первого элемента IMAGE_IMPORT_DESCRIPTOR)
      IMAGE_IMPORT_DESCRIPTOR *iid
        = (IMAGE_IMPORT_DESCRIPTOR*)(pimage + ioh->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress );

      while(iid->Characteristics)
      {
        //HMODULE hDll = GetModuleHandleA((PCHAR)pimage + iid->Name);
        for( PIMAGE_THUNK_DATA Thunk = (PIMAGE_THUNK_DATA)RVATOVA(iid->FirstThunk, pimage);
          Thunk->u1.Ordinal ; Thunk++ )
        {
          if(Thunk->u1.Function == oldFunc)
          {
            DWORD op;
            VirtualProtect((void*)(Thunk),4,PAGE_READWRITE, &op);
            SIZE_T written;
            WriteProcessMemory(GetCurrentProcess(), (void*)(Thunk),(void*)&newFunc,4,&written);
            VirtualProtect((void*)(Thunk),4,op, &op);
            if(written!=4)
              return 1;
          }
        }
        iid++;
      }
    }
  }
  return 0;
}

static int DuplicateOutput()
{
  return OverrideFunction(adr_WriteFile, (DWORD)_WriteFile);
}

static void CheckParentProcess()
{
  DWORD process = GetCurrentProcessId();
  HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  PROCESSENTRY32 pe = { 0 };
  PROCESSENTRY32 self = { 0 };
  pe.dwSize = sizeof(PROCESSENTRY32);
  std::list<PROCESSENTRY32> vss;

  if( Process32First(h, &pe))
  {
    do 
    {
      vss.push_back(pe);
      if(pe.th32ProcessID == process)
        self = pe;
    } while( Process32Next(h, &pe));

    pe = self;
    if(pe.th32ProcessID != 0)
    {
      while(pe.th32ParentProcessID != 0)
      {
        for(std::list<PROCESSENTRY32>::iterator v = vss.begin(); v != vss.end(); ++v)
        {
          if(v->th32ProcessID == pe.th32ParentProcessID)
          {
            pe = *v;
#ifdef UNICODE
            if(wcscmp(pe.szExeFile, L"devenv.exe") == 0)
#else
            if(strcmp(pe.szExeFile, "devenv.exe") == 0)
#endif
            {
              char path[MAX_PATH];
              GetCurrentDirectoryA(MAX_PATH, path);
              if(strcmp("\\make", path + strlen(path) - 5) == 0)
                SetCurrentDirectoryA("..\\bin");
              goto CPP_FIN;
            }
            break;
          }
        }
        if(pe.th32ProcessID == self.th32ProcessID)
          break;
        else
          self = pe;
      }
    }
  }

CPP_FIN:
  CloseHandle(h);
}

static string gLogName;
static int AppInit()
{
  CheckParentProcess();
  
  ::gLogName = Aux::CreateCoolFileName("log\\", "itlOutput", "log");

  logFile = CreateFileA(::gLogName.c_str(),
    GENERIC_WRITE,
    FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE,
    NULL,
    OPEN_ALWAYS,
    FILE_ATTRIBUTE_NORMAL,
    NULL);
  SetFilePointer(logFile, 0, 0, FILE_END);
  //DWORD written;
  //WriteFile(logFile, "***Output dumping started***\n\n\n", 31, &written, NULL);
  adr_WriteFile = (DWORD) WriteFile;
  DuplicateOutput();
  return 0;
}

void Aux::SetOutputFileName(const string& fname)
{
  if (MoveFileA(::gLogName.c_str(), fname.c_str()))
    ::gLogName = fname;
}

static int ___app_init___ = AppInit();

#endif