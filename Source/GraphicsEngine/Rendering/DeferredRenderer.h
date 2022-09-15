#pragma once
#include "Camera.h"
#include "Model/Model.h"
#include "RenderStructs.h"
#include "Lights/DirectionalLight.h"
#include "Lights/AmbientLight.h"

class DeferredRenderer
{
public:
	DeferredRenderer() = default;
	void Initialize();
	void GenerateGBuffer(const std::shared_ptr<Camera>& aCamera, const std::vector<std::shared_ptr<Model>>& aModelList);
	void Render(const std::shared_ptr<Camera>& aCamera, const std::shared_ptr<DirectionalLight>& aDirectionalLight,
				const std::shared_ptr<AmbientLight>& anAmbientLight, const std::vector<std::shared_ptr<Light>>& someLights, RenderMode aRenderMode);

private:
	struct SceneLightBuffer
	{
		LightBufferData DirectionalLight;
		LightBufferData Lights[MAX_DEFERRED_LIGHTS];

		unsigned NumLights;
		Vector3f Padding;
	} mySceneLightBufferData;

	std::shared_ptr<PixelShader> myPixelShader;
	std::shared_ptr<VertexShader> myFullscreenShader;
	std::shared_ptr<PixelShader> myEnvironmentShader;

	FrameBufferData myFrameBufferData;
	ObjectBufferData myObjectBufferData{};
	//MaterialData myMaterialBufferData;
	//LightBufferData myLightBufferData;

	ComPtr<ID3D11Buffer> myFrameBuffer;
	ComPtr<ID3D11Buffer> myObjectBuffer;
	ComPtr<ID3D11Buffer> myMaterialBuffer;
	ComPtr<ID3D11Buffer> myLightBuffer;
};