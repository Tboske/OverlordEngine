#include "stdafx.h"
#include "Bomb.h"

#include "Player.h"
#include "Materials/DiffuseMaterial.h"
#include "Scenes/Project/ProjectScene.h"


Bomb::Bomb()
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

GameObject* RaycastExplosion(PhysxProxy* pPhysxProx, const PxVec3& pos, const PxVec3& dir)
{
	// make an extra variable to add an extra point for the shader

	PxQueryFilterData filterData;
	filterData.data.word0 = ~static_cast<PxU32>(CollisionGroup::Group1 | CollisionGroup::Group9);

	PxRaycastBuffer hit;
	if (pPhysxProx->Raycast(pos, dir, PX_MAX_F32, hit
		, PxHitFlag::eDEFAULT, filterData))
	{
		const auto& go = static_cast<BaseComponent*>(hit.getAnyHit(static_cast<PxU32>(0)).actor->userData)->GetGameObject();

		if (go->GetTag() != L"Bomb")
			return go;

		const auto& go2 = static_cast<BaseComponent*>(hit.getAnyHit(static_cast<PxU32>(1)).actor->userData)->GetGameObject();
		return go2;
	}
	return nullptr;
}


void Bomb::Update(const SceneContext& sc)
{
	if (!m_IsActive)
		return;

	auto dTime = sc.pGameTime->GetElapsed();

	m_LiveTime += dTime;

	if (m_LiveTime > m_Duration)
	{
		auto pos = GetTransform()->GetWorldPosition();
		pos.y += 0.5f;

		// cast rays in 4 directions
		const auto& physxProxy = GetScene()->GetPhysxProxy();
		auto* scene = static_cast<ProjectScene*>(GetScene());
		// north
		if (auto go = RaycastExplosion(physxProxy, PhysxHelper::ToPxVec3(pos), { 0,0,1 }))
			scene->GetArena()->RemoveChild(go, true);
		// east
		if (auto go = RaycastExplosion(physxProxy, PhysxHelper::ToPxVec3(pos), { 1,0,0 }))
			scene->GetArena()->RemoveChild(go, true);
		// south
		if (auto go = RaycastExplosion(physxProxy, PhysxHelper::ToPxVec3(pos), { 0,0,-1 }))
			scene->GetArena()->RemoveChild(go, true);
		// west
		if (auto go = RaycastExplosion(physxProxy, PhysxHelper::ToPxVec3(pos), { -1,0,0 }))
			scene->GetArena()->RemoveChild(go, true);

		// cleanup bomb
		m_IsActive = false;
		// put the bomb out of the arena, to make it invisible
		GetTransform()->Translate(0, -10, 0);
	}
}

