#include "stdafx.h"
#include "BombEffectComponent.h"
#include "Misc/ExplosionMaterial.h"

ExplosionMaterial* BombEffectComponent::m_pMaterial{};

BombEffectComponent::BombEffectComponent(const std::wstring& file)
	: m_AssetFile(file)
	, m_ElapsedVal(0)
{
	m_ExplosionSettings.duration = 0.2f;
	m_ExplosionSettings.intensity = 1.f;

	m_pExplosionArray.reserve(150);
}

BombEffectComponent::~BombEffectComponent()
{
	SafeRelease(m_pVertexBuffer);
}

void BombEffectComponent::ActivateEffect(const std::vector<ExplosionVertex>& positions)
{
	m_pExplosionArray = positions;

	UpdateBuffer();

	m_ElapsedVal = 0;
	m_pMaterial->SetVariable_Scalar(L"gSize", 0);

	m_IsActive = true;
}

void BombEffectComponent::Initialize(const SceneContext& sceneContext)
{
	if (m_pMaterial == nullptr)
		m_pMaterial = MaterialManager::Get()->CreateMaterial<ExplosionMaterial>();

	CreateVertexBuffer(sceneContext);

	m_pExplosionTexture = ContentManager::Load<TextureData>(m_AssetFile);
}

float ExplosionEquation(float elapsed, ExplosionSettings settings)
{
	// move from [0, pi] to [0, 1] and then to [0, duration]
	const float unit = elapsed * PxPi / settings.duration;

	return fabs(sinf(unit - PxPi) * settings.intensity);
}

void BombEffectComponent::Update(const SceneContext& sceneContext)
{
	if (!m_IsActive)
		return;

	auto elapsed = sceneContext.pGameTime->GetElapsed();
	m_ElapsedVal += elapsed;

	// set sin wave

	if (m_ElapsedVal >= m_ExplosionSettings.duration)
	{
		m_ElapsedVal = 0;
		m_IsActive = false;
	}
	
	m_pMaterial->SetVariable_Scalar(L"gSize"
		, ExplosionEquation(m_ElapsedVal, m_ExplosionSettings));
}

void BombEffectComponent::Draw(const SceneContext& sceneContext)
{
	m_pMaterial->SetVariable_Matrix(L"gWorldViewProj", sceneContext.pCamera->GetViewProjection());
	m_pMaterial->SetVariable_Texture(L"gTexture", m_pExplosionTexture);


	const auto techniqueContext = m_pMaterial->GetTechniqueContext();

	auto pDeviceContext = sceneContext.d3dContext.pDeviceContext;
	//Set Inputlayout
	pDeviceContext->IASetInputLayout(techniqueContext.pInputLayout);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	//Set Vertex Buffer
	constexpr UINT offset = 0;
	constexpr UINT stride = sizeof(ExplosionVertex);
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride,	&offset);

	//DRAW
	D3DX11_TECHNIQUE_DESC techDesc{};
	techniqueContext.pTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		techniqueContext.pTechnique->GetPassByIndex(p)->Apply(0, pDeviceContext);
		pDeviceContext->Draw((UINT)m_pExplosionArray.size(), 0);
	}
}

void BombEffectComponent::CreateVertexBuffer(const SceneContext& sceneContext)
{
	SafeRelease(m_pVertexBuffer);

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(ExplosionVertex) * (UINT)m_pExplosionArray.capacity();
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;

	HANDLE_ERROR(sceneContext.d3dContext.pDevice->CreateBuffer(&bufferDesc, nullptr, &m_pVertexBuffer));
}

void BombEffectComponent::UpdateBuffer() const
{
	const auto scene = m_pGameObject->GetScene();
	if (!scene)
	{
#if _DEBUG
		Logger::LogWarning(L"MeshDrawComponent::ActivateEffect > Can't update buffer, Component is not part of a scene. (= No DeviceContext)");
#endif
		return;
	}

	const auto d3d11 = scene->GetSceneContext().d3dContext;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	d3d11.pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource);
	memcpy(mappedResource.pData, m_pExplosionArray.data(), sizeof(ExplosionVertex) * m_pExplosionArray.capacity());
	d3d11.pDeviceContext->Unmap(m_pVertexBuffer, 0);

}
