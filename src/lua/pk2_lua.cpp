//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################

/**
 * @brief
 * Lua utils by SaturninTheAlien
 */

#include "pk2_lua.hpp"
#include <string>

#include "lua_sprites.hpp"

#include "engine/PLog.hpp"
#include "engine/PFile.hpp"

#include "episode/episodeclass.hpp"
#include "game/game.hpp"

#include "system.hpp"

namespace PK2lua{



std::string PK2GetLuaFile(const std::string&name){
    PFile::Path file = Episode->Get_Dir(name);
    if(FindAsset(&file, "lua" PE_SEP)){
        return file.GetContentAsString();
    }
    else{
        file = Episode->Get_Dir(name+".lua");
        if(FindAsset(&file, "lua" PE_SEP)){
            return file.GetContentAsString();
        }
    }
    return "";
}

/**
 * @brief 
 * Override lua require to enable loading modules from a zipped episode
 */

void OverrideLuaRequire(sol::state& lua){

    lua["_pk2_get_lua_file"] = PK2GetLuaFile;

    sol::state* lua_ptr = &lua;

    lua["_pk2_load_string"] = [lua_ptr](const std::string&lua_code){
        return lua_ptr->safe_script(lua_code);
    };

    static const char * require_decorator = 
        "local _require = require \n"
        "require = function(mod_name) \n"
            "tmp = _pk2_get_lua_file(mod_name) \n"
            "if tmp ~= \"\" then \n"
                "return _pk2_load_string(tmp) \n"
            "elseif _require ~= nil then \n"
                "return _require(mod_name) \n"
            "end \n"
        "end \n";

    lua.safe_script(require_decorator);
}

sol::state* CreateGameLuaVM(EpisodeClass* episode){

    PFile::Path main_lua_f = episode->Get_Dir("main.lua");
    if(!FindAsset(&main_lua_f, "lua" PE_SEP)){
        PLog::Write(PLog::INFO, "PK2lua", "No Lua scripting in this episode");
        return nullptr;
    }
    PLog::Write(PLog::INFO, "PK2lua", "Loading main.lua script");
    std::string main_lua = main_lua_f.GetContentAsString();
    
    PLog::Write(PLog::INFO, "PK2lua", "Creating lua VM");
    sol::state* lua = new sol::state();
    lua->open_libraries(sol::lib::base,
            sol::lib::string,
            sol::lib::table,
            sol::lib::math,
            sol::lib::coroutine,
            sol::lib::utf8,
            sol::lib::package);

    OverrideLuaRequire(*lua);

    PLog::Write(PLog::INFO, "PK2lua", "Running main.lua");
    ExposePrototypeClass(*lua);
    ExposeSpriteClass(*lua);

    lua->safe_script(main_lua);
    return lua;
}

}