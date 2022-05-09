#include "stdafx.h"
#include "BoxMaterial.h"


BoxMaterial::BoxMaterial()
	: Material<BoxMaterial>(L"Effects/Project/BoxShader.fx")
{
	
}

void BoxMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gDiffuseMap", m_pDiffuseTexture);
}

void BoxMaterial::SetNormalMap(const std::wstring& assetFile)
{
	m_pNormalTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gNormalMap", m_pNormalTexture);
}

void BoxMaterial::SetColorAdd(float r, float g, float b)
{
	SetVariable_Vector(L"gColorAdd", XMFLOAT3{r, g, b});
}

void BoxMaterial::InitializeEffectVariables()
{
	
}
