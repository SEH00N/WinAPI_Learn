#include "pch.h"
#include "StartWindow.h"
#include "GameScene.h"

StartWindow::StartWindow()
{
}

StartWindow::~StartWindow()
{
}

void StartWindow::Init()
{
	GET_SINGLE(SceneManager)->RegisterScene(L"GameScene", make_shared<GameScene>());
	GET_SINGLE(SceneManager)->LoadScene(L"GameScene");
}

void StartWindow::Update(float dt)
{
}

void StartWindow::Render(HDC hdc)
{
}

void StartWindow::Release()
{
}
