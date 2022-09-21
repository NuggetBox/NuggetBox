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
	float ScaleMultiplier = 1.0f;

	Utility::Vector4f StartColor = {1.0f, 1.0f, 1.0f, 1.0f };
	Utility::Vector4f EndColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	Utility::Vector3f StartSpeed = { 0.0f, 100.0f, 0.0f };
	Utility::Vector3f EndSpeed = { 0.0f, -200.0f, 0.0f };
	float SpeedMultiplier = 1.0f;

	bool UseAcceleration = false;
	Utility::Vector3f Acceleration = { 0.0f, -982.0f, 0.0f };
	float MaxSpeed = 2500.0f;

	//TODO: Blend mode setting?
	//BlendMode BlendingMode = BlendMode::Additive;

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