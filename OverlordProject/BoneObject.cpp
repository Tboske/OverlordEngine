#include "stdafx.h"
#include "BoneObject.h"


BoneObject::BoneObject(BaseMaterial* pMaterial, float length)
	: m_Length(length)
	, m_pMaterial(pMaterial)
{
}

BoneObject::~BoneObject()
{
	
}

void BoneObject::AddBone(BoneObject* pBone)
{
	pBone->GetTransform()->Translate(m_Length, 0, 0);
	AddChild(pBone);
}

void BoneObject::CalculateBindPose()
{
	FXMMATRIX world = XMLoadFloat4x4(&GetTransform()->GetWorld());
	XMVECTOR determ = XMMatrixDeterminant(world);
	XMStoreFloat4x4(&m_BindPose, XMMatrixInverse(&determ, world));

	for (BoneObject* pBone : GetChildren<BoneObject>())
		pBone->CalculateBindPose();
}

void BoneObject::Initialize(const SceneContext&)
{
	auto* pEmpty = AddChild(new GameObject());
	auto* pModel = pEmpty->AddComponent(new ModelComponent(L"Meshes/Bone.ovm"));
		pModel->SetMaterial(m_pMaterial);
	pEmpty->GetTransform()->Rotate(XMFLOAT3{ 0,-90,0 });
	pEmpty->GetTransform()->Scale(m_Length, m_Length, m_Length);
}
