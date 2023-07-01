// Shooting.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "Shooting.h"
#include "StartWindow.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    shared_ptr<StartWindow> window = make_shared<StartWindow>();

    if (window)
        window->Run(hInstance, lpCmdLine, nCmdShow, 500, 700);

    return 0;
}
