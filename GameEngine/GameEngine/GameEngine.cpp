// GameEngine.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "GameEngine.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE g_hInst;                                // 현재 인스턴스입니다.
HWND g_hWnd;
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void Init();
void Update(float dt);
void Render();
void Release();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GAMEENGINE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAMEENGINE));

    MSG msg;
    memset(&msg, 0, sizeof(msg));

    Init();

    while (true)
    {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == msg.message)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			static float fStartTime = (float)timeGetTime();
            float fNowTime = (float)timeGetTime();
            float fDeltaTime = (fNowTime - fStartTime) * 0.001f;

            //if (fDeltaTime >= (1.0f / 60.0f))
            //{
                Update(fDeltaTime);
                Render();

                fStartTime = fNowTime;
            //}

        }
    }

    Release();
    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAMEENGINE));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;//MAKEINTRESOURCEW(IDC_GAMEENGINE);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    g_hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    g_hWnd = hWnd;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

HDC g_hBackDC;
HBITMAP g_hBackBitmap, g_hOldBackBitmap;
HBITMAP g_hPlayerBitmap;
RECT g_rtWindow;

struct fPoint {
    float x;
    float y;
};

fPoint g_ptPlayer = { 100.0f, 100.0f };
POINT g_ptMouse;

void Init()
{
    HDC hdc = GetDC(g_hWnd);
    g_hBackDC = CreateCompatibleDC(hdc);

    GetClientRect(g_hWnd, &g_rtWindow);
    g_hBackBitmap = CreateCompatibleBitmap(hdc, g_rtWindow.right, g_rtWindow.bottom);
    g_hOldBackBitmap = (HBITMAP)SelectObject(g_hBackDC, g_hBackBitmap);

    g_hPlayerBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
    ReleaseDC(g_hWnd, hdc);
}

void Update(float dt)
{
    static float elapsedTime = 0.0f;
    static int frameCount = 0;

    frameCount++;
    elapsedTime += dt;
    if (elapsedTime >= 1.0f)
    {
        static TCHAR szTemp[256];
        wsprintf(szTemp, _T("FPS : %d"), frameCount);
        SetWindowText(g_hWnd, szTemp);

        frameCount = 0;
        elapsedTime = 0.0f;
    }


    if (GetAsyncKeyState(VK_UP) && 0x8000)
    {
        g_ptPlayer.y -= 100.0f * dt;
    }
    if (GetAsyncKeyState(VK_DOWN) && 0x8000)
    {
        g_ptPlayer.y += 100.0f * dt;
    }
    if (GetAsyncKeyState(VK_LEFT) && 0x8000)
    {
        g_ptPlayer.x -= 100.0f * dt;
    }
    if (GetAsyncKeyState(VK_RIGHT) && 0x8000)
    {
        g_ptPlayer.x += 100.0f * dt;
    }

    GetCursorPos(&g_ptMouse);
    
    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
    {
        g_ptMouse.x = g_ptPlayer.x;
        g_ptMouse.y = g_ptPlayer.y;

        ClientToScreen(g_hWnd, &g_ptMouse);
        SetCursorPos(g_ptMouse.x, g_ptMouse.y);
    }
}

void Render()
{
    HDC hdc;
    hdc = GetDC(g_hWnd);

    FillRect(g_hBackDC, &g_rtWindow, (HBRUSH)COLOR_BACKGROUND);

    HDC hMemDC = CreateCompatibleDC(hdc);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(hMemDC, g_hPlayerBitmap);
    
    BITMAP bit;
    GetObject(g_hPlayerBitmap, sizeof(BITMAP), &bit);

    GdiTransparentBlt(g_hBackDC, g_ptPlayer.x, g_ptPlayer.y, bit.bmWidth, bit.bmHeight, hMemDC, 0, 0, bit.bmWidth, bit.bmHeight, RGB(255, 0, 255));

    TCHAR str[128];
    wsprintf(str, _T("윈도우 기준 좌표 (%04d, %04d)"), g_ptMouse.x, g_ptMouse.y);
    TextOut(g_hBackDC, 10, 10, str, lstrlen(str));

    ScreenToClient(g_hWnd, &g_ptMouse);
    wsprintf(str, _T("프로그램 기준 좌표 (%04d, %04d)"), g_ptMouse.x, g_ptMouse.y);
    TextOut(g_hBackDC, 10, 30, str, lstrlen(str));

    BitBlt(hdc, 0, 0, g_rtWindow.right, g_rtWindow.bottom, g_hBackDC, 0, 0, SRCCOPY);

    SelectObject(hMemDC, oldBitmap);
    //DeleteObject(oldBitmap);

    ReleaseDC(g_hWnd, hdc);
}

void Release()
{
    DeleteObject(g_hPlayerBitmap);
    DeleteObject(g_hBackBitmap);
    DeleteDC(g_hBackDC);
}
