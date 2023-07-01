#include "pch.h"
#include "BulletManager.h"
#include "Bullet.h"
#include "Pawn.h"

void BulletManager::Init()
{
	m_Bullets.clear();
}

void BulletManager::Update(float dt)
{
	list<shared_ptr<Bullet>>::iterator iter = m_Bullets.begin();
	while ( iter != m_Bullets.end() )
	{
		(*iter)->Update(dt);

		// ������ ���� üũ
		if ((*iter)->IsValid() == false)
		{
			iter = m_Bullets.erase(iter);
			continue;
		}

		// Pawn list �浹 üũ
		bool bErase = false;
		list<weak_ptr<Pawn>>::iterator iterPawn;
		for (iterPawn = m_Pawns.begin(); iterPawn != m_Pawns.end(); ++iterPawn)
		{
			auto pawn = (*iterPawn).lock();
			if (pawn == nullptr)
				continue;

			RECT rt = pawn->GetRect();
			// iter : bullet�� �浹 üũ
			if ((*iter)->IsCollision(rt) == true)
			{
				auto owner = (*iter)->GetOwnerPawn().lock();
				// ���� �߻�ü�� ���� �浹
				if (owner == pawn)
					continue;

				// ���� PawnType ������ �浹 ó�� ����
				if (owner && owner->GetPawnType() == PAWN_TYPE::ENEMY && pawn->GetPawnType() == PAWN_TYPE::ENEMY)
					continue;

				pawn->OnDamageProcess(-(*iter)->GetDamage());
				if (pawn->GetHP() <= 0)
				{
					if (owner)
						owner->OnCollisionProcess(pawn->GetScore());
				}

				iter = m_Bullets.erase(iter);
				bErase = true;
				break;
			}
		}

		if (bErase == false)
			iter++;
	}
}

void BulletManager::Render(HDC hdc)
{
	list<shared_ptr<Bullet>>::iterator iter;
	for (iter = m_Bullets.begin(); iter != m_Bullets.end(); ++iter)
	{
		(*iter)->Render(hdc);
	}
}

void BulletManager::Release()
{
	list<shared_ptr<Bullet>>::iterator iter;
	for (iter = m_Bullets.begin(); iter != m_Bullets.end(); ++iter)
	{
		(*iter)->Release();
	}
	m_Bullets.clear();
}

void BulletManager::CreateBullet(shared_ptr<Bullet> bullet)
{
	if (bullet)
	{
		m_Bullets.push_back(bullet);
	}
}

void BulletManager::AddPawn(weak_ptr<Pawn> pawn)
{
	if (pawn.lock())
	{
		m_Pawns.push_back(pawn);
	}
}

void BulletManager::DeletePawn(weak_ptr<Pawn> pawn)
{
	list<weak_ptr<Pawn>>::iterator iter = m_Pawns.begin();
	while (iter != m_Pawns.end())
	{
		if (pawn.lock() == (*iter).lock())
			iter = m_Pawns.erase(iter);
		else
			iter++;
	}
}
