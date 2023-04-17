// PointInRect.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "PointInRect.h"

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
    LoadStringW(hInstance, IDC_POINTINRECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_POINTINRECT));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_POINTINRECT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = nullptr;//MAKEINTRESOURCEW(IDC_POINTINRECT);
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
    static RECT startRect;
    static HBITMAP bgBitmap, catBitmap, startBitmap;
    static BITMAP bit;
    static int bgbx, bgby, catbx, catby, startbx, startby;
    HDC hMemDC;

    switch (message)
    {

    case WM_CREATE:
    {
        bgBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
        GetObject(bgBitmap, sizeof(BITMAP), &bit);

        bgbx = bit.bmWidth;
        bgby = bit.bmHeight;

        catBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
        GetObject(catBitmap, sizeof(BITMAP), &bit);

        catbx = bit.bmWidth;
        catby = bit.bmHeight;

        startBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP7));
        GetObject(startBitmap, sizeof(BITMAP), &bit);

        startbx = bit.bmWidth;
        startby = bit.bmHeight;

        startRect.left = (bgbx - startbx) / 2;
        startRect.right = (bgbx - startbx) / 2 + startbx;
        startRect.top = (bgby - startby) / 2 + 50;
        startRect.bottom = (bgby - startby) / 2 + 50 + startby;
    }
        break;
    case WM_LBUTTONDOWN:
    {
        POINT pt;
        pt.x = LOWORD(lParam);
        pt.y = HIWORD(lParam);

        if (PtInRect(&startRect, pt))
            MessageBox(hWnd, _T("시작!!"), _T("Collision MSG BOX"), MB_OK);
    }
        break;
    case WM_PAINT:
    {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		hMemDC = CreateCompatibleDC(hdc);
		HBITMAP Oldbit = (HBITMAP)SelectObject(hMemDC, bgBitmap);

		GdiTransparentBlt(hdc, 0, 0, bgbx, bgby, hMemDC, 0, 0, bgbx, bgby, RGB(255, 0, 255));

        SelectObject(hMemDC, catBitmap);
        GdiTransparentBlt(hdc, (bgbx - catbx) / 2, (bgby - catby) / 2 - 50, catbx, catby, hMemDC, 0, 0, catbx, catby, RGB(255, 0, 255));

        SelectObject(hMemDC, startBitmap);
        GdiTransparentBlt(hdc, startRect.left, startRect.top, startbx, startby, hMemDC, 0, 0, startbx, startby, RGB(255, 0, 255));

        TextOut(hdc, (bgbx - 200) / 2, bgby - 100, _T("Start를 클릭하면 시작합니다."), 18);

		SelectObject(hMemDC, Oldbit);
		DeleteDC(hMemDC);
        DeleteObject(Oldbit);

		EndPaint(hWnd, &ps);
    }
        break;
    case WM_DESTROY:
        DeleteObject(bgBitmap);
        DeleteObject(catBitmap);
        DeleteObject(startBitmap);

        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}