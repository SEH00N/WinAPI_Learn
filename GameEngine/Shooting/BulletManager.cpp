#include "pch.h"
#include "BulletManager.h"
#include "Bullet.h"

void BulletManager::Init()
{
	m_bullets.clear();
}

void BulletManager::Update(float dt)
{
	list<shared_ptr<Bullet>>::iterator iter = m_bullets.begin();

	while (iter != m_bullets.end())
	{
		(*iter)->Update(dt);

		if ((*iter)->IsValid() == false)
			iter = m_bullets.erase(iter);
		else
			iter++;
	}
}

void BulletManager::Render(HDC hdc)
{
	list<shared_ptr<Bullet>>::iterator iter;

	for (iter = m_bullets.begin(); iter != m_bullets.end(); ++iter)
		(*iter)->Render(hdc);
}

void BulletManager::Release()
{
	list<shared_ptr<Bullet>>::iterator iter;

	for (iter = m_bullets.begin(); iter != m_bullets.end(); ++iter)
		(*iter)->Release();

	m_bullets.clear();
}

void BulletManager::CreateBullet(shared_ptr<Bullet> bullet)
{
	if (bullet)
	{
		m_bullets.push_back(bullet);
	}
}
