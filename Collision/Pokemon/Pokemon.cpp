// Pokemon.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Pokemon.h"

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

void MoveRect(RECT& rt, int x, int y)
{
    rt.left += x;
    rt.right += x;
    rt.top += y;
    rt.bottom += y;
}

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
    LoadStringW(hInstance, IDC_POKEMON, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_POKEMON));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_POKEMON));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = nullptr; //MAKEINTRESOURCEW(IDC_POKEMON);
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
    static HDC hMemDC;
    static RECT pikachuRT, squirtleRT;
    static HBITMAP bg, pikachu, squirtle;
    static BITMAP bit;
    static int bgBx, bgBy, pikachuBx, pikachuBy, squirtleBx, squirtleBy;
    static int posX = 0, posY = 0;
    static int moveSpeed = 3;

    switch (message)
    {
    case WM_CREATE:
    {
        bg = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
        GetObject(bg, sizeof(BITMAP), &bit);

        bgBx = bit.bmWidth;
        bgBy = bit.bmHeight;

        pikachu = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
        GetObject(pikachu, sizeof(BITMAP), &bit);

        pikachuBx = bit.bmWidth;
        pikachuBy = bit.bmHeight;

        pikachuRT.left = posX;
        pikachuRT.right = pikachuBx + posX;
        pikachuRT.top = posY;
        pikachuRT.bottom = pikachuBy + posY;

        squirtle = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));
        GetObject(squirtle, sizeof(BITMAP), &bit);

        squirtleBx = bit.bmWidth;
        squirtleBy = bit.bmHeight;

        squirtleRT.left = 150;
        squirtleRT.right = squirtleRT.left + squirtleBx;
        squirtleRT.top = 150;
        squirtleRT.bottom = squirtleRT.top + squirtleBy;
    }
        break;
    case WM_KEYDOWN :
    {

        RECT rt;
        switch (wParam)
        {
        case VK_UP:
            posY -= moveSpeed;
            MoveRect(pikachuRT, 0, -moveSpeed);
            if (IntersectRect(&rt, &pikachuRT, &squirtleRT))
            {
                posY += moveSpeed;
                MoveRect(pikachuRT, 0, moveSpeed);
            }
            break;
        case VK_DOWN: 
            posY += moveSpeed;
            MoveRect(pikachuRT, 0, moveSpeed);
            if (IntersectRect(&rt, &pikachuRT, &squirtleRT))
            {
                posY -= moveSpeed;
                MoveRect(pikachuRT, 0, -moveSpeed);
            }
            break;
        case VK_LEFT:
            posX -= moveSpeed;
            MoveRect(pikachuRT, -moveSpeed, 0);
            if (IntersectRect(&rt, &pikachuRT, &squirtleRT))
            {
                posX += moveSpeed;
                MoveRect(pikachuRT, moveSpeed, 0);
            }
            break;
        case VK_RIGHT:
            posX += moveSpeed;
            MoveRect(pikachuRT, +moveSpeed, 0);
            if (IntersectRect(&rt, &pikachuRT, &squirtleRT))
            {
                posX -= moveSpeed;
                MoveRect(pikachuRT, -moveSpeed, 0);
            }
            break;
        }

        InvalidateRect(hWnd, nullptr, true);
    }
    break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            hMemDC = CreateCompatibleDC(hdc);

            HBITMAP oldBitmap = (HBITMAP)SelectObject(hMemDC, bg);
            BitBlt(hdc, 0, 0, bgBx, bgBy, hMemDC, 0, 0, SRCCOPY);

            SelectObject(hMemDC, squirtle);
            GdiTransparentBlt(hdc, 150, 150, squirtleBx, squirtleBy, hMemDC, 0, 0, squirtleBx, squirtleBy, RGB(255, 0, 255));

            SelectObject(hMemDC, pikachu);
            GdiTransparentBlt(hdc, posX, posY, pikachuBx, pikachuBy, hMemDC, 0, 0, pikachuBx, pikachuBy, RGB(255, 0, 255));

            SelectObject(hMemDC, oldBitmap);
            DeleteObject(oldBitmap);
            DeleteDC(hMemDC);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        DeleteObject(bg);
        DeleteObject(pikachu);
        DeleteObject(squirtle);

        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}