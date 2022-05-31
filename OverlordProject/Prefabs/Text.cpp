#include "stdafx.h"
#include "Text.h"



Text::Text()
	: m_pFont(ContentManager::Load<SpriteFont>(L"SpriteFonts/Consolas_32.fnt"))
{
	
}

void Text::Initialize(const SceneContext& )
{

}

void Text::Update(const SceneContext&)
{
	TextRenderer::Get()->DrawText(m_pFont, m_Text, m_Position, m_Color);
}
