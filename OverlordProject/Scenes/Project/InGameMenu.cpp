#include "stdafx.h"
#include "InGameMenu.h"

#include "Prefabs/Button.h"

void InGameMenu::Initialize()
{
	// Background
	AddChild(new Button(0, 0, L"", { 1, 1, 1, 1 }, 0, 200, 100));

	// Continue Button
	m_pContinue = AddChild(new Button(0, 12, L"Continue", { 0,1,1,1 }, 0.5f, 20, 5));

	// Restart Button
	m_pRestart = AddChild(new Button(0, 6, L"Restart", { 0,1,1,1 }, 0.5f, 20, 5));

	// Main menu Button
	m_pMainMenu = AddChild(new Button(0, 0, L"Main Menu", { 0,1,1,1 }, 0.5f, 18, 5));

	// Exit Button
	m_pExit = AddChild(new Button(0, -8, L"Exit", { 1, 0,0,1 }, 0.5f, 16, 5));


	// Camera
	auto* pCamera = AddChild(new FixedCamera());
	pCamera->GetTransform()->Translate(0, 0, -60);
	pCamera->GetTransform()->Rotate(0, 0, 0);
	m_pCameraComponent = pCamera->GetComponent<CameraComponent>();
	SetActiveCamera(m_pCameraComponent);
}

void InGameMenu::Update()
{
	const auto& gc = GetSceneContext();
	SceneManager* sm = SceneManager::Get();

	if (m_SceneContext.pInput->IsKeyboardKey(InputState::released, VK_ESCAPE))
		SceneManager::Get()->SetActiveGameScene(L"InGameMenu");

	if (!gc.pInput->IsMouseButton( InputState::released, VK_LBUTTON))
		return;

	GameObject* pClickedObject = m_pCameraComponent->Pick();

	if (pClickedObject == m_pContinue)
		sm->SetActiveGameScene(L"ProjectScene");		
	
	else if (pClickedObject == m_pRestart)
		sm->SetActiveGameScene(L"ProjectScene");

	else if (pClickedObject == m_pMainMenu)
		sm->SetActiveGameScene(L"MainMenu");

	else if (pClickedObject == m_pExit)
		PostQuitMessage(0);
}

void InGameMenu::Draw()
{

}
