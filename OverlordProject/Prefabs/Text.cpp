#include "stdafx.h"
#include "Text.h"

#include "CubePrefab.h"

Text::Text(XMFLOAT2 pos, XMFLOAT2 padding, TextPivot pivot, const std::wstring& text, bool visibility, const XMFLOAT4& color)
	: m_Position(pos)
	, m_PivotOffset(pivot)
	, m_Padding(padding)
	, m_Visible(visibility)
	, m_Color(color)
	, m_Text(text)
	, m_pFont(ContentManager::Load<SpriteFont>(L"SpriteFonts/Arial_32.fnt"))
{
	UpdateOffset();
}

void Text::SetText(const std::wstring& text)
{
	m_Text = text;

	UpdateOffset();
}

void Text::Update(const SceneContext&)
{
	if (!m_Visible)
		return;

	if (m_pFont->GetName() == L"")
		return;

	TextRenderer::Get()->DrawText(m_pFont, m_Text, m_Position, m_Color);
}


void Text::UpdateOffset()
{
	const auto& size = m_pFont->GetSize();

	switch (m_PivotOffset)
	{
	case leftTop:
		m_Position = m_Padding;
		break;

	case rightTop:
		m_Position.x -= size / 3.f * m_Text.size();

		m_Position.x -= m_Padding.x;
		m_Position.y += m_Padding.y;
		break;

	case leftBottom:
		m_Position.y -= size;

		m_Position.x += m_Padding.x;
		m_Position.y -= m_Padding.y;
		break;

	case rightBottom:
		m_Position.x -= size / 3.f * m_Text.size();
		m_Position.y -= size;

		m_Position.x -= m_Padding.x;
		m_Position.y -= m_Padding.y;
		break;

	case center:
		m_Position.x -= (size / 3.f * m_Text.size()) /2;
		m_Position.y -= size / 2;
		break;
	}
}
