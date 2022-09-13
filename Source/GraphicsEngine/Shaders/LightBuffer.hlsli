struct LightData
{
	float3 Color;
	float Intensity;

	float3 Direction;
	float Range;

	float3 Position;
	float Attenuation;

	float SpotInnerRadius;
	float SpotOuterRadius;
	uint LightType;
	float Padding;
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