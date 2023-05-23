#include "pch.h"
#include "GameScene.h"
#include "Player.h"
#include "Enemy.h"
#include "BulletManager.h"
#include "EnemyManager.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	srand((unsigned int)time(NULL));

	m_player = make_shared<Player>(250, 600);

	if (m_player)
		m_player->Init();	

	GET_SINGLE(BulletManager)->Init();
	GET_SINGLE(EnemyManager)->Init(m_player, GAME_STAGE::BOSS);
}

void GameScene::Update(float dt)
{
	if (m_player)
		m_player->Update(dt);

	GET_SINGLE(BulletManager)->Update(dt);
	GET_SINGLE(EnemyManager)->Update(dt);
}

void GameScene::Render(HDC hdc)
{
	if (m_player)
		m_player->Render(hdc);

	GET_SINGLE(BulletManager)->Render(hdc);
	GET_SINGLE(EnemyManager)->Render(hdc);
}

void GameScene::Release()
{
	if (m_player)
		m_player->Release();

	GET_SINGLE(BulletManager)->Release();
	GET_SINGLE(EnemyManager)->Release();
}
