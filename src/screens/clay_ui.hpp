//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
/**
 * @brief 
 * New UI layout engine(Clay) util ported by AbdullahTrees to replace hardcoded menu layouts.
 */
#pragma once

#include <3rd_party/clay.h>

#include "engine/Piste.hpp"
#include "engine/PInput.hpp"

#include <cstring>

namespace ClayUI {

    enum {
        SMALL_FONT,
        BIG_FONT,
    } Text;

    void Init();
    void Update();

    void Draw_ClayLayout(Clay_RenderCommandArray to_draw);

    Clay_String MakeClayString(const char *str, bool copy=false);
}
