#include "Common.hlsli"
#include "PostProcessStructs.hlsli"
#include "ShaderStructs.hlsli"
#include "Tonemaps.hlsl"

PostProcessPixelOutput main(PostProcessVertexToPixel input)
{
	PostProcessPixelOutput result;

	const float3 color = albedoTexture.Sample(defaultSampler, input.UV).rgb;
	const uint mode = 2;

	switch(mode)
	{
		case 1:
			result.Color.rgb = LinearToGamma(Tonemap_Reinhard2(color));
			break;
		case 2:
			result.Color.rgb = Tonemap_UnrealEngine(color);
			break;
		case 3:
			result.Color.rgb = LinearToGamma(Tonemap_ACES(color));
			break;
		case 4:
			result.Color.rgb = LinearToGamma(Tonemap_Lottes(color));
			break;
		default:
			result.Color.rgb = LinearToGamma(color);
			break;
	}

	result.Color.a = 1;
	return result;
}