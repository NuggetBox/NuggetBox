#pragma once
#include "ForwardRenderer.h"
#include "Scene.h"

enum class RenderMode //Update RenderModeToString when adding new rendermodes
{
	Default,
	UV,
	VertexColor,
	VertexNormal,
	PixelNormal,
	AlbedoMap,
	NormalMap,
	DiffuseLight,
	DiffuseLightNoAlbedo,
	AmbientLight,
	AmbientLightNoAlbedo,
	AmbientOcclusion,
	Roughness,
	Metalness,
	Emissiveness,
	COUNT
};

class GraphicsEngine
{
public:
	GraphicsEngine() = default;
	bool Initialize(unsigned someX, unsigned someY, unsigned someWidth, unsigned someHeight, bool enableDeviceDebug);

	void InitializeWindow(unsigned someX, unsigned someY, unsigned someWidth, unsigned someHeight);

	void InputRenderMode();

	void BeginFrame();
	void EndFrame();
	void RenderFrame();

	[[nodiscard]] SIZE FORCEINLINE GetWindowSize() const { return myWindowSize; }
	[[nodiscard]] HWND FORCEINLINE GetWindowHandle() const { return myWindowHandle; }

	void SetRenderMode(RenderMode aRenderMode);
	RenderMode GetRenderMode() const;

private:
	std::string RenderModeToString(RenderMode aRenderMode);

	SIZE myWindowSize{0, 0};
	HWND myWindowHandle{};

	ForwardRenderer myForwardRenderer;
	RenderMode myRenderMode = RenderMode::Default;

	Scene myScene;

	// Container window message pump.
	static LRESULT CALLBACK WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
};