#include "NuggetBox.pch.h"
#include "ParticleEmitter.h"

#include "InputHandler.h"
#include "DebugLogger.h"
#include "json.hpp"
#include "Timer.h"
#include "UtilityFunctions.hpp"

void ParticleEmitter::Initialize(const ParticleEmitterTemplate& aTemplate, bool aStart)
{
	myEmitterSettings = aTemplate.EmitterSettings;
	myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	myStride = sizeof(ParticleVertex);
	myOffset = 0;

	myMaxParticles = static_cast<size_t>(ceilf(myEmitterSettings.SpawnRate * myEmitterSettings.LifeTime) * 1.1f);
	myParticles.resize(myMaxParticles);

	myAvailableParticles.Initialize(myMaxParticles);
	InitializeEmission();

	myIsEmitting = aStart;
	mySpawnTimer = 0;

	//Create empty Vertex Buffer?
	D3D11_BUFFER_DESC vertexBufferDesc{};
	vertexBufferDesc.ByteWidth = static_cast<UINT>(static_cast<UINT>(myMaxParticles) * sizeof(ParticleVertex));
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

void ParticleEmitter::LoadAndInitialize(const std::filesystem::path& aTemplatePath, bool aStart)
{
	if (!ourEmitterTemplateRegistry.contains(aTemplatePath.string()))
	{
		Initialize(ourEmitterTemplateRegistry.at(aTemplatePath.string()), aStart);
	}
	else
	{
		Initialize(Load(aTemplatePath.string()), aStart);
	}
}

void ParticleEmitter::InitializeEmission()
{
	for (int i = 0; i < myParticles.size(); ++i)
	{
		//Mark particles as available to spawn, set lifetime very negative to represent dead particle
		myAvailableParticles.EnqueueUnsafe(i);
		myParticles[i].LifeTime = -D3D10_FLOAT32_MAX;
	}
}

void ParticleEmitter::Start()
{
	myIsEmitting = true;
}

void ParticleEmitter::Pause()
{
	myIsEmitting = false;
}

void ParticleEmitter::ClearParticles()
{
	//Make all particles available for spawn
	myAvailableParticles.Initialize(myMaxParticles);
	InitializeEmission();
	mySpawnTimer = 0;
}

void ParticleEmitter::Update()
{
	//TODO: Put somewhere reasonable
	if (InputHandler::GetKeyDown('J'))
	{
		Start();
	}
	else if (InputHandler::GetKeyDown('K'))
	{
		Pause();
	}
	else if (InputHandler::GetKeyDown('L'))
	{
		ClearParticles();
	}

	if (myIsEmitting)
	{
		//Spawn new particle, multiple if 1 particle/frame is not enough
		while (mySpawnTimer <= 0)
		{
			mySpawnTimer += 1 / myEmitterSettings.SpawnRate;

			InitParticle(myAvailableParticles.Dequeue());
		}

		mySpawnTimer -= Timer::GetDeltaTime();
	}

	for (size_t i = 0; i < myParticles.size(); ++i)
	{
		ParticleVertex& particle = myParticles[i];
		particle.LifeTime += Timer::GetDeltaTime();

		//Do not show dead particles
		if (particle.LifeTime <= 0.0f)
		{
			particle.Color.w = 0.0f;
		}
		else
		{
			if (particle.LifeTime >= myEmitterSettings.LifeTime)
			{
				//Particle surpassed lifetime, make available for spawn
				myAvailableParticles.EnqueueUnsafe(i);
				particle.LifeTime = -D3D10_FLOAT32_MAX;
			}
			else
			{
				particle.Velocity.y -= myEmitterSettings.GravityScale * Timer::GetDeltaTime();
				//TODO: Air resistance/Limit to speed?

				particle.Position.x += particle.Velocity.x * Timer::GetDeltaTime();
				particle.Position.y += particle.Velocity.y * Timer::GetDeltaTime();
				particle.Position.z += particle.Velocity.z * Timer::GetDeltaTime();

				float lifePercentage = particle.LifeTime / myEmitterSettings.LifeTime;

				/*particle.Velocity.x = Utility::Lerp(myEmitterSettings.StartVelocity.x, myEmitterSettings.EndVelocity.x, lifePercentage);
				particle.Velocity.y = Utility::Lerp(myEmitterSettings.StartVelocity.y, myEmitterSettings.EndVelocity.y, lifePercentage);
				particle.Velocity.z = Utility::Lerp(myEmitterSettings.StartVelocity.z, myEmitterSettings.EndVelocity.z, lifePercentage);*/

				particle.Scale.x = Utility::Lerp(myEmitterSettings.StartScale.x, myEmitterSettings.EndScale.x, lifePercentage);
				particle.Scale.y = Utility::Lerp(myEmitterSettings.StartScale.y, myEmitterSettings.EndScale.y, lifePercentage);
				particle.Scale.z = Utility::Lerp(myEmitterSettings.StartScale.z, myEmitterSettings.EndScale.z, lifePercentage);

				particle.Color.x = Utility::Lerp(myEmitterSettings.StartColor.x, myEmitterSettings.EndColor.x, lifePercentage);
				particle.Color.y = Utility::Lerp(myEmitterSettings.StartColor.y, myEmitterSettings.EndColor.y, lifePercentage);
				particle.Color.z = Utility::Lerp(myEmitterSettings.StartColor.z, myEmitterSettings.EndColor.z, lifePercentage);
				particle.Color.w = Utility::Lerp(myEmitterSettings.StartColor.w, myEmitterSettings.EndColor.w, lifePercentage);
			}
		}
	}
}

void ParticleEmitter::Bind() const
{
	D3D11_MAPPED_SUBRESOURCE bufferData;
	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	AssertIfFailed(DX11::Context->Map(myVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData));
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

	loadedSettings.StartScale.x = data["StartScale"][0];
	loadedSettings.StartScale.y = data["StartScale"][1];
	loadedSettings.StartScale.z = data["StartScale"][2];

	loadedSettings.EndScale.x = data["EndScale"][0];
	loadedSettings.EndScale.y = data["EndScale"][1];
	loadedSettings.EndScale.z = data["EndScale"][2];

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

void ParticleEmitter::InitParticle(size_t aParticleIndex, float aLifeTime)
{
	myParticles[aParticleIndex].Position.x = GetTransform().GetXPosition();
	myParticles[aParticleIndex].Position.y = GetTransform().GetYPosition();
	myParticles[aParticleIndex].Position.z = GetTransform().GetZPosition();
	myParticles[aParticleIndex].Color = myEmitterSettings.StartColor;

	//TODO: Temp cone emitter code
	float x = (std::rand() % 200) - 100;
	float y = (std::rand() % 200) - 100;

	float c = sqrt(x * x + y * y);
	x /= c;
	y /= c;

	x *= 50;
	y *= 50;

	myParticles[aParticleIndex].Velocity = myEmitterSettings.StartVelocity + Utility::Vector3f(x, 0, y);
	myParticles[aParticleIndex].Scale = myEmitterSettings.StartScale;
	myParticles[aParticleIndex].LifeTime = aLifeTime;
}