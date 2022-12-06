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
			result.ObjectColor.rgb = LinearToGamma(Tonemap_Reinhard2(color));
			break;
		case 2:
			result.ObjectColor.rgb = Tonemap_UnrealEngine(color);
			break;
		case 3:
			result.ObjectColor.rgb = LinearToGamma(Tonemap_ACES(color));
			break;
		case 4:
			result.ObjectColor.rgb = LinearToGamma(Tonemap_Lottes(color));
			break;
		default:
			result.ObjectColor.rgb = LinearToGamma(color);
			break;
	}

	result.ObjectColor.a = 1;
	return result;
}