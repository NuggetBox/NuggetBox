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
	float4 myVertexPosition : POSITION;
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
	//uint4 myBoneIDs : BONEIDS;
	//float4 myBoneWeights : BONEWEIGHTS;
};

struct PixelOutput
{
	float4 myColor : SV_TARGET;
};

//TODO: Put Deferred stuff somewhere else?
struct GBufferOutput
{
	float4 myAlbedo : SV_TARGET0;
	float4 myNormal : SV_TARGET1;
	float4 myMaterial : SV_TARGET2;

	//WS Normal Channels
	float4 myVertexNormal : SV_TARGET3;

	//WS Channel
	float4 myWorldPosition : SV_TARGET4;

	float myAmbientOcclusion : SV_TARGET5;
};

struct DeferredVertexInput
{
	unsigned int myIndex : SV_VertexID;
};

struct DeferredVertexToPixel
{
	float4 myPosition : SV_POSITION;
	float2 myUV : UV0;
};

struct DeferredPixelOutput
{
	float4 Color : SV_TARGET;
};

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

//TODO: clean mess
Texture2D vertexNormalTexture : register(t3);
Texture2D worldPositionTexture : register(t4);
Texture2D ambientOcclusionTexture : register(t5);

TextureCube environmentTexture : register(t10);