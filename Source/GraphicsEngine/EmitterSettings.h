#pragma once
#include "Math/Vector.h"

struct EmitterSettings
{
	float SpawnRate = 1.0f;
	float SpawnAngle = 0.0f;
	float LifeTime = 3.0f;
	Utility::Vector3f StartVelocity;
	Utility::Vector3f EndVelocity;
	float GravityScale = 0.0f;
	Utility::Vector3f StartScale = {1, 1, 1};
	Utility::Vector3f EndScale = {1, 1, 1};
	Utility::Vector4f StartColor = {1, 1, 1, 1};
	Utility::Vector4f EndColor = {1, 1, 1, 1};
};