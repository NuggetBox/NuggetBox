struct PostProcessVertexToPixel
{
	float4 Position : SV_POSITION;
	float2 UV : TEXCOORD;
};

struct PostProcessPixelOutput
{
	float4 Color : SV_TARGET;
};

Texture2D TextureSlot1 : register(t40);
Texture2D TextureSlot2 : register(t41);