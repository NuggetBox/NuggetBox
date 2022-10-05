#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	static std::shared_ptr<PointLight> Create(Vector3f aColor, float anIntensity, Vector3f aPosition, float aRange);

	void SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer) override;

	void ResetAllViewMatrix();
	void SetViewMatrix(unsigned aDir, unsigned aToSlot = 0);

	void SetRange(float aRange) { myLightBufferData.Range = aRange; }
	float GetRange() const { return myLightBufferData.Range; }

	//TODO: Update function for changing light buffer position data for example to be equal to transform pos

	void ClearShadowMap(unsigned anIndex = 0) override;
	void SetShadowMapAsTarget(unsigned anIndex) override;
	void BindShadowMapAsResource(unsigned aSlot) override;
	void RemoveShadowMapResource(unsigned aSlot) override;

private:
	//Do we need extra depthstencils or can we just draw to the same one everytime?
	std::array<std::shared_ptr<DepthStencil>, 5> myExtraShadowMaps;
};