#pragma once

class BoxMaterial : public Material<BoxMaterial>
{
public:
	BoxMaterial();
	~BoxMaterial() override = default;
	BoxMaterial(const BoxMaterial&) = delete;
	BoxMaterial(BoxMaterial&&) noexcept = delete;
	BoxMaterial& operator=(const BoxMaterial&) = delete;
	BoxMaterial& operator=(BoxMaterial&&) noexcept = delete;

	void SetDiffuseTexture(const std::wstring& assetFile);
	void SetNormalMap(const std::wstring& assetFile);
	void SetColorAdd(float r, float g, float b);

protected:
	virtual void InitializeEffectVariables() override;

private:
	TextureData* m_pDiffuseTexture = nullptr;
	TextureData* m_pNormalTexture = nullptr;
};
