#include "pch.h"
#include "IntroScene.h"

IntroScene::IntroScene()
{
}

IntroScene::~IntroScene()
{
}

void IntroScene::Init()
{
	m_imageBG = make_shared<Image>();
	if (m_imageBG)
		m_imageBG = GET_SINGLE(ImageManager)->AddImage(L"IntroBG", L"Resources/UI/BG.bmp", false);

	m_imageTitle = make_shared<Image>();
	if (m_imageTitle)
		m_imageTitle = GET_SINGLE(ImageManager)->AddImage(L"IntroTitle", L"Resources/UI/SpaceGame.bmp");

	m_imageShip = make_shared<Image>();
	if (m_imageShip)
		m_imageShip = GET_SINGLE(ImageManager)->AddImage(L"IntroShip", L"Resources/UI/StarShip.bmp");

	m_btnPlay = make_shared<Image>();
	if (m_btnPlay)
		m_btnPlay = GET_SINGLE(ImageManager)->AddImage(L"PlayBTN", L"Resources/UI/Play_BTN.bmp");

	m_btnExit = make_shared<Image>();
	if (m_btnExit)
		m_btnExit = GET_SINGLE(ImageManager)->AddImage(L"ExitBTN", L"Resources/UI/Exit_BTN.bmp");
}

void IntroScene::Update(float dt)
{
	if (INPUT->GetButtonDown(KEY_TYPE::LCLICK))
	{
		POINT pos = INPUT->GetMousePos();

		RECT rt;
		if (m_btnExit)
		{
			rt= m_btnExit->GetBoundingBox();
			if (PtInRect(&rt, pos))
				PostQuitMessage(0);
		}

		if (m_btnPlay)
		{
			rt = m_btnPlay->GetBoundingBox();
			if (PtInRect(&rt, pos))
				GET_SINGLE(SceneManager)->LoadScene(L"GameScene");
		}
	}
}

void IntroScene::Render(HDC hdc)
{
	int nWidth = g_Engine->GetWidth();
	int nHeight = g_Engine->GetHeight();

	if (m_imageBG)
		m_imageBG->DrawBitmap(hdc, 0, 0, nWidth, nHeight);

	if (m_imageTitle)
		m_imageTitle->DrawBitmap(hdc, nWidth / 2, nHeight / 5, 400, 50);

	if (m_imageShip)
		m_imageShip->DrawBitmap(hdc, nWidth / 2, nHeight / 2);

	if (m_btnPlay)
		m_btnPlay->DrawBitmap(hdc, nWidth / 2, nHeight - 170, 200, 60);

	if (m_btnExit)
		m_btnExit->DrawBitmap(hdc, nWidth / 2, nHeight - 100, 200, 60);
}

void IntroScene::Release()
{
	if (m_imageBG)
		m_imageBG->Release();

	if (m_imageTitle)
		m_imageTitle->Release();

	if (m_imageShip)
		m_imageShip->Release();

	if (m_btnPlay)
		m_btnPlay->Release();

	if (m_btnExit)
		m_btnExit->Release();

	GET_SINGLE(ImageManager)->DeleteAll();
}
