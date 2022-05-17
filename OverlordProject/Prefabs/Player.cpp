#include "stdafx.h"
#include "Player.h"

#include "Bomb.h"
#include "Materials/DiffuseMaterial_Skinned.h"

Player::Player()
{
	const auto pPhysxMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.0f);

	{
		m_pRigid = AddComponent(new RigidBodyComponent());
		m_pRigid->SetConstraint(RigidBodyConstraint::TransY, false);
		m_pRigid->SetConstraint(RigidBodyConstraint::AllRot, false);
		m_pRigid->AddCollider(PxCapsuleGeometry(1, 0.25), *pPhysxMaterial, false, { 0,1.25,0 });

		m_pRigid->SetCollisionGroup(CollisionGroup::Group0);
		m_pRigid->SetCollisionIgnoreGroups(CollisionGroup::Group1);

		GetTransform()->Scale(0.02f);
		GetTransform()->Translate(-20, 0, -15);
	}

}

Player::~Player()
{

}

void Player::Initialize(const SceneContext& sceneContext)
{
	m_pBomb = GetScene()->AddChild(new Bomb());


	const auto pPlayerMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Skinned>();
	pPlayerMaterial->SetDiffuseTexture(L"Textures/Project/Player_diffuse.png");

	const auto pModel = AddComponent(new ModelComponent(L"Meshes/Project/Player.ovm"));
	pModel->SetMaterial(pPlayerMaterial);
	m_pAnimator = pModel->GetAnimator();
	m_pAnimator->Play();

	//Input
	auto inputAction = InputAction(MoveLeft, InputState::down, VK_LEFT);
	sceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(MoveRight, InputState::down, VK_RIGHT);
	sceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(MoveForward, InputState::down, VK_UP);
	sceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(MoveBackward, InputState::down, VK_DOWN);
	sceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(DropBomb, InputState::released, VK_SPACE);
	sceneContext.pInput->AddInputAction(inputAction);
}

void Player::Draw(const SceneContext&)
{
}

void Player::Update(const SceneContext& sceneContext)
{

	//## Input Gathering (move)
	XMFLOAT3 move = XMFLOAT3(0, 0, 0);
	float rot{};
	bool isMoving = false;
	if (sceneContext.pInput->IsActionTriggered(MoveForward))
	{
		move.z += m_MoveSpeed * 10;
		rot = 180.f;
		isMoving = true;
	}
	if (sceneContext.pInput->IsActionTriggered(MoveBackward))
	{
		move.z -= m_MoveSpeed * 10;
		rot = 0.f;
		isMoving = true;
	}
	if (sceneContext.pInput->IsActionTriggered(MoveRight))
	{
		move.x += m_MoveSpeed * 10;
		rot = 270.f;
		isMoving = true;
	}
	if (sceneContext.pInput->IsActionTriggered(MoveLeft))
	{
		move.x -= m_MoveSpeed * 10;
		rot = 90.f;
		isMoving = true;
	}

	if (isMoving)
	{
		m_pRigid->SetDensity(0.2f);
		m_pRigid->AddForce(move, PxForceMode::eIMPULSE);
		GetTransform()->Rotate(0, rot, 0);
	}
	else
	{
		GetTransform()->Translate(GetTransform()->GetPosition());
	}

	// bomb
	if (sceneContext.pInput->IsActionTriggered(DropBomb))
	{
		// we cannot make
		if (!m_pBomb->IsActive())
			m_pBomb->Activate(GetTransform()->GetPosition());
	}
}
