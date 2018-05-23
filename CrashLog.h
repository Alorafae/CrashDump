
/*

Crash dump handler for post morteum crash debugging

Alora Fae

*/

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <DbgHelp.h>

LONG WINAPI CrashDump(EXCEPTION_POINTERS* exPointer)
{
  MINIDUMP_EXCEPTION_INFORMATION miniInfo;

  SYSTEMTIME sysTime;
  GetSystemTime(&sysTime);

  wchar_t* tStamp = new wchar_t[64];
  swprintf_s(tStamp, sizeof(wchar_t)*64, L"MiniDump_%02d-%02d-%02d_%02d-%02d.dmp", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute);

  miniInfo.ThreadId = GetCurrentThreadId();
  miniInfo.ExceptionPointers = exPointer;
  miniInfo.ClientPointers = false;

  MiniDumpWriteDump(GetCurrentProcess(),
    GetCurrentProcessId(),
    CreateFileW(LPCWSTR(tStamp), GENERIC_READ | GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL),
    MiniDumpWithDataSegs,
    &miniInfo,
    NULL, NULL);

  return EXCEPTION_CONTINUE_SEARCH;
}

LONG WINAPI LogCrash(EXCEPTION_POINTERS* exPointer)
{
  DWORD EIP = exPointer->ContextRecord->Eip;
  //exPointer->ContextRecord->Rip; // 64 bit
  CrashDump(exPointer);

  return EXCEPTION_CONTINUE_SEARCH;
}

#define CRASH_DUMP SetUnhandledExceptionFilter(LogCrash); \
  PULONG pulong = NULL; \
    ULONG ulong = 16384; \
      pulong = &ulong; \
        SetThreadStackGuarantee(pulong);
