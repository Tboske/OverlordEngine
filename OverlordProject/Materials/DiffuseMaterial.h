#pragma once

class DiffuseMaterial : public Material<DiffuseMaterial>
{
public:
	DiffuseMaterial();
	~DiffuseMaterial() override = default;
	DiffuseMaterial(const DiffuseMaterial&) = delete;
	DiffuseMaterial(DiffuseMaterial&&) noexcept = delete;
	DiffuseMaterial& operator=(const DiffuseMaterial&) = delete;
	DiffuseMaterial& operator=(DiffuseMaterial&&) noexcept = delete;

	void SetDiffuseTexture(const std::wstring& assetFile);

protected:
	virtual void InitializeEffectVariables() override;

private:
	TextureData* m_pDiffuseTexture = nullptr;
};
