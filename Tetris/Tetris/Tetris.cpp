// Tetris.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Tetris.h"

#define MAX_LOADSTRING 100

#pragma region DEFINE

// 테트리스 정보
#define B_WIDTH     12
#define B_HEIGHT    22
#define B_SIZE      24

// 점수 정보
#define SCORE_X     (B_WIDTH + 2)
#define SCORE_Y     2

// 다음 블록 출력 정보
#define NEXTB_X (B_WIDTH + 1)
#define NEXTB_Y (B_HEIGHT - 7)

// 현재 블록 생성 위치
#define START_X     (B_WIDTH/2)
#define START_Y     3

#define NEXTB_XCNT 11
#define NEXTB_YCNT 7

// 랜덤 디파인
#define random(n) (rand()%n)

#pragma endregion

#pragma region VARIABLE

const POINT Shape[][4][4] = {
	{
		{ 0, 0, 1, 0, 2, 0, -1, 0 },
		{ 0, 0, 0, 1, 0, -1, 0, -2 },
		{ 0, 0, 1, 0, 2, 0, -1, 0 },
		{ 0, 0, 0, 1, 0, -1, 0, -2 }
	},
	{
		{ 0, 0, 1, 0, 0, 1, 1, 1 },
		{ 0, 0, 1, 0, 0, 1, 1, 1 },
		{ 0, 0, 1, 0, 0, 1, 1, 1 },
		{ 0, 0, 1, 0, 0, 1, 1, 1 }
	},
	{
		{ 0, 0, -1, -1, 0, -1, 1, 0 },
		{ 0, 0, -1, 0, -1, 1, 0, -1 },
		{ 0, 0, -1, -1, 0, -1, 1, 0 },
		{ 0, 0, -1, 0, -1, 1, 0, -1 }
	},
	{
		{ 0, 0, -1, 0, 0, -1, 1, -1 },
		{ 0, 0, 0, 1, -1, 0, -1, -1 },
		{ 0, 0, -1, 0, 0, -1, 1, -1 },
		{ 0, 0, 0, 1, -1, 0, -1, -1 }
	},
	{
		{ 0, 0, -1, 0, 1, 0, -1, -1 },
		{ 0, 0, 0, -1, 0, 1, -1, 1 },
		{ 0, 0, -1, 0, 1, 0, 1, 1 },
		{ 0, 0, 0, -1, 0, 1, 1, -1 },
	},
	{
		{ 0, 0, 1, 0, -1, 0, 1, -1 },
		{ 0, 0, 0, 1, 0, -1, -1, -1 },
		{ 0, 0, 1, 0, -1, 0, -1, 1 },
		{ 0, 0, 0, -1, 0, 1, 1, 1 },
	},
	{
		{ 0, 0, -1, 0, 1, 0, 0, -1},
		{ 0, 0, -1, 0, 0, -1, 0, 1},
		{ 0, 0, -1, 0, 1, 0, 0, 1 },
		{ 0, 0, 0, -1, 0, 1, 1, 0 },
	}
};

enum TILE {
	BRICK,
	WALL = sizeof(Shape) / sizeof(Shape[0]),
	EMPTY,
};

enum Tag_Status {
	GAMEOVER,
	RUNNING,
	GAMEPAUSE
};

int g_board[B_HEIGHT][B_WIDTH];

int g_score;
int g_brickNum;

HBITMAP g_hBitmap[EMPTY + 1];
Tag_Status g_gameStatus;

POINT g_nowPos;
int g_nowBrick, g_nowRotate;

int g_nextBrick;

int g_interval;

#pragma endregion

//#pragma region FUNCTION

void DrawScreen(HDC hdc);
void PrintTile(HDC hdc, int x, int y, int color);
BOOL MoveDown();
void MakeNewBrick();
int GetAround(int x, int y, int brick, int rotate);
void FullCheck();

//#pragma endregion


// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
HWND hWndMain;
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
	LoadStringW(hInstance, IDC_TETRIS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TETRIS));

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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TETRIS));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TETRIS);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
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
	switch (message)
	{
	case WM_CREATE:
	{
		hWndMain = hWnd;

		RECT rt;
		SetRect(&rt, 0, 0, (NEXTB_X + NEXTB_XCNT) * B_SIZE, B_HEIGHT * B_SIZE);

		AdjustWindowRect(&rt, WS_CAPTION | WS_SYSMENU | WS_MINIMIZE, TRUE);
		SetWindowPos(hWndMain, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, SWP_NOMOVE | SWP_NOZORDER);

		srand((unsigned int)GetTickCount64());
		g_gameStatus = GAMEOVER;

		for (int i = 0; i < EMPTY + 1; i++)
			g_hBitmap[i] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1 + i));

		for (int y = 0; y < B_HEIGHT; ++y)
			for (int x = 0; x < B_WIDTH; ++x)
				g_board[y][x] = (y == 0 || y == B_HEIGHT - 1 || x == 0 || x == B_WIDTH - 1) ? WALL : EMPTY;
	}
	break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다:
		switch (wmId)
		{
			case IDM_START:
			{
				if (g_gameStatus != GAMEOVER)
					break;

				for (int y = 0; y < B_HEIGHT; ++y)
					for (int x = 0; x < B_WIDTH; ++x)
						g_board[y][x] = (y == 0 || y == B_HEIGHT - 1 || x == 0 || x == B_WIDTH - 1) ? WALL : EMPTY;

				g_score = 0;
				g_brickNum = 0;
				g_gameStatus = RUNNING;

				g_nowPos = { START_X, START_Y };
				g_nowBrick = 0;
				g_nowRotate = 0;

				g_nextBrick = random(sizeof(Shape)/sizeof(Shape[0]));
				MakeNewBrick();

				g_interval = 1000;

				SetTimer(hWnd, 1, g_interval, nullptr);
			}
			break;
			case IDM_PAUSE:
			{
				if (g_gameStatus == RUNNING)
				{
					g_gameStatus = GAMEPAUSE;
					KillTimer(hWnd, 1);
				}
				else if (g_gameStatus == GAMEPAUSE)
				{
					g_gameStatus = RUNNING;
					SetTimer(hWnd, 1, g_interval, nullptr);
				}
			}
				break;
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
		break;
	case WM_TIMER:
	{
		if (MoveDown() == TRUE)
		{
			MakeNewBrick();
		}
	}
	break;
	case WM_KEYDOWN:
	{
		if (g_gameStatus != RUNNING || g_nowBrick == -1)
			return 0;

		switch (wParam)
		{
		case VK_LEFT:
		{
			if(GetAround(g_nowPos.x - 1, g_nowPos.y, g_nowBrick, g_nowRotate) == EMPTY)
			{
				g_nowPos.x--;
				InvalidateRect(hWnd, nullptr, false);
			}
		}
			break;
		case VK_RIGHT:
		{
			if (GetAround(g_nowPos.x + 1, g_nowPos.y, g_nowBrick, g_nowRotate) == EMPTY)
			{
				g_nowPos.x++;
				InvalidateRect(hWnd, nullptr, false);
			}
		}
			break;
		case VK_DOWN:
		{
			if (MoveDown() == TRUE)
				MakeNewBrick();
		}
			break;
		case VK_UP:
		{
			int nrot = (g_nowRotate + 1) % 4;
			if (GetAround(g_nowPos.x, g_nowPos.y, g_nowBrick, nrot) == EMPTY)
			{
				g_nowRotate = nrot;
				InvalidateRect(hWnd, nullptr, false);
			}
		}
			break;
		case VK_SPACE:
		{
			while (MoveDown() == FALSE);
			MakeNewBrick();
		}
			break;
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		HDC hBackDC;
		HBITMAP backBitmap, oldBackBitmap;
		RECT rt;
		GetClientRect(hWnd, &rt);

		hBackDC = CreateCompatibleDC(hdc);
		backBitmap = CreateCompatibleBitmap(hdc, rt.right, rt.bottom);
		oldBackBitmap = (HBITMAP)SelectObject(hBackDC, backBitmap);

		FillRect(hBackDC, &rt, (HBRUSH)GetStockObject(WHITE_BRUSH));
		DrawScreen(hBackDC);
		BitBlt(hdc, 0, 0, rt.right, rt.bottom, hBackDC, 0, 0, SRCCOPY);

		SelectObject(hBackDC, oldBackBitmap);
		DeleteDC(hBackDC);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
	{
		for (int i = 0; i < EMPTY + 1; ++i)
			DeleteObject(g_hBitmap[i]);

		PostQuitMessage(0);
	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void DrawScreen(HDC hdc)
{
	for (int y = 0; y < B_HEIGHT; ++y)
		for (int x = 0; x < B_WIDTH; ++x)
			PrintTile(hdc, x, y, g_board[y][x]);

	if (g_gameStatus != GAMEOVER && g_nowBrick != -1)
	{
		for (int i = 0; i < 4; ++i)
		{
			PrintTile(
				hdc,
				g_nowPos.x + Shape[g_nowBrick][g_nowRotate][i].x,
				g_nowPos.y + Shape[g_nowBrick][g_nowRotate][i].y,
				g_nowBrick
			);
		}
	}

	for (int y = NEXTB_Y; y < NEXTB_Y + NEXTB_YCNT; ++y)
		for (int x = NEXTB_X; x < NEXTB_X + NEXTB_XCNT; ++x)
			if (x == NEXTB_X || x == NEXTB_X + NEXTB_XCNT - 1 || y == NEXTB_Y || y == NEXTB_Y + NEXTB_YCNT - 1)
				PrintTile(hdc, x, y, WALL);
			else
				PrintTile(hdc, x, y, EMPTY);

	if (g_gameStatus != GAMEOVER && g_nextBrick)
		for (int i = 0; i < 4; ++ i)
			PrintTile(hdc, (NEXTB_X + NEXTB_XCNT / 2) + Shape[g_nextBrick][0][i].x, (NEXTB_Y + NEXTB_YCNT/2)+Shape[g_nextBrick][0][i].y, g_nextBrick);

	TCHAR str[128];
	lstrcpy(str, _T("▣ TETRIS ▣"));
	TextOut(hdc, SCORE_X * B_SIZE, SCORE_Y * B_SIZE, str, lstrlen(str));

	wsprintf(str, _T("SCORE : %5d"), g_score);
	TextOut(hdc, SCORE_X * B_SIZE, (SCORE_Y + 1) * B_SIZE, str, lstrlen(str));

	wsprintf(str, _T("BLOCK : %5d"), g_brickNum);
	TextOut(hdc, SCORE_X * B_SIZE, (SCORE_Y + 2) * B_SIZE, str, lstrlen(str));
}

void PrintTile(HDC hdc, int x, int y, int color)
{
	HDC hMemDC;
	HBITMAP oldBitmap;

	hMemDC = CreateCompatibleDC(hdc);
	oldBitmap = (HBITMAP)SelectObject(hMemDC, g_hBitmap[color]);

	BitBlt(hdc, x * B_SIZE, y * B_SIZE, B_SIZE, B_SIZE, hMemDC, 0, 0, SRCCOPY);

	SelectObject(hMemDC, oldBitmap);
	DeleteDC(hMemDC);
}

BOOL MoveDown()
{
	if (GetAround(g_nowPos.x, g_nowPos.y + 1, g_nowBrick, g_nowRotate) != EMPTY)
	{
		FullCheck();
		return true;
	}

	g_nowPos.y++;

	InvalidateRect(hWndMain, nullptr, false);
	UpdateWindow(hWndMain);
	return false;
}

void MakeNewBrick()
{
	g_brickNum++;
	g_nowBrick = g_nextBrick;

	g_nextBrick = random(sizeof(Shape) / sizeof(Shape[0]));
	
	g_nowPos = { START_X, START_Y };
	g_nowRotate = 0;

	InvalidateRect(hWndMain, nullptr, false);

	if (GetAround(g_nowPos.x, g_nowPos.y + 1, g_nowBrick, g_nowRotate) != EMPTY)
	{
		KillTimer(hWndMain, 1);
		g_gameStatus = GAMEOVER;
		MessageBox(hWndMain, _T("GAME OVER"), _T("▣ TETRIS ▣"), MB_OK);
	}
}

int GetAround(int x, int y, int brick, int rotate)
{
	int k = EMPTY;

	for (int i = 0; i < 4; ++i)
		k = min(k, g_board[y + Shape[brick][rotate][i].y][x + Shape[brick][rotate][i].x]);

	return k;
}

void FullCheck()
{
	static int ScoreInc[] = { 0, 1, 3, 8, 20 };
	int count = 0;

	for (int i = 0; i < 4; ++i)
		g_board[g_nowPos.y + Shape[g_nowBrick][g_nowRotate][i].y][g_nowPos.x + Shape[g_nowBrick][g_nowRotate][i].x] = g_nowBrick;

	g_nowBrick = -1;

	int x, y;
	for (y = 1; y < B_HEIGHT - 1; ++y)
	{
		for (x = 1; x < B_WIDTH - 1; ++x)
		{
			if (g_board[y][x] == EMPTY)
				break;
		}

		if (x == B_WIDTH - 1)
		{
			++count;
			for (int ty = y; ty > 1; --ty)
			{
				for (x = 1; x < B_WIDTH - 1; ++x)
				{
					g_board[ty][x] = g_board[ty - 1][x];
				}
			}

			InvalidateRect(hWndMain, nullptr, false);
			UpdateWindow(hWndMain);
			Sleep(150);
		}
	}

	g_score += ScoreInc[count];

	if (g_brickNum % 10 == 0 && g_interval > 200)
	{
		g_interval -= 50;
		SetTimer(hWndMain, 1, g_interval, nullptr);
	}
}