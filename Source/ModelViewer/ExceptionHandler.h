#include <Windows.h>
#include <dbghelp.h>
#include <shellapi.h>
#include <shlobj.h>
#include <strsafe.h>

/*
	TODO: Read https://learn.microsoft.com/en-us/windows/win32/dxtecharts/crash-dump-analysis

	If program crashes outside of Visual Studio create popup with the following text:
	”Program crashed! A minidump was created at [PATH]/MiniDump.dmp, please tell a programmer.”

	Include dbghelp.h and add corresponding .lib file
*/

void RunGame();
void CreateMiniDump(EXCEPTION_POINTERS* someExceptionPointers);
LONG WINAPI ExceptionFilterFunction(_EXCEPTION_POINTERS* aExceptionP);

// TODO: Try and catch should be moved to our actual program entry point. Then remove this code.
int main(int argc, char* argv[])
{
	__try
	{
		RunGame(); 
	}
	__except (ExceptionFilterFunction(GetExceptionInformation()))
	{

	}
}

void CreateMiniDump(EXCEPTION_POINTERS* someExceptionPointers)
{
	// Do your best!
    BOOL bMiniDumpSuccessful;
    WCHAR szPath[MAX_PATH];
    WCHAR szFileName[MAX_PATH];
	const wchar_t* szAppName = L"AppName";
	const wchar_t* szVersion = L"v1.0";
    DWORD dwBufferSize = MAX_PATH;
    HANDLE hDumpFile;
    SYSTEMTIME stLocalTime;
    MINIDUMP_EXCEPTION_INFORMATION ExpParam;

    GetLocalTime(&stLocalTime);
    GetTempPath(dwBufferSize, szPath);

    StringCchPrintf(szFileName, MAX_PATH, L"%s%s", szPath, szAppName);
    CreateDirectory(szFileName, NULL);

    StringCchPrintf(szFileName, MAX_PATH, L"%s%s\\%s-%04d%02d%02d-%02d%02d%02d-%ld-%ld.dmp",
        szPath, szAppName, szVersion,
        stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay,
        stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond,
        GetCurrentProcessId(), GetCurrentThreadId());
    hDumpFile = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

    ExpParam.ThreadId = GetCurrentThreadId();
    ExpParam.ExceptionPointers = someExceptionPointers;
    ExpParam.ClientPointers = TRUE;

    bMiniDumpSuccessful = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
        hDumpFile, MiniDumpWithDataSegs, &ExpParam, NULL, NULL);
}

LONG WINAPI ExceptionFilterFunction(_EXCEPTION_POINTERS* aExceptionP)
{
	CreateMiniDump(aExceptionP);

	return EXCEPTION_EXECUTE_HANDLER;
}