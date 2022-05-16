#include "stdafx.h"
#include "Bomb.h"

#include "Player.h"
#include "Materials/DiffuseMaterial.h"


Bomb::Bomb(Player* pPlayer)
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

	GetTransform()->Translate(pPlayer->GetTransform()->GetPosition());
	GetTransform()->Scale(0.02f);
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
		const auto& go = static_cast<BaseComponent*>(hit.getAnyHit(static_cast<PxU32>(0)).actor->userData);
		

		return go->GetGameObject();
	}
	return nullptr;
}

void Bomb::Update(const SceneContext& sc)
{
	auto dTime = sc.pGameTime->GetElapsed();

	m_LiveTime += dTime;

	if (m_LiveTime > m_Duration)
	{
		auto pos = GetTransform()->GetWorldPosition();
		pos.y += 0.5f;

		// cast rays in 4 directions
		const auto& physxProxy = GetScene()->GetPhysxProxy();
		// north
		if (auto go = RaycastExplosion(physxProxy, PhysxHelper::ToPxVec3(pos), { 0,0,1 }))
			GetScene()->RemoveChild(go);
		// east
		if (auto go = RaycastExplosion(physxProxy, PhysxHelper::ToPxVec3(pos), { 1,0,0 }))
			GetScene()->RemoveChild(go);
		// south
		if (auto go = RaycastExplosion(physxProxy, PhysxHelper::ToPxVec3(pos), { 0,0,-1 }))
			GetScene()->RemoveChild(go);
		// west
		if (auto go = RaycastExplosion(physxProxy, PhysxHelper::ToPxVec3(pos), { -1,0,0 }))
			GetScene()->RemoveChild(go);

		// cleanup bomb
		GetScene()->RemoveChild(this);
		m_Exploded = true;
	}
}

