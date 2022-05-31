#pragma once

class Score;
class Bomb;

class Player final : public GameObject
{
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

	Score* m_pScore = nullptr;

	XMFLOAT3 m_Color = { 1.f, 0.f, 0.f };
};

