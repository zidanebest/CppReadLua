#include "LuaData.h"
#include "LuaTable.h"
#include <cassert>




LuaData::LuaData(lua_State* L, int index)
{
    switch (lua_type(L,index))
    {
    case LUA_TNUMBER:
        data.reset(new double(lua_tonumber(L,index)));
        type=LuaType::LuaNumber;
        break;
    case LUA_TBOOLEAN:
        data.reset(new bool(lua_toboolean(L,index)));
        type=LuaType::LuaBool;
        break;
    case LUA_TSTRING:
        data.reset(new const char*(lua_tostring(L,index)));
        type=LuaType::LuaString;
        break;
    case LUA_TTABLE:
        data.reset(new LuaTable(L,index));
        type=LuaType::LuaTable;
        break;
    default:assert(false);
    }
}
