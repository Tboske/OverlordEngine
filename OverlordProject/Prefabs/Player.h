#pragma once

class Text;
class Bomb;

class Player final : public GameObject
{
	enum PlayerState
	{
		Moving,
		Idle,
		DroppingBomb,
		Dead
	};
public:
	enum InputIds
	{
		MoveLeft,
		MoveRight,
		MoveForward,
		MoveBackward,
		DropBomb
	};

	Player(int player, float x, float z);
	virtual ~Player() override;

	void AddInput(const SceneContext& sc, InputIds input, UINT virtualKey);

	bool IsDead() const { return m_State == Dead ? true : false; }
	void Kill();

protected:
	virtual void Initialize(const SceneContext&) override;
	virtual void Draw(const SceneContext&) override;
	virtual void Update(const SceneContext&) override;

private:
	Bomb* m_pBomb = nullptr;

	RigidBodyComponent* m_pRigid = nullptr;
	ModelAnimator* m_pAnimator{};
	float m_MoveSpeed = 0.1f;
	float m_TimerBeforeDespawn = 0.f;
	float m_DespawnTime = 2.f;
	int m_Player = 0;

	Text* m_pScoreBoard = nullptr;
	PlayerState m_State = Moving; // default to the first animation

	XMFLOAT4 m_Color = { 1.f, 0.f, 0.f, 1.f };


	void ChangeState(PlayerState state);
	void PlayerDied(float dT);
};

