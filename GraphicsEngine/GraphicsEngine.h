#pragma once
#include "ForwardRenderer.h"
#include "Scene.h"

class GraphicsEngine
{
	SIZE myWindowSize{0, 0};
	HWND myWindowHandle{};

	//DX11 myFramework;
	ForwardRenderer myForwardRenderer;

	Scene myScene;

	// Container window message pump.
	static LRESULT CALLBACK WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

public:
	bool Initialize(unsigned someX, unsigned someY, unsigned someWidth, unsigned someHeight, bool enableDeviceDebug);

	void BeginFrame();
	void EndFrame();
	void RenderFrame();

	[[nodiscard]] SIZE FORCEINLINE GetWindowSize() const { return myWindowSize; }
	[[nodiscard]] HWND FORCEINLINE GetWindowHandle() const { return myWindowHandle; }
};