#include "stdafx.h"
#include "Button.h"

#include "CubePrefab.h"

#include "Text.h"

Button::Button(float x, float y, const std::wstring& text, XMFLOAT4 color, float layer, float width, float height)
{
	AddChild(new CubePrefab(width, height, 0.1f, color));
	GetTransform()->Translate(x, y, -layer);

	const auto pPhysxMaterial = PxGetPhysics().createMaterial(0.f, 0.f, 0.0f);

	auto pRigid = AddComponent(new RigidBodyComponent(true));
	pRigid->AddCollider(PxBoxGeometry(width / 2, height / 2, 0.1f), *pPhysxMaterial);
	pRigid->SetCollisionGroup(CollisionGroup::Group5);


	AddChild(new Text(
		{ x + 640 - width / 2, 360 - (y * 14) - height / 2 }	, 
		{ 0,0 }, 
		center, 
		text
	));
}
