#include "LuaType.h"
#include <cassert>
#include <memory>
#include "LuaTable.h"


//定义：获取LuaData的数据，包含类型检查
#define LUA_READER_GET_DATA(type)\
std::shared_ptr<type> GetData##type(std::shared_ptr<void> data){\
return  std::static_pointer_cast<type>(data);\
}\

#define LUA_DECLARE_GET_TYPE(checkType)\
checkType GetDataCheck##checkType(const LuaData& luaData)\
{\
std::shared_ptr<checkType> value;\
    if(LuaType::##checkType!=luaData.type)\
    {\
        LUA_ASSERT(false,"Error type");\
        value=nullptr;\
    }\
    else\
    {\
        value=GetData##checkType(luaData.data);\
    }\
return *value;\
}\

LUA_READER_GET_DATA(LuaNumber);
LUA_READER_GET_DATA(LuaBool);
LUA_READER_GET_DATA(LuaString);

LUA_DECLARE_GET_TYPE(LuaNumber);
LUA_DECLARE_GET_TYPE(LuaBool);
LUA_DECLARE_GET_TYPE(LuaString);

LUA_READER_GET_DATA(LuaTable);
const LuaTable& GetDataCheckLuaTable(const LuaData& luaData)
{
    std::shared_ptr<LuaTable> value;
    if(LuaType::LuaTable!=luaData.type)
    {
        LUA_ASSERT(false,"Error type");
        value=nullptr;
    }
    else
    {
        value=GetDataLuaTable(luaData.data);
    }
    return *value;
}

//类型判断函数
#define DEFINE_CHECK_TYPE(type) \
bool IsLuaType##type(lua_State* L,int index)\
{\
return LUA_T##type==lua_type(L,index);\
}\

DEFINE_CHECK_TYPE(TABLE)
DEFINE_CHECK_TYPE(NUMBER)
DEFINE_CHECK_TYPE(STRING)
DEFINE_CHECK_TYPE(BOOLEAN)