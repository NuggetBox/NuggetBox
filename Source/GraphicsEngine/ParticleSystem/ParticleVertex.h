#pragma once
#include "Math/Vector.h"	

struct ParticleVertex
{
	//Color.w < 0.0f == particle dead
	Utility::Vector4f Position = { 0, 0, 0, 1 };
	Utility::Vector4f Color = { 1, 1, 1, 1 };
	Utility::Vector3f Velocity = { 0, 0, 0 };
	Utility::Vector3f Scale = { 1, 1, 1 };
	float LifeTime = 0;
};