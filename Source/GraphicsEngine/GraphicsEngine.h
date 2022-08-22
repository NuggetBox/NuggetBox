#pragma once
#include "ForwardRenderer.h"
#include "Scene.h"

enum class RenderMode
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
	Linear,
	COUNT //Update RenderModeToString when adding new RenderModes
};

enum class BlendState
{
	None,
	AlphaBlend,
	Additive,
	COUNT
};

enum class DepthStencilState
{
	ReadWrite,
	ReadOnly,
	COUNT
};

class GraphicsEngine
{
public:
	GraphicsEngine() = default;
	bool Initialize(unsigned someX, unsigned someY, unsigned someWidth, unsigned someHeight, bool enableDeviceDebug);

	void InitializeWindow(unsigned someX, unsigned someY, unsigned someWidth, unsigned someHeight);
	static void ResizeWindow(HWND aHWND, UINT someWidth, UINT someHeight);

	void InputRenderMode();

	void BeginFrame();
	void EndFrame();
	void RenderFrame();

	[[nodiscard]] SIZE FORCEINLINE GetWindowSize() const { return myWindowSize; }
	[[nodiscard]] HWND FORCEINLINE GetWindowHandle() const { return myWindowHandle; }

	void SetBlendState(BlendState aBlendState);
	void SetDepthStencilState(DepthStencilState aDepthStencilState);

	void SetRenderMode(RenderMode aRenderMode);
	RenderMode GetRenderMode() const;

private:
	void SetupBlendStates();
	void SetupDepthStencilStates();

	std::string RenderModeToString(RenderMode aRenderMode);

	std::array<ComPtr<ID3D11BlendState>, static_cast<unsigned int>(BlendState::COUNT)> myBlendStates;
	std::array<ComPtr<ID3D11DepthStencilState>, static_cast<unsigned int>(DepthStencilState::COUNT)> myDepthStencilStates;

	SIZE myWindowSize{0, 0};
	HWND myWindowHandle{};

	ForwardRenderer myForwardRenderer;
	RenderMode myRenderMode = RenderMode::Default;

	Scene myScene;

	// Container window message pump.
	static LRESULT CALLBACK WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
};