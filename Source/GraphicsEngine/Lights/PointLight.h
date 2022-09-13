#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	static std::shared_ptr<PointLight> Create(Vector3f aColor, float anIntensity, Vector3f aPosition, float aRange);

	void SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer) override;

	void SetRange(float aRange) { myLightBufferData.Range = aRange; }

	float GetRange() const { return myLightBufferData.Range; }
};