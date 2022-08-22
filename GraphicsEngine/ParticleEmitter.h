#pragma once
#include "EmitterSettings.h"
#include "ParticleVertex.h"
#include "DX11.h"
#include "Texture.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GeometryShader.h"
#include "Math/Transform.hpp"
#include "SceneObject.h"

struct ParticleEmitterTemplate
{
	std::filesystem::path TexturePath;
	EmitterSettings EmitterSettings;
	Transform Transform;
};

class ParticleEmitter : public SceneObject
{
public:
	//virtual ~ParticleEmitter() = default;
	ParticleEmitter() = default;

	void Initialize(const std::filesystem::path& aTemplatePath);
	void Initialize(const ParticleEmitterTemplate& aTemplate);

	void Update(float aDeltaTime);
	void Bind() const;

	void Draw() const;

	static ParticleEmitterTemplate Load(const std::filesystem::path& aTemplatePath);

	const EmitterSettings& GetEmitterSettings() const { return myEmitterSettings; }

private:
	void InitParticle(size_t aParticleIndex);

	inline static std::unordered_map<std::string, ParticleEmitterTemplate> ourEmitterTemplateRegistry;

	EmitterSettings myEmitterSettings;
	std::vector<ParticleVertex> myParticles;

	ComPtr<ID3D11Buffer> myVertexBuffer;

	UINT myStride;
	UINT myOffset;
	UINT myPrimitiveTopology;

	//ComPtr<ID3D11VertexShader> myVertexShader;
	//ComPtr<ID3D11PixelShader> myPixelShader;
	//ComPtr<ID3D11GeometryShader> myGeometryShader;
	std::shared_ptr<VertexShader> myVertexShader;
	std::shared_ptr<PixelShader> myPixelShader;
	std::shared_ptr<GeometryShader> myGeometryShader;

	//ComPtr<ID3D11InputLayout> myInputLayout;

	std::shared_ptr<Texture> myTexture;
};