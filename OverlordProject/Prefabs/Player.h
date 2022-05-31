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
	int m_player = 0;

	Text* m_pScoreBoard = nullptr;
	PlayerState m_State = Moving; // default to the first animation

	XMFLOAT3 m_Color = { 1.f, 0.f, 0.f };


	void ChangeState(PlayerState state);
};

