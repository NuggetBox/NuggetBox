#pragma once
#include "Math/Vector.h"

enum class EmitterType
{
	Cone,
	Sphere
};

struct EmitterSettings
{
	EmitterType EmitterType = EmitterType::Cone;

	float SpawnRate = 50.0f;

	float MinLifeTime = 5.0f;
	float MaxLifeTime = 5.0f;

	Utility::Vector3f StartScale = {1.0f, 1.0f, 1.0f};
	Utility::Vector3f EndScale = {1.0f, 1.0f, 1.0f};
	//A random value between 1 and MinScaleMultiplier will be randomized, start & end scale will be multiplied by it for a particle. For scale variation between particles.
	//At 1 all particles will follow exact start/end scale. At 2, particles will be 1-2x scale compared to start/end scale with random distribution
	float ScaleMultiplier = 1.0f;

	Utility::Vector4f StartColor = {1.0f, 1.0f, 1.0f, 1.0f };
	Utility::Vector4f EndColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	Utility::Vector3f StartSpeed = { 0.0f, 100.0f, 1.0f };
	Utility::Vector3f EndSpeed = { 0.0f, -200.0f, 0.0f };
	//If different from 1, all particles will not have the same start/end speed
	float SpeedMultiplier = 1.0f;

	bool UseAcceleration = false;
	Utility::Vector3f Acceleration = { 0.0f, -982.0f, 0.0f };
	//Maxspeed will be used to cap speed if acceleration is used
	float MaxSpeed = 2500.0f;

	//TODO: Blend mode setting
	//BlendMode BlendingMode

	bool Looping = true;
	bool ScaledDeltaTime = true;

	//Cone
	float InnerRadius = 10.0f;
	float OuterRadius = 50.0f;

	bool SpawnOnEdge = true;
	bool AimForEdge = true;

	//Sphere
	float Radius = 50.0f;

	bool SpawnOnSurface = true;
};