//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once
#include <string>
#include <map>
#include <functional>
#include "spriteclass_constants.hpp"


class SpriteClass;

enum AI_TRIGGER{  
    AI_TRIGGER_NONE,
    AI_TRIGGER_ANYWAY, //on each game tick 
    AI_TRIGGER_ALIVE, // on each game tick if sprite->energy>0

    AI_TRIGGER_SPAWN,  //on spawn
    AI_TRIGGER_DAMAGE, //if the sprite is damaged
    AI_TRIGGER_DEATH,  //if the sprite dies

    AI_TRIGGER_SKULLS_CHANGED, //if skull blocks are changed

    AI_TRIGGER_BONUS_COLLECTED, //if bonus sprite is collected

    //considering them
    AI_TRIGGER_PLAYER_ABOVE,
    AI_TRIGGER_PLAYER_BELOW,
    AI_TRIGGER_PLAYER_IN_FRONT,
};

namespace SpriteAI{


class AI_Class{
public:
    AI_Class() = default;
    int id = AI_NONE;
    int info_id = -1;
    int trigger = AI_TRIGGER_NONE;
    std::function<void(SpriteClass* sprite)> func;

    bool apply_to_player = false;
    bool apply_to_creatures = false;
    bool apply_to_bonuses = false;
    bool apply_to_backgrounds = false;
};

void Init();
extern std::map<int, AI_Class> sprite_ai_table;

}