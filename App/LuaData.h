#pragma once
#include <cassert>
#include <cstdint>

extern "C"{
#include "src/lua.h"
}


//GetData 类型检查
enum class LuaType:uint8_t
{
    Number=0,
    Bool,
    String,
    Table
};

struct LuaData
{
    LuaType type;
    void* data;
    LuaData(lua_State* L,int index);
};