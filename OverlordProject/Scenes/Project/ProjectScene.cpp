#include "stdafx.h"
#include "ProjectScene.h"

#include "Prefabs/Player.h"
#include "Materials/DiffuseMaterial.h"
#include "Materials/DiffuseMaterial_Skinned.h"
#include "Materials/UberMaterial.h"
#include "Materials/Project/BoxMaterial.h"
#include "Prefabs/Character.h"

ProjectScene::~ProjectScene()
{
	
}


void ProjectScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;
#ifndef _DEBUG
	if (DebugRenderer::IsEnabled())
		DebugRenderer::ToggleDebugRenderer();
#endif


	const auto pPhysxMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.0f);

	InitArena(pPhysxMaterial);
	InitBox(pPhysxMaterial, 0, 0);
	InitBox(pPhysxMaterial, -2.5, 0);
	InitBox(pPhysxMaterial, 0, -2.5);
	InitBox(pPhysxMaterial, -7.5, -2.5);

	m_pPlayer = AddChild(new Player());
}

void ProjectScene::Update()
{

}

void ProjectScene::Draw()
{
	
}

void ProjectScene::OnGUI()
{
	m_pFloorMaterial->DrawImGui();
	m_pBoxMaterial->DrawImGui();
	m_pBlockMaterial->DrawImGui();
}

void ProjectScene::InitArena(PxMaterial* physxMat)
{
	m_pFloorMaterial = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	m_pFloorMaterial->SetDiffuseTexture(L"Textures/Project/FloorTile6_diffuse.png");
	m_pFloorMaterial->SetNormalTexture(L"Textures/Project/FloorTileClean_normal.png");
	m_pFloorMaterial->SetSpecularTexture(L"Textures/Project/FloorTileClean_spec.png");
	m_pFloorMaterial->SetVariable_Scalar(L"gUseEnvironmentMapping", false);
	m_pFloorMaterial->SetVariable_Scalar(L"gUseSpecularPhong", false);
	m_pFloorMaterial->SetVariable_Scalar(L"gUseSpecularBlinn", false);
	m_pFloorMaterial->SetVariable_Scalar(L"gFlipGreenChannel", true);

	m_pFloor = AddChild(new GameObject());
	{
		auto* pModelComp = m_pFloor->AddComponent(new ModelComponent(L"Meshes/Project/Floor.ovm"));
		pModelComp->SetMaterial(m_pFloorMaterial);

		auto* pRigid = m_pFloor->AddComponent(new RigidBodyComponent(true));
		pRigid->SetCollisionGroup(CollisionGroup::Group9);
		auto* pConvex = ContentManager::Load<PxConvexMesh>(L"Meshes/Project/Floor.ovpc");
		pRigid->AddCollider(PxConvexMeshGeometry(pConvex, PxMeshScale({10, 1, 10})), *physxMat);

		//m_pFloor->GetTransform()->Scale(10, 1, 10);
	}

	auto pBorderMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pBorderMat->SetDiffuseTexture(L"Textures/Project/Box_Diffuse.jpeg");

#pragma region Borders
	GameObject* pBorderLeft = m_pFloor->AddChild(new GameObject());
	{
		auto* pModelComp = pBorderLeft->AddComponent(new ModelComponent(L"Meshes/Project/BorderVertical.ovm"));
		pModelComp->SetMaterial(pBorderMat);

		auto* pRigid = pBorderLeft->AddComponent(new RigidBodyComponent(true));
		auto* pConvex = ContentManager::Load<PxConvexMesh>(L"Meshes/Project/BorderVertical.ovpc");
		pRigid->AddCollider(PxConvexMeshGeometry(pConvex), *physxMat);
	}

	GameObject* pBorderRight = m_pFloor->AddChild(new GameObject());
	{
		auto* pModelComp = pBorderRight->AddComponent(new ModelComponent(L"Meshes/Project/BorderVertical.ovm"));
		pModelComp->SetMaterial(pBorderMat);

		auto* pRigid = pBorderRight->AddComponent(new RigidBodyComponent(true));
		auto* pConvex = ContentManager::Load<PxConvexMesh>(L"Meshes/Project/BorderVertical.ovpc");
		pRigid->AddCollider(PxConvexMeshGeometry(pConvex), *physxMat);

		pBorderRight->GetTransform()->Rotate(0, 180, 0);
	}

	GameObject* pBorderTop = m_pFloor->AddChild(new GameObject());
	{
		auto* pModelComp = pBorderTop->AddComponent(new ModelComponent(L"Meshes/Project/BorderHorizontal.ovm"));
		pModelComp->SetMaterial(pBorderMat);

		auto* pRigid = pBorderTop->AddComponent(new RigidBodyComponent(true));
		auto* pConvex = ContentManager::Load<PxConvexMesh>(L"Meshes/Project/BorderHorizontal.ovpc");
		pRigid->AddCollider(PxConvexMeshGeometry(pConvex), *physxMat);
	}

	GameObject* pBorderBottom = m_pFloor->AddChild(new GameObject());
	{
		auto* pModelComp = pBorderBottom->AddComponent(new ModelComponent(L"Meshes/Project/BorderHorizontal.ovm"));
		pModelComp->SetMaterial(pBorderMat);

		auto* pRigid = pBorderBottom->AddComponent(new RigidBodyComponent(true));
		auto* pConvex = ContentManager::Load<PxConvexMesh>(L"Meshes/Project/BorderHorizontal.ovpc");
		pRigid->AddCollider(PxConvexMeshGeometry(pConvex), *physxMat);
		
		pBorderBottom->GetTransform()->Rotate(0, 180, 0);
	}
#pragma endregion

	// some ugly hardcoded shenanigans
	for (float x = -17.5f; x <= 17.5f; x += 5.f)
		for (float y = -12.5; y <= 12.5f; y += 5.f)
			InitBlock(physxMat, x, y);
}

void ProjectScene::InitBox(PxMaterial* physxMat, float x, float z)
{
	m_pBoxMaterial = MaterialManager::Get()->CreateMaterial<BoxMaterial>();
	m_pBoxMaterial->SetDiffuseTexture(L"Textures/Project/Box_Diffuse.jpeg");
	m_pBoxMaterial->SetNormalMap(L"Textures/Project/Box_Normal.png");

	auto pBox = AddChild(new GameObject());
	{
		auto* pModelComp = pBox->AddComponent(new ModelComponent(L"Meshes/Project/Crate.ovm"));
		pModelComp->SetMaterial(m_pBoxMaterial);

		auto* pRigid = pBox->AddComponent(new RigidBodyComponent(true));
		auto* pConvex = ContentManager::Load<PxConvexMesh>(L"Meshes/Project/Crate.ovpc");
		pRigid->AddCollider(PxConvexMeshGeometry(pConvex), *physxMat);

		pBox->GetTransform()->Translate(x, 1.25, z);
	}
}

void ProjectScene::InitBlock(PxMaterial* physxMat, float x, float z)
{
	m_pBlockMaterial = MaterialManager::Get()->CreateMaterial<BoxMaterial>();
	m_pBlockMaterial->SetColorAdd(0.15f, 0.13f, 0.1f);
	m_pBlockMaterial->SetVariable_Scalar(L"gUseColor", true);
	m_pBlockMaterial->SetNormalMap(L"Textures/Project/Box_Normal.png");

	const auto pBlock = m_pFloor->AddChild(new GameObject());
	{
		auto* pModelComp = pBlock->AddComponent(new ModelComponent(L"Meshes/Project/Crate.ovm"));
		pModelComp->SetMaterial(m_pBlockMaterial);

		auto* pRigid = pBlock->AddComponent(new RigidBodyComponent(true));
		pRigid->SetCollisionGroup(CollisionGroup::Group9);
		auto* pConvex = ContentManager::Load<PxConvexMesh>(L"Meshes/Project/Crate.ovpc");
		pRigid->AddCollider(PxConvexMeshGeometry(pConvex), *physxMat);
		
		pBlock->GetTransform()->Translate(x, 1.25, z);
		pBlock->GetTransform()->Scale(1.2f, 1.2f, 1.2f);
	}
}