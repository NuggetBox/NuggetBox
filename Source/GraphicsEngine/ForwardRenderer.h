#pragma once
#include <wrl.h>
#include <vector>
#include <memory>
#include <d3d11.h>

#include "AmbientLight.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "Model.h"
#include "Light.h"
#include "ParticleSystem.h"
#include "RenderStructs.h"

using namespace Microsoft::WRL;
using namespace Utility;

struct ID3D11Buffer;

enum class RenderMode;

class ForwardRenderer
{
public:
	ForwardRenderer() = default;

	void Initialize();

	void RenderModels(const std::shared_ptr<Camera>& aCamera, const std::vector<std::shared_ptr<Model>>& aModelList, const std::shared_ptr<
				DirectionalLight> aDirectionalLight, const std::shared_ptr<AmbientLight> anAmbientLight, RenderMode aRenderMode);
	void RenderParticles(const std::shared_ptr<Camera>& aCamera, 
						 const std::vector<std::shared_ptr<ParticleSystem>>& aParticleSystemList, 
						 RenderMode aRenderMode);

private:
	//ComPtr<ID3D11RasterizerState> myRasterizerSate;

	FrameBufferData myFrameBufferData;
	ObjectBufferData myObjectBufferData{};
	MaterialData myMaterialBufferData;
	//LightBufferData myLightBufferData;

	ComPtr<ID3D11Buffer> myFrameBuffer;
	ComPtr<ID3D11Buffer> myObjectBuffer;
	ComPtr<ID3D11Buffer> myMaterialBuffer;
	ComPtr<ID3D11Buffer> myLightBuffer;
};