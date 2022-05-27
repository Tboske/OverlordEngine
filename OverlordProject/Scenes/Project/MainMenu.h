#pragma once

class Button;

class MainMenu final : public GameScene
{
public:
	MainMenu() : GameScene(L"MainMenu") {}
	~MainMenu() override = default;
	MainMenu(const MainMenu&) = delete;
	MainMenu(MainMenu&&) noexcept = delete;
	MainMenu& operator=(const MainMenu&) = delete;
	MainMenu& operator=(MainMenu&&) noexcept = delete;

protected:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Draw() override;

private:
	Button* m_pPlay = nullptr;
	Button* m_pExit = nullptr;

	CameraComponent* m_pCameraComponent = nullptr;
	
};

