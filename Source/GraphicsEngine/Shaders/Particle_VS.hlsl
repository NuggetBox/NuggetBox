#include "CBuffers.hlsli"
#include "ParticleStructs.hlsli"

ParticleVertexData main(ParticleVertexData input)
{
	ParticleVertexData result;

	const float4 particleLocalPosition = input.Position;
	const float4 particleWorldPosition = mul(OB_ToWorld, particleLocalPosition);

	result.Position = particleWorldPosition;
	result.ObjectColor = input.ObjectColor;
	result.LifeTime = input.LifeTime;
	result.Scale = input.Scale;
	result.Velocity = input.Velocity;

	return result;
}