#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
	static std::shared_ptr<DirectionalLight> Create(Utility::Vector3f aColor, float anIntensity, Utility::Vector3f aRotation);

	void SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer) override;

	//Update every frame, following the camera to create a new view matrix for rendering shadows with the best resolution possible
	void Update(const Utility::Vector3f& aCameraPosition);
};