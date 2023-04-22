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

#define SCREEN_WIDTH			800
#define SCREEN_HEIGHT			600

#define PI						3.14159265

#define DECLARE_SINGLE(type)	\
private:						\
	type() {}					\
	~type() {}					\
								\
public:							\
	static type* GetInstance()	\
	{							\
		static type instance;	\
		return &instance;		\
	}							\

#define GET_SINGLE(type)		type::GetInstance()

#define TIMER					g_Engine->GetTimer()->GetDeltaTime()
#define INPUT					g_Engine->GetInput()

extern shared_ptr<class H00NEngine> g_Engine;