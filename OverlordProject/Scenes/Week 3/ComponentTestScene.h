#pragma once


class ComponentTestScene : public GameScene
{
public:
	ComponentTestScene() : GameScene(L"ComponentTestScene") {}
	~ComponentTestScene() = default;
	ComponentTestScene(const ComponentTestScene&) = delete;
	ComponentTestScene(ComponentTestScene&&) noexcept = delete;
	ComponentTestScene& operator=(const ComponentTestScene&) = delete;
	ComponentTestScene& operator=(ComponentTestScene&&) noexcept = delete;


protected:
	virtual void Initialize() override;

private:

};

