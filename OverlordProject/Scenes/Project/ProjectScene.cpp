#include "stdafx.h"
#include "ProjectScene.h"


#include "Prefabs/Player.h"

#include "Materials/DiffuseMaterial.h"
#include "Materials/UberMaterial.h"
#include "Materials/Project/BoxMaterial.h"

ProjectScene::~ProjectScene()
{
	
}


void ProjectScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.drawUserDebug = true;
#ifndef _DEBUG
	if (DebugRenderer::IsEnabled())
		DebugRenderer::ToggleDebugRenderer();
#endif


	const auto pPhysxMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.0f);


	m_pCamera = AddChild(new FixedCamera());
	m_pCamera->GetTransform()->Translate(0, 45, 0);
	m_pCamera->GetTransform()->Rotate(90, 0, 0);
	SetActiveCamera(m_pCamera->GetComponent<CameraComponent>());

	InitArena(pPhysxMaterial);

#pragma region boxes
	for (int y = -6; y <= 6; ++y)
	{
		for (int x = -8; x <= 8; ++x)
		{
			if (x <= -7 && y <= -5)
				continue;
			if (x >= 7  && y >= 5 )
				continue;
			if (x <= -7 && y >= 5 )
				continue;
			if (x >= 7  && y <= -5)
				continue;

			InitBox(pPhysxMaterial, float(x) * 2.5f, float(y) * 2.5f);

			if (y % 2 != 0)
				++x;
		}
	}

#pragma endregion

	// some ugly hardcoded shenanigans
	for (float x = -17.5f; x <= 17.5f; x += 5.f)
		for (float y = -12.5; y <= 12.5f; y += 5.f)
			InitBlock(pPhysxMaterial, x, y);


	m_pPlayer1 = AddChild(new Player(0, -20, -15));
	m_pPlayer1->AddInput(m_SceneContext, Player::MoveForward	, 'W');
	m_pPlayer1->AddInput(m_SceneContext, Player::MoveBackward	, 'S');
	m_pPlayer1->AddInput(m_SceneContext, Player::MoveLeft		, 'A');
	m_pPlayer1->AddInput(m_SceneContext, Player::MoveRight		, 'D');
	m_pPlayer1->AddInput(m_SceneContext, Player::DropBomb		, VK_SPACE);

	m_pPlayer2 = AddChild(new Player(1, 20, 15));
	m_pPlayer2->AddInput(m_SceneContext, Player::MoveForward	, VK_UP);
	m_pPlayer2->AddInput(m_SceneContext, Player::MoveBackward	, VK_DOWN);
	m_pPlayer2->AddInput(m_SceneContext, Player::MoveLeft		, VK_LEFT);
	m_pPlayer2->AddInput(m_SceneContext, Player::MoveRight		, VK_RIGHT);
	m_pPlayer2->AddInput(m_SceneContext, Player::DropBomb		, VK_NUMPAD0);
}

