#pragma once
#include "Core/EngineDefines.h"
#include "Math/Matrix4x4.hpp"
#include "Math/Vector.h"

struct FrameBufferData
{
	Utility::Matrix4x4<float> View;
	Utility::Matrix4x4<float> Projection;

	Utility::Vector4f FrustrumCorners[4];

	Utility::Vector3f CamTranslation;
	UINT RenderMode;

	Utility::Vector2<unsigned> Resolution;
	Utility::Vector2f Padding;

	float NearPlane;
	float FarPlane;
	float DeltaTime;
	float TotalTime;
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
	Utility::Matrix4x4<float> ViewMatrix[6];
	Utility::Matrix4x4<float> ProjectionMatrix;

	Utility::Vector3f Color;
	float Intensity;

	Utility::Vector3f Direction;
	float Range;

	Utility::Vector3f Position;
	float Attenuation;

	float SpotInnerRadius;
	float SpotOuterRadius;
	unsigned LightType;
	bool CastShadows;

	float NearPlane;
	float FarPlane;
	Utility::Vector2f Padding;
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