#include "NuggetBox.pch.h"
#include "ParticleEmitter.h"

void ParticleEmitter::Initialize(const std::filesystem::path& aTemplatePath)
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

void ParticleEmitter::Initialize(const ParticleEmitterTemplate& aTemplate)
{
	myTexture = Texture::Load(aTemplate.TexturePath);

	myEmitterSettings = aTemplate.EmitterSettings;
	myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

	const size_t maxParticles = static_cast<size_t>(ceilf(myEmitterSettings.SpawnRate * myEmitterSettings.LifeTime));
	myParticles.resize(maxParticles);

	for (size_t i = 0; i < maxParticles; ++i)
	{
		InitParticle(i);
	}

	//Create empty Vertex Buffer?
	D3D11_BUFFER_DESC vertexBufferDesc{};
	vertexBufferDesc.ByteWidth = maxParticles * sizeof(ParticleVertex);
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexSubresourceData{};
	vertexSubresourceData.pSysMem = &myParticles[0];

	ID3D11Buffer* vertexBuffer;
	AssertIfFailed(DX11::Device->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &vertexBuffer));
	//

	myVertexShader = VertexShader::Load("Particle_VS.cso", InputLayoutType::ParticleVertex);
	myPixelShader = PixelShader::Load("Particle_PS.cso");
	myGeometryShader = GeometryShader::Load("Particle_GS.cso");

	SetTransform(aTemplate.Transform);
}

void ParticleEmitter::Update(float aDeltaTime)
{
	for (size_t i = 0; i < myParticles.size(); ++i)
	{
		ParticleVertex& particle = myParticles[i];

		particle.LifeTime += aDeltaTime;

		if (particle.LifeTime >= myEmitterSettings.LifeTime)
		{
			InitParticle(i);
		}

		//TODO: gravity scale
		particle.Velocity.y -= 9.82f * aDeltaTime;
		particle.Position.x += particle.Velocity.x * aDeltaTime;
		particle.Position.y += particle.Velocity.y * aDeltaTime;
		particle.Position.z += particle.Velocity.z * aDeltaTime;
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
	//TODO: Create load function
	ParticleEmitterTemplate yo;
	EmitterSettings em;
	em.LifeTime = 3;
	em.SpawnRate = 2;
	yo.EmitterSettings = em;
	return yo;
}

void ParticleEmitter::InitParticle(size_t aParticleIndex)
{
	myParticles[aParticleIndex].Color = myEmitterSettings.StartColor;
	//myParticles[aParticleIndex].Position =
	myParticles[aParticleIndex].Scale = Utility::Vector3f(myEmitterSettings.StartSize);
	myParticles[aParticleIndex].Velocity = myEmitterSettings.StartVelocity;
}
