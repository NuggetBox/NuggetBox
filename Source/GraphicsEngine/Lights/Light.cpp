#include "NuggetBox.pch.h"
#include "Light.h"

//void Light::Initialize(Vector3f aColor, float anIntensity)
//{
//	myLightBufferData.Color = aColor;
//	myLightBufferData.Intensity = anIntensity;
//}

void Light::SetLightViewport()
{
	myShadowMap->SetViewport();
}

void Light::ClearShadowMap()
{
	myShadowMap->Clear();
}

void Light::BindShadowMap()
{
	myShadowMap->Bind();
}