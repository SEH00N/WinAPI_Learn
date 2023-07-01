#include "pch.h"
#include "ClearScene.h"

ClearScene::ClearScene()
{
}

ClearScene::~ClearScene()
{
}

void ClearScene::Init()
{
	m_imageBG = make_shared<Image>();
	if (m_imageBG)
		m_imageBG = GET_SINGLE(ImageManager)->AddImage(L"IntroBG", L"Resources/UI/BG.bmp", false);

	//m_imageLose = make_shared<Image>();
	//if (m_imageLose)
	//	m_imageLose = GET_SINGLE(ImageManager)->AddImage(L"YouLose", L"Resources/UI/YouLose.bmp");

	m_imageWin = make_shared<Image>();
	if (m_imageWin)
		m_imageWin = GET_SINGLE(ImageManager)->AddImage(L"YouWin", L"Resources/UI/YouWin.bmp");

	m_btnRestart = make_shared<Image>();
	if (m_btnRestart)
		m_btnRestart = GET_SINGLE(ImageManager)->AddImage(L"RestartBTN", L"Resources/UI/Replay_BTN.bmp");

	m_btnExit = make_shared<Image>();
	if (m_btnExit)
		m_btnExit = GET_SINGLE(ImageManager)->AddImage(L"ExitBTN", L"Resources/UI/Exit_BTN.bmp");
}

void ClearScene::Update(float dt)
{
	if (INPUT->GetButtonDown(KEY_TYPE::LCLICK))
	{
		POINT pos = INPUT->GetMousePos();

		RECT rt;
		if (m_btnExit)
		{
			rt = m_btnExit->GetBoundingBox();
			if (PtInRect(&rt, pos))
				PostQuitMessage(0);
		}

		if (m_btnRestart)
		{
			rt = m_btnRestart->GetBoundingBox();
			if (PtInRect(&rt, pos))
				GET_SINGLE(SceneManager)->LoadScene(L"IntroScene");
		}
	}
}

void ClearScene::Render(HDC hdc)
{
	int nWidth = g_Engine->GetWidth();
	int nHeight = g_Engine->GetHeight();

	if (m_imageBG)
		m_imageBG->DrawBitmap(hdc, 0, 0, nWidth, nHeight);

	if (m_imageWin)
		m_imageWin->DrawBitmap(hdc, nWidth / 2, nHeight / 3, 400, 50);

	if (m_btnRestart)
		m_btnRestart->DrawBitmap(hdc, nWidth / 2, nHeight - 170, 200, 60);

	if (m_btnExit)
		m_btnExit->DrawBitmap(hdc, nWidth / 2, nHeight - 100, 200, 60);
}

void ClearScene::Release()
{
	if (m_imageBG)
		m_imageBG->Release();

	if (m_imageWin)
		m_imageWin->Release();

	//if (m_imageLose)
	//	m_imageLose->Release();

	if (m_btnRestart)
		m_btnRestart->Release();

	if (m_btnExit)
		m_btnExit->Release();

	GET_SINGLE(ImageManager)->DeleteAll();
}
