cbuffer FrameBuffer : register(b0)
{
	float4x4 FB_ToView;
	float4x4 FB_ToProjection;

	float3 FB_CamTranslation;
	unsigned int FB_RenderMode;

	float FB_NearPlane;
	float FB_FarPlane;
	float FB_DeltaTime;
	float FB_TotalTime;
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