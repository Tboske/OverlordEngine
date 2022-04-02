
class BoneObject;

class SoftwareSkinningScene_2 final : public GameScene
{
public:
	SoftwareSkinningScene_2();
	~SoftwareSkinningScene_2() override;
	SoftwareSkinningScene_2(const SoftwareSkinningScene_2&) = delete;
	SoftwareSkinningScene_2(SoftwareSkinningScene_2&&) noexcept = delete;
	SoftwareSkinningScene_2& operator=(const SoftwareSkinningScene_2&) = delete;
	SoftwareSkinningScene_2& operator=(SoftwareSkinningScene_2&&) noexcept = delete;

protected:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void OnGUI() override;

private:
	BoneObject* m_pBone0{}, * m_pBone1{};

	float m_BoneRotation{};
	int m_RotationSign{ 1 };
	bool m_AutoRotate = false;

	struct VertexSoftwareSkinned
	{
		VertexSoftwareSkinned(XMFLOAT3 pos, XMFLOAT3 norm, XMFLOAT4 color)
			: transformedVertex{ pos, norm, color }
		{}

		VertexPosNormCol transformedVertex{};
		VertexPosNormCol originalVertex{};
	};
	void InitVertices(float length);

	MeshDrawComponent* m_pMeshDraw{};
	std::vector<VertexSoftwareSkinned> m_SkinnedVertices{};
};
