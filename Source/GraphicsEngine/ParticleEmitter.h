#pragma once
#include <Queue.hpp>

#include "EmitterSettings.h"
#include "ParticleVertex.h"
#include "DX11.h"
#include "Texture.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GeometryShader.h"
#include "SceneObject.h"

struct ParticleEmitterTemplate
{
	std::filesystem::path TexturePath;
	EmitterSettings EmitterSettings;
	//Transform Transform;
};

class ParticleEmitter : public SceneObject
{
public:
	//virtual ~ParticleEmitter() = default;
	ParticleEmitter() = default;

	void Initialize(const ParticleEmitterTemplate& aTemplate, bool aStart = true);
	void LoadAndInitialize(const std::filesystem::path& aTemplatePath, bool aStart = true);

	void InitializeEmission();
	void Start();
	void Pause();
	void ClearParticles();

	void Update();
	void Bind() const;

	void Draw() const;

	static ParticleEmitterTemplate Load(const std::filesystem::path& aTemplatePath);

	const EmitterSettings& GetEmitterSettings() const { return myEmitterSettings; }

private:
	void InitParticle(size_t aParticleIndex, float aLifeTime = 0.0f);

	inline static std::unordered_map<std::string, ParticleEmitterTemplate> ourEmitterTemplateRegistry;

	EmitterSettings myEmitterSettings;
	std::vector<ParticleVertex> myParticles;

	Utility::Queue<size_t> myAvailableParticles;
	float mySpawnTimer;
	bool myIsEmitting;

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