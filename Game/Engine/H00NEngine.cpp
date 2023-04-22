#include "pch.h"
#include "H00NEngine.h"
#include "SceneManager.h"

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

	// 변수 초기화
	m_timer = make_shared<Timer>();
	if (m_timer)
		m_timer->Init();

	m_input = make_shared<Input>();
	if (m_input)
		m_input->Init(m_hWnd);

	Init();
}

void H00NEngine::MainUpdate()
{
	if (!m_timer)
		return;

	m_timer->Update();
	UINT fps = m_timer->GetFPS();

	WCHAR text[100] = _T("");
	::wsprintf(text, _T("FPS : %d"), fps);
	::SetWindowText(m_hWnd, text);

	if(m_input)
		m_input->Update();

	float fDt = m_timer->GetDeltaTime();

	GET_SINGLE(SceneManager)->Update(fDt);

	Update(fDt);
}

void H00NEngine::MainRender()
{
	HDC hdc = GetDC(m_hWnd);
	RECT winRect{ 0, 0, (LONG)m_nWidth, (LONG)m_nHeight };
	FillRect(m_hBackDC, &winRect, (HBRUSH)COLOR_BACKGROUND);

	GET_SINGLE(SceneManager)->Render(m_hBackDC);

	Render(m_hBackDC);

	BitBlt(hdc, 0, 0, m_nWidth, m_nHeight, m_hBackDC, 0, 0, SRCCOPY);
	ReleaseDC(m_hWnd, hdc);
}

void H00NEngine::Cleanup()
{
	DeleteObject(m_hBitmap);
	DeleteDC(m_hBackDC);

	GET_SINGLE(SceneManager)->Release();

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
