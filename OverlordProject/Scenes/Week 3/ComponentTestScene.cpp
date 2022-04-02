#include "stdafx.h"
#include "ComponentTestScene.h"

#include "Prefabs/SpherePrefab.h"

void ComponentTestScene::Initialize()
{
	auto pDefaultMat = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.8f);

	GameSceneExt::CreatePhysXGroundPlane(*this, pDefaultMat);

	// sphere 1
	auto pSphereObject = AddChild(new SpherePrefab(1.f));
	auto pSphereActor = pSphereObject->AddComponent(new RigidBodyComponent());
	pSphereActor->AddCollider(PxSphereGeometry(1.f), *pDefaultMat);
	pSphereObject->GetTransform()->Translate(0.f, 30.f, 0.f);
	pSphereActor->SetCollisionGroup(CollisionGroup::Group1);

	// sphere 2
	pSphereObject = AddChild(new SpherePrefab(1.f, 10, {1,0,0,1}));
	pSphereActor = pSphereObject->AddComponent(new RigidBodyComponent());
	pSphereActor->AddCollider(PxSphereGeometry(1.f), *pDefaultMat);
	pSphereObject->GetTransform()->Translate(0.f, 20.f, 0.f);
	pSphereActor->SetCollisionIgnoreGroups(CollisionGroup::Group1);

	// sphere 3
	pSphereObject = AddChild(new SpherePrefab(1.f, 10, {0,1,0,1}));
	pSphereActor = pSphereObject->AddComponent(new RigidBodyComponent());
	pSphereActor->AddCollider(PxSphereGeometry(1.f), *pDefaultMat);
	pSphereObject->GetTransform()->Translate(0.f, 10.f, 0.f);
}
