
class BoneObject;

class SoftwareSkinningScene_1 final : public GameScene
{
public:
	SoftwareSkinningScene_1();
	~SoftwareSkinningScene_1() override;
	SoftwareSkinningScene_1(const SoftwareSkinningScene_1&) = delete;
	SoftwareSkinningScene_1(SoftwareSkinningScene_1&&) noexcept = delete;
	SoftwareSkinningScene_1& operator=(const SoftwareSkinningScene_1&) = delete;
	SoftwareSkinningScene_1& operator=(SoftwareSkinningScene_1&&) noexcept = delete;

protected:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void OnGUI() override;

private:
	BoneObject* m_pBone0{}, * m_pBone1{};

	float m_BoneRotation{};
	int m_RotationSign{ 1 };
	bool m_AutoRotate = true;
};
