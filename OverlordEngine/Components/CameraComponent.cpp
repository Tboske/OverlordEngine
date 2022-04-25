#include "stdafx.h"
#include "CameraComponent.h"

CameraComponent::CameraComponent() :
	m_FarPlane(2500.0f),
	m_NearPlane(0.1f),
	m_FOV(XM_PIDIV4),
	m_Size(25.0f),
	m_PerspectiveProjection(true)
{
	XMStoreFloat4x4(&m_Projection, XMMatrixIdentity());
	XMStoreFloat4x4(&m_View, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewInverse, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjection, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjectionInverse, XMMatrixIdentity());
}

void CameraComponent::Update(const SceneContext& sceneContext)
{
	// see https://stackoverflow.com/questions/21688529/binary-directxxmvector-does-not-define-this-operator-or-a-conversion
	using namespace DirectX;

	XMMATRIX projection{};

	if (m_PerspectiveProjection)
	{
		projection = XMMatrixPerspectiveFovLH(m_FOV, sceneContext.aspectRatio, m_NearPlane, m_FarPlane);
	}
	else
	{
		const float viewWidth = (m_Size > 0) ? m_Size * sceneContext.aspectRatio : sceneContext.windowWidth;
		const float viewHeight = (m_Size > 0) ? m_Size : sceneContext.windowHeight;
		projection = XMMatrixOrthographicLH(viewWidth, viewHeight, m_NearPlane, m_FarPlane);
	}

	const XMVECTOR worldPosition = XMLoadFloat3(&GetTransform()->GetWorldPosition());
	const XMVECTOR lookAt = XMLoadFloat3(&GetTransform()->GetForward());
	const XMVECTOR upVec = XMLoadFloat3(&GetTransform()->GetUp());

	const XMMATRIX view = XMMatrixLookAtLH(worldPosition, worldPosition + lookAt, upVec);
	const XMMATRIX viewInv = XMMatrixInverse(nullptr, view);
	const XMMATRIX viewProjectionInv = XMMatrixInverse(nullptr, view * projection);

	XMStoreFloat4x4(&m_Projection, projection);
	XMStoreFloat4x4(&m_View, view);
	XMStoreFloat4x4(&m_ViewInverse, viewInv);
	XMStoreFloat4x4(&m_ViewProjection, view * projection);
	XMStoreFloat4x4(&m_ViewProjectionInverse, viewProjectionInv);
}

void CameraComponent::SetActive(bool active)
{
	if (m_IsActive == active) return;

	const auto pGameObject = GetGameObject();
	ASSERT_IF(!pGameObject, L"Failed to set active camera. Parent game object is null");

	if (!pGameObject) return; //help the compiler... (C6011)
	const auto pScene = pGameObject->GetScene();
	ASSERT_IF(!pScene, L"Failed to set active camera. Parent game scene is null");

	m_IsActive = active;
	pScene->SetActiveCamera(active?this:nullptr); //Switch to default camera if active==false
}

GameObject* CameraComponent::Pick(CollisionGroup ignoreGroups) const
{
	const auto& gc = GetScene()->GetSceneContext();

	// ndc
	const POINT mouse = gc.pInput->GetMousePosition();

	const float hWidth = static_cast<float>(GetScene()->GetSceneContext().windowWidth / 2);
	const float hHeight = static_cast<float>(GetScene()->GetSceneContext().windowHeight / 2);
	float x = (mouse.x - hWidth) / hWidth;
	float y = -((mouse.y - hHeight) / hHeight);

	// near far point
	const auto vp = XMLoadFloat4x4(&GetViewProjectionInverse());
	const auto nearPoint = XMVector3TransformCoord({ x, y, 0.f, 0.f }, vp);
	const auto farPoint = XMVector3TransformCoord({ x,y,1.f, 0.f }, vp);

	// make it a PxVec3
	DirectX::XMFLOAT3 direction;
	XMStoreFloat3(&direction, DirectX::XMVectorSubtract(farPoint, nearPoint));
	physx::PxVec3 dirVec = PhysxHelper::ToPxVec3(direction);
	dirVec.normalize();

	DirectX::XMFLOAT3 nP;
	XMStoreFloat3(&nP, nearPoint);

	// Raycast
	physx::PxQueryFilterData filterData;
	filterData.data.word0 = ~static_cast<physx::PxU32>(ignoreGroups);

	physx::PxRaycastBuffer hit;
	const auto& physxProxy = GetGameObject()->GetScene()->GetPhysxProxy();
	if (physxProxy->Raycast(PhysxHelper::ToPxVec3(nP), dirVec, PX_MAX_F32, hit
							, physx::PxHitFlag::eDEFAULT, filterData))
	{
		const auto& go = static_cast<BaseComponent*>(hit.getAnyHit(static_cast<PxU32>(0)).actor->userData);
		return go->GetGameObject();
	}
	return nullptr;
}