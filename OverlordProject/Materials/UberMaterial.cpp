#include "stdafx.h"
#include "UberMaterial.h"

UberMaterial::UberMaterial()
	: Material(L"Effects/UberShader.fx")
{
	
}
void UberMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gTextureDiffuse", m_pDiffuseTexture);
}

void UberMaterial::SetSpecularTexture(const std::wstring& assetFile)
{
	m_pSpecularTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gTextureSpecularIntensity", m_pSpecularTexture);
}

void UberMaterial::SetNormalTexture(const std::wstring& assetFile)
{
	m_pNormalTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gTextureNormal", m_pNormalTexture);
}

void UberMaterial::SetCubeMap(const std::wstring& assetFile)
{
	m_pCubeMap = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gCubeEnvironment", m_pCubeMap);
}

void UberMaterial::SetOpacityMap(const std::wstring& assetFile)
{
	m_pOpacityMap = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gTextureOpacity", m_pOpacityMap);
}

void UberMaterial::InitializeEffectVariables()
{
	SetVariable_Vector(L"gLightDirection", { 0.577f, 0.577f, 0.577f });

	//diffuse
	SetVariable_Scalar(L"gUseTextureDiffuse", true);
	SetVariable_Vector(L"gColorDiffuse", { 1,1,1,1 });

	//specular
	SetVariable_Vector(L"gColorSpecular", { 1,1,1,1 });
	SetVariable_Scalar(L"gUseTextureSpecularIntensity", true);
	SetVariable_Scalar(L"gShininess", 40);
	SetVariable_Scalar(L"gUseSpecularBlinn", true);
	SetVariable_Scalar(L"gUseSpecularPhong", true);

	//ambient
	SetVariable_Vector(L"gColorAmbient", { 0.842f, 0.556f, 0.387f, 1.f });
	SetVariable_Scalar(L"gAmbientIntensity", 0);

	//normals
	SetVariable_Scalar(L"gFlipGreenChannel", false);
	SetVariable_Scalar(L"gUseTextureNormal", true);

	//cubemap
	SetVariable_Scalar(L"gUseEnvironmentMapping", true);
	SetVariable_Scalar(L"gReflectionStrength", 0.6f);
	SetVariable_Scalar(L"gRefractionStrength", 0.1f);
	SetVariable_Scalar(L"gRefractionIndex", 0.3f);

	//fresnel
	SetVariable_Scalar(L"gUseFresnelFalloff", true);
	SetVariable_Scalar(L"gFresnelPower", 1.7f);
	SetVariable_Scalar(L"gFresnelHardness", 0.2f);
	
}
