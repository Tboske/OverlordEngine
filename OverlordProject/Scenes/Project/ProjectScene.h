#pragma once

class Player;
class Character;
class DiffuseMaterial_Skinned;
class BoxMaterial;
class DiffuseMaterial;
class UberMaterial;



class ProjectScene : public GameScene
{
public:
	ProjectScene() : GameScene(L"ProjectScene") {}
	~ProjectScene() override;
	ProjectScene(const ProjectScene& other) = delete;
	ProjectScene(ProjectScene&& other) noexcept = delete;
	ProjectScene& operator=(const ProjectScene& other) = delete;
	ProjectScene& operator=(ProjectScene&& other) noexcept = delete;

	GameObject* GetArena() const { return m_pArena; }

protected:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void OnGUI() override;

private:
	GameObject* m_pArena = nullptr;
	
	UberMaterial* m_pFloorMaterial = nullptr;
	BoxMaterial* m_pBoxMaterial = nullptr;
	BoxMaterial* m_pBlockMaterial = nullptr;

	Player* m_pPlayer1 = nullptr;
	Player* m_pPlayer2 = nullptr;
	FixedCamera* m_pCamera = nullptr;

	void InitArena(PxMaterial* );
	void InitBox(PxMaterial* physxMat, float x, float z);

	void InitBlock(PxMaterial* physxMat, float x, float z);
	void InitRock(DiffuseMaterial* pRockMat, float x, float z, float scale, float rot = 0);
};