void ProjectScene::Update()
{
	if (m_SceneContext.pInput->IsKeyboardKey(InputState::released, VK_ESCAPE))
		SceneManager::Get()->SetActiveGameScene(L"InGameMenu");
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

	m_pArena = AddChild(new GameObject());

	auto pFloor = m_pArena->AddChild(new GameObject());
	{
		auto* pModelComp = pFloor->AddComponent(new ModelComponent(L"Meshes/Project/Floor.ovm"));
		pModelComp->SetMaterial(m_pFloorMaterial);

		auto* pRigid = pFloor->AddComponent(new RigidBodyComponent(true));
		pRigid->SetCollisionGroup(CollisionGroup::Group9);
		auto* pConvex = ContentManager::Load<PxConvexMesh>(L"Meshes/Project/Floor.ovpc");
		pRigid->AddCollider(PxConvexMeshGeometry(pConvex), *physxMat);

		pFloor->GetTransform()->Scale(10, 1, 10);
	}

#pragma region Borders
	auto pBorderMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pBorderMat->SetDiffuseTexture(L"Textures/Project/Rock1.jpg");

	GameObject* pBorderLeft = m_pArena->AddChild(new GameObject());
	{
		auto* pModelComp = pBorderLeft->AddComponent(new ModelComponent(L"Meshes/Project/BorderVertical.ovm"));
		pModelComp->SetMaterial(pBorderMat);

		auto* pRigid = pBorderLeft->AddComponent(new RigidBodyComponent(true));
		auto* pConvex = ContentManager::Load<PxConvexMesh>(L"Meshes/Project/BorderVertical.ovpc");
		pRigid->AddCollider(PxConvexMeshGeometry(pConvex), *physxMat);
	}

	GameObject* pBorderRight = m_pArena->AddChild(new GameObject());
	{
		auto* pModelComp = pBorderRight->AddComponent(new ModelComponent(L"Meshes/Project/BorderVertical.ovm"));
		pModelComp->SetMaterial(pBorderMat);

		auto* pRigid = pBorderRight->AddComponent(new RigidBodyComponent(true));
		auto* pConvex = ContentManager::Load<PxConvexMesh>(L"Meshes/Project/BorderVertical.ovpc");
		pRigid->AddCollider(PxConvexMeshGeometry(pConvex), *physxMat);

		pBorderRight->GetTransform()->Rotate(0, 180, 0);
	}

	GameObject* pBorderTop = m_pArena->AddChild(new GameObject());
	{
		auto* pModelComp = pBorderTop->AddComponent(new ModelComponent(L"Meshes/Project/BorderHorizontal.ovm"));
		pModelComp->SetMaterial(pBorderMat);

		auto* pRigid = pBorderTop->AddComponent(new RigidBodyComponent(true));
		auto* pConvex = ContentManager::Load<PxConvexMesh>(L"Meshes/Project/BorderHorizontal.ovpc");
		pRigid->AddCollider(PxConvexMeshGeometry(pConvex), *physxMat);
	}

	GameObject* pBorderBottom = m_pArena->AddChild(new GameObject());
	{
		auto* pModelComp = pBorderBottom->AddComponent(new ModelComponent(L"Meshes/Project/BorderHorizontal.ovm"));
		pModelComp->SetMaterial(pBorderMat);

		auto* pRigid = pBorderBottom->AddComponent(new RigidBodyComponent(true));
		auto* pConvex = ContentManager::Load<PxConvexMesh>(L"Meshes/Project/BorderHorizontal.ovpc");
		pRigid->AddCollider(PxConvexMeshGeometry(pConvex), *physxMat);
		
		pBorderBottom->GetTransform()->Rotate(0, 180, 0);
	}
#pragma endregion


#pragma region Rocks
	auto pRockMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pRockMat->SetDiffuseTexture(L"Textures/Project/Rock1.jpg");

	// bigger rocks
	InitRock(pRockMat, -31.f, -10.f, 15.f);
	InitRock(pRockMat, -28.f,  10.f, 14.f, 90);
	InitRock(pRockMat, -30.f,  1.f , 12.f, 90);
	InitRock(pRockMat,  30.f, -10.f, 15.f, 40);
	InitRock(pRockMat,  27.f,  -2.f, 11.f, 260);
	InitRock(pRockMat,  32.f,  10.f, 15.f, 170);

	// smaller rocks
	InitRock(pRockMat, 28.f, 19.f, 7.f, 260);
	InitRock(pRockMat, 25.f, 16.f, 5.f, 140);
	InitRock(pRockMat, 25.f, 11.f, 7.f, 160);
	InitRock(pRockMat, 26.f, 5.5f, 8.f, 80);
	InitRock(pRockMat, 24.5f,  2.f, 5.f, 40);
	InitRock(pRockMat, 25.f, -5.f, 7.f, 220);
	InitRock(pRockMat, 25.5f, -14.5f, 7.f, 20);
	
	InitRock(pRockMat, -25.f, 16.f, 5.f, 140);
	InitRock(pRockMat, -25.f, 11.f, 7.f, 160);
	InitRock(pRockMat, -26.f, 5.5f, 8.f, 80);
	InitRock(pRockMat, -24.f, 2.f, 5.f, 40);
	InitRock(pRockMat, -24.f, 0.f, 4.f, 10);
	InitRock(pRockMat, -25.5f, -3.f, 7.f, 220);
	InitRock(pRockMat, -26.f, -6.5f, 8.f, 110);
	InitRock(pRockMat, -27.5f, -9.f, 10.f, 210);
	InitRock(pRockMat, -26.5f, -14.f, 7.f, 260);
	InitRock(pRockMat, -24.5f, -19.f, 4.f, 60);
#pragma endregion
}

void ProjectScene::InitBox(PxMaterial* physxMat, float x, float z)
{
	m_pBoxMaterial = MaterialManager::Get()->CreateMaterial<BoxMaterial>();
	m_pBoxMaterial->SetDiffuseTexture(L"Textures/Project/Box_Diffuse.jpeg");
	m_pBoxMaterial->SetNormalMap(L"Textures/Project/Box_Normal.png");

	auto pBox = m_pArena->AddChild(new GameObject());
	{
		auto* pModelComp = pBox->AddComponent(new ModelComponent(L"Meshes/Project/Crate.ovm"));
		pModelComp->SetMaterial(m_pBoxMaterial);

		auto* pRigid = pBox->AddComponent(new RigidBodyComponent(true));
		auto* pConvex = ContentManager::Load<PxConvexMesh>(L"Meshes/Project/Crate.ovpc");
		pRigid->AddCollider(PxConvexMeshGeometry(pConvex, PxMeshScale(1.2f)), *physxMat);

		pBox->SetTag(L"Box");
		pBox->GetTransform()->Translate(x, 1.25, z);
	}
}

void ProjectScene::InitBlock(PxMaterial* physxMat, float x, float z)
{
	m_pBlockMaterial = MaterialManager::Get()->CreateMaterial<BoxMaterial>();
	m_pBlockMaterial->SetColorAdd(0.15f, 0.13f, 0.1f);
	m_pBlockMaterial->SetVariable_Scalar(L"gUseColor", true);
	m_pBlockMaterial->SetNormalMap(L"Textures/Project/Box_Normal.png");

	const auto pBlock = m_pArena->AddChild(new GameObject());
	{
		auto* pModelComp = pBlock->AddComponent(new ModelComponent(L"Meshes/Project/Crate.ovm"));
		pModelComp->SetMaterial(m_pBlockMaterial);
		auto* pRigid = pBlock->AddComponent(new RigidBodyComponent(true));
		auto* pConvex = ContentManager::Load<PxConvexMesh>(L"Meshes/Project/Crate.ovpc");
		pRigid->AddCollider(PxConvexMeshGeometry(pConvex), *physxMat);
		
		pBlock->GetTransform()->Translate(x, 1.25, z);
		pBlock->GetTransform()->Scale(1.2f, 1.2f, 1.2f);
	}
}

void ProjectScene::InitRock(DiffuseMaterial* pRockMat, float x, float z, float scale, float rot)
{
	auto pRock = m_pArena->AddChild(new GameObject());
	{
		auto* pModel = pRock->AddComponent(new ModelComponent(L"Meshes/Project/Rock.ovm"));
		pModel->SetMaterial(pRockMat);

		pRock->GetTransform()->Translate(x, scale/2, z);
		pRock->GetTransform()->Scale(scale);
		pRock->GetTransform()->Rotate(0, rot, 0);
	}
}

