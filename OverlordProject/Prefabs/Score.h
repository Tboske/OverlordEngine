#pragma once

class Score final : public GameObject
{
public:
	Score();
	~Score() override = default;
	Score(const Score&) = delete;
	Score(Score&&) noexcept = delete;
	Score& operator=(const Score&) = delete;
	Score& operator=(Score&&) noexcept = delete;

	void SetColor(const XMFLOAT3& color) { m_Color = {color.x, color.y, color.z, 1.f}; }
	void SetPosition(const XMFLOAT2& pos) { m_Position = pos; }

protected:
	virtual void Initialize(const SceneContext&) override;
	virtual void Update(const SceneContext&) override;

private:
	SpriteFont* m_pFont;

	XMFLOAT2 m_Position;
	XMFLOAT4 m_Color {1,0,0,1};

	std::wstring m_Score{L"0"};
};

