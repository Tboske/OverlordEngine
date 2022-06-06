#pragma once

class BombEffectComponent;
class Player;

class Bomb final : public GameObject
{
public:
	Bomb(Player* pPlayer);
	virtual ~Bomb() override = default;

	bool IsActive() const { return m_IsActive; }
	void Activate(const XMFLOAT3& pos);

protected:
	virtual void Update(const SceneContext&) override;
	virtual void Initialize(const SceneContext&) override;
private:
	// add particle material
	BombEffectComponent* m_pBombEffect = nullptr;

	bool m_IsActive = false;
	bool m_Exploded = false;

	float m_LiveTime = 0.f;
	float m_Duration = 4.f;

	FMOD::Sound* m_pExplosionEffect = nullptr;

	Player* m_pPlayer = nullptr;
};

