#include "stdafx.h"
#include "PongScene.h"


#include "Prefabs/CubePrefab.h"
#include "Prefabs/SpherePrefab.h"

PongScene::~PongScene()
{
}

void PongScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;

	DebugRenderer::ToggleDebugRenderer();

	PxPhysics& physics = PxGetPhysics();

	PxMaterial* pDefaultMat = physics.createMaterial(0.f, 0.f, 1.f);

	// Ball
	m_pSphere = new SpherePrefab(1.f, 20, { 1.f, 1.f, 1.f, 1.f });
	m_pSphere->GetTransform()->Translate(0.f, 1.f, 0.f);
	{
		m_pSphereRigid = m_pSphere->AddComponent(new RigidBodyComponent());
		m_pSphereRigid->SetConstraint(RigidBodyConstraint::TransY, true);
		m_pSphereRigid->SetDensity(0.f);
		m_pSphereRigid->AddCollider(PxSphereGeometry(0.5f), *pDefaultMat);
	}
	AddChild(m_pSphere);
	ResetBall();

	// players
	{
		const PxVec3 playerDim = PxVec3(2.f, 2.f, 5.f);
		const PxBoxGeometry playerGeom(playerDim / 2);

		// player 1
		m_pPlayer1 = new CubePrefab(playerDim.x, playerDim.y, playerDim.z, { 0.f, 0.f, 1.f, 1.f });
		m_pPlayer1->GetTransform()->Translate(-15.f, 1.1f, 0.f);
		{
			RigidBodyComponent* pPlayer1Rigid = m_pPlayer1->AddComponent(new RigidBodyComponent());
			pPlayer1Rigid->SetKinematic(true);
			pPlayer1Rigid->AddCollider(playerGeom, *pDefaultMat);
		}
		AddChild(m_pPlayer1);

		// player 2
		m_pPlayer2 = new CubePrefab(playerDim.x, playerDim.y, playerDim.z, { 1.f, 0.f, 0.f, 1.f });
		m_pPlayer2->GetTransform()->Translate(15.f, 1.1f, 0.f);
		{
			RigidBodyComponent* pPlayer2Rigid = m_pPlayer2->AddComponent(new RigidBodyComponent());
			pPlayer2Rigid->SetKinematic(true);
			pPlayer2Rigid->AddCollider(playerGeom, *pDefaultMat);
		}
		AddChild(m_pPlayer2);
	}

	// static objects
	{
		// ground
		const PxVec3 groundDim = PxVec3(50.f, 2.f, 30.f);
		const PxBoxGeometry groundGeom(groundDim / 2);

		GameObject* pGround = new CubePrefab(groundDim.x, groundDim.y, groundDim.z, { 0.f, 0.f, 0.f, 1.f });
		pGround->GetTransform()->Translate(0.f, -1.f, 0.f);
		auto pGroundRigid = pGround->AddComponent(new RigidBodyComponent(true));
		pGroundRigid->AddCollider(groundGeom, *pDefaultMat);
		AddChild(pGround);

		// walls
		const PxVec3 wallDim = PxVec3(50.f, 2.f, 2.f);
		const PxBoxGeometry wallGeom(wallDim / 2);

		// top wall
		GameObject* pWall = new CubePrefab(wallDim.x, wallDim.y, wallDim.z, { 1.f, 1.f, 1.f, 1.f });
		pWall->GetTransform()->Translate(0.f, 0.5f, 10.f);
		auto pWallRigid = pWall->AddComponent(new RigidBodyComponent(true));
		pWallRigid->AddCollider(wallGeom, *pDefaultMat);
		AddChild(pWall);

		// bottom wall
		pWall = new CubePrefab(wallDim.x, wallDim.y, wallDim.z, { 1.f, 1.f, 1.f, 1.f });
		pWall->GetTransform()->Translate(0.f, 0.5f, -10.f);
		pWallRigid = pWall->AddComponent(new RigidBodyComponent(true));
		pWallRigid->AddCollider(wallGeom, *pDefaultMat);
		AddChild(pWall);
	}

	// camera
	FixedCamera* pCamera = new FixedCamera();
	pCamera->GetTransform()->Translate(0.f, 26.f, 0.f);
	pCamera->GetTransform()->Rotate(90.f, 0.f, 0.f);
	AddChild(pCamera);
	SetActiveCamera(pCamera->GetComponent<CameraComponent>());

	// input
	m_SceneContext.pInput->AddInputAction(InputAction(0, InputState::down, 'W'));
	m_SceneContext.pInput->AddInputAction(InputAction(1, InputState::down, 'S'));
	m_SceneContext.pInput->AddInputAction(InputAction(2, InputState::down, VK_UP));
	m_SceneContext.pInput->AddInputAction(InputAction(3, InputState::down, VK_DOWN));

}

void PongScene::Update()
{

	TransformComponent* pTransform = m_pPlayer1->GetTransform();
	if (m_SceneContext.pInput->IsActionTriggered(0))
		MovePlayer(pTransform, m_MoveSpeed);
	if (m_SceneContext.pInput->IsActionTriggered(1))
		MovePlayer(pTransform, -m_MoveSpeed);

	pTransform = m_pPlayer2->GetTransform();
	if (m_SceneContext.pInput->IsActionTriggered(2))
		MovePlayer(pTransform, m_MoveSpeed);
	if (m_SceneContext.pInput->IsActionTriggered(3))
		MovePlayer(pTransform, -m_MoveSpeed);
	
	//reset if it passed a player
	if (m_pSphereRigid->GetTransform()->GetPosition().x > m_ResetDistance || m_pSphereRigid->GetTransform()->GetPosition().x < -m_ResetDistance)
		 ResetBall();
}

void PongScene::OnGUI()
{
	ImGui::TextColored({ 0.f, 0.f, 1.f, 1.f }, "Input: W / S");
	ImGui::Spacing();
	ImGui::TextColored({ 1.f, 0.f, 0.f, 1.f }, "Input: UP / DOWN");
}

void PongScene::OnSceneActivated()
{
	if (DebugRenderer::IsEnabled())
		DebugRenderer::ToggleDebugRenderer();
}

void PongScene::OnSceneDeactivated()
{
	DebugRenderer::ToggleDebugRenderer();
}

void PongScene::MovePlayer(TransformComponent* pTransform, float speed) const
{
	XMFLOAT3 position = pTransform->GetPosition();
	position.z += speed;
	position.z = PxClamp(position.z, -m_Constraint, m_Constraint);

	pTransform->Translate(position);
}

void PongScene::ResetBall() const
{
	m_pSphereRigid->GetTransform()->Translate(0.f, 0.f, 0.f);
	m_pSphereRigid->AddForce(
		{
		float(((rand() % 2) * 2) - 1) * MathHelper::randF(15.f, 25.f),
		0.f,
		MathHelper::randF(-20.f, 20.f)
		},
		PxForceMode::eIMPULSE
	);
}
