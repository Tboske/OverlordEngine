#pragma once

class CubePrefab;
class SpherePrefab;

class PongScene final: public GameScene
{
public:
	PongScene() : GameScene(L"PongScene") {}
	~PongScene() override;
	PongScene(const PongScene&) = delete;
	PongScene(PongScene&&) noexcept = delete;
	PongScene& operator=(const PongScene&) = delete;
	PongScene& operator=(PongScene&&) noexcept = delete;

protected:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void OnGUI() override;
	virtual void OnSceneActivated() override;
	virtual void OnSceneDeactivated() override;
private:
	// ball
	SpherePrefab* m_pSphere = nullptr;
	RigidBodyComponent* m_pSphereRigid = nullptr;

	// players
	CubePrefab* m_pPlayer1 = nullptr;
	CubePrefab* m_pPlayer2 = nullptr;

	float m_MoveSpeed = 0.15f;
	float m_Constraint = 6.f;

	float m_ResetDistance = 30.f;
	float m_MaxSpeed = 20.f;

	void MovePlayer(TransformComponent* pTransform, float speed) const;
	void ResetBall() const;
};
