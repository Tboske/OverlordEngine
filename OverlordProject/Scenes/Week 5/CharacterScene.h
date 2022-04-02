#pragma once
class Character;

class CharacterScene : public GameScene
{
public:
	CharacterScene() :GameScene(L"CharacterScene") {}
	~CharacterScene() override = default;
	CharacterScene(const CharacterScene& other) = delete;
	CharacterScene(CharacterScene&& other) noexcept = delete;
	CharacterScene& operator=(const CharacterScene& other) = delete;
	CharacterScene& operator=(CharacterScene&& other) noexcept = delete;

protected:
	virtual void Initialize() override;
	virtual void OnGUI() override;

private:
	enum InputIds
	{
		CharacterMoveLeft,
		CharacterMoveRight,
		CharacterMoveForward,
		CharacterMoveBackward,
		CharacterJump
	};

	Character* m_pCharacter{};
};

