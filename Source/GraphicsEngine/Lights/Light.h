#pragma once
#include "Scene/SceneObject.h"
#include "Rendering/DX11.h"
#include "Rendering/RenderStructs.h"
#include "Rendering/DepthStencil.h"

enum class LightType
{
	DirectionalLight = 0,
	AmbientLight = 1,
	PointLight = 2,
	SpotLight = 3
};

class Light : public SceneObject
{
public:
	Light() = default;
	~Light() override = default;
	//virtual void Initialize(Vector3f aColor, float anIntensity);

	virtual void SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer) = 0;
	void SetLightViewport();
	void ClearShadowMap();
	void BindShadowMap();

	const LightBufferData& GetLightBuffer() const { return myLightBufferData; }

	Vector3f GetColor() const { return myLightBufferData.Color; }
	float GetIntensity() const { return myLightBufferData.Intensity; }
	Vector3f GetDirection() const { return myLightBufferData.Direction; }

protected:
	LightBufferData myLightBufferData;
	std::shared_ptr<DepthStencil> myShadowMap;
};