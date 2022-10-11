#include "PostProcessStructs.hlsli"
#include "ShaderStructs.hlsli"

PostProcessPixelOutput main(PostProcessVertexToPixel input)
{
	PostProcessPixelOutput result;
	float4 color = TextureSlot1.Sample(defaultSampler, input.UV);

	if (color.a < 0.05f)
	{
		discard;
		result.Color = 0;
		return result;
	}

	result.Color = color;
	return result;
}