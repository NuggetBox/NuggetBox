struct LightData
{
	float4x4 ViewMatrix[6];
	float4x4 ProjectionMatrix;

	float3 Color;
	float Intensity;

	float3 Direction;
	float Range;

	float3 Position;
	float Attenuation;

	float SpotInnerRadius;
	float SpotOuterRadius;
	uint LightType;
	bool CastShadows;

	float NearPlane;
	float FarPlane;
	float2 Padding;
};

//Remember to define MAX_LIGHTS before including this
cbuffer LightBuffer : register(b3)
{
	LightData LB_DirectionalLight;
	LightData LB_Lights[MAX_LIGHTS];
	uint LB_NumLights;
	float3 LB_Padding;

	/*float3 LB_Color;
	float LB_Intensity;
	float3 LB_Direction;
	float LB_padding;*/
}

Texture2D dirLightShadowMap : register(t10);
Texture2D spotLightShadowMap : register(t20);
Texture2D pointLightShadowMaps[6] : register(t30); //30-36