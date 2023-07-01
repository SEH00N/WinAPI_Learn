#pragma once
#include "Scene.h"
#include "BackGround.h"

class Player;
class GameScene : public Scene
{
public:
	GameScene();
	virtual ~GameScene();

public:
	void Init() override;
	void Update(float dt)override;
	void Render(HDC hdc)override;
	void Release()override;

private:
	shared_ptr<Player>	m_player;
	shared_ptr<BackGround>	m_scrollBG;

	GAME_STAGE			m_eStage;
};

