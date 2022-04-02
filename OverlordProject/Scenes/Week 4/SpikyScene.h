#pragma once

class SpikyMaterial;

class SpikyScene final : public GameScene
{
public:
	SpikyScene() : GameScene(L"SpikyScene"){}
	~SpikyScene() override = default;
	SpikyScene(const SpikyScene&) = delete;
	SpikyScene(SpikyScene&&) noexcept = delete;
	SpikyScene& operator=(const SpikyScene&) = delete;
	SpikyScene& operator=(SpikyScene&&) noexcept = delete;

protected:
	virtual void Initialize() override;
	virtual void OnSceneActivated() override;
	virtual void OnSceneDeactivated() override;
	virtual void OnGUI() override;

private:
	SpikyMaterial* m_pMaterial = nullptr;
	GameObject* m_pSphere = nullptr;
};

