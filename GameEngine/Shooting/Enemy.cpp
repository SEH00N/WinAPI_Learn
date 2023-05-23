#include "pch.h"
#include "Enemy.h"
#include "Bullet.h"
#include "BulletManager.h"

Enemy::Enemy() : Pawn(0, 0)
{
	m_eEnemyType = ENEMY_TYPE::ENEMY_01;
	m_fMaxCreation = 3.0f;
	m_fCreationTime = 1.0f;
	m_fCurrentTime = 0.0f;
	m_fAngle = 90.0f * Deg2Rad;
}

Enemy::Enemy(float x, float y, int width, int height, float scale, float speed, ENEMY_TYPE type) : Pawn(x, y, width, height, scale, speed)
{
	m_eEnemyType = type;
	m_fMaxCreation = 3.0f;
	m_fCreationTime = 1.0f;
	m_fCurrentTime = 0.0f;
	m_fAngle = 90.0f * Deg2Rad;
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{
	m_fCreationTime = rand() % (int)m_fMaxCreation + 1;

	m_imageEnemy = make_shared<Image>();
	if (m_imageEnemy)
	{
		switch (m_eEnemyType)
		{
		case ENEMY_TYPE::ENEMY_01:
			m_imageEnemy = GET_SINGLE(ImageManager)->AddImage(L"Enemy01", L"Resources/Image/Enemy01.bmp");
			break;
		case ENEMY_TYPE::ENEMY_02:
			m_imageEnemy = GET_SINGLE(ImageManager)->AddImage(L"Enemy01", L"Resources/Image/Enemy02.bmp");
			break;
		case ENEMY_TYPE::ENEMY_03:
			m_imageEnemy = GET_SINGLE(ImageManager)->AddImage(L"Enemy01", L"Resources/Image/Enemy03.bmp");
			break;
		case ENEMY_TYPE::ENEMY_04:
			m_imageEnemy = GET_SINGLE(ImageManager)->AddImage(L"Enemy01", L"Resources/Image/Enemy04.bmp");
			break;
		case ENEMY_TYPE::BOSS:
			m_imageEnemy = GET_SINGLE(ImageManager)->AddImage(L"Enemy01", L"Resources/Image/Boss.bmp");
			break;
		default:
			m_imageEnemy = GET_SINGLE(ImageManager)->AddImage(L"Enemy01", L"Resources/Image/Enemy01.bmp");
			break;
		}

		if (m_nWidth == 0)
			m_nWidth = m_imageEnemy->GetWidth();
		if (m_nHeight == 0)
			m_nHeight = m_imageEnemy->GetHeight();
	}

	m_hpBar = make_shared<ProgressBar>();
	if (m_hpBar)
	{
		m_hpBar->Init(
			L"Resources/Image/bar_front_green.bmp",
			L"Resources/Image/bar_back.bmp",
			m_fPosX - (m_nWidth * m_fScale) / 2,
			m_fPosY + (m_nHeight * m_fScale) / 2,
			int(m_nWidth * m_fScale),
			10
		);
	}
}

void Enemy::Update(float dt)
{
	m_fPosX += cosf(m_fAngle) * m_fSpeed * dt;
	m_fPosY += sinf(m_fAngle) * m_fSpeed * dt;

	RECT rt;
	GetClientRect(g_Engine->GetWndHandle(), &rt);
	rt.bottom = rt.bottom / 3;

	if (!PtInRect(&rt, POINT{ (int)m_fPosX, (int)m_fPosY }))
	{
		m_fSpeed *= -1;
	}

	m_fCurrentTime += dt;
	if (m_fCurrentTime > m_fCreationTime)
	{
		CreateBullet();

		m_fCurrentTime = 0.0f;
	}

	if (m_hpBar)
	{
		m_hpBar->SetGauge(m_fHP, m_fMaxHP);
		m_hpBar->SetPosition(m_fPosX - (m_nWidth * m_fScale) / 2, m_fPosY + (m_nHeight * m_fScale) / 2);
	}
}

void Enemy::Render(HDC hdc)
{
	if (m_imageEnemy)
	{
		m_imageEnemy->DrawBitmap(hdc, (int)m_fPosX, (int)m_fPosY, (int)(m_nWidth * m_fScale), (int)(m_nHeight * m_fScale));
	}

	if (m_hpBar)
		m_hpBar->Render(hdc);
}

void Enemy::Release()
{
	if (m_imageEnemy)
		m_imageEnemy->Release();

	if (m_hpBar)
		m_hpBar->Release();
}

void Enemy::CreateBullet()
{
	if (m_eEnemyType == ENEMY_TYPE::ENEMY_01)
	{
		shared_ptr<Bullet> bullet = make_shared<Bullet>(m_fPosX, m_fPosY);

		if (bullet)
		{
			bullet->SetAngle(90.0f * Deg2Rad);
			bullet->SetOwnerPawn(weak_from_this());
			bullet->Init(L"projectile04_3", L"Resources/Image/projectile04_3.bmp");

			GET_SINGLE(BulletManager)->CreateBullet(bullet);
		}
	}
	else if (m_eEnemyType == ENEMY_TYPE::ENEMY_02)
	{
		shared_ptr<Bullet> bullet = make_shared<Bullet>(m_fPosX, m_fPosY);

		if (bullet)
		{
			bullet->SetAngle(90.0f * Deg2Rad);
			bullet->SetOwnerPawn(weak_from_this());
			bullet->Init(L"projectile04_3", L"Resources/Image/projectile04_3.bmp");

			GET_SINGLE(BulletManager)->CreateBullet(bullet);
		}

		shared_ptr<Bullet> bullet2 = make_shared<Bullet>(m_fPosX, m_fPosY + m_nHeight);

		if (bullet2)
		{
			bullet2->SetAngle(90.0f * Deg2Rad);
			bullet2->SetOwnerPawn(weak_from_this());
			bullet2->Init(L"projectile04_3", L"Resources/Image/projectile04_3.bmp");

			GET_SINGLE(BulletManager)->CreateBullet(bullet2);
		}
	}
	else if (m_eEnemyType == ENEMY_TYPE::ENEMY_03)
	{
		shared_ptr<Bullet> bullet = make_shared<Bullet>(m_fPosX, m_fPosY);

		auto player = m_player.lock();
		if (bullet)
		{
			float angle = getAngle(m_fPosX, -m_fPosY, player->GetPosX(), -player->GetPosY());
			bullet->SetAngle(angle);
			bullet->SetOwnerPawn(weak_from_this());
			bullet->Init(L"projectile04_3", L"Resources/Image/projectile04_3.bmp");

			GET_SINGLE(BulletManager)->CreateBullet(bullet);
		}
	}
	else if (m_eEnemyType == ENEMY_TYPE::ENEMY_04)
	{
		shared_ptr<Bullet> bullet = make_shared<Bullet>(m_fPosX, m_fPosY);
		shared_ptr<Bullet> bullet2 = make_shared<Bullet>(m_fPosX, m_fPosY);
		shared_ptr<Bullet> bullet3 = make_shared<Bullet>(m_fPosX, m_fPosY);

		if (bullet)
		{
			bullet->SetAngle(90.0f * Deg2Rad);
			bullet->SetOwnerPawn(weak_from_this());
			bullet->Init(L"projectile04_3", L"Resources/Image/projectile04_3.bmp");

			GET_SINGLE(BulletManager)->CreateBullet(bullet);
		}

		if (bullet2)
		{
			bullet2->SetAngle(105.0f * Deg2Rad);
			bullet2->SetOwnerPawn(weak_from_this());
			bullet2->Init(L"projectile04_3", L"Resources/Image/projectile04_3.bmp");

			GET_SINGLE(BulletManager)->CreateBullet(bullet2);
		}

		if (bullet3)
		{
			bullet3->SetAngle(75.0f * Deg2Rad);
			bullet3->SetOwnerPawn(weak_from_this());
			bullet3->Init(L"projectile04_3", L"Resources/Image/projectile04_3.bmp");

			GET_SINGLE(BulletManager)->CreateBullet(bullet3);
		}
	}
	else if (m_eEnemyType == ENEMY_TYPE::BOSS)
	{
		int nRandom = rand() % 3;
		if (nRandom == 0)
		{
			for (int i = 0; i < 10; ++i)
			{
				shared_ptr<Bullet> bullet = make_shared<Bullet>(m_fPosX, m_fPosY);

				if (bullet)
				{
					bullet->SetAngle(2 * PI / 10 * i);
					bullet->SetOwnerPawn(weak_from_this());
					bullet->Init(L"projectile05_3", L"Resources/Image/projectile05_3.bmp");

					GET_SINGLE(BulletManager)->CreateBullet(bullet);
				}
			}
		}
		else if (nRandom == 1)
		{
			for (int i = 0; i < 10; ++i)
			{
				shared_ptr<Bullet> bullet = make_shared<Bullet>(m_fPosX, m_fPosY);

				if (bullet)
				{
					bullet->SetAngle(PI / 10 * i);
					bullet->SetOwnerPawn(weak_from_this());
					bullet->Init(L"projectile03_3", L"Resources/Image/projectile03_3.bmp");

					GET_SINGLE(BulletManager)->CreateBullet(bullet);
				}
			}
		}
		else
		{
			auto player = m_player.lock();

			for (int i = 0; i < 3; ++i)
			{
				shared_ptr<Bullet> bullet = make_shared<Bullet>(m_fPosX, m_fPosY + m_nHeight * i);

				if (bullet)
				{
					float angle = getAngle(m_fPosX, -m_fPosY, player->GetPosX(), -player->GetPosY());
					bullet->SetAngle(angle);
					bullet->SetOwnerPawn(weak_from_this());
					bullet->Init(L"projectile04_3", L"Resources/Image/projectile04_3.bmp");

					GET_SINGLE(BulletManager)->CreateBullet(bullet);
				}
			}
		}
	}
}
