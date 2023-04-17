#include "pch.h"
#include "H00NEngine.h"

H00NEngine::H00NEngine() : m_hBackDC(NULL), m_hBitmap(NULL)
{
	
}

H00NEngine::~H00NEngine()
{

}

void H00NEngine::Startup()
{
	g_Engine = shared_from_this();

	HDC hdc = GetDC(m_hWnd);

	m_hBackDC = CreateCompatibleDC(hdc);
	m_hBitmap = CreateCompatibleBitmap(hdc, m_nWidth, m_nHeight);

	SelectObject(m_hBackDC, m_hBitmap);

	ReleaseDC(m_hWnd, hdc);

	Init();
}

void H00NEngine::MainUpdate()
{
	float fDt = 0.0f;
	Update(fDt);
}

void H00NEngine::MainRender()
{
	HDC hdc = GetDC(m_hWnd);
	RECT winRect{ 0, 0, (LONG)m_nWidth, (LONG)m_nHeight };
	FillRect(m_hBackDC, &winRect, (HBRUSH)COLOR_BACKGROUND);

	Render(m_hBackDC);

	BitBlt(hdc, 0, 0, m_nWidth, m_nHeight, m_hBackDC, 0, 0, SRCCOPY);
	ReleaseDC(m_hWnd, hdc);
}

void H00NEngine::Cleanup()
{
	DeleteObject(m_hBitmap);
	DeleteDC(m_hBackDC);

	Release();
}

void H00NEngine::Init()
{
}

void H00NEngine::Update(float dt)
{
}

void H00NEngine::Render(HDC hdc)
{
}

void H00NEngine::Release()
{
}
