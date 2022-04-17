#pragma once

#include <cassert>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include "LuaType.h"
#include "LuaData.h"
#include "LuaContext.h"

extern "C"{
#include "src/lua.h"
}


class LuaTable
{
    using TableType=std::unordered_map<std::string,LuaData>;
    using TablePtr=std::shared_ptr<TableType>;
    
private:  
     std::string ToString(lua_State* L,int index) const
     {
         auto type=lua_type(L,index);
         switch (lua_type(L,index))
         {
         case LUA_TSTRING:
             return lua_tostring(L,index);
         default:LUA_ASSERT(false,"");return 0;
         }
     }
    int PositiveIndex(lua_State* L,int index) const;
    
private:
    template<typename T,typename ...Arg>
    void InitHelper(lua_State* L,T t,Arg ...arg);
    template<typename T>
    void InitHelper(lua_State* L,T t);
public:
    template<typename ...Arg>
    LuaTable(const char* globalName,Arg ...arg);
    LuaTable(const char* globalName);
    LuaTable(const LuaTable&);
    LuaTable(lua_State* L,int index);
    void operator=(const LuaTable& table)
    {
        m_Table=table.m_Table;
    }

/** 按索引取值 **/
private:
    template <typename T>
    double GetValue(T key, Lua_Type_Number) const;

    template <typename T>
    bool GetValue(T key, Lua_Type_Bool) const;

    template <typename T>
    LuaString GetValue(T key, Lua_Type_String) const;

    template <typename T>
    std::shared_ptr<LuaTable> GetValue(T key, Lua_Type_Table) const;

    template <typename T>
    const LuaData& GetValue(T key) const;

public:  
    template<typename T,typename ...Arg>
    double GetNumber(T key,Arg...arg) const;
    
    template<typename T>
    double GetNumber(T key) const;
    
    template<typename T,typename ...Arg>
    bool GetBool(T key,Arg...arg) const;
    
    template<typename T>
    bool GetBool(T key) const;

    template<typename T,typename ...Arg>
    const char* GetString(T key,Arg...arg) const;

    template<typename T>
    std::string GetString(T key) const;

    template<typename T,typename ...Arg>
    std::shared_ptr<LuaTable> GetTable(T key,Arg...arg) const;

    template<typename T>
    std::shared_ptr<LuaTable> GetTable(T key) const;

    template<typename T,typename ...Arg>
    const LuaData& GetData(T key,Arg...arg) const;

    template<typename T>
    LuaData GetData(T key) const;

public:
    //遍历元素
    void TraverseTable(std::function<bool(std::string key,const LuaData& value)> func) const;
    
    //遍历元素，遇到表递归
    void TraverseTableRecursive(std::function<bool(std::string key,const LuaData& value)> func) const;
public:
    TablePtr m_Table;
    
};

template<typename ...Arg>
LuaTable::LuaTable(const char* globalName,Arg ...arg)
{
    lua_State* L=LuaContext::Get().GetLuaState();
    lua_getglobal(L,globalName);
    LUA_ASSERT(IsLuaTypeTABLE(L,-1),"invalid table name");
    InitHelper(L,arg...); //直到将目标table置于index=-1处
    
    int index=PositiveIndex(L,-1);

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
    lua_pop(L,1);
}

void PushElement(lua_State* L,int e);
void PushElement(lua_State* L,const char* e);

template<typename T,typename ...Arg>
void LuaTable::InitHelper(lua_State* L,T t,Arg ...arg)
{
    PushElement(L,t);
    lua_gettable(L,-2);
    lua_remove(L,-2); //将之前压入的table弹出
    LUA_ASSERT(IsLuaTypeTABLE(L,-1),"invalid table name");
    InitHelper(L,arg...);
}
template<typename T>
void LuaTable::InitHelper(lua_State* L,T t)
{
    PushElement(L,t);
    lua_gettable(L,-2);
    lua_remove(L,-2); //将之前压入的table弹出
    LUA_ASSERT(IsLuaTypeTABLE(L,-1),"invalid table name %s",lua_typename(L,-1));
}

template <typename T>
double LuaTable::GetValue(T key, Lua_Type_Number) const
{
    std::string realKey=std::string(key);
    auto it=m_Table->find(realKey);
    LUA_ASSERT(it!=m_Table->end(),"Cant find value");
    return  GetDataCheckLuaNumber(it->second);
}

template <typename T>
bool LuaTable::GetValue(T key, Lua_Type_Bool) const
{
    std::string realKey=std::string(key);
    auto it=m_Table->find(realKey);
    LUA_ASSERT(it!=m_Table->end(),"Cant find value");
    return  GetDataCheckLuaBool(it->second);
}

template <typename T>
LuaString LuaTable::GetValue(T key, Lua_Type_String) const
{
    std::string realKey=std::string(key);
    auto it=m_Table->find(realKey);
    LUA_ASSERT(it!=m_Table->end(),"Cant find value");
    return  GetDataCheckLuaString(it->second);
}

template <typename T>
std::shared_ptr<LuaTable> LuaTable::GetValue(T key, Lua_Type_Table) const
{
    std::string realKey=std::string(key);
    auto it=m_Table->find(realKey);
    LUA_ASSERT(it!=m_Table->end(),"Cant find value");
    return  GetDataCheckLuaTable(it->second);
}
template<typename T>
const LuaData& LuaTable::GetValue(T key) const
{
    std::string realKey=std::string(key);
    auto it=m_Table->find(realKey);
    LUA_ASSERT(it!=m_Table->end(),"Cant find value");
    return (it->second);
}

template <typename T, typename ... Arg>
double LuaTable::GetNumber(T key, Arg... arg) const
{
    std::shared_ptr<LuaTable> table=GetValue(key,Lua_Type_Table());
    return table->GetNumber(arg...);
}

template <typename T>
double LuaTable::GetNumber(T key) const
{
    double value=GetValue(key,Lua_Type_Number());
    return value;
}

template <typename T, typename ... Arg>
bool LuaTable::GetBool(T key, Arg... arg) const
{
    std::shared_ptr<LuaTable> table=GetValue(key,Lua_Type_Table());
    return table->GetBool(arg...);
}

template <typename T>
bool LuaTable::GetBool(T key) const
{
    bool value=GetValue(key,Lua_Type_Bool());
    return value;
}

template <typename T, typename ... Arg>
const char* LuaTable::GetString(T key, Arg... arg) const
{
    std::shared_ptr<LuaTable> table=GetValue(key,Lua_Type_Table());
    return table->GetString(arg...);
}

template <typename T>
std::string LuaTable::GetString(T key) const
{
    const char* value=GetValue(key,Lua_Type_String());
    return value;
}

template <typename T, typename ... Arg>
std::shared_ptr<LuaTable> LuaTable::GetTable(T key, Arg... arg) const
{
    std::shared_ptr<LuaTable> table=GetValue(key,Lua_Type_Table());
    return table->GetTable(arg...);
}

template <typename T>
std::shared_ptr<LuaTable> LuaTable::GetTable(T key) const
{
    std::shared_ptr<LuaTable> value=GetValue(key,Lua_Type_Table());
    return value;
}

template <typename T, typename ... Arg>
const LuaData& LuaTable::GetData(T key, Arg... arg) const
{
    std::shared_ptr<LuaTable> table=GetValue(key,Lua_Type_Table());
    return table->GetData(arg...);
}

template <typename T>
LuaData LuaTable::GetData(T key) const
{
    return GetValue(key);
}
