#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
	static std::shared_ptr<DirectionalLight> Create(Vector3f aColor, float anIntensity, Vector3f aRotation);

	void SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer) override;

	//Update every frame, following the camera to create a new view matrix for rendering shadows with the best resolution possible
	void Update(const Vector3f& aCameraPosition);
};