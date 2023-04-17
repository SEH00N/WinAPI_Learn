#pragma once
#include "H00NEngine.h"

class Game : public H00NEngine
{
public:
	Game();
	virtual ~Game();

public:
	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;
};

