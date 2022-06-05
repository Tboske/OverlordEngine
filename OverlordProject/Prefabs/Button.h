#pragma once

class Text;

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
	

private:
	Text* m_pText;
};

