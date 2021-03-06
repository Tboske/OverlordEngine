#pragma once

class Text;
class Player;
class Character;
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
	virtual void OnSceneActivated() override;
	virtual void OnSceneDeactivated() override;
	virtual void OnGUI() override;

private:
	GameObject* m_pArena = nullptr;
	
	UberMaterial* m_pFloorMaterial = nullptr;
	BoxMaterial* m_pBoxMaterial = nullptr;
	BoxMaterial* m_pBlockMaterial = nullptr;

	std::vector<Player*> m_Players;

	Text* m_pEndGameScreen = nullptr;
	Text* m_pContinueScreen = nullptr;
	bool m_GameEnded = false;


	FMOD::Channel* m_pBackgroundMusic = nullptr;
	FixedCamera* m_pCamera = nullptr;

	void InitArena(PxMaterial* );
	void InitBox(PxMaterial* physxMat, float x, float z);

	void InitBlock(PxMaterial* physxMat, float x, float z);
	void InitRock(DiffuseMaterial* pRockMat, float x, float z, float scale, float rot = 0);
};

