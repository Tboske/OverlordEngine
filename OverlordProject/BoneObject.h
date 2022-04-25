#pragma once

class BoneObject : public GameObject
{
public:
	BoneObject(BaseMaterial* pMaterial, float length = 5.f);
	~BoneObject() override;
	BoneObject(const BoneObject&) = delete;
	BoneObject(BoneObject&&) noexcept = delete;
	BoneObject& operator=(const BoneObject&) = delete;
	BoneObject& operator=(BoneObject&&) noexcept = delete;

	void AddBone(BoneObject* pBone);

	const XMFLOAT4X4& GetBindPose() const { return m_BindPose; }
	void CalculateBindPose();

protected:
	virtual void Initialize(const SceneContext&) override;

private:
	float m_Length{};
	BaseMaterial* m_pMaterial{};

	XMFLOAT4X4 m_BindPose{};
};

