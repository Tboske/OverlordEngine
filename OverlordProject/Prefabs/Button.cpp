#include "stdafx.h"
#include "Button.h"

#include "CubePrefab.h"

Button::Button(float x, float y, const std::wstring& text, DirectX::XMFLOAT4 color, float layer, float width,
			   float height)
	: m_Text(text)
	, m_Position(x + 640 - width / 2, 360 - (y * 14) - height / 2)
{
	AddChild(new CubePrefab(width, height, 0.1f, color));
	GetTransform()->Translate(x, y, -layer);

	const auto pPhysxMaterial = PxGetPhysics().createMaterial(0.f, 0.f, 0.0f);

	auto pRigid = AddComponent(new RigidBodyComponent(true));
	pRigid->AddCollider(PxBoxGeometry(width / 2, height / 2, 0.1f), *pPhysxMaterial);
	pRigid->SetCollisionGroup(CollisionGroup::Group5);
	
}

void Button::Initialize(const SceneContext&)
{
	m_pFont = ContentManager::Load<SpriteFont>(L"SpriteFonts/Arial_32.fnt");
}

void Button::Draw(const SceneContext&)
{
	if (m_pFont->GetName() == L"")
		return;

	TextRenderer::Get()->DrawText(
		m_pFont,
		m_Text,
		m_Position,
		(XMFLOAT4)Colors::Green
	);	
}

