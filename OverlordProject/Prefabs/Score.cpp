#include "stdafx.h"
#include "Score.h"



Score::Score()
	: m_pFont(ContentManager::Load<SpriteFont>(L"SpriteFonts/Consolas_32.fnt"))
{
	
}

void Score::Initialize(const SceneContext& )
{

}

void Score::Update(const SceneContext&)
{
	TextRenderer::Get()->DrawText(m_pFont, m_Score, m_Position, m_Color);
}
