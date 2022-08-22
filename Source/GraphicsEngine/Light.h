#pragma once
#include "SceneObject.h"
#include "DX11.h"

struct LightBufferData
{
	Vector3f Color;
	float Intensity;
	Vector3f Direction;
	float padding;
};

class Light : SceneObject
{
public:
	~Light() override = default;
	virtual void Initialize(Vector3f aColor, float anIntensity);

	virtual void SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer) = 0;

	Vector3f GetColor() const { return myLightBufferData.Color; }
	float GetIntensity() const { return myLightBufferData.Intensity; }
	Vector3f GetDirection() const { return myLightBufferData.Direction; }

protected:
	LightBufferData myLightBufferData;
};