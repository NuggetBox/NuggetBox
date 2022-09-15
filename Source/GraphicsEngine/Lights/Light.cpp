#include "NuggetBox.pch.h"
#include "Light.h"

//void Light::Initialize(Vector3f aColor, float anIntensity)
//{
//	myLightBufferData.Color = aColor;
//	myLightBufferData.Intensity = anIntensity;
//}

void Light::ClearShadowMap()
{
	if (myShadowMap)
	{
		myShadowMap->Clear();
	}
}

void Light::SetShadowMapAsTarget()
{
	if (myShadowMap)
	{
		myShadowMap->SetRenderTarget();
	}
}

void Light::BindShadowMapAsResource(unsigned aSlot)
{
	if (myShadowMap)
	{
		myShadowMap->BindShadowResource(aSlot);
	}
}