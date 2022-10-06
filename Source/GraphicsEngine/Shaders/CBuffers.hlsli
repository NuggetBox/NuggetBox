cbuffer FrameBuffer : register(b0)
{
	float4x4 FB_ToView;
	float4x4 FB_ToProjection;

	float4 FB_FrustrumCorners[4];

	float3 FB_CamTranslation;
	uint FB_RenderMode;

	uint2 FB_Resolution;
	float2 FB_Padding;

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
	bool OB_IsInstanced;

	float2 OB_padding;
}

cbuffer MaterialBuffer : register(b2)
{
	float3 MB_Albedo;
	float MB_padding;
}