#include "NuggetBox.pch.h"
#include "PointLight.h"

std::shared_ptr<PointLight> PointLight::Create(Utility::Vector3f aColor, float anIntensity, Utility::Vector3f aPosition, float aRange)
{
	PointLight pointLight;
	ZeroMemory(&pointLight.myLightBufferData, sizeof(LightBufferData));

	pointLight.myLightBufferData.Color = aColor;
	pointLight.myLightBufferData.Intensity = anIntensity;
	pointLight.myLightBufferData.Position = aPosition;
	pointLight.myLightBufferData.Range = aRange;
	pointLight.myLightBufferData.LightType = static_cast<unsigned>(LightType::PointLight);

	pointLight.SetPosition(aPosition);

	//TODO: Tweak values to make for good shadow rendering, pointlight
	constexpr float nearPlane = 0.1f;
	constexpr float farPlane = 30000.0f;
	constexpr POINT shadowResolution = { 2048, 2048 };
	constexpr POINT projectionSize = { 2048, 2048 };

	pointLight.ResetAllViewMatrix();

	//MAGIC NUMBER DONT TOUCH 90.05745f FOV
	pointLight.myLightBufferData.ProjectionMatrix = Utility::Matrix4f::CreateProjectionMatrixPerspective(static_cast<float>(projectionSize.x), static_cast<float>(projectionSize.y), nearPlane, farPlane, 90);
	pointLight.myLightBufferData.CastShadows = true;

	pointLight.myLightBufferData.FarPlane = farPlane;
	pointLight.myLightBufferData.NearPlane = nearPlane;

	pointLight.myShadowMap = DepthStencil::Create(static_cast<int>(shadowResolution.x), static_cast<int>(shadowResolution.y));

	for (int i = 0; i < 5; ++i)
	{
		pointLight.myExtraShadowMaps[i] = DepthStencil::Create(static_cast<int>(shadowResolution.x), static_cast<int>(shadowResolution.y));
	}

	DEBUGLOG("Created a Point Light");
	return std::make_shared<PointLight>(pointLight);
}

void PointLight::SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer)
{
}

void PointLight::ResetAllViewMatrix()
{
	for (unsigned i = 0; i < 6; ++i)
	{
		SetViewMatrix(i, i);
	}
}

void PointLight::SetViewMatrix(unsigned aDir, unsigned aToSlot)
{
	Transform newView = myTransform;

	switch (aDir)
	{
		//Forward
		case 0:
		{
			newView.SetRotation(0, 0, 0);
			break;
		}
		//Backward
		case 1:
		{
			newView.SetRotation(0, 180, 0);
			break;
		}
		//Right
		case 2:
		{
			newView.SetRotation(0, 90, 0);
			break;
		}
		//Left
		case 3:
		{
			newView.SetRotation(0, 270, 0);
			break;
		}
		//Up
		case 4:
		{
			newView.SetRotation(270, 0, 0);
			break;
		}
		//Down
		case 5:
		{
			newView.SetRotation(90, 0, 0);
			break;
		}
	}

	myLightBufferData.ViewMatrix[aToSlot] = Utility::Matrix4f::GetFastInverse(newView.GetMatrix());
}


void PointLight::ClearShadowMap(unsigned anIndex)
{
	if (anIndex == 0)
	{
		Light::ClearShadowMap();
	}
	else
	{
		if (myExtraShadowMaps[anIndex - 1])
		{
			myExtraShadowMaps[anIndex - 1]->Clear();
		}
	}
}

void PointLight::SetShadowMapAsTarget(unsigned anIndex)
{
	if (anIndex == 0)
	{
		Light::SetShadowMapAsTarget();
	}
	else
	{
		if (myExtraShadowMaps[anIndex - 1])
		{
			myExtraShadowMaps[anIndex - 1]->SetAsRenderTarget();
		}
	}
}

void PointLight::BindShadowMapAsResource(unsigned aSlot)
{
	Light::BindShadowMapAsResource(aSlot);

	for (int i = 0; i < 5; ++i)
	{
		if (myExtraShadowMaps[i])
		{
			myExtraShadowMaps[i]->SetAsResource(aSlot + i + 1);
		}
	}
}

void PointLight::RemoveShadowMapResource(unsigned aSlot)
{
	Light::RemoveShadowMapResource(aSlot);

	for (int i = 0; i < 5; ++i)
	{
		if (myExtraShadowMaps[i])
		{
			myExtraShadowMaps[i]->RemoveResource(aSlot + i + 1);
		}
	}
}
