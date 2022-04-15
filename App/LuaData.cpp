#include "LuaTable.h"
#include "LuaData.h"

LuaData::LuaData(lua_State* L, int index)
    :data(nullptr)
{
    switch (lua_type(L,index))
    {
    case LUA_TNUMBER:
        data=new double(lua_tonumber(L,index));
        type=LuaType::Number;
        break;
    case LUA_TBOOLEAN:
        data=new bool(lua_toboolean(L,index));
        type=LuaType::Bool;
        break;
    case LUA_TSTRING:
        data=new const char*(lua_tostring(L,index));
        type=LuaType::String;
        break;
    case LUA_TTABLE:
        data=new LuaTable(L,index);
        type=LuaType::Table;
        break;
    default:assert(false);
    }
}