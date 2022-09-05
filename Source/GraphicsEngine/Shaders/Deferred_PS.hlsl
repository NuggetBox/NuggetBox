#include "ShaderStructs.hlsli"

GBufferOutput main(VertexToPixel input)
{
	GBufferOutput result;

	const float4 albedo = albedoTexture.Sample(defaultSampler, input.myUV0).rgba;

	if (albedo.a <= 0.05f)
	{
		discard;
		return result;
	}

	const float3 normalMap = normalTexture.Sample(defaultSampler, input.myUV0).agb;
	const float4 surface = surfaceTexture.Sample(defaultSampler, input.myUV0);

	const float ambientOcclusion = normalMap.b;

	//Construct TangentSpaceMatrix
	const float3x3 TangentSpaceMatrix = float3x3
	(
		normalize(input.myTangent),
		normalize(input.myBinormal),
		normalize(input.myNormal)
	);

	//Unpack pixel normal from normal map
	//Daniel has * 0.75 here? float3 pixelNormal = normalMap * 0.75;
	float3 pixelNormal = normalMap;
	pixelNormal.z = 0;
	pixelNormal = 2.0f * pixelNormal - 1;
	pixelNormal.z = sqrt(1 - saturate(pixelNormal.x + pixelNormal.x + pixelNormal.y + pixelNormal.y));
	pixelNormal = normalize(pixelNormal);
	pixelNormal = normalize(mul(pixelNormal, TangentSpaceMatrix));

	result.myAlbedo = albedo;
	result.myNormal = float4(pixelNormal, 1.0f);
	result.myMaterial = surface;
	result.myVertexNormal = float4(normalize(input.myNormal), 1.0f);
	result.myWorldPosition = input.myVertexPosition;
	result.myAmbientOcclusion = ambientOcclusion;

	return result;
}