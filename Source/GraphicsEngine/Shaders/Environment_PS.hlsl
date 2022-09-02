#include "PBRFunctions.hlsli"
#include "ShaderStructs.hlsli"

DeferredPixelOutput main(DeferredVertexToPixel input)
{
	DeferredPixelOutput result;

	const float4 albedo = albedoTexture.Sample(defaultSampler, input.myUV);

	if (albedo.a <= 0.05f)
	{
		discard;
		return result;
	}

	//Sample GBuffer Texture channels
	const float3 normal = normalTexture.Sample(defaultSampler, input.myUV).rgb;
	const float4 surface = surfaceTexture.Sample(defaultSampler, input.myUV);
	const float3 vertexNormal = vertexNormalTexture.Sample(defaultSampler, input.myUV).rgb;
	const float4 worldPosition = worldPositionTexture.Sample(defaultSampler, input.myUV);
	const float ambientOcclusion = ambientOcclusionTexture.Sample(defaultSampler, input.myUV).r;
	//

	//Extract values from surface texture
	const float metalness = surface.r;
	const float roughness = surface.g;
	const float emissive = surface.b;
	const float emissiveStrength = surface.a;
	//

	//Get toeye, speccolor, diffusecolor
	const float3 toEye = normalize(FB_CamTranslation.xyz - worldPosition.xyz);
	const float3 specularColor = lerp((float3)0.04f, albedo, metalness);
	const float3 diffuseColor = lerp((float3)0.00f, albedo, 1 - metalness);
	//calc ambient
	const float3 ambientLighting = EvaluateAmbience(environmentTexture, normal, vertexNormal, toEye, roughness, ambientOcclusion, diffuseColor, specularColor, defaultSampler);
	//calc direct
	const float3 directionalLighting = EvaluateDirectionalLight(diffuseColor, specularColor, normal, roughness, LB_Color, LB_Intensity, -LB_Direction, toEye);
	//assemble light, lineartogamma
	result.Color.rgb = LinearToGamma(ambientLighting + directionalLighting);
	result.Color.a = 1.0f;

	//TODO: Rendermodes for deferred rendering

	return result;
}