#pragma once
#include <wrl.h>
#include <vector>
#include <memory>
#include <d3d11.h>

#include "Lights/AmbientLight.h"
#include "Camera.h"
#include "Lights/DirectionalLight.h"
#include "Model/Model.h"
#include "ParticleSystem/ParticleSystem.h"
#include "RenderStructs.h"

using namespace Microsoft::WRL;

struct ID3D11Buffer;

enum class RenderMode;

class ForwardRenderer
{
public:
	ForwardRenderer() = default;

	void Initialize();

	void RenderModels(const std::shared_ptr<Camera>& aCamera, const std::vector<std::shared_ptr<Model>>& aModelList, const std::shared_ptr<
					  DirectionalLight> aDirectionalLight, const std::shared_ptr<AmbientLight> anAmbientLight, const std::vector<std::
					  shared_ptr<Light>>& someLights, RenderMode aRenderMode);
	void RenderParticles(const std::shared_ptr<Camera>& aCamera, 
						 const std::vector<std::shared_ptr<ParticleSystem>>& aParticleSystemList, 
						 RenderMode aRenderMode);

	void RenderParticles(const std::shared_ptr<Camera>& aCamera, ParticleEmitter aEmitter);

private:
	//ComPtr<ID3D11RasterizerState> myRasterizerSate;
	struct SceneLightBuffer
	{
		LightBufferData DirectionalLight;
		LightBufferData Lights[MAX_FORWARD_LIGHTS];

		unsigned NumLights;
		Vector3f Padding;
	} mySceneLightBufferData;

	FrameBufferData myFrameBufferData;
	ObjectBufferData myObjectBufferData{};
	MaterialData myMaterialBufferData;
	//LightBufferData myLightBufferData;

	ComPtr<ID3D11Buffer> myFrameBuffer;
	ComPtr<ID3D11Buffer> myObjectBuffer;
	ComPtr<ID3D11Buffer> myMaterialBuffer;
	ComPtr<ID3D11Buffer> myLightBuffer;
};