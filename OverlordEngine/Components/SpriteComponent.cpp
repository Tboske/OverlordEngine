#include "stdafx.h"
#include "SpriteComponent.h"

SpriteComponent::SpriteComponent(const std::wstring& spriteAsset, const XMFLOAT2& pivot, const XMFLOAT4& color):
	m_SpriteAsset(spriteAsset),
	m_Pivot(pivot),
	m_Color(color)
{}

void SpriteComponent::Initialize(const SceneContext& /*sceneContext*/)
{
	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteComponent::SetTexture(const std::wstring& spriteAsset)
{
	m_SpriteAsset = spriteAsset;
	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteComponent::Draw(const SceneContext& )
{
	if (!m_pTexture)
		return;

		//Here you need to draw the SpriteComponent using the Draw of the sprite renderer
		// The sprite renderer is a singleton
		// you will need to position (X&Y should be in screenspace, Z contains the depth between [0,1]), the rotation and the scale from the owning GameObject
		// You can use the MathHelper::QuaternionToEuler function to help you with the z rotation
	auto* pTrans = m_pGameObject->GetComponent<TransformComponent>();

	const XMFLOAT2 pos{ pTrans->GetPosition().x , pTrans->GetPosition().y };
	auto rot = MathHelper::QuaternionToEuler(pTrans->GetRotation()).z;

	SpriteRenderer::Get()->AppendSprite(m_pTexture, pos, m_Color, m_Pivot, {1.f, 1.f}, rot);
}
