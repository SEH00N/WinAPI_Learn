#pragma once
#include "Actor.h"

class Pawn : public Actor
{
public:
	Pawn();
	Pawn(float x, float y, int width = 0, int height =  0, float scale = 1.0f, float speed = 100.0f);
	virtual ~Pawn();

public:
	virtual void Init();
	virtual void Update(float dt);
	virtual void Render(HDC hdc);
	virtual void Release();

public:
	RECT		GetRect();

	void		SetSpeed(float speed)	{ m_fSpeed = speed;		}
	void		SetHP(float hp)			{ m_fHP = hp;			}
	void		SetMaxHP(float maxHP)	{ m_fMaxHP = maxHP;		}
	void		SetDamage(float damage) { m_fDamage = damage;	}
	void		AddHP(float hp)			{ m_fHP += hp;			}
	float		GetSpeed()				{ return m_fSpeed;		}
	float		GetHP()					{ return m_fHP;			}
	float		GetMaxHP()				{ return m_fMaxHP;		}
	float		GetDamage()				{ return m_fDamage;		}

protected:
	float		m_fSpeed;
	float		m_fHP;
	float		m_fMaxHP;
	float		m_fDamage;
};

