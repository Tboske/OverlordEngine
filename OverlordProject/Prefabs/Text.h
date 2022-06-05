#pragma once

enum TextPivot
{
	leftTop,
	rightTop,
	leftBottom,
	rightBottom,
	center
};

class Text final : public GameObject
{
public:
	Text(XMFLOAT2 pos, XMFLOAT2 padding, TextPivot pivot = leftTop, const std::wstring& text = L"Text", bool visibility = true
		, const XMFLOAT4& color = { 1,1,1,1 }
	);
	~Text() override = default;
	Text(const Text&) = delete;
	Text(Text&&) noexcept = delete;
	Text& operator=(const Text&) = delete;
	Text& operator=(Text&&) noexcept = delete;

	
	void SetText(const std::wstring& text);
	void SetVisible(bool visible) { m_Visible = visible; }

protected:
	virtual void Update(const SceneContext&) override;
	

private:
	XMFLOAT2 m_Position{0,0};
	TextPivot m_PivotOffset = leftTop;
	XMFLOAT2 m_Padding{ 5,5 };

	bool m_Visible = true;
	SpriteFont* m_pFont;
	XMFLOAT4 m_Color {1,0,0,1};
	std::wstring m_Text{L"0"};


	void UpdateOffset();
};

