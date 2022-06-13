struct VertexInput
{
	float4 myPosition : POSITION;
	float4 myVertexColor0 : COLOR0;
	float4 myVertexColor1 : COLOR1;
	float4 myVertexColor2 : COLOR2;
	float4 myVertexColor3 : COLOR3;
	float2 myUV0 : UV0;
	float2 myUV1 : UV1;
	float2 myUV2 : UV2;
	float2 myUV3 : UV3;
	float3 myNormal : NORMAL;
	float3 myTangent : TANGENT;
	float3 myBinormal : BINORMAL;
	uint4 myBoneIDs : BONEIDS;
	float4 myBoneWeights : BONEWEIGHTS;
};

struct VertexToPixel
{
	float4 myPosition : SV_POSITION;
	float4 myVertexColor0 : COLOR0;
	float4 myVertexColor1 : COLOR1;
	float4 myVertexColor2 : COLOR2;
	float4 myVertexColor3 : COLOR3;
	float2 myUV0 : UV0;
	float2 myUV1 : UV1;
	float2 myUV2 : UV2;
	float2 myUV3 : UV3;
	float3 myNormal : NORMAL;
	float3 myTangent : TANGENT;
	float3 myBinormal : BINORMAL;
	uint4 myBoneIDs : BONEIDS;
	float4 myBoneWeights : BONEWEIGHTS;
};

struct PixelOutput
{
	float4 myColor : SV_TARGET;
};

cbuffer FrameBuffer : register(b0)
{
	float4x4 FB_ToView;
	float4x4 FB_ToProjection;
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

cbuffer LightBuffer : register(b3)
{
	float3 LB_Color;
	float LB_Intensity;
	float3 LB_Direction;
	float LB_padding;
}

SamplerState defaultSampler : register(s0);

Texture2D albedoTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D surfaceTexture : register(t2);

TextureCube environmentTexture : register(t10);