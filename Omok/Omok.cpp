// Omok.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Omok.h"

#define MAX_LOADSTRING 100

#pragma region define

#define WIN_WIDTH   590
#define WIN_HEIGHT  600

#define X_COUNT     19
#define Y_COUNT     19

#define START_X     50
#define START_Y     50

#define DOL_SIZE    26
#define DOL_RADIUS  DOL_SIZE / 2

#define XPOS(x) (START_X + (x) * DOL_SIZE)
#define YPOS(y) (START_Y + (y) * DOL_SIZE)

#pragma endregion

#pragma region enum

enum OMOKDOL {
    NONE = 0,   //비어있는 칸
    BLACK = 1,  //흑돌
    WHITE = 2,  //백돌
};

enum GAMESTATE {
    READY = 0,
    BLACK_WIN = 1,
    WHITE_WIN = 2,
    TIE_GAME = 3,
};

#pragma endregion

#pragma region varialble

unsigned char g_omokBoard[Y_COUNT][X_COUNT];
bool g_turn = true; //true : 흑돌, false : 백돌
int g_count = 0;

GAMESTATE g_gameState = GAMESTATE::READY;
const WCHAR* GAMESTATE_STR[] = { _T("READY"), _T("BLACK_WIN"), _T("WHITE_WIN"), _T("TIE_GAME") };

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

#pragma region function

void OnReset(HWND hWnd);
void OnPaint(HWND hWnd);
void OnLButtonDown(HWND hWnd, int x, int y);
bool five_check(OMOKDOL dol, int dy, int dx, int y, int x);
GAMESTATE isGameOver();

#pragma endregion

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
    LoadStringW(hInstance, IDC_OMOK, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OMOK));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OMOK));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = nullptr; //MAKEINTRESOURCEW(IDC_OMOK);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
      50, 50, WIN_WIDTH, WIN_HEIGHT, nullptr, nullptr, hInstance, nullptr);

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
            OnReset(hWnd);
        }
        break;
    case WM_LBUTTONDOWN:
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            OnLButtonDown(hWnd, x, y);
            
            GAMESTATE result = isGameOver();
            switch (result)
            {
            case READY:
                break;
            case BLACK_WIN:
            case WHITE_WIN:
            case TIE_GAME:
            {
                WCHAR str[100];
                wsprintf(str, _T("WIN : %s"), GAMESTATE_STR[result]);
                if (MessageBox(hWnd, str, _T("Omok"), MB_YESNO) == IDYES)
                    OnReset(hWnd);
                else
                    PostQuitMessage(0);
            }
                break;
            default:
                break;
            }
        }
        break;
    case WM_PAINT:
        {
            OnPaint(hWnd);
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

void OnReset(HWND hWnd)
{
    for (int y = 0; y < Y_COUNT; y++)
    {
        for (int x = 0; x < X_COUNT; x++)
        {
            g_omokBoard[y][x] = OMOKDOL::NONE;
        }
    }

    g_turn = true;

    g_gameState = GAMESTATE::READY;

    InvalidateRect(hWnd, nullptr, true);
}

void OnPaint(HWND hWnd)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);

    RECT bgRect;
    bgRect.left = START_X;
    bgRect.top = START_Y;
    bgRect.right = bgRect.left + (X_COUNT - 1) * DOL_SIZE;
    bgRect.bottom = bgRect.top + (Y_COUNT - 1) * DOL_SIZE;

    HBRUSH bgBrush = CreateSolidBrush(RGB(244, 176, 77));
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, bgBrush);

    Rectangle(hdc, bgRect.left, bgRect.top, bgRect.right, bgRect.bottom);

    SelectObject(hdc, oldBrush);
    DeleteObject(bgBrush);

    for (int x = 1; x < X_COUNT - 1; ++x)
    {
        MoveToEx(hdc, XPOS(x), YPOS(0), nullptr);
        LineTo(hdc, XPOS(x), YPOS(Y_COUNT - 1));
    }

    for (int y = 1; y < Y_COUNT - 1; ++y)
    {
        MoveToEx(hdc, XPOS(0), YPOS(y), nullptr);
        LineTo(hdc, XPOS(X_COUNT - 1), YPOS(y));
    }

    SelectObject(hdc, GetStockObject(LTGRAY_BRUSH));

    int outline = 4;
    
    if (g_turn) //검은돌
    {
        Ellipse(
            hdc, 
            WIN_WIDTH / 2 - DOL_SIZE * 2 - DOL_RADIUS - outline, START_Y / 2 - DOL_RADIUS - outline,
            WIN_WIDTH / 2 - DOL_SIZE * 2 + DOL_RADIUS + outline, START_Y / 2 + DOL_RADIUS + outline);
    }
    else
    {
        Ellipse(
            hdc,
            WIN_WIDTH / 2 + DOL_SIZE * 2 - DOL_RADIUS - outline, START_Y / 2 - DOL_RADIUS - outline,
            WIN_WIDTH / 2 + DOL_SIZE * 2 + DOL_RADIUS + outline, START_Y / 2 + DOL_RADIUS + outline);
    }

    SelectObject(hdc, GetStockObject(BLACK_BRUSH));
    Ellipse(hdc,
        WIN_WIDTH / 2 - DOL_SIZE * 2 - DOL_RADIUS, START_Y / 2 - DOL_RADIUS,
        WIN_WIDTH / 2 - DOL_SIZE * 2 + DOL_RADIUS, START_Y / 2 + DOL_RADIUS);

    SelectObject(hdc, GetStockObject(WHITE_BRUSH));
    Ellipse(hdc,
        WIN_WIDTH / 2 + DOL_SIZE * 2 - DOL_RADIUS, START_Y / 2 - DOL_RADIUS,
        WIN_WIDTH / 2 + DOL_SIZE * 2 + DOL_RADIUS, START_Y / 2 + DOL_RADIUS);

    for (int y = 0; y < Y_COUNT; ++y)
    {
        for (int x = 0; x < X_COUNT; ++x)
        {
            if (g_omokBoard[y][x] == OMOKDOL::NONE)
                continue;
            else if (g_omokBoard[y][x] == OMOKDOL::BLACK)
                SelectObject(hdc, GetStockObject(BLACK_BRUSH));
            else
                SelectObject(hdc, GetStockObject(WHITE_BRUSH));

            Ellipse(hdc, XPOS(x) - DOL_RADIUS, YPOS(y) - DOL_RADIUS, XPOS(x) + DOL_RADIUS, YPOS(y) + DOL_RADIUS);
        }
    }

    EndPaint(hWnd, &ps);
}

