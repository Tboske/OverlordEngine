#include "stdafx.h"
#include "ParticleEmitterComponent.h"
#include "Misc/ParticleMaterial.h"

ParticleMaterial* ParticleEmitterComponent::m_pParticleMaterial{};

ParticleEmitterComponent::ParticleEmitterComponent(const std::wstring& assetFile, const ParticleEmitterSettings& emitterSettings, UINT particleCount):
	m_ParticlesArray(new Particle[particleCount]),
	m_ParticleCount(particleCount), //How big is our particle buffer?
	m_MaxParticles(particleCount), //How many particles to draw (max == particleCount)
	m_AssetFile(assetFile),
	m_EmitterSettings(emitterSettings)
{
	m_enablePostDraw = true; //This enables the PostDraw function for the component
}

ParticleEmitterComponent::~ParticleEmitterComponent()
{
	SafeDelete(m_ParticlesArray);

	SafeRelease(m_pVertexBuffer);
}

void ParticleEmitterComponent::Initialize(const SceneContext& sceneContext)
{
	if (m_pParticleMaterial == nullptr)
		m_pParticleMaterial = MaterialManager::Get()->CreateMaterial<ParticleMaterial>();

	CreateVertexBuffer(sceneContext);

	m_pParticleTexture = ContentManager::Load<TextureData>(L"Textures/Smoke.png");
}

void ParticleEmitterComponent::CreateVertexBuffer(const SceneContext& sceneContext)
{
	SafeRelease(m_pVertexBuffer);

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = m_ParticleCount * sizeof(VertexParticle);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;

	HANDLE_ERROR(sceneContext.d3dContext.pDevice->CreateBuffer(&bufferDesc, nullptr, &m_pVertexBuffer));
}

void ParticleEmitterComponent::Update(const SceneContext& sceneContext)
{
	float particleInterval = (m_EmitterSettings.maxEnergy - m_EmitterSettings.minEnergy) * m_MaxParticles;

	auto elapsed = sceneContext.pGameTime->GetElapsed();
	m_LastParticleSpawn += elapsed;

	// validate particles
	m_ActiveParticles = 0;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	sceneContext.d3dContext.pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(&static_cast<VertexParticle*>(mappedResource.pData)[0], m_ParticlesArray, sizeof(VertexParticle) * m_ParticleCount);

	VertexParticle* pBuffer = (VertexParticle*)mappedResource.pData;
	for (UINT i = 0; i < m_ParticleCount; ++i)
	{
		Particle& particle = m_ParticlesArray[i];
		bool spawnedOrActive = false;
		if (particle.isActive)
		{
			UpdateParticle(particle, elapsed);
			spawnedOrActive = true;
		}
		else
		{
			if (m_LastParticleSpawn >= particleInterval)
			{
				SpawnParticle(particle);
				spawnedOrActive = true;
			}
		}

		if (spawnedOrActive)
		{
			pBuffer[m_ActiveParticles] = particle.vertexInfo;
			++m_ActiveParticles;
		}
	}

	sceneContext.d3dContext.pDeviceContext->Unmap(m_pVertexBuffer, 0);

}

void ParticleEmitterComponent::UpdateParticle(Particle& p, float elapsedTime) const
{
	if (!p.isActive)
		return;

	p.currentEnergy -= elapsedTime;
	if (p.currentEnergy < 0)
	{
		p.isActive = false;
		return;
	}

	// update pos
	XMVECTOR pos = XMLoadFloat3(&p.vertexInfo.Position);
	pos += XMLoadFloat3(&m_EmitterSettings.velocity) * elapsedTime;
	XMStoreFloat3(&p.vertexInfo.Position, pos);

	float lifePercent = p.currentEnergy / p.totalEnergy;
	// update color
	p.vertexInfo.Color = m_EmitterSettings.color;
	p.vertexInfo.Color.w *= lifePercent;

	// update size
	p.vertexInfo.Size = (p.initialSize - p.sizeChange) * lifePercent + p.sizeChange;

}

void ParticleEmitterComponent::SpawnParticle(Particle& p)
{
	p.isActive = true;
	p.totalEnergy = MathHelper::randF(m_EmitterSettings.minEnergy, m_EmitterSettings.maxEnergy);
	p.currentEnergy = p.totalEnergy;

	// pos init
	XMFLOAT3 unitVector = { 1,0,0 };
	auto randomMatrix = XMMatrixRotationRollPitchYaw(MathHelper::randF(-XM_PI, XM_PI), MathHelper::randF(-XM_PI, XM_PI), MathHelper::randF(-XM_PI, XM_PI));
	auto unit = XMVector3TransformNormal(XMLoadFloat3(&unitVector), randomMatrix);

	unit *= MathHelper::randF(m_EmitterSettings.minEmitterRadius, m_EmitterSettings.maxEmitterRadius);
	XMStoreFloat3(&p.vertexInfo.Position, unit);

	// size init
	p.initialSize = MathHelper::randF(m_EmitterSettings.minSize, m_EmitterSettings.maxSize);
	p.vertexInfo.Size = p.initialSize;
	p.sizeChange = MathHelper::randF(m_EmitterSettings.minScale, m_EmitterSettings.maxScale);

	// rot init
	p.vertexInfo.Rotation = MathHelper::randF(-XM_PI, XM_PI);

	// color init
	p.vertexInfo.Color = m_EmitterSettings.color;
}

void ParticleEmitterComponent::PostDraw(const SceneContext& sceneContext)
{
	m_pParticleMaterial->SetVariable_Matrix(L"gWorldViewProj", sceneContext.pCamera->GetViewProjection());
	m_pParticleMaterial->SetVariable_Matrix(L"gViewInverse", sceneContext.pCamera->GetViewInverse());
	m_pParticleMaterial->SetVariable_Texture(L"gParticleTexture", m_pParticleTexture);


	auto techniqueContext = m_pParticleMaterial->GetTechniqueContext();

	auto pDeviceContext = sceneContext.d3dContext.pDeviceContext;
	//Set Inputlayout
	pDeviceContext->IASetInputLayout(techniqueContext.pInputLayout);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	//Set Vertex Buffer
	const UINT offset = 0, stride = sizeof(VertexParticle);
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride,
		&offset);

	//DRAW
	D3DX11_TECHNIQUE_DESC techDesc{};
	techniqueContext.pTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		techniqueContext.pTechnique->GetPassByIndex(p)->Apply(0, pDeviceContext);
		pDeviceContext->DrawIndexed(m_ParticleCount, 0, 0);
	}	
}

void ParticleEmitterComponent::DrawImGui()
{
	if(ImGui::CollapsingHeader("Particle System"))
	{
		ImGui::SliderUInt("Count", &m_ParticleCount, 0, m_MaxParticles);
		ImGui::InputFloatRange("Energy Bounds", &m_EmitterSettings.minEnergy, &m_EmitterSettings.maxEnergy);
		ImGui::InputFloatRange("Size Bounds", &m_EmitterSettings.minSize, &m_EmitterSettings.maxSize);
		ImGui::InputFloatRange("Scale Bounds", &m_EmitterSettings.minScale, &m_EmitterSettings.maxScale);
		ImGui::InputFloatRange("Radius Bounds", &m_EmitterSettings.minEmitterRadius, &m_EmitterSettings.maxEmitterRadius);
		ImGui::InputFloat3("Velocity", &m_EmitterSettings.velocity.x);
		ImGui::ColorEdit4("Color", &m_EmitterSettings.color.x, ImGuiColorEditFlags_NoInputs);
	}
}