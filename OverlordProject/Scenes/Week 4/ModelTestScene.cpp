#include "stdafx.h"
#include "ModelTestScene.h"

#include "Materials/DiffuseMaterial.h"


ModelTestScene::~ModelTestScene()
{
	
}

void ModelTestScene::Initialize()
{
	auto* pChairMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pChairMaterial->SetDiffuseTexture(L"Textures/Chair_Dark.dds");

	auto pDefaultMat = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.8f);
	
	GameSceneExt::CreatePhysXGroundPlane(*this, pDefaultMat);
	
	// chair
	m_pChair = new GameObject();
	{
		auto* pModelComp = m_pChair->AddComponent(new ModelComponent(L"Meshes/Chair.ovm"));
		pModelComp->SetMaterial(pChairMaterial);
		auto* pRigid = m_pChair->AddComponent(new RigidBodyComponent());
		auto* pConvexChair = ContentManager::Load<PxConvexMesh>(L"Meshes/Chair.ovpc");
		pRigid->AddCollider(PxConvexMeshGeometry(pConvexChair), *pDefaultMat);
	}
	AddChild(m_pChair);
	m_pChair->GetTransform()->Translate(0.f, 2.f, 0.f);
}

void ModelTestScene::Update()
{
}
