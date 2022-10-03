#pragma once
#include "ForwardRenderer.h"
#include "DeferredRenderer.h"
#include "PostProcessRenderer.h"
#include "Scene/Scene.h"
#include "RenderStructs.h"
#include "GBuffer.h"
#include "ShadowRenderer.h"
#include "Scene/Editor.h"
#include "Rendering/RenderTarget.h"

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
	Off,
	COUNT
};

enum class SamplerState
{
	Default,
	PointClamp,
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
	void SetSamplerState(SamplerState aSamplerState, unsigned aSlot);

	void SetRenderMode(RenderMode aRenderMode);
	RenderMode GetRenderMode() const;

private:
	void SetupBlendStates();
	void SetupDepthStencilStates();
	void SetupSamplerStates();

	void ResetStates();
	void ResetViewport();

	void CameraControls(std::shared_ptr<Camera> aCamera);
	float myCameraSpeed = 100.0f;

	std::string RenderModeToString(RenderMode aRenderMode);

	std::array<ComPtr<ID3D11BlendState>, static_cast<unsigned int>(BlendState::COUNT)> myBlendStates;
	std::array<ComPtr<ID3D11DepthStencilState>, static_cast<unsigned int>(DepthStencilState::COUNT)> myDepthStencilStates;
	std::array<ComPtr<ID3D11SamplerState>, static_cast<unsigned int>(SamplerState::COUNT)> mySamplerStates;

	SIZE myWindowSize{0, 0};
	HWND myWindowHandle{};

	Editor myEditor;

	Scene myScene;

	RenderMode myRenderMode = RenderMode::Default;

	ForwardRenderer myForwardRenderer;
	DeferredRenderer myDeferredRenderer;
	std::unique_ptr<GBuffer> myGBuffer;
	ShadowRenderer myShadowRenderer;
	PostProcessRenderer myPostProcessRenderer;

	std::shared_ptr<RenderTarget> myIntermediateTargetA;
	std::shared_ptr<RenderTarget> myIntermediateTargetB;
	std::shared_ptr<RenderTarget> myHalfSizeTarget;
	std::shared_ptr<RenderTarget> myQuarterSizeTarget;
	std::shared_ptr<RenderTarget> myBlurTargetA;
	std::shared_ptr<RenderTarget> myBlurTargetB;

	Vector4<float> myClearColor = { 0.6f, 0.2f, 0.4f, 1.0f };

	bool myLerpAnimations;

	bool myDragAccept;
	void AcceptFiles(HWND aHwnd);

	void HandleDroppedFile(const std::filesystem::path& aPath);

	// Container window message pump.
	static LRESULT CALLBACK WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
};