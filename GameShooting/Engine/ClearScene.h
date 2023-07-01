#pragma once

#include "Scene.h"

class ClearScene : public Scene
{
public:
	ClearScene();
	virtual ~ClearScene();

public:
	void Init() override;
	void Update(float dt)override;
	void Render(HDC hdc)override;
	void Release()override;

private:
	shared_ptr<Image> m_imageBG;
	//shared_ptr<Image> m_imageLose;
	shared_ptr<Image> m_imageWin;
	shared_ptr<Image> m_btnRestart;
	shared_ptr<Image> m_btnExit;
};

