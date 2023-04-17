#pragma once

#include <memory>
#include <windows.h>
#include <tchar.h>
#include <string>
#include <ctime>
#include <vector>
#include <array>
#include <list>
#include <map>

using namespace std;

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "msimg32.lib")

#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600

#define PI				3.14159265

extern shared_ptr<class H00NEngine> g_Engine;