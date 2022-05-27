#include "stdafx.h"
#include "MainMenu.h"

#include "ProjectScene.h"
#include "Prefabs/Button.h"


void MainMenu::Initialize()
{
	// background
	AddChild(new Button(0, 0, L"", { 1, 1, 1, 1 }, 0, 200, 100));

	// Play Button
	m_pPlay = new Button(0, 8, L"Start", { 0,1,1,1 }, 0.5f, 20, 5);
	AddChild(m_pPlay);

	// Exit Button
	m_pExit = new Button(0, -6, L"Exit", { 1, 0,0,1 }, 0.5f, 16, 5);
	AddChild(m_pExit);

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
