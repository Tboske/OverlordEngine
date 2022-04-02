#pragma once


class ModelTestScene final : public GameScene
{
public:
	ModelTestScene() : GameScene(L"ModelTestScene") {}
	~ModelTestScene() override;
	ModelTestScene(const ModelTestScene&) = delete;
	ModelTestScene(ModelTestScene&&) noexcept = delete;
	ModelTestScene& operator=(const ModelTestScene&) = delete;
	ModelTestScene& operator=(ModelTestScene&&) noexcept = delete;

protected:
	virtual void Initialize() override;
	virtual void Update() override;

private:
	GameObject* m_pChair = nullptr;

};

