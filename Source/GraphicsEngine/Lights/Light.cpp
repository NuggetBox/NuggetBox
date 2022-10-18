#include "NuggetBox.pch.h"
#include "Light.h"

//void Light::Initialize(Vector3f aColor, float anIntensity)
//{
//	myLightBufferData.Color = aColor;
//	myLightBufferData.Intensity = anIntensity;
//}

void Light::ClearShadowMap(unsigned anIndex)
{
	anIndex;

	if (myShadowMap)
	{
		myShadowMap->Clear();
	}
}

void Light::SetShadowMapAsTarget(unsigned anIndex)
{
	anIndex;

	if (myShadowMap)
	{
		myShadowMap->SetAsRenderTarget();
	}
}

void Light::BindShadowMapAsResource(unsigned aSlot)
{
	if (myShadowMap)
	{
		myShadowMap->SetAsResource(aSlot);
	}
}

void Light::RemoveShadowMapResource(unsigned aSlot)
{
	if (myShadowMap)
	{
		myShadowMap->RemoveResource(aSlot);
	}
}