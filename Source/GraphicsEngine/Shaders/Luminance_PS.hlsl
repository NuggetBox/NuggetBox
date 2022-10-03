#include "PostProcessStructs.hlsli"
#include "ShaderStructs.hlsli"

PostProcessPixelOutput main(PostProcessVertexToPixel input)
{
	PostProcessPixelOutput result;

	const float4 color = TextureSlot1.Sample(defaultSampler, input.UV);

	if (color.a < 0.05f)
	{
		discard;
		result.Color = 0;
		return result;
	}

	const float3 resource = color.rgb;

	//Gain, no cutoff
	/*{
		float luminance = dot(resource, float3(0.2126f, 0.7152f, 0.0722f));
		result.Color.rgb = resource * pow(luminance, 5);
	}*/

	//Flat cutoff
	/*{
		float luminance = dot(resource, float3(0.2126f, 0.7152f, 0.0722f));
		float cutOff = 0.8f;
		if (luminance >= cutOff)
			result.Color.rgb = resource;
		else
			result.Color.rgb = 0;
	}*/

	//Gain, cutoff
	{
		float luminance = dot(resource, float3(0.2126f, 0.7152f, 0.0722f));
		float cutOff = 0.5f;
		luminance = saturate(luminance - cutOff);
		result.Color.rgb = resource * luminance * (1.f / cutOff);
		result.Color.a = 1.0f;
	}

	//Flat cutoff with smooth edges
	/*{
		float luminance = dot(resource, float3(0.2126f, 0.7152f, 0.0722f));
		float cutOff = 0.8f;

		if (luminance >= cutOff)
		{
			result.Color.rgb = resource;
		}
		else if (luminance >= cutOff * 0.5f)
		{
			float fade = luminance / cutOff;
			fade = pow(fade, 5);
			result.Color.rgb = resource * fade;
		}
		else
		{
			result.Color.rgb = 0;
		}
	}*/

	result.Color.a = 1.0f;
	return result;
}
