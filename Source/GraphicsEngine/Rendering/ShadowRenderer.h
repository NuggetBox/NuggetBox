#pragma once
#include "DX11.h"
#include "RenderStructs.h"
#include "Lights/Light.h"
#include "Model/Model.h"

class ShadowRenderer
{
public:
	void Initialize();
	void RenderShadowPassPerLight(const std::shared_ptr<Light>& aLight, const std::vector<std::shared_ptr<Model>>& aModelList);
	void RenderShadowPassPerModel(const std::shared_ptr<Model>& aModel, const std::vector<std::shared_ptr<Light>>& aLightList);

private:
	FrameBufferData myFrameBufferData;
	ObjectBufferData myObjectBufferData;

	ComPtr<ID3D11Buffer> myFrameBuffer;
	ComPtr<ID3D11Buffer> myObjectBuffer;
	ComPtr<ID3D11Buffer> myMaterialBuffer;
};