#pragma once
#include <wrl.h>
#include <vector>
#include <memory>
#include <d3d11.h>

#include "Math/Matrix4x4.hpp"
#include "Camera.h"
#include "Model.h"

using namespace Microsoft::WRL;
using namespace Utility;

struct ID3D11Buffer;

struct FrameBufferData
{
	Matrix4x4<float> View;
	Matrix4x4<float> Projection;
};

struct ObjectBufferData
{
	Matrix4x4<float> World;
	Matrix4x4<float> BoneData[MAX_BONES];
	alignas(16) bool HasBones = false;
	//bool HasBones = false;
	//Vector3f padding;
};

//struct MaterialBufferData
//{
//	Vector3f Albedo;
//	float padding;
//};

class ForwardRenderer
{
public:
	void Initialize();
	void Render(const std::shared_ptr<Camera>& aCamera, const std::vector<std::shared_ptr<Model>>& aModelList);

private:
	//ComPtr<ID3D11RasterizerState> myRasterizerSate;

	FrameBufferData myFrameBufferData;
	ObjectBufferData myObjectBufferData{};
	//MaterialData myMaterialBufferData;

	ComPtr<ID3D11Buffer> myFrameBuffer;
	ComPtr<ID3D11Buffer> myObjectBuffer;
	//ComPtr<ID3D11Buffer> myMaterialBuffer;
};