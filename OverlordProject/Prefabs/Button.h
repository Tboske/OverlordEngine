#pragma once

class Button final : public GameObject
{
public:
	Button(float x
		, float y
		, const std::wstring& text = L"Button"
		, XMFLOAT4 color = { 1,1,1,1 }
		, float layer = 0
		, float width = 10
		, float height = 5
	);
	~Button() override = default;
	Button(const Button&) = delete;
	Button(Button&&) noexcept = delete;
	Button& operator=(const Button&) = delete;
	Button& operator=(Button&&) noexcept = delete;

protected:
	void Initialize(const SceneContext&) override;
	void Draw(const SceneContext&) override;

private:
	std::wstring m_Text;
	XMFLOAT2 m_Position;
	SpriteFont* m_pFont = nullptr;
};

