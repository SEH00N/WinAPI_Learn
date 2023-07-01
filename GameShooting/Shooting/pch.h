#pragma once

#include "EnginePch.h"
#include "BTSEngine.h"

#pragma comment(lib, "Engine.lib")

enum class PAWN_TYPE
{
	NONE = 0,
	PLAYER,
	ENEMY,
};

enum class ENEMY_TYPE
{
	ENEMY_01 = 0,
	ENEMY_02,
	ENEMY_03,
	ENEMY_04,
	ENEMY_05,
	ENEMY_06,
	BOSS,
};

enum class GAME_STAGE
{
	STAGE_01 = 0,
	STAGE_02,
	STAGE_03,
	STAGE_04,
	STAGE_05,
	STAGE_06,
	STAGE_BOSS,
};