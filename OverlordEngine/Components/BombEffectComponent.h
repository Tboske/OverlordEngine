#pragma once
#include "BaseComponent.h"

struct ExplosionSettings
{
	float duration;
	float intensity;
};

class ExplosionMaterial;
class BombEffectComponent final : public BaseComponent
{
public:

	BombEffectComponent(const std::wstring& assetFile);
	~BombEffectComponent() override;
	BombEffectComponent(const BombEffectComponent& other) = delete;
	BombEffectComponent(BombEffectComponent&& other) noexcept = delete;
	BombEffectComponent& operator=(const BombEffectComponent& other) = delete;
	BombEffectComponent& operator=(BombEffectComponent&& other) noexcept = delete;

	
	void ActivateEffect(const std::vector<ExplosionVertex>& positions);
	bool IsActive() const { return m_IsActive; }

protected:
	virtual void Initialize(const SceneContext& sceneContext) override;
	virtual void Update(const SceneContext&) override;
	virtual void Draw(const SceneContext&) override;

private:
	TextureData* m_pExplosionTexture{};
	static ExplosionMaterial* m_pMaterial;
	std::wstring m_AssetFile;
	float m_ElapsedVal;
	bool m_IsActive = false;

	ExplosionSettings m_ExplosionSettings{};

	std::vector<ExplosionVertex> m_pExplosionArray;
	ID3D11Buffer* m_pVertexBuffer{};

	void CreateVertexBuffer(const SceneContext& sceneContext);
	void UpdateBuffer() const;
};