void OnLButtonDown(HWND hWnd, int x, int y)
{
    // 바둑판 범위 안에 있는가?

    if (x > (XPOS(0) - DOL_RADIUS) &&
        y > (YPOS(0) - DOL_RADIUS) &&
        x < (XPOS(X_COUNT - 1) + DOL_RADIUS) &&
        y < (YPOS(Y_COUNT - 1) + DOL_RADIUS))
    {
        int ux = (x - START_X + DOL_RADIUS) / DOL_SIZE;
        int uy = (y - START_Y + DOL_RADIUS) / DOL_SIZE;

        if (g_omokBoard[uy][ux] == OMOKDOL::NONE)
        {
            if (g_turn)
                g_omokBoard[uy][ux] = OMOKDOL::BLACK;
            else
                g_omokBoard[uy][ux] = OMOKDOL::WHITE;

            g_turn = !g_turn;
            g_count++;
            InvalidateRect(hWnd, nullptr, true);
        }
    }
}

bool five_check(OMOKDOL dol, int dy, int dx, int y, int x)
{
    for (int i = 0; i < 5; ++i)
        if (g_omokBoard[y + (dy * i)][x + (dx * i)] != dol)
            return false;

    return true;
}

GAMESTATE isGameOver()
{
    GAMESTATE result = GAMESTATE::READY;
    static const int four = 4;

    for (int y = 0; y < Y_COUNT; ++y)
    {
        for (int x = 0; x < X_COUNT; ++x)
        {
            if (g_omokBoard[y][x] == OMOKDOL::NONE)
                continue;

            if (x + four < X_COUNT && five_check(OMOKDOL::BLACK, 0, 1, y, x))
                return GAMESTATE::BLACK_WIN;
            if (x + four < X_COUNT && five_check(OMOKDOL::WHITE, 0, 1, y, x))
                return GAMESTATE::WHITE_WIN;

            if (y + four < Y_COUNT && five_check(OMOKDOL::BLACK, 1, 0, y, x))
                return GAMESTATE::BLACK_WIN;
            if (y + four < Y_COUNT && five_check(OMOKDOL::WHITE, 1, 0, y, x))
                return GAMESTATE::WHITE_WIN;

            if (x - four >= 0 && y - four >= 0 && five_check(OMOKDOL::BLACK, -1, -1, y, x))
                return GAMESTATE::BLACK_WIN;
            if (x - four >= 0 && y - four >= 0 && five_check(OMOKDOL::WHITE, -1, -1, y, x))
                return GAMESTATE::WHITE_WIN;

            if (x + four <= X_COUNT && y - four <= Y_COUNT && five_check(OMOKDOL::BLACK, -1, 1, y, x))
                return GAMESTATE::BLACK_WIN;
            if (x + four <= X_COUNT && y - four <= Y_COUNT && five_check(OMOKDOL::WHITE, -1, 1, y, x))
                return GAMESTATE::WHITE_WIN;
        }
    }

    if (g_count == X_COUNT * Y_COUNT)
        result = GAMESTATE::TIE_GAME;

    return result;
}
