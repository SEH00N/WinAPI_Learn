#include "pch.h"
#include "EnemyManager.h"
#include "Pawn.h"
#include "Enemy.h"

void EnemyManager::Init(weak_ptr<Pawn> player, GAME_STAGE stage)
{
	m_player = player;
	CreateEnemy(stage);
}

void EnemyManager::Update(float dt)
{
	list<shared_ptr<Enemy>>::iterator iter = m_enemies.begin();
	while (iter != m_enemies.end())
	{
		(*iter)->Update(dt);

		if ((*iter)->GetHP() <= 0)
		{
			iter = m_enemies.erase(iter);
		}
		else
			iter++;
	}
}

void EnemyManager::Render(HDC hdc)
{
	list<shared_ptr<Enemy>>::iterator iter;
	for (iter = m_enemies.begin(); iter != m_enemies.end(); ++iter)
		(*iter)->Render(hdc);
}

void EnemyManager::Release()
{
	list<shared_ptr<Enemy>>::iterator iter;
	for (iter = m_enemies.begin(); iter != m_enemies.end(); ++iter)
		(*iter)->Release();

	m_enemies.clear();
}

void EnemyManager::CreateEnemy(GAME_STAGE stage)
{
	if (m_enemies.empty() == false)
		m_enemies.clear();

	switch (stage)
	{
	case GAME_STAGE::STAGE_01:
		for (int i = 0; i < 3; ++i)
		{
			shared_ptr<Enemy> enemy = make_shared<Enemy>(100 + i * 100, 100, 0, 0, 1.0f, 50.0f, ENEMY_TYPE::ENEMY_01);
			if (enemy)
			{
				enemy->SetPlayer(m_player);
				enemy->SetMaxHP(10);
				enemy->SetHP(10);
				enemy->SetMaxCreation(3.0f);
				enemy->SetAngle(0.0f * Deg2Rad);
				enemy->Init();
				m_enemies.push_back(enemy);
			}
		}
		break;
	case GAME_STAGE::STAGE_02:
		for (int i = 0; i < 5; ++i)
		{
			shared_ptr<Enemy> enemy = make_shared<Enemy>(40 + i * 100, 100, 0, 0, 1.0f, 50.0f, ENEMY_TYPE::ENEMY_02);
			if (enemy)
			{
				enemy->SetPlayer(m_player);
				enemy->SetMaxHP(30);
				enemy->SetHP(30);
				enemy->SetMaxCreation(3.0f);
				enemy->SetAngle(0.0f * Deg2Rad);
				enemy->Init();
				m_enemies.push_back(enemy);
			}
		}
		break;
	case GAME_STAGE::STAGE_03:
		for (int i = 0; i < 7; ++i)
		{
			shared_ptr<Enemy> enemy = make_shared<Enemy>(100 + i * 50, 100, 0, 0, 1.0f, 50.0f, ENEMY_TYPE::ENEMY_03);
			if (enemy)
			{
				enemy->SetPlayer(m_player);
				enemy->SetMaxHP(50);
				enemy->SetHP(50);
				enemy->SetMaxCreation(2.0f);
				enemy->SetAngle(90.0f * Deg2Rad);
				enemy->Init();
				m_enemies.push_back(enemy);
			}
		}
		break;
	case GAME_STAGE::STAGE_04:
		for (int i = 0; i < 4; ++i)
		{
			shared_ptr<Enemy> enemy = make_shared<Enemy>(100 + i * 100, 100, 0, 0, 1.0f, 50.0f, ENEMY_TYPE::ENEMY_04);
			if (enemy)
			{
				enemy->SetPlayer(m_player);
				enemy->SetMaxHP(100);
				enemy->SetHP(100);
				enemy->SetMaxCreation(3.0f);
				enemy->SetAngle(90.0f * Deg2Rad);
				enemy->Init();
				m_enemies.push_back(enemy);
			}
		}
		break;
	case GAME_STAGE::BOSS:
	{
		shared_ptr<Enemy> enemy = make_shared<Enemy>(g_Engine->GetWidth() / 2, 100, 0, 0, 1.0f, 50.0f, ENEMY_TYPE::BOSS);
		if (enemy)
		{
			enemy->SetPlayer(m_player);
			enemy->SetMaxHP(1000);
			enemy->SetHP(1000);
			enemy->SetMaxCreation(1.0f);
			enemy->SetAngle(0.0f * Deg2Rad);
			enemy->Init();
			m_enemies.push_back(enemy);
		}
	}
		break;
	default:
		break;
	}
}
