#include "ShaderStructs.hlsli"

PixelOutput main(VertexToPixel input)
{
	PixelOutput result;

	//Single color
	//result.myColor = input.myVertexColor0;
	//result.myColor = float4(MB_Albedo, 1);

	//Solid material color with variation
	//const float average = (input.myVertexColor0.r + input.myVertexColor0.g + input.myVertexColor0.b) / 3;
	//result.myColor = average * float4(MB_Albedo, 1);

	// UV coordinate visualization
	//result.myColor = float4(input.myUV0.r, input.myUV0.g, 0, 1);

	//Albedo Texture sampling
	const float3 albedo = albedoTexture.Sample(defaultSampler, input.myUV0).rgb;
	/*result.myColor.rgb = albedo;
	result.myColor.a = 1;*/

	//Normal debugging
	const float3x3 TangentSpaceMatrix = float3x3
	(
		normalize(input.myTangent),
		normalize(input.myBinormal),
		normalize(input.myNormal)
	);

	float3 normalMap = normalTexture.Sample(defaultSampler, input.myUV0).agr;
	normalMap.z = 0;
	normalMap = 2.0f * normalMap - 1;
	normalMap.z = sqrt(1 - saturate(normalMap.x + normalMap.x + normalMap.y + normalMap.y));
	normalMap = normalize(normalMap);
	const float3 pixelNormal = normalize(mul(normalMap, TangentSpaceMatrix));

	float3 debugNormal = pixelNormal;
	const float signedLength = (debugNormal.r + debugNormal.g + debugNormal.b) / 3;
	if (signedLength < 0)
	{
		debugNormal = float3(1 - abs(debugNormal));
	}
	/*result.myColor.rgb = debugNormal;
	result.myColor.a = 1;*/

	//Lambertian reflection
	//Ip=L•NCI
	// Color of lighted pixel = Direction towards light DOT Pixel normal * Light color * Light intensity
	const float3 L = -1 * normalize(LB_Direction);
	const float3 N = pixelNormal;
	const float LdotN = saturate(dot(L, N));
	const float3 C = LB_Color;
	const float I = LB_Intensity;

	const float3 finalPixelColor = LdotN * C * I;

	const float3 diffuse = albedo * finalPixelColor;

	//IBL: Image based lightning
	const float3 environment = environmentTexture.SampleLevel(defaultSampler, input.myNormal, 4).rgb;
	const float3 ambient = albedo * environment;

	//Lighting applied
	result.myColor.rgb = saturate(ambient + diffuse);
	result.myColor.a = 1;

	return result;
}