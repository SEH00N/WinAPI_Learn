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

	GET_SINGLE(SoundManager)->AddSound(_T("Attack"), _T("../Resources/Sound/Character_attack.wav"));
	GET_SINGLE(SoundManager)->AddSound(_T("BGM"), _T("../Resources/Sound/Thinking Out Loud.WAV"), true, true);
	GET_SINGLE(SoundManager)->AddSound(_T("Linda"), _T("../Resources/Sound/Linda.mp3"), true, true);

	GET_SINGLE(SoundManager)->Play(_T("BGM"), 0.5f);

	m_player = GET_SINGLE(ImageManager)->AddImage(_T("Player"), _T("../Resources/Image/Pikachu.bmp"));
	m_map = GET_SINGLE(ImageManager)->AddImage(_T("Map"), _T("../Resources/Image/Map.bmp"));
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
}

void StartScene::Render(HDC hdc)
{
	Rectangle(hdc, m_posX, m_posY, m_posX + 100, m_posY + 100);
	
	if (m_map)
		m_map->DrawBitmap(hdc, 0, 0, 800, 800);	
	if (m_player)
	{
		m_player->DrawBitmap(hdc, 200, 100, 200, 200);
		m_player->DrawBitmap(hdc, 200, 250);

		m_player->DrawAlpha(hdc, 600, 300, 100, 200, 200);
		m_player->DrawAlpha(hdc, 600, 450, 150);

		m_player->DrawRotate(hdc, 600, 400, 45, 200, 200);
		//m_player->DrawRotate(hdc, 600, 550, 100, 200, 200);
	}
}

void StartScene::Release()
{
}
