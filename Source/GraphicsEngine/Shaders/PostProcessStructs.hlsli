struct PostProcessVertexToPixel
{
	float4 Position : SV_POSITION;
	float2 UV : TEXCOORD;
};

struct PostProcessPixelOutput
{
	float4 Color : SV_TARGET;
};

Texture2D TextureSlot1 : register(t0);
Texture2D TextureSlot2 : register(t1);
Texture2D TextureSlot3 : register(t2);
Texture2D TextureSlot4 : register(t3);
Texture2D TextureSlot5 : register(t4);
Texture2D TextureSlot6 : register(t5);
Texture2D TextureSlot7 : register(t6);
Texture2D TextureSlot8 : register(t7);
Texture2D TextureSlot9 : register(t8);
Texture2D TextureSlot10 : register(t9);
Texture2D TextureSlot11 : register(t10);
Texture2D TextureSlot12 : register(t11);