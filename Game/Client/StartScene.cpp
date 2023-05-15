#include "pch.h"
#include "StartScene.h"

StartScene::StartScene()
{
}

StartScene::~StartScene()
{
}

void StartScene::Init()
{
	m_posX = 100.0f;
	m_posY = 100.0f;

	m_offsetY = 0;
	m_offsetSpeed = 200;

	GET_SINGLE(SoundManager)->AddSound(_T("Attack"), _T("../Resources/Sound/Character_attack.wav"));
	GET_SINGLE(SoundManager)->AddSound(_T("BGM"), _T("../Resources/Sound/Thinking Out Loud.WAV"), true, true);
	GET_SINGLE(SoundManager)->AddSound(_T("Linda"), _T("../Resources/Sound/Linda.mp3"), true, true);

	GET_SINGLE(SoundManager)->Play(_T("BGM"), 0.5f);

	m_player = GET_SINGLE(ImageManager)->AddImage(_T("Player"), _T("../Resources/Image/Pikachu.bmp"));
	m_map = GET_SINGLE(ImageManager)->AddImage(_T("BGImage"), _T("../Resources/Image/bgImage.bmp"));
	m_map->SetCenter(FALSE);

	m_progressBar = make_shared<ProgressBar>();

	if (m_progressBar)
	{

























		m_progressBar->Init(L"../Resources/Image/bar_front.bmp", L"../Resources/Image/bar_back.bmp", 0, 500, 500, 30);
	}
}

void StartScene::Update(float dt)
{
	if (INPUT->GetButton(KEY_TYPE::UP))
		m_posY -= 100.0f * dt;
	if (INPUT->GetButton(KEY_TYPE::DOWN))
		m_posY += 100.0f * dt;
	if (INPUT->GetButton(KEY_TYPE::RIGHT))
		m_posX += 100.0f * dt;
	if (INPUT->GetButton(KEY_TYPE::LEFT))
		m_posX -= 100.0f * dt;

	m_offsetY -= m_offsetSpeed * dt;
}

void StartScene::Render(HDC hdc)
{
	RECT rtDraw = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	if (m_map)
		m_map->DrawLoopBitmap(hdc, &rtDraw, 0, m_offsetY);	
	if (m_player)
	{
		m_player->DrawBitmap(hdc, 200, 100, 200, 200);

		RECT rc = m_player->GetBoundingBox();
		SelectObject(hdc, GetStockObject(NULL_BRUSH));
		Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

		m_player->DrawBitmap(hdc, 200, 250);

		m_player->DrawAlpha(hdc, 600, 300, 100, 200, 200);
		m_player->DrawAlpha(hdc, 600, 450, 150);

		m_player->DrawRotate(hdc, 600, 400, 45, 200, 200);
		//m_player->DrawRotate(hdc, 600, 550, 100, 200, 200);
	}

	if (m_progressBar)
		m_progressBar->Render(hdc);
}

void StartScene::Release()
{
}
