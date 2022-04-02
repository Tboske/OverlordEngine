#include "stdafx.h"
#include "SoftwareSkinningScene_1.h"

#include "BoneObject.h"
#include "Materials/ColorMaterial.h"

SoftwareSkinningScene_1::SoftwareSkinningScene_1()
	: GameScene(L"SoftwareSkinningScene_1")
{
	m_SceneContext.settings.enableOnGUI = true;	
}

SoftwareSkinningScene_1::~SoftwareSkinningScene_1()
{
	
}

void SoftwareSkinningScene_1::Initialize()
{
	auto* pColor = MaterialManager::Get()->CreateMaterial<ColorMaterial>();

	GameObject* pRoot = AddChild(new GameObject());

	m_pBone0 = new BoneObject(pColor, 15.f);
	pRoot->AddChild(m_pBone0);

	m_pBone1 = new BoneObject(pColor, 15.f);
	m_pBone0->AddBone(m_pBone1);
}

void SoftwareSkinningScene_1::Update()
{
	if (m_AutoRotate)
	{
		static float dTime = 0;
		dTime += GetSceneContext().pGameTime->GetElapsed();
		m_BoneRotation = sin(dTime) * 45;
	}

	m_pBone0->GetTransform()->Rotate(0, 0, m_BoneRotation);
	m_pBone1->GetTransform()->Rotate(0, 0, -m_BoneRotation * 2.f);
}

void SoftwareSkinningScene_1::OnGUI()
{
	ImGui::DragFloat("bone rotation", &m_BoneRotation, 0.2f, -45.f, 45.f);
	ImGui::Checkbox("Auto Rotate", &m_AutoRotate);
}
	