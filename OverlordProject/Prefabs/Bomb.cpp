#include "stdafx.h"
#include "Bomb.h"

#include "Player.h"
#include "Components/BombEffectComponent.h"
#include "Materials/DiffuseMaterial.h"
#include "Scenes/Project/ProjectScene.h"


Bomb::Bomb(Player* pPlayer)
	: m_pPlayer{ pPlayer }
{
	const auto pBombMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pBombMaterial->SetDiffuseTexture(L"Textures/Project/Bomb.png");

	const auto pRigid = AddComponent(new RigidBodyComponent(true));
	pRigid->SetCollisionGroup(CollisionGroup::Group1);
	pRigid->SetCollisionIgnoreGroups(CollisionGroup::Group0);

	auto* pConvex = ContentManager::Load<PxConvexMesh>(L"Meshes/Project/Bomb.ovpc");
	const auto pPhysxMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.0f);
	pRigid->AddCollider(PxConvexMeshGeometry(pConvex, PxMeshScale(0.02f)), *pPhysxMaterial);

	const auto pModel = AddComponent(new ModelComponent(L"Meshes/Project/Bomb.ovm"));
	pModel->SetMaterial(pBombMaterial);
	
	GetTransform()->Scale(0.02f);	
	SetTag(L"Bomb");
	// put the bomb out of the arena, to make it invisible
	GetTransform()->Translate(0, -10, 0);
}

void Bomb::Activate(const XMFLOAT3& pos)
{
	m_IsActive = true;
	m_LiveTime = 0.f;
	GetTransform()->Translate(pos);
}

// if the player is hit, the Gameobject pointer is filled with the player pointer
GameObject* RaycastExplosion(PhysxProxy* pPhysxProx, const PxVec3& pos, const PxVec3& dir, PxRaycastHit& info, std::vector<Player*>& pPlayer)
{
	PxQueryFilterData filterData;
	filterData.data.word0 = ~static_cast<PxU32>(CollisionGroup::Group1 | CollisionGroup::Group6 | CollisionGroup::Group9);

	PxRaycastBuffer hit;
	if (pPhysxProx->Raycast(pos, dir, 200.f, hit, PxHitFlag::eDEFAULT, filterData))
	{
		for (PxU32 i = 0; i < hit.getNbAnyHits(); ++i)
		{
			info = hit.getAnyHit(i);
			auto go = static_cast<BaseComponent*>(info.actor->userData)->GetGameObject();

			if (go->GetTag() == L"Player")
				pPlayer.push_back(dynamic_cast<Player*>(go));

			if (go->GetTag() == L"Box")
				return go;
		}

		// no box was hit, return first hit then
		info = hit.getAnyHit(1);
	}
	return nullptr;
}

void AddVertices(std::vector<ExplosionVertex>& vertices, const XMFLOAT3& origin, const XMFLOAT3& hitPos)
{
	auto vOrigin = XMLoadFloat3(&origin);
	auto vHitPos = XMLoadFloat3(&hitPos);

	// calc distance
	auto dist = vOrigin - vHitPos;
	dist = XMVector3Length(dist);
	XMFLOAT3 deltaDist{};
	XMStoreFloat3(&deltaDist, dist);

	const float nrVertices = deltaDist.x;
	for (float i = 0; i <= nrVertices; i += 1.f)
	{
		ExplosionVertex& vert = vertices.emplace_back();

		float d = i / nrVertices;
		auto pos = XMVectorLerp(vOrigin, vHitPos, d);
		XMStoreFloat3(&vert.Position, pos);

		vert.Size = 0.f;
	}
}


void Bomb::Update(const SceneContext& sc)
{
	if (!m_IsActive)
		return;
	

	auto dTime = sc.pGameTime->GetElapsed();
	m_LiveTime += dTime;

	auto pos = GetTransform()->GetWorldPosition();
	pos.y += 1.5f;

#if _DEBUG
	auto end = pos;
	end.z += 25.f;
	DebugRenderer::DrawLine(pos, end);

	end = pos;
	end.z -= 25.f;
	DebugRenderer::DrawLine(pos, end);

	end = pos;
	end.x += 25.f;
	DebugRenderer::DrawLine(pos, end);

	end = pos;
	end.x -= 25.f;
	DebugRenderer::DrawLine(pos, end);
#endif



	if (m_LiveTime > m_Duration)
	{
		if (m_Exploded)
		{
			if (m_pBombEffect->IsActive())
			{
				// cleanup bomb
				m_IsActive = false;
				m_Exploded = false;
				// put the bomb out of the arena, to make it invisible
				GetTransform()->Translate(0, -10, 0);
			}
		}
		else
		{
			std::vector<ExplosionVertex> bombExplosion;
			std::vector<Player*> hitPlayers;

			// cast rays in 4 directions
			const auto& physxProxy = GetScene()->GetPhysxProxy();
			auto* scene = static_cast<ProjectScene*>(GetScene());
			PxRaycastHit hitInfo;
			
			// north
			if (auto go = RaycastExplosion(physxProxy, PhysxHelper::ToPxVec3(pos), { 0,0,1 }, hitInfo, hitPlayers))
				scene->GetArena()->RemoveChild(go, true);
			AddVertices(bombExplosion, pos, MathHelper::ToXMFloat3(hitInfo.position));

			// east
			if (auto go = RaycastExplosion(physxProxy, PhysxHelper::ToPxVec3(pos), { 1,0,0 }, hitInfo, hitPlayers))
				scene->GetArena()->RemoveChild(go, true);
			AddVertices(bombExplosion, pos, MathHelper::ToXMFloat3(hitInfo.position));

			// south
			if (auto go = RaycastExplosion(physxProxy, PhysxHelper::ToPxVec3(pos), { 0,0,-1 }, hitInfo, hitPlayers))
				scene->GetArena()->RemoveChild(go, true);
			AddVertices(bombExplosion, pos, MathHelper::ToXMFloat3(hitInfo.position));

			// west
			if (auto go = RaycastExplosion(physxProxy, PhysxHelper::ToPxVec3(pos), { -1,0,0 }, hitInfo, hitPlayers))
				scene->GetArena()->RemoveChild(go, true);
			AddVertices(bombExplosion, pos, MathHelper::ToXMFloat3(hitInfo.position));


			// kill the players that were hit
			for (const auto& player : hitPlayers)
				player->Kill();

			m_pBombEffect->ActivateEffect(bombExplosion);
			m_Exploded = true;
		}
	}
}

void Bomb::Initialize(const SceneContext&)
{
	m_pBombEffect = AddComponent(new BombEffectComponent(L"Textures/FireBall.png"));

}

