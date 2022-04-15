#include "LuaTable.h"

#define LUA_READER_GET_DATA(type)\
type* GetData##type(void* data){\
return  static_cast<type*>(data);\
}\

#define LUA_DECLARE_GET_VALUE_TYPE(checkType)\
checkType GetDataCheck##checkType(const LuaData& luaData)\
{\
checkType* value=LuaType::##checkType!=luaData.type ? (assert(false,"Error type"),nullptr):(GetData##checkType(luaData.data));\
return *value;\
}\

//#define LUA_READER_DECLARE_COMMON_TYPE
using Number=double;
using Bool=bool;
using String=const char*;

LUA_READER_GET_DATA(Number);
LUA_READER_GET_DATA(Bool);
LUA_READER_GET_DATA(String);

LUA_DECLARE_GET_VALUE_TYPE(Number);
LUA_DECLARE_GET_VALUE_TYPE(Bool);
LUA_DECLARE_GET_VALUE_TYPE(String);

int LuaTable::PositiveIndex(lua_State* L, int index) const
{
    if(index<0)
    {
        return lua_gettop(L)+index+1;
    }
    return index;
}

LuaTable::LuaTable(lua_State* L, int index)
{
    index=PositiveIndex(L,index);
    table.reset(new std::unordered_map<unsigned int,LuaData*>());
    lua_pushnil(L);
    while(lua_next(L,index))
    {
        //复制一个key
        lua_pushvalue(L,-2);
        const char* key=lua_tostring(L,-1);
        
        table->insert(std::pair<unsigned int,LuaData*>(CalculateKeyHash(L,-1),new LuaData(L,-2)));
        lua_pop(L,2);
    }
    //弹出table
    //lua_pop(L,1);
}

double LuaTable::GetValue(const char* key) const
{
    LuaData* data=(table->operator[](Hash(key)));
    return  GetDataCheckNumber(*data);
}

double LuaTable::operator()(const char* key, L_Double) const
{
    LuaData* data=(table->operator[](Hash(key)));
    return  GetDataCheckNumber(*data);
}


const LuaTable& LuaTable::operator()(const char* key, L_Table) const
{
    LuaData* data=(table->operator[](Hash(key)));
    return *(LuaTable*)data->data;
}

const LuaTable& LuaTable::GetTable(const char* key) const
{
    LuaData* data=(table->operator[](Hash(key)));
    return *(LuaTable*)data->data;
}
