#pragma once

class Player;

class Bomb final : public GameObject
{
public:
	Bomb();
	virtual ~Bomb() override = default;

	bool IsActive() { return m_IsActive; }
	void Activate(const XMFLOAT3& pos);

protected:
	virtual void Update(const SceneContext&) override;
private:
	// add particle material

	bool m_IsActive = false;

	float m_LiveTime = 0.f;
	float m_Duration = 4.f;
};

