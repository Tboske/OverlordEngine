#pragma once

class ScreenShakeComponent : public BaseComponent
{
public:
	ScreenShakeComponent() = default;
	~ScreenShakeComponent() override;
	ScreenShakeComponent(const ScreenShakeComponent&) = delete;
	ScreenShakeComponent(ScreenShakeComponent&&) noexcept = delete;
	ScreenShakeComponent& operator=(const ScreenShakeComponent&) = delete;
	ScreenShakeComponent& operator=(ScreenShakeComponent&&) noexcept = delete;

	void Shake(float intensity, float duration);

protected:
	virtual void Update(const SceneContext&) override;
	virtual void Initialize(const SceneContext& sceneContext) override;

private:
	bool m_Active = false;
	float m_DurationMax = 1.f;
	float m_Duration = 0.f;
	float m_Intensity = 0.2f;

	XMFLOAT3 m_OriginalPos = { 0,0,0 };
	TransformComponent* m_TransformComponent = nullptr;
};

