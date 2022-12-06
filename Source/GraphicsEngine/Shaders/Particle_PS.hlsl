#include "ShaderStructs.hlsli"
#include "ParticleStructs.hlsli"

ParticlePixelOutput main(ParticleGeometryToPixel input)/* : SV_TARGET*/
{
	ParticlePixelOutput result;
	float4 textureColor = albedoTexture.Sample(defaultSampler, input.UV);
	result.ObjectColor.rgba = textureColor.rgba * input.ObjectColor.rgba;

	//Helps avoid extra alpha testing
	if (textureColor.a <= 0.05f)
		discard;

	return result;
}