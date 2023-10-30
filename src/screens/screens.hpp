//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "engine/types.hpp"
#include "engine/PFile.hpp"

enum SCREEN {
	SCREEN_NOT_SET,
	SCREEN_FIRST_START,
	SCREEN_INTRO,
	SCREEN_MENU,
	SCREEN_MAP,
	SCREEN_GAME,
	SCREEN_SCORING,
	SCREEN_END,

	SCREEN_LEVEL_ERROR,
};

extern int current_screen;
extern int next_screen;

extern uint menu_valittu_id;
extern uint menu_valinta_id;

extern PFile::Path lua_script_path;

void Fade_Quit();

void Screen_Intro_Init();
void Screen_Menu_Init();
void Screen_Map_Init();
void Screen_InGame_Init();
void Screen_ScoreCount_Init();
void Screen_Ending_Init();

void Screen_Intro();
void Screen_Menu();
void Screen_Map();
void Screen_InGame();
void Screen_ScoreCount();
void Screen_Ending();

void Screen_First_Start();
void Screen_Loop();