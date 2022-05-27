#pragma once


class ExplosionMaterial : public Material<ExplosionMaterial>
{
public:
	ExplosionMaterial();
	~ExplosionMaterial() override = default;
	ExplosionMaterial(const ExplosionMaterial& other) = delete;
	ExplosionMaterial(ExplosionMaterial&& other) noexcept = delete;
	ExplosionMaterial& operator=(const ExplosionMaterial& other) = delete;
	ExplosionMaterial& operator=(ExplosionMaterial&& other) noexcept = delete;

protected:
	virtual void InitializeEffectVariables() override;

};

