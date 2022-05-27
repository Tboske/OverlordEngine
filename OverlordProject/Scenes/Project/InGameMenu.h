#pragma once

class Button;

class InGameMenu final : public GameScene
{
public:
	InGameMenu() : GameScene(L"InGameMenu") {}
	~InGameMenu() override = default;
	InGameMenu(const InGameMenu&) = delete;
	InGameMenu(InGameMenu&&) noexcept = delete;
	InGameMenu& operator=(const InGameMenu&) = delete;
	InGameMenu& operator=(InGameMenu&&) noexcept = delete;

protected:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Draw() override;

private:
	Button* m_pContinue = nullptr;
	Button* m_pRestart = nullptr;
	Button* m_pMainMenu = nullptr;
	Button* m_pExit = nullptr;

	CameraComponent* m_pCameraComponent = nullptr;
};

