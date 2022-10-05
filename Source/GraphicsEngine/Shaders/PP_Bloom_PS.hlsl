#include "Common.hlsli"
#include "PostProcessStructs.hlsli"
#include "ShaderStructs.hlsli"

PostProcessPixelOutput main(PostProcessVertexToPixel input)
{
	PostProcessPixelOutput result;

	const float4 resource1 = TextureSlot1.Sample(defaultSampler, input.UV);
	const float4 resource2 = TextureSlot2.Sample(defaultSampler, input.UV);

	//Additive
	/*{
		result.Color.rgb = resource1 + resource2;
	}*/

	//Scaled
	{
		float3 scaledResource = resource1.rgb * (1.0f - saturate(resource2.rgb));
		result.Color.rgb = scaledResource + resource2.rgb;
	}

	//Luminance based
	/*{
		const float luminance = dot(resource1, float3(0.2126f, 0.7152f, 0.0722f));
		const float3 scaledResource = resource2 * (1.0f - luminance);
		result.Color.rgb = resource1 + scaledResource;
	}*/

	result.Color.a = 1.0f;
	return result;
}