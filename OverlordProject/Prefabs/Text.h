#pragma once

class Text final : public GameObject
{
public:
	Text();
	~Text() override = default;
	Text(const Text&) = delete;
	Text(Text&&) noexcept = delete;
	Text& operator=(const Text&) = delete;
	Text& operator=(Text&&) noexcept = delete;

	void SetColor(const XMFLOAT3& color) { m_Color = {color.x, color.y, color.z, 1.f}; }
	void SetPosition(const XMFLOAT2& pos) { m_Position = pos; }
	void SetText(const std::wstring& text) { m_Text = text; }

protected:
	virtual void Initialize(const SceneContext&) override;
	virtual void Update(const SceneContext&) override;

private:
	SpriteFont* m_pFont;

	XMFLOAT2 m_Position{0,0};
	XMFLOAT4 m_Color {1,0,0,1};

	std::wstring m_Text{L"0"};
};

