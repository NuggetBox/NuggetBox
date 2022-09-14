#pragma once
#include "Light.h"

class SpotLight : public Light
{
public:
	static std::shared_ptr<SpotLight> Create(Vector3f aColor, float anIntensity, Vector3f aPosition, float aRange, Vector3f aDirection, float anInnerRadius, float anOuterRadius);

	void SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer) override;

	void SetRange(float aRange) { myLightBufferData.Range = aRange; };
	void SetInnerRadius(float aRadius) { myLightBufferData.SpotInnerRadius = aRadius; };
	void SetOuterRadius(float aRadius) { myLightBufferData.SpotOuterRadius = aRadius; };

	float GetRange() const { return myLightBufferData.Range; }
	float GetInnerRadius() const { return myLightBufferData.SpotInnerRadius; }
	float GetOuterRadius() const { return myLightBufferData.SpotOuterRadius; }
};