﻿// Sokoban.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Sokoban.h"

#define MAX_LOADSTRING 100

#pragma region DEFINE

#define X_SIZE      32
#define Y_SIZE      32
#define X_COUNT     18
#define Y_COUNT     13
#define X_CORRECT   16
#define Y_CORRECT   39
#define WIN_WIDTH   X_SIZE * X_COUNT + X_CORRECT
#define WIN_HEIGHT  Y_SIZE * Y_COUNT + Y_CORRECT

#define TILE        '#'
#define HERO        '@'
#define ROAD        ' '
#define BOX         'B'
#define GOAL        'X'

#define MAXSTAGE    2

#pragma endregion

#pragma region Variable

unsigned char g_ucMap[Y_COUNT][X_COUNT + 1];
unsigned char g_ucStageMap[MAXSTAGE][Y_COUNT][X_COUNT + 1] = { 
    {
        "##################",
        "##################",
        "##################",
        "##################",
        "##################",
        "##################",
        "##  ##############",
        "#  B#########   @#",
        "#   # ### ###    #",
        "#        B      X#",
        "##### ### # #   X#",
        "#####     ########",
        "##################",
    }, 
    {
        "##################",
        "##################",
        "##################",
        "#####   ##########",
        "#####B  ##########",
        "#####  B##########",
        "###  B B #########",
        "### # ## #########",
        "#   # ## ####  XX#",
        "# B  B   @     XX#",
        "##### ### # #  XX#",
        "#####     ########",
        "##################",
    } 
};

int g_nPosX;
int g_nPosY;
int g_nKeyCount;
int g_nStage;
int g_nMatch;
int g_nClearCount;

HBITMAP g_hbmSelBitmap;     // 선택된 주인공 비트맵
HBITMAP g_hbmPlayerDown;
HBITMAP g_hbmPlayerUp;
HBITMAP g_hbmPlayerLeft;
HBITMAP g_hbmPlayerRight;
HBITMAP g_hbmTile;
HBITMAP g_hbmRoad;
HBITMAP g_hbmBox;
HBITMAP g_hbmGoal;

#pragma endregion

#pragma region Function

void Stage_Change(int nStage);

#pragma endregion

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
    LoadStringW(hInstance, IDC_SOKOBAN, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SOKOBAN));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SOKOBAN));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = nullptr; //MAKEINTRESOURCEW(IDC_SOKOBAN);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, WIN_WIDTH, WIN_HEIGHT, nullptr, nullptr, hInstance, nullptr);

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
    switch (message)
    {
    case WM_CREATE:
    {
        g_hbmPlayerDown = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
        g_hbmPlayerUp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
        g_hbmPlayerLeft = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));
        g_hbmPlayerRight = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP4));

        g_hbmTile = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP5));
        g_hbmRoad = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP6));
        g_hbmBox = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP7));
        g_hbmGoal = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP8));

        g_nStage = 0;
        Stage_Change(g_nStage);
    }
        break;
    case WM_KEYDOWN:
    {
        RECT rc;
        rc.left = g_nPosX * X_SIZE;
        rc.top = g_nPosY * Y_SIZE;
        rc.right = rc.left + X_SIZE;
        rc.bottom = rc.top + Y_SIZE;

        switch (wParam)
        {
        case VK_DOWN:
            g_hbmSelBitmap = g_hbmPlayerDown;
            if (TILE != g_ucMap[g_nPosY + 1][g_nPosX])
            {
                rc.bottom += Y_SIZE;
                if (BOX == g_ucMap[g_nPosY + 1][g_nPosX])
                {
                    if (ROAD == g_ucMap[g_nPosY + 2][g_nPosX])
                    {
                        rc.bottom += Y_SIZE;
                        g_ucMap[g_nPosY + 2][g_nPosX] = BOX;
                    }
                    else
                        break;
                }

                g_ucMap[g_nPosY][g_nPosX] = ROAD;
                ++g_nPosY;
                g_ucMap[g_nPosY][g_nPosX] = HERO;
                ++g_nKeyCount;
            }
            break;
        case VK_UP:
            g_hbmSelBitmap = g_hbmPlayerUp;
            if (TILE != g_ucMap[g_nPosY - 1][g_nPosX])
            {
                rc.top -= Y_SIZE;
                if (BOX == g_ucMap[g_nPosY - 1][g_nPosX])
                {
                    if (ROAD == g_ucMap[g_nPosY - 2][g_nPosX])
                    {
                        rc.top -= Y_SIZE;
                        g_ucMap[g_nPosY - 2][g_nPosX] = BOX;
                    }
                    else
                        break;
                }

                g_ucMap[g_nPosY][g_nPosX] = ROAD;
                --g_nPosY;
                g_ucMap[g_nPosY][g_nPosX] = HERO;
                ++g_nKeyCount;
            }
            break;
        case VK_RIGHT:
            g_hbmSelBitmap = g_hbmPlayerRight;
            if (TILE != g_ucMap[g_nPosY][g_nPosX + 1])
            {
                rc.right += X_SIZE;
                if (BOX == g_ucMap[g_nPosY][g_nPosX + 1])
                {
                    if (ROAD == g_ucMap[g_nPosY][g_nPosX + 2])
                    {
                        rc.right += X_SIZE;
                        g_ucMap[g_nPosY][g_nPosX + 2] = BOX;
                    }
                    else
                        break;
                }

                g_ucMap[g_nPosY][g_nPosX] = ROAD;
                ++g_nPosX;
                g_ucMap[g_nPosY][g_nPosX] = HERO;
                ++g_nKeyCount;
            }
            break;
        case VK_LEFT:
            g_hbmSelBitmap = g_hbmPlayerLeft;
            if (TILE != g_ucMap[g_nPosY][g_nPosX - 1])
            {
                rc.left -= X_SIZE;
                if (BOX == g_ucMap[g_nPosY][g_nPosX - 1])
                {
                    if (ROAD == g_ucMap[g_nPosY][g_nPosX - 2])
                    {
                        rc.left -= X_SIZE;
                        g_ucMap[g_nPosY][g_nPosX - 2] = BOX;
                    }
                    else
                        break;
                }

                g_ucMap[g_nPosY][g_nPosX] = ROAD;
                --g_nPosX;
                g_ucMap[g_nPosY][g_nPosX] = HERO;
                ++g_nKeyCount;
            }
            break;
        default:
            break;
        }

        InvalidateRect(hWnd, &rc, false);
    }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            HDC hMemDC = CreateCompatibleDC(hdc);
            g_nClearCount = 0;
            for (int y = 0; y < Y_COUNT; ++y)
            {
                for (int x = 0; x < X_COUNT; ++x)
                {
                    if (TILE == g_ucMap[y][x])
                        SelectObject(hMemDC, g_hbmTile);
                    else if (HERO == g_ucMap[y][x])
                        SelectObject(hMemDC, g_hbmSelBitmap);
                    else if (BOX == g_ucMap[y][x])
                    {
                        SelectObject(hMemDC, g_hbmBox);
                        if (GOAL == g_ucStageMap[g_nStage][y][x])
                            ++g_nClearCount;
                    }
                    else if (GOAL == g_ucStageMap[g_nStage][y][x])
                        SelectObject(hMemDC, g_hbmGoal);
                    else
                        SelectObject(hMemDC, g_hbmRoad);

                    BitBlt(hdc, x * X_SIZE, y * Y_SIZE, X_SIZE, Y_SIZE, hMemDC, 0, 0, SRCCOPY);
                }

            }
            DeleteDC(hMemDC);

            if (g_nMatch == g_nClearCount)
            {
                if (g_nStage + 1 == MAXSTAGE)
                {
                    if (MessageBox(hWnd, _T("Game Clear!!"), _T("RESTART?"), MB_YESNO) == IDYES)
                        Stage_Change(++g_nStage);
                    else
                        SendMessage(hWnd, WM_DESTROY, 0, 0);
                }
                else
                {
                    Stage_Change(++g_nStage);
                }

                InvalidateRect(hWnd, nullptr, true);
            }

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        DeleteObject(g_hbmPlayerDown);
        DeleteObject(g_hbmPlayerUp);
        DeleteObject(g_hbmPlayerLeft);
        DeleteObject(g_hbmPlayerRight);
        DeleteObject(g_hbmTile);
        DeleteObject(g_hbmRoad);
        DeleteObject(g_hbmBox);
        DeleteObject(g_hbmGoal);

        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void Stage_Change(int nStage)
{
    g_nStage = nStage % MAXSTAGE;
    g_nKeyCount = 0;
    g_nMatch = 0;

    g_hbmSelBitmap = g_hbmPlayerDown;

    for (int y = 0; y < Y_COUNT; ++y)
    {
        for (int x = 0; x < X_COUNT; ++x)
        {
            g_ucMap[y][x] = g_ucStageMap[g_nStage][y][x];
            if (HERO == g_ucMap[y][x])
            {
                g_nPosX = x;
                g_nPosY = y;
            }

            if (GOAL == g_ucMap[y][x])
            {
                g_ucMap[y][x] = ROAD;
                ++g_nMatch;
            }
        }
    }
}
