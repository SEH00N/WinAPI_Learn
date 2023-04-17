#include "framework.h"
#include "Client.h"
#include "pch.h"
#include "Game.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	shared_ptr<Game> game = make_shared<Game>();

	if (game)
		game->Run(hInstance, lpCmdLine, nCmdShow);

	return 0;
}