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
	/*result.myColor.rgb = albedoTexture.Sample(defaultSampler, input.myUV0).rgb;
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
	result.myColor.rgb = debugNormal;
	result.myColor.a = 1;

	return result;
}