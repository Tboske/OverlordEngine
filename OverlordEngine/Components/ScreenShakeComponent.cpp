#include "stdafx.h"
#include "ScreenShakeComponent.h"


ScreenShakeComponent::~ScreenShakeComponent()
{
	m_TransformComponent = nullptr;
}

void ScreenShakeComponent::Shake(float intensity, float duration)
{
	m_DurationMax = duration;
	m_Intensity = intensity;

	m_Duration = 0;
	m_Active = true;
}

void ScreenShakeComponent::Update(const SceneContext& sceneContext)
{
	if (!m_Active)
		return;

	m_Duration += sceneContext.pGameTime->GetElapsed();

	if (m_Duration > m_DurationMax)
	{
		m_TransformComponent->Translate(m_OriginalPos);
		m_Active = false;
		return;
	}

	m_TransformComponent->Translate(
		m_OriginalPos.x + MathHelper::randF(-m_Intensity, m_Intensity),
		m_OriginalPos.y + MathHelper::randF(-m_Intensity, m_Intensity),
		m_OriginalPos.z + MathHelper::randF(-m_Intensity, m_Intensity)
	);
}

void ScreenShakeComponent::Initialize(const SceneContext&)
{
	m_TransformComponent = GetGameObject()->GetTransform();

	m_OriginalPos = m_TransformComponent->GetPosition();
}
