#include "NuggetBox.pch.h"
#include "PointLight.h"

std::shared_ptr<PointLight> PointLight::Create(Vector3f aColor, float anIntensity, Vector3f aPosition, float aRange)
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
	constexpr float nearPlane = 1.0f;
	constexpr float farPlane = 15000.0f;
	constexpr POINT shadowResolution = { 2048, 2048 };
	constexpr POINT projectionSize = { 2048, 2048 };

	pointLight.ResetAllViewMatrix();

	pointLight.myLightBufferData.ProjectionMatrix = Matrix4f::CreateProjectionMatrixPerspective(projectionSize.x, projectionSize.y, nearPlane, farPlane, 90);
	pointLight.myLightBufferData.CastShadows = true;

	pointLight.myLightBufferData.FarPlane = farPlane;
	pointLight.myLightBufferData.NearPlane = nearPlane;

	pointLight.myShadowMap = DepthStencil::Create(shadowResolution.x, shadowResolution.y);

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

	myLightBufferData.ViewMatrix[aToSlot] = Matrix4f::GetFastInverse(newView.GetMatrix());
}


void PointLight::ClearShadowMap()
{
	Light::ClearShadowMap();

	for (int i = 0; i < 5; ++i)
	{
		if (myExtraShadowMaps[i])
		{
			myExtraShadowMaps[i]->Clear();
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
			myExtraShadowMaps[anIndex - 1]->SetRenderTarget();
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
			myExtraShadowMaps[i]->BindShadowResource(aSlot + i + 1);
		}
	}
}