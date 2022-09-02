#pragma once
#include "EngineDefines.h"
#include "Math/Matrix4x4.hpp"

struct FrameBufferData
{
	Utility::Matrix4x4<float> View;
	Utility::Matrix4x4<float> Projection;
	Utility::Vector3f CamTranslation;
	UINT RenderMode;
};

struct ObjectBufferData
{
	Utility::Matrix4x4<float> World;
	Utility::Matrix4x4<float> BoneData[MAX_BONES];
	alignas(16) bool HasBones = false;
	//bool HasBones = false;
	//Vector3f padding;
};

struct LightBufferData
{
	Vector3f Color;
	float Intensity;
	Vector3f Direction;
	float padding;
};

enum class RenderMode
{
	Default,
	UV,
	VertexColor,
	VertexNormal,
	PixelNormal,
	AlbedoMap,
	NormalMap,
	DiffuseLight,
	DiffuseLightNoAlbedo,
	AmbientLight,
	AmbientLightNoAlbedo,
	AmbientOcclusion,
	Roughness,
	Metalness,
	Emissiveness,
	Linear,
	COUNT //Update RenderModeToString in GraphicsEngine when adding new RenderModes
};