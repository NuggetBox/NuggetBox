#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
	static std::shared_ptr<DirectionalLight> Create(Vector3f aColor, float anIntensity, Vector3f aDirection);

	void SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer) override;
};