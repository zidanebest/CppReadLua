#include "LuaTable.h"

int LuaTable::PositiveIndex(lua_State* L, int index) const
{
    if(index<0)
    {
        return lua_gettop(L)+index+1;
    }
    return index;
}
LuaTable::LuaTable(const char* globalName)
{
    lua_State* L=LuaContext::Get().GetLuaState();
    lua_getglobal(L,globalName);
    LUA_ASSERT(IsLuaTypeTABLE(L,-1),"invalid table name");
    
    int index=PositiveIndex(L,-1);
    m_Table.reset(new TableType());
    lua_pushnil(L);
    std::cout<<lua_gettop(L)<< std::endl;
    while(lua_next(L,index))
    {
        //复制一个key
        lua_pushvalue(L,-2);
        //auto key=lua_tostring(L,-1);
        //std::cout<<lua_gettop(L)<< std::endl;
        lua_tostring(L,-1);
        //std::cout<<lua_gettop(L)<< std::endl;
        m_Table->insert(std::pair<std::string,LuaData>(ToString(L,-1),LuaData(L,-2)));
        lua_pop(L,2);
    }
    lua_pop(L,1);
}


LuaTable::LuaTable(const LuaTable& table)
{
    m_Table=table.m_Table;
}

LuaTable::LuaTable(lua_State* L, int index)
{
    index=PositiveIndex(L,index);
    m_Table.reset(new TableType());
    lua_pushnil(L);
    while(lua_next(L,index))
    {
        //复制一个key
        lua_pushvalue(L,-2);
        const char* key=lua_tostring(L,-1);
        
        m_Table->insert(std::pair<std::string,LuaData>(ToString(L,-1),LuaData(L,-2)));
        lua_pop(L,2);
    }
}

void LuaTable::TraverseTable(std::function<bool(std::string key, const LuaData& value)> func) const
{
    for(auto it=m_Table->begin();it!=m_Table->end();++it)
    {
        if(!func(it->first,it->second))
        {
            return;
        }    
    }
}

void LuaTable::TraverseTableRecursive(std::function<bool(std::string key, const LuaData& value)> func) const
{
    LUA_ASSERT(!m_Table->empty(),"table is empty");
    for(auto it=m_Table->begin();it!=m_Table->end();++it)
    {
        if(!func(it->first,it->second))
        {
            return;
        }
        if(it->second.IsTable())
        {
            it->second.Cast2Table().TraverseTableRecursive(func);
        }
    }
}

void PushElement(lua_State* L, int e)
{
    lua_pushinteger(L,e);
}

void PushElement(lua_State* L, const char* e)
{
    lua_pushstring(L,e);
}

