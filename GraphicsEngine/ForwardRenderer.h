#pragma once
#include <wrl.h>
#include <vector>
#include <memory>
#include <d3d11.h>

#include "AmbientLight.h"
#include "Math/Matrix4x4.hpp"
#include "Camera.h"
#include "DirectionalLight.h"
#include "Model.h"
#include "Light.h"
#include "ParticleSystem.h"

using namespace Microsoft::WRL;
using namespace Utility;

struct ID3D11Buffer;

enum class RenderMode;

struct FrameBufferData
{
	Matrix4x4<float> View;
	Matrix4x4<float> Projection;
	Vector3f CamTranslation;
	UINT RenderMode;
};

struct ObjectBufferData
{
	Matrix4x4<float> World;
	Matrix4x4<float> BoneData[MAX_BONES];
	alignas(16) bool HasBones = false;
	//bool HasBones = false;
	//Vector3f padding;
};

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
	LightBufferData myLightBufferData;

	ComPtr<ID3D11Buffer> myFrameBuffer;
	ComPtr<ID3D11Buffer> myObjectBuffer;
	ComPtr<ID3D11Buffer> myMaterialBuffer;
	ComPtr<ID3D11Buffer> myLightBuffer;
};