#include "NuggetBox.pch.h"
#include "ParticleEmitter.h"

#include "DebugLogger.h"
#include "json.hpp"
#include "Timer.h"

void ParticleEmitter::Initialize(const ParticleEmitterTemplate& aTemplate)
{
	myEmitterSettings = aTemplate.EmitterSettings;
	myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	myStride = sizeof(ParticleVertex);
	myOffset = 0;

	const size_t maxParticles = static_cast<size_t>(ceilf(myEmitterSettings.SpawnRate * myEmitterSettings.LifeTime));
	myParticles.resize(maxParticles);

	for (size_t i = 0; i < maxParticles; ++i)
	{
		InitParticle(i);
	}

	//Create empty Vertex Buffer?
	D3D11_BUFFER_DESC vertexBufferDesc{};
	vertexBufferDesc.ByteWidth = static_cast<UINT>(static_cast<UINT>(maxParticles) * sizeof(ParticleVertex));
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA vertexSubresourceData{};
	vertexSubresourceData.pSysMem = &myParticles[0];

	AssertIfFailed(DX11::Device->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, myVertexBuffer.GetAddressOf()));
	//

	myVertexShader = VertexShader::Load("Shaders/Particle_VS.cso", InputLayoutType::ParticleVertex);
	myPixelShader = PixelShader::Load("Shaders/Particle_PS.cso");
	myGeometryShader = GeometryShader::Load("Shaders/Particle_GS.cso");

	myTexture = Texture::Load(aTemplate.TexturePath);
	//SetTransform(aTemplate.Transform);
}

void ParticleEmitter::LoadAndInitialize(const std::filesystem::path& aTemplatePath)
{
	if (!ourEmitterTemplateRegistry.contains(aTemplatePath.string()))
	{
		Initialize(ourEmitterTemplateRegistry.at(aTemplatePath.string()));
	}
	else
	{
		Initialize(Load(aTemplatePath.string()));
	}
}

void ParticleEmitter::Update()
{
	tempTimer += Timer::GetDeltaTime();

	if (tempTimer >= myEmitterSettings.SpawnRate)
	{
		tempTimer = 0;
		InitParticle(0);
	}

	for (size_t i = 0; i < myParticles.size(); ++i)
	{
		ParticleVertex& particle = myParticles[i];

		particle.LifeTime += Timer::GetDeltaTime();

		if (particle.LifeTime >= myEmitterSettings.LifeTime)
		{
			InitParticle(i);
		}

		if (particle.LifeTime <= 0)
		{
			particle.Color.w = 0;
		}
		else
		{
			particle.Color.w = 1;
		}

		//TODO: gravity scale
		//particle.Velocity.y -= 9.82f * Timer::GetDeltaTime();

		particle.Position.x += particle.Velocity.x * Timer::GetDeltaTime();
		particle.Position.y += particle.Velocity.y * Timer::GetDeltaTime();
		particle.Position.z += particle.Velocity.z * Timer::GetDeltaTime();
	}
}

void ParticleEmitter::Bind() const
{
	D3D11_MAPPED_SUBRESOURCE bufferData;
	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	//AssertIfFailed(DX11::Context->Map(myVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData));
	HRESULT resu = DX11::Context->Map(myVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	assert(resu == S_OK);
	memcpy_s(bufferData.pData, sizeof(ParticleVertex) * myParticles.size(), &myParticles[0], sizeof(ParticleVertex) * myParticles.size());
	DX11::Context->Unmap(myVertexBuffer.Get(), 0);

	DX11::Context->IASetPrimitiveTopology(static_cast<D3D_PRIMITIVE_TOPOLOGY>(myPrimitiveTopology));
	DX11::Context->IASetVertexBuffers(0, 1, myVertexBuffer.GetAddressOf(), &myStride, &myOffset);

	myVertexShader->Bind();
	myPixelShader->Bind();
	myGeometryShader->Bind();

	if (myTexture)
	{
		myTexture->SetAsResource(0);
	}
}

void ParticleEmitter::Draw() const
{
	DX11::Context->Draw(static_cast<UINT>(myParticles.size()), 0);
}

ParticleEmitterTemplate ParticleEmitter::Load(const std::filesystem::path& aTemplatePath)
{
	if (ourEmitterTemplateRegistry.contains(aTemplatePath.string()))
	{
		DEBUGLOG("Loaded Particle Emitter " + aTemplatePath.filename().string() + " from registry successfully");
		return ourEmitterTemplateRegistry.at(aTemplatePath.string());
	}

	ParticleEmitterTemplate loadedTemplate;

	if (!std::filesystem::exists(aTemplatePath))
	{
		DEBUGERROR("File " + aTemplatePath.string() + " was trying to be read as a ParticleEmitterTemplate file but doesn't exist");
		return loadedTemplate;
	}

	using json = nlohmann::json;
	std::ifstream file(aTemplatePath.string());
	json data = json::parse(file);

	//TODO: Add fail-safe/backwards compatibility
	loadedTemplate.TexturePath = data["TexturePath"].get<std::string>();

	EmitterSettings loadedSettings;
	loadedSettings.SpawnRate = data["SpawnRate"];
	loadedSettings.SpawnAngle = data["SpawnAngle"];
	loadedSettings.LifeTime = data["LifeTime"];
	loadedSettings.StartVelocity.x = data["StartVelocity"][0];
	loadedSettings.StartVelocity.y = data["StartVelocity"][1];
	loadedSettings.StartVelocity.z = data["StartVelocity"][2];
	loadedSettings.EndVelocity.x = data["EndVelocity"][0];
	loadedSettings.EndVelocity.y = data["EndVelocity"][1];
	loadedSettings.EndVelocity.z = data["EndVelocity"][2];
	loadedSettings.GravityScale = data["GravityScale"];
	loadedSettings.StartSize = data["StartSize"];
	loadedSettings.EndSize = data["EndSize"];
	loadedSettings.StartColor.x = data["StartColor"][0];
	loadedSettings.StartColor.y = data["StartColor"][1];
	loadedSettings.StartColor.z = data["StartColor"][2];
	loadedSettings.StartColor.w = data["StartColor"][3];
	loadedSettings.EndColor.x = data["EndColor"][0];
	loadedSettings.EndColor.y = data["EndColor"][1];
	loadedSettings.EndColor.z = data["EndColor"][2];
	loadedSettings.EndColor.w = data["EndColor"][3];

	loadedTemplate.EmitterSettings = loadedSettings;

	DEBUGLOG("Loaded Particle Emitter " + aTemplatePath.filename().string() + " successfully");

	ourEmitterTemplateRegistry.insert(std::pair(aTemplatePath.string(), loadedTemplate));
	return loadedTemplate;
}

void ParticleEmitter::InitParticle(size_t aParticleIndex)
{
	myParticles[aParticleIndex].Position.x = GetTransform().GetXPosition();
	myParticles[aParticleIndex].Position.y = GetTransform().GetYPosition();
	myParticles[aParticleIndex].Position.z = GetTransform().GetZPosition();
	myParticles[aParticleIndex].Color = myEmitterSettings.StartColor;
	myParticles[aParticleIndex].Velocity = myEmitterSettings.StartVelocity;
	myParticles[aParticleIndex].Scale = Utility::Vector3f(myEmitterSettings.StartSize);
	myParticles[aParticleIndex].LifeTime = 0;
}
