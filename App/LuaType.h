#pragma once
#include <cstdint>
#include <memory>

extern "C"{
#include "src/lua.h"
}


class LuaTable;
struct LuaData;

enum class LuaType:uint8_t
{
    LuaNumber=0,
    LuaBool,
    LuaString,
    LuaTable
};

class Lua_Type_Number{};
class Lua_Type_Bool{};
class Lua_Type_String{};
class Lua_Type_Table{};

#define LUA_READER_DECLARE_COMMON_TYPE
 using LuaNumber=double;
 using LuaBool=bool;
 using LuaString=const char*;

LUA_READER_DECLARE_COMMON_TYPE

//声明：获取LuaData的数据，包含类型检查
 #define LUA_DECLARE_GET_FUNCTIONS(type)\
     type* GetData##type(void* data);\
     type GetDataCheck##type(const LuaData& luaData);\

LUA_DECLARE_GET_FUNCTIONS(LuaNumber);
LUA_DECLARE_GET_FUNCTIONS(LuaBool);
LUA_DECLARE_GET_FUNCTIONS(LuaString);

std::shared_ptr<LuaTable> GetDataLuaTable(std::shared_ptr<void> data);
std::shared_ptr<LuaTable> GetDataCheckLuaTable(const LuaData& luaData);
 

#define DECLARE_CHECK_TYPE(type) \
bool IsLuaType##type(lua_State* L,int index);\

DECLARE_CHECK_TYPE(TABLE)
DECLARE_CHECK_TYPE(NUMBER)
DECLARE_CHECK_TYPE(STRING)
DECLARE_CHECK_TYPE(BOOLEAN)

