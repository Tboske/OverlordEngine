#include "stdafx.h"
#include "MainMenu.h"

#include "ProjectScene.h"
#include "Prefabs/Button.h"
#include "Materials/DiffuseMaterial.h"
#include "Prefabs/CubePrefab.h"
#include "Prefabs/Text.h"


void MainMenu::Initialize()
{
	auto pBackgroundMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pBackgroundMat->SetDiffuseTexture(L"Textures/Project/Menu.png");

	// background
	auto pBackGround = AddChild(new GameObject());
	{
		auto* pModelComp = pBackGround->AddComponent(new ModelComponent(L"Meshes/Project/Floor.ovm"));
		pModelComp->SetMaterial(pBackgroundMat);

		pBackGround->GetTransform()->Translate(-30, -26, 0);
		pBackGround->GetTransform()->Rotate(-90, 90, 90);
		pBackGround->GetTransform()->Scale(100, 1, 58);
	}


	const auto pPhysxMaterial = PxGetPhysics().createMaterial(0.f, 0.f, 0.0f);

	// Play Button
	float x = -20, y = 15, w = 20, h = 5;
	auto pPlayButton = AddChild(new GameObject());
	{
		m_pPlay = pPlayButton->AddChild(new CubePrefab(w, h, 0.1f, { 0.8f,0,0,1 }));
		m_pPlay->GetTransform()->Translate(x, y, -0.5f);

		auto pRigid = m_pPlay->AddComponent(new RigidBodyComponent(true));
			pRigid->AddCollider(PxBoxGeometry(w / 2, h / 2, 0.1f), *pPhysxMaterial);
			pRigid->SetCollisionGroup(CollisionGroup::Group5);


		pPlayButton->AddChild(new Text(
			{ 300, 140 },
			{ 0,0 },
			center,
			L"Start"
		));
	}

	// Exit Button
	x = -19, y = 6, w = 16, h = 5;
	auto pExitButton = AddChild(new GameObject());
	{
		m_pExit = pExitButton->AddChild(new CubePrefab(w, h, 0.1f, { 0.8f,0,0,1 }));
		m_pExit->GetTransform()->Translate(x, y, -0.5f);

		auto pRigid = m_pExit->AddComponent(new RigidBodyComponent(true));
		pRigid->AddCollider(PxBoxGeometry(w / 2, h / 2, 0.1f), *pPhysxMaterial);
		pRigid->SetCollisionGroup(CollisionGroup::Group5);


		pExitButton->AddChild(new Text(
			{ 340, 275 },
			{ 0,0 },
			center,
			L"Exit"
		));
	}

	// Camera
	auto* pCamera = AddChild(new FixedCamera());
		pCamera->GetTransform()->Translate(0, 0, -60);
		pCamera->GetTransform()->Rotate(0, 0, 0);
	m_pCameraComponent = pCamera->GetComponent<CameraComponent>();
	SetActiveCamera(m_pCameraComponent);
}

void MainMenu::Update()
{
	const auto& sc = GetSceneContext();
	auto* sm = SceneManager::Get();


	if (!sc.pInput->IsMouseButton(InputState::released, VK_LBUTTON))
		return;

	GameObject* pClickedObject = m_pCameraComponent->Pick();

	if (pClickedObject == m_pPlay)
		sm->SetActiveGameScene(L"ProjectScene");
	

	else if (pClickedObject == m_pExit)
		PostQuitMessage(0);
}

void MainMenu::Draw()
{

}

void MainMenu::OnSceneActivated()
{
	SceneManager::Get()->RemoveGameScene(m_ProjectScene, true);

	m_ProjectScene = new ProjectScene();
	SceneManager::Get()->AddGameScene(m_ProjectScene);
}
