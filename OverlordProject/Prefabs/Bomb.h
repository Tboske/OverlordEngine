#pragma once

class Player;

class Bomb final : public GameObject
{
public:
	Bomb(Player* pPlayer);
	virtual ~Bomb() override = default;

	bool CanDelete() { return m_Exploded; }

protected:
	virtual void Update(const SceneContext&) override;
private:
	// add particle material

	bool m_Exploded = false;

	float m_LiveTime = 0.f;
	float m_Duration = 4.f;
};

