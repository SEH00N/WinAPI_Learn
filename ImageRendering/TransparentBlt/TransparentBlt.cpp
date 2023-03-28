// TransparentBlt.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "TransparentBlt.h"
#include "Resource.h"

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
    LoadStringW(hInstance, IDC_TRANSPARENTBLT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TRANSPARENTBLT));

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

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TRANSPARENTBLT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = nullptr;//MAKEINTRESOURCEW(IDC_TRANSPARENTBLT);
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hMemDC;
    static HBITMAP MyBitmap, BGBitmap;
    static BITMAP bit;
    static int bx, by, bgbx, bgby, bgOriginBx, bgOriginBy;

    switch (message)
    {
    case WM_CREATE:
        MyBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
        BGBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
        GetObject(MyBitmap, sizeof(BITMAP), &bit);

        bx = bit.bmWidth;
        by = bit.bmHeight;

        GetObject(BGBitmap, sizeof(BITMAP), &bit);

        bgbx = bgOriginBx = bit.bmWidth;
        bgby = bgOriginBy = bit.bmHeight;

        SetTimer(hWnd, 1, 40, nullptr);

        break;
    case WM_TIMER:
        InvalidateRect(hWnd, nullptr, true);
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        RECT rt;
        GetClientRect(hWnd, &rt);

        bgbx = rt.right - rt.left;
        bgby = rt.bottom - rt.top;

        hMemDC = CreateCompatibleDC(hdc);
        HBITMAP Oldbit = (HBITMAP)SelectObject(hMemDC, BGBitmap);
        GdiTransparentBlt(hdc, 0, 0, bgbx, bgby, hMemDC, 0, 0, bgOriginBx, bgOriginBy, RGB(255, 0, 255));

        SelectObject(hMemDC, Oldbit);

        Oldbit = (HBITMAP)SelectObject(hMemDC, MyBitmap);
        GdiTransparentBlt(hdc, 0, 0, bx, by, hMemDC, 0, 0, bx, by, RGB(255, 0, 255));

        SelectObject(hMemDC, Oldbit);
        DeleteDC(hMemDC);

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);

        DeleteObject(MyBitmap);
        DeleteObject(BGBitmap);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}