// Animation.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Animation.h"
#include "resource.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
    LoadStringW(hInstance, IDC_ANIMATION, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ANIMATION));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ANIMATION));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = nullptr;//MAKEINTRESOURCEW(IDC_ANIMATION);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

#define ANIMATION_X_COUNT 4
#define ANIMATION_Y_COUNT 4

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HDC hBackDC;
    static HBITMAP hBackBitmap, hOldBackBitmap;
    static HBITMAP hPlayerBitmap, hBGBitmap;
    static int nRender = 0, dir = 0;
    static int nSizeX, nSizeY, nBGSizeX, nBGSizeY;
    static int nPosX, nPosY;
    static int nMoveAmount = 8;
    static RECT rcBack;

    switch (message)
    {
    case WM_CREATE:
    {
        HDC hdc = GetDC(hWnd);
        hBackDC = CreateCompatibleDC(hdc);
        GetClientRect(hWnd, &rcBack);
        hBackBitmap = CreateCompatibleBitmap(hdc, rcBack.right, rcBack.bottom);
        hOldBackBitmap = (HBITMAP)SelectObject(hBackDC, hBackBitmap);
        ReleaseDC(hWnd, hdc);

        hPlayerBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));
        hBGBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
        nRender = 0;

        BITMAP bit;
        GetObject(hPlayerBitmap, sizeof(BITMAP), &bit);
        nSizeX = bit.bmWidth / ANIMATION_X_COUNT;
        nSizeY = bit.bmHeight / ANIMATION_Y_COUNT;

        GetObject(hBGBitmap, sizeof(BITMAP), &bit);
        nBGSizeX = bit.bmWidth;
        nBGSizeY = bit.bmHeight;

        nPosX = nPosY = 0;

        SetTimer(hWnd, 1, 100, nullptr);
    }
        break;
    case WM_TIMER:
    {
        nRender = ++nRender % ANIMATION_X_COUNT;
        InvalidateRect(hWnd, nullptr, false);
    }
        break;
    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_UP:
            dir = 3;
            if(nPosY - nMoveAmount > 0)
                nPosY -= nMoveAmount;
            break;
        case VK_DOWN:
            dir = 0;
            if (nPosY + nMoveAmount + nSizeY < nBGSizeY)
                nPosY += nMoveAmount;
            break;
        case VK_LEFT:
            dir = 1;
            if (nPosX - nMoveAmount > 0)
                nPosX -= nMoveAmount;
            break;
        case VK_RIGHT:
            dir = 2;
            if (nPosX + nMoveAmount + nSizeX < nBGSizeX)
            nPosX += nMoveAmount;
            break;
        default:
            break;
        }

        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        HDC hMemDC = CreateCompatibleDC(hdc);

        FillRect(hBackDC, &rcBack, (HBRUSH)GetStockObject(GRAY_BRUSH));

        HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBGBitmap);

        BitBlt(hBackDC, 0, 0, nBGSizeX, nBGSizeY, hMemDC, 0, 0, SRCCOPY);

        //BitBlt(hdc, 0, 0, nSizeX, nSizeY, hMemDC, nSizeX * nRender, 0, SRCCOPY);
        SelectObject(hMemDC, hPlayerBitmap);
        GdiTransparentBlt(hBackDC, nPosX, nPosY, nSizeX, nSizeY, hMemDC, nSizeX * nRender, nSizeY * dir, nSizeX, nSizeY, RGB(255, 0, 255));

        SelectObject(hMemDC, hOldBitmap);
        DeleteDC(hMemDC);

        BitBlt(hdc, 0, 0, rcBack.right, rcBack.bottom, hBackDC, 0, 0, SRCCOPY);
        //SelectObject(hBackDC, hOldBackBitmap);

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        DeleteObject(hPlayerBitmap);
        DeleteObject(hBGBitmap);
        DeleteObject(hBackBitmap);
        DeleteDC(hBackDC);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}