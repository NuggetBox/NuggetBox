// Exclude things we don't need from the Windows headers
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"

#include "Modelviewer.h"
#include "Rendering/GraphicsEngine.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

auto APIENTRY wWinMain(_In_ HINSTANCE hInstance,
					   _In_opt_ HINSTANCE hPrevInstance,
					   _In_ LPWSTR lpCmdLine,
					   _In_ int nCmdShow)->int
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

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

    MSG msg = {nullptr};

    GraphicsEngine graphicsEngine;

    const SIZE windowSize = {1600, 900};

    bool bShouldRun = graphicsEngine.Initialize(
		(GetSystemMetrics(SM_CXSCREEN) - windowSize.cx) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - windowSize.cy) / 2,
		windowSize.cx,
		windowSize.cy,
		true);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
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

	return 0;
}