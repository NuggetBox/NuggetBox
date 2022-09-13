#include "../Core/EngineDefines.h"

cbuffer FrameBuffer : register(b0)
{
	float4x4 FB_ToView;
	float4x4 FB_ToProjection;
	float3 FB_CamTranslation;
	unsigned int FB_RenderMode;
}

cbuffer ObjectBuffer : register(b1)
{
	float4x4 OB_ToWorld;
	float4x4 OB_BoneData[128];
	bool OB_HasBones;
	//float3 OB_padding;
}

cbuffer MaterialBuffer : register(b2)
{
	float3 MB_Albedo;
	float MB_padding;
}