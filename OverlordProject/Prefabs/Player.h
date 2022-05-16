#pragma once

class Bomb;

class Player final : public GameObject
{
public:
	Player();
	virtual ~Player() override;
protected:
	virtual void Initialize(const SceneContext&) override;
	virtual void Draw(const SceneContext&) override;
	virtual void Update(const SceneContext&) override;

private:
	Bomb* m_pBomb = nullptr;

	RigidBodyComponent* m_pRigid = nullptr;
	ModelAnimator* m_pAnimator{};
	float m_MoveSpeed = 0.1f;

	enum InputIds
	{
		MoveLeft,
		MoveRight,
		MoveForward,
		MoveBackward,
		DropBomb
	};
};

