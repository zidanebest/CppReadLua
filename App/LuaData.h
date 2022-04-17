#pragma once

#include <iostream>
#include <memory>
#include <ostream>

#include "LuaLog.h"

#include "LuaType.h"


extern "C" {
#include "src/lua.h"
}

#define DECLARE_LUA_IS_TYPE(luaType)\
    bool Is##luaType() const{\
        return type==LuaType::Lua##luaType;\
    }\

inline std::ostream& operator<<(std::ostream& os,const LuaData& data);

struct LuaData
{
    std::shared_ptr<void> data;
    LuaType type;
    LuaData(lua_State* L, int index);
    // ~LuaData()
    // {
    //     std::cout<<"data: "<<*this<< std::endl;
    // }
    DECLARE_LUA_IS_TYPE(Number)
    DECLARE_LUA_IS_TYPE(Bool)
    DECLARE_LUA_IS_TYPE(String)
    DECLARE_LUA_IS_TYPE(Table)

    inline LuaNumber Cast2Number() const
    {
        LUA_ASSERT(type==LuaType::LuaNumber,"Cant cast to target number");
        return LuaNumber(*(std::static_pointer_cast<LuaNumber>(data)));
    }
    inline LuaBool Cast2Bool() const
    {
        LUA_ASSERT(type==LuaType::LuaBool,"Cant cast to target bool");
        return LuaBool(*(std::static_pointer_cast<LuaBool>(data)));
    }

    inline LuaString Cast2String() const
    {
        LUA_ASSERT(type==LuaType::LuaString,"Cant cast to target string");
        return LuaString(*(std::static_pointer_cast<LuaString>(data)));
    }

    inline const LuaTable& Cast2Table() const
    {
        LUA_ASSERT(type==LuaType::LuaTable,"Cant cast to target table");
        return *(std::static_pointer_cast<LuaTable>(data));
    }
    
    
};
std::ostream& operator<<(std::ostream& os,const LuaData& data)
{
    switch (data.type)
    {
    case LuaType::LuaBool:
        os<<data.Cast2Bool();
        break;
    case LuaType::LuaNumber:
        os<<data.Cast2Number();
        break;
    case LuaType::LuaString:
        os<<data.Cast2String();
        break;
    case LuaType::LuaTable:
        os<<"table";
        break;
    }
    return os;
}