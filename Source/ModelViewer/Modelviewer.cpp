// Exclude things we don't need from the Windows headers
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"

#include "Modelviewer.h"
#include "Rendering/GraphicsEngine.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

#include <Windows.h>
#include <dbghelp.h>
#include <shellapi.h>
#include <shlobj.h>
#include <strsafe.h>
#include <minidumpapiset.h>

#include <filesystem>

#pragma comment(lib, "DbgHelp.lib")

/*
	TODO: Read https://learn.microsoft.com/en-us/windows/win32/dxtecharts/crash-dump-analysis

	If program crashes outside of Visual Studio create popup with the following text:
	”Program crashed! A minidump was created at [PATH]/MiniDump.dmp, please tell a programmer.”

	Include dbghelp.h and add corresponding .lib file
*/

void CreateMiniDump(EXCEPTION_POINTERS* someExceptionPointers);
LONG WINAPI ExceptionFilterFunction(_EXCEPTION_POINTERS* aExceptionP);
void Run();
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void CreateMiniDump(EXCEPTION_POINTERS* someExceptionPointers)
{
	// Do your best!
	BOOL bMiniDumpSuccessful;

	auto curPath = std::filesystem::current_path().wstring() + L"\\MiniDumps\\";
	auto szPath = curPath.c_str();

	//WCHAR szPath[MAX_PATH] = yo;
	WCHAR szFileName[MAX_PATH];
	const wchar_t* szVersion = L"v13.65";
	DWORD dwBufferSize = MAX_PATH;
	HANDLE hDumpFile;
	SYSTEMTIME stLocalTime;
	MINIDUMP_EXCEPTION_INFORMATION ExpParam;

	GetLocalTime(&stLocalTime);
	//GetTempPath(dwBufferSize, szPath);

	StringCchPrintf(szFileName, MAX_PATH, L"%s", szPath);
	CreateDirectory(szFileName, NULL);

	StringCchPrintf(szFileName, MAX_PATH, L"%s%s-%04d%02d%02d-%02d%02d%02d-%ld-%ld.dmp",
		szPath, szVersion,
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

	std::wstring wstring(szFileName);
	auto message = L"Program crashed! A minidump was created at " + wstring + L", please tell a programmer.";
	auto cstr = std::filesystem::path(message).string().c_str();
	MessageBoxA(NULL, cstr, "Error!", MB_ICONEXCLAMATION | MB_OK);
}

LONG WINAPI ExceptionFilterFunction(_EXCEPTION_POINTERS* aExceptionP)
{
	CreateMiniDump(aExceptionP);

	return EXCEPTION_EXECUTE_HANDLER;
}

void Run()
{
	MSG msg = { nullptr };

	GraphicsEngine graphicsEngine;

	const SIZE windowSize = { 1600, 900 };

	bool bShouldRun = graphicsEngine.Initialize(
		(GetSystemMetrics(SM_CXSCREEN) - windowSize.cx) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - windowSize.cy) / 2,
		windowSize.cx,
		windowSize.cy,
		true);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(graphicsEngine.GetWindowHandle());
	ImGui_ImplDX11_Init(DX11::Device.Get(), DX11::Context.Get());
	ImGui::StyleColorsClassic();
	//io.Fonts->AddFontFromFileTTF("imgui/misc/Roboto-Medium.ttf", 16);

	while (bShouldRun)
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			ImGui_ImplWin32_WndProcHandler(graphicsEngine.GetWindowHandle(), msg.message, msg.wParam, msg.lParam);

			TranslateMessage(&msg);
			DispatchMessage(&msg);

			// Should update input here.

			if (msg.message == WM_QUIT)
			{
				bShouldRun = false;
			}
		}

		// REMEMBER!
		// The frame update for the game does NOT happen inside the PeekMessage loop.
		// This would cause the game to only update if there are messages and also run
		// the update several times per frame (once for each message).

		graphicsEngine.BeginFrame();

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		graphicsEngine.RenderFrame();

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		graphicsEngine.EndFrame();
	}
}

auto APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)->int
{
	/*UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);*/

	//Open console window in debug
#ifdef _DEBUG
	AllocConsole();
	FILE* newstdin = nullptr;
	FILE* newstdout = nullptr;
	FILE* newstderr = nullptr;

	freopen_s(&newstdin, "CONIN$", "r", stdin);
	freopen_s(&newstdout, "CONOUT$", "w", stdout);
	freopen_s(&newstderr, "CONOUT$", "w", stderr);
#endif

	__try
	{
		Run();
	}
	__except (ExceptionFilterFunction(GetExceptionInformation()))
	{
	}

	return 0;
}