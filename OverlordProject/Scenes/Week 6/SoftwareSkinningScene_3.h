
class BoneObject;

class SoftwareSkinningScene_3 final : public GameScene
{
public:
	SoftwareSkinningScene_3();
	~SoftwareSkinningScene_3() override;
	SoftwareSkinningScene_3(const SoftwareSkinningScene_3&) = delete;
	SoftwareSkinningScene_3(SoftwareSkinningScene_3&&) noexcept = delete;
	SoftwareSkinningScene_3& operator=(const SoftwareSkinningScene_3&) = delete;
	SoftwareSkinningScene_3& operator=(SoftwareSkinningScene_3&&) noexcept = delete;

protected:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void OnGUI() override;

private:
	BoneObject* m_pBone0{}, * m_pBone1{};

	float m_BoneRotation{};
	int m_RotationSign{ 1 };
	bool m_AutoRotate = true;

	struct VertexSoftwareSkinned
	{
		VertexSoftwareSkinned(XMFLOAT3 pos, XMFLOAT3 norm, XMFLOAT4 color, float blendWeight0, float blendWeight1)
			: transformedVertex{ pos, norm, color }
			, originalVertex( pos, norm, color )
			, blendWeight0(blendWeight0)
			, blendWeight1(blendWeight1)
		{}

		VertexPosNormCol transformedVertex{};
		VertexPosNormCol originalVertex{};
		float blendWeight0;
		float blendWeight1;
	};
	void InitVertices(float length);

	MeshDrawComponent* m_pMeshDraw{};
	std::vector<VertexSoftwareSkinned> m_SkinnedVertices{};
};
