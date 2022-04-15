#pragma once
#include <cassert>
#include <memory>
#include <string>
#include <unordered_map>

#include "LuaData.h"
extern "C"{
#include "src/lua.h"
}

class L_Int{};
class L_Double{};
class L_String{};
class L_Table{};

class LuaTable
{
private:
    unsigned int Hash(std::string str) const
    {
        return std::hash<std::string>()(str);
    }
    unsigned int Hash(int i) const
    {
        return std::hash<int>()(i);
    }
    unsigned int CalculateKeyHash(lua_State* L,int index) const
    {
        switch (lua_type(L,index))
        {
        case LUA_TNUMBER:
            return Hash(lua_tointeger(L,index));
        case LUA_TSTRING:
            return Hash(lua_tostring(L,index));
        default:assert(false);return 0;
        }
    }
    int PositiveIndex(lua_State* L,int index) const;
public:
    LuaTable(lua_State* L,int index);
    
    double GetValue(const char* key) const;
    double operator()(const char* key,L_Double) const;
    const LuaTable& operator()(const char*key,L_Table) const;
    const LuaTable& GetTable(const char* key) const;
private:
    std::shared_ptr<std::unordered_map<unsigned int,LuaData*>> table;
    
};
