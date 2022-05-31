#include "stdafx.h"
#include "Player.h"

#include "Score.h"
#include "Bomb.h"
#include "Materials/DiffuseMaterial_Skinned.h"

Player::Player(int player, float x, float z)
	: m_player( player )
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
		GetTransform()->Translate(x, 0, z);
		SetTag(L"Player");
	}


	switch (player)
	{
	case 0:
		m_Color = { 1.f,0.f,0.f };
		break;
	case 1:
		m_Color = { 0.f,1.f,0.f };
		break;
	case 2:
		m_Color = { 0.f,0.f,1.f };
		break;
	case 3:
		m_Color = { 1.f,1.f,0.f };
		break;
	}
}

Player::~Player()
{

}


// function for 2 ints to unique single
// https://stackoverflow.com/questions/919612/mapping-two-integers-to-one-in-a-unique-and-deterministic-way
int UniqueInputID(int player, Player::InputIds inputID)
{
	return (player * 100) + inputID;
}

void Player::AddInput(const SceneContext& sc, InputIds input, UINT virtualKey)
{

	sc.pInput->AddInputAction(InputAction( UniqueInputID(m_player, input), InputState::down, virtualKey));
}

void Player::Initialize(const SceneContext&)
{
	m_pBomb = GetScene()->AddChild(new Bomb(this));


	const auto pPlayerMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Skinned>();
	pPlayerMaterial->SetDiffuseTexture(L"Textures/Project/Player_diffuse.png");
	pPlayerMaterial->SetVariable_Scalar(L"gUseColor", true);
	pPlayerMaterial->SetVariable_Vector(L"gPlayerColor", m_Color);


	const auto pModel = AddComponent(new ModelComponent(L"Meshes/Project/Player.ovm"));
	pModel->SetMaterial(pPlayerMaterial);
	m_pAnimator = pModel->GetAnimator();
	m_pAnimator->Play();



	m_pScore = AddChild(new Score());
	m_pScore->SetColor(m_Color);
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
	
	if (sceneContext.pInput->IsActionTriggered(UniqueInputID(m_player, MoveForward)))
	{
		move.z += m_MoveSpeed * 10;
		rot = 180.f;
		isMoving = true;
	}
	if (sceneContext.pInput->IsActionTriggered(UniqueInputID(m_player, MoveBackward)))
	{
		move.z -= m_MoveSpeed * 10;
		rot = 0.f;
		isMoving = true;
	}
	if (sceneContext.pInput->IsActionTriggered(UniqueInputID(m_player, MoveRight)))
	{
		move.x += m_MoveSpeed * 10;
		rot = 270.f;
		isMoving = true;
	}
	if (sceneContext.pInput->IsActionTriggered(UniqueInputID(m_player, MoveLeft)))
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
	if (sceneContext.pInput->IsActionTriggered(UniqueInputID(m_player, DropBomb)))
	{
		// we cannot make
		if (!m_pBomb->IsActive())
			m_pBomb->Activate(GetTransform()->GetPosition());
	}
}
