#include "stdafx.h"
#include "Player.h"

#include "Text.h"
#include "Bomb.h"
#include "Materials/DiffuseMaterial_Skinned.h"

Player::Player(int player, float x, float z)
	: m_Player( player )
{
	const auto pPhysxMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.0f);

	{
		m_pRigid = AddComponent(new RigidBodyComponent());
		m_pRigid->SetConstraint(RigidBodyConstraint::TransY, false);
		m_pRigid->SetConstraint(RigidBodyConstraint::AllRot, false);
		m_pRigid->AddCollider(PxCapsuleGeometry(1.2f, 0.08f), *pPhysxMaterial, false, { 0,1.5f,0 });

		m_pRigid->SetCollisionGroup(CollisionGroup::Group0);
		m_pRigid->SetCollisionIgnoreGroups(CollisionGroup::Group1 | CollisionGroup::Group5 | CollisionGroup::Group6);
		m_pRigid->SetDensity(0.2f);
		
		GetTransform()->Scale(0.02f);
		GetTransform()->Translate(x, 0, z);
		SetTag(L"Player");
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

	sc.pInput->AddInputAction(InputAction( UniqueInputID(m_Player, input), InputState::down, virtualKey));
}

void Player::Kill()
{
	ChangeState(Dead);
}

void Player::Initialize(const SceneContext&)
{
	m_pBomb = GetScene()->AddChild(new Bomb(this));

	const auto& sc = GetScene()->GetSceneContext();
	switch (m_Player)
	{
	case 0:
		m_Color = { 1.f,0.f,0.f, 1.f };
		m_pScoreBoard = AddChild(new Text({ 0, 0 }, { 10, 10 }, leftTop, L"0", true, m_Color));
		break;
	case 1:
		m_Color = { 0.f,1.f,0.f, 1.f };
		m_pScoreBoard = AddChild(new Text({ sc.windowWidth, 0 }, { 10, 10 }, rightTop, L"0", true, m_Color));
		break;
	case 2:
		m_Color = { 0.f,0.f,1.f, 1.f };
		m_pScoreBoard = AddChild(new Text({ 0, sc.windowHeight }, { 10, 10 }, leftBottom, L"0", true, m_Color));
		break;
	case 3:
		m_Color = { 1.f,1.f,0.f, 1.f };
		m_pScoreBoard = AddChild(new Text({ sc.windowWidth, sc.windowHeight }, { 10, 10 }, rightBottom, L"0", true, m_Color));
		break;
	}

	const auto pPlayerMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Skinned>();
	pPlayerMaterial->SetDiffuseTexture(L"Textures/Project/Player_diffuse.png");
	pPlayerMaterial->SetVariable_Scalar(L"gUseColor", true);
	pPlayerMaterial->SetVariable_Vector(L"gPlayerColor", m_Color);


	const auto pModel = AddComponent(new ModelComponent(L"Meshes/Project/Player.ovm"));
	pModel->SetMaterial(pPlayerMaterial);
	m_pAnimator = pModel->GetAnimator();
	m_pAnimator->Play();
}

void Player::Draw(const SceneContext&)
{
}

void Player::Update(const SceneContext& sceneContext)
{
	float dT = sceneContext.pGameTime->GetElapsed();

	if (m_State == Dead)
	{
		PlayerDied(dT);
		return;
	}

	//## Input Gathering (move)
	XMFLOAT3 move = XMFLOAT3(0, 0, 0);
	float rot{};
	bool isMoving = false;
	
	if (sceneContext.pInput->IsActionTriggered(UniqueInputID(m_Player, MoveForward)))
	{
		move.z += m_MoveSpeed * 10;
		rot = 180.f;
		isMoving = true;
	}
	if (sceneContext.pInput->IsActionTriggered(UniqueInputID(m_Player, MoveBackward)))
	{
		move.z -= m_MoveSpeed * 10;
		rot = 0.f;
		isMoving = true;
	}
	if (sceneContext.pInput->IsActionTriggered(UniqueInputID(m_Player, MoveRight)))
	{
		move.x += m_MoveSpeed * 10;
		rot = 270.f;
		isMoving = true;
	}
	if (sceneContext.pInput->IsActionTriggered(UniqueInputID(m_Player, MoveLeft)))
	{
		move.x -= m_MoveSpeed * 10;
		rot = 90.f;
		isMoving = true;
	}

	if (isMoving)
	{
		m_pRigid->AddForce(move, PxForceMode::eVELOCITY_CHANGE);
		GetTransform()->Rotate(0, rot, 0);

		ChangeState(Moving);
	}
	else
	{
		GetTransform()->Translate(GetTransform()->GetPosition());


		ChangeState(Idle);
	}

	// bomb
	if (sceneContext.pInput->IsActionTriggered(UniqueInputID(m_Player, DropBomb)))
	{
		// we cannot make
		if (!m_pBomb->IsActive())
			m_pBomb->Activate(GetTransform()->GetPosition());
	}
}

void Player::ChangeState(PlayerState state)
{
	if (state == m_State)
		return;

	m_State = state;
	m_pAnimator->SetAnimation(state);
	m_pAnimator->Play();
}

void Player::PlayerDied(float dT)
{
	m_TimerBeforeDespawn += dT;
	if (m_DespawnTime < m_TimerBeforeDespawn)
		m_pAnimator->Pause();

	GetTransform()->Translate(GetTransform()->GetPosition());
	m_pRigid->SetCollisionGroup(CollisionGroup::Group6);
}
