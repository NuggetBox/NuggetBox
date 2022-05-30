#include "ShaderStructs.hlsli"

PixelOutput main(VertexToPixel input)
{
	PixelOutput result;

	//Single color
	//result.myColor = input.myVertexColor0;
	//result.myColor = float4(MB_Albedo, 1);

	//const float average = (input.myVertexColor0.r + input.myVertexColor0.g + input.myVertexColor0.b) / 3;
	//result.myColor = average * float4(MB_Albedo, 1);

	// UV coordinate visualization
	//result.myColor = float4(input.myUV0.r, input.myUV0.g, 0, 1);

	result.myColor.rgb = albedoTexture.Sample(defaultSampler, input.myUV0).rgb;
	result.myColor.a = 1;

	return result;
}