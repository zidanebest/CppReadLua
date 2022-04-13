extern "C"{
#include "src/lua.h"
#include "src/lualib.h"
#include "src/lauxlib.h"
}

#include <iostream>
using namespace std;

int GetGlobalInt(lua_State* L,const char* var)
{
    int isnum,result;
    lua_getglobal(L,var);
    result=(int)lua_tointegerx(L,-1,&isnum);
    if(!isnum)
    {
        //error;
    }
    lua_pop(L,1);
    return result;
}
int GetTableField(lua_State* L,const char* var)
{
    lua_pushstring(L,var);
    lua_gettable(L,-2);
    int num;
    int result=(int)lua_tointegerx(L,-1,&num);
    if(!num)
    {
        cout<<"Not num"<<endl;
    }
    lua_pop(L,1);
    return result;
}
int GetTableField(lua_State* L,int var)
{
    lua_pushinteger(L,var);
    lua_gettable(L,-2);
    int num;
    int result=(int)lua_tointegerx(L,-1,&num);
    if(!num)
    {
        cout<<"Not num"<<endl;
    }
    lua_pop(L,1);
    return result;
}

void Load(lua_State* L,const char* fileName)
{
    if(luaL_loadfile(L,fileName)||lua_pcall(L,0,0,0))
    {
        //error    
    }
}
// class LuaReader
// {
// public:
//     LuaReader(const char* fileName):L(luaL_newstate())
//     {
//         if(luaL_loadfile(L,fileName)||lua_pcall(L,0,0,0,0))
//         {
//             //error
//             
//         }
//     }
//     // int Get
//     
// private:
//     lua_State* L;
// };

template<typename T>
ostream& error(ostream& os,T info)
{
    return os<<info<<endl;   
}
template<typename T,typename ...Args>
ostream& error(ostream& os,T info,Args ...args)
{
    os<<info<<",";
    return error(os,args...);
}

// 错误检测
#define DECLARE_CHECK_TYPE(type) \
bool IsLuaType##type(lua_State* L,int index)\
{\
return LUA_T##type==lua_type(L,index);\
}\

// Get函数
#define DECLARE_READER_TYPE(type) \
public:\
    using Int=int;\
    using String=const char*;\
private:\
template<typename T,typename ...Args>\
type Get##type##FromTableHelper(T key,Args... args)\
{\
    LuaPushElement(L,key);\
    if(!IsLuaTypeTABLE(L,-2))\
        error(cout,"lua_gettable error:not a table");\
    lua_gettable(L,-2);\
    type value=Get##type##FromTableHelper(args...);\
    lua_pop(L,1);\
    return value;\
}\
template<typename T>\
type Get##type##FromTableHelper(T key)\
{\
    LuaPushElement(L,key);\
    if(!IsLuaTypeTABLE(L,-2))\
        error(cout,"lua_gettable error:not a table");\
    lua_gettable(L,-2);\
    type value=(type)LuaTo##type(L,-1);\
    lua_pop(L,1);\
    return value;\
}\
public:\
template<typename T,typename ...Args>\
type Get##type##FromTable(T key,Args... args)\
{\
    lua_getglobal(L,m_TableName);\
    if(!IsLuaTypeTABLE(L,-1)){\
        error(cout,"Error table name");\
    }\
    return Get##type##FromTableHelper(key,args...);\
}\



class LuaReader
{
    DECLARE_CHECK_TYPE(TABLE)
    DECLARE_CHECK_TYPE(NUMBER)
    DECLARE_CHECK_TYPE(STRING)
    DECLARE_CHECK_TYPE(BOOLEAN)
private:
    void LuaPushElement(lua_State* L,int e)
    {
        lua_pushinteger(L,e);
    }
    void LuaPushElement(lua_State* L,const char* e)
    {
        lua_pushstring(L,e);
    }
    lua_Integer LuaToInt(lua_State* L,int index)
    {
        if(!IsLuaTypeNUMBER(L,index))
        {
            error(cout,"Target value not Int");
        }
        return lua_tointeger(L,index);
    }
    const char* LuaToString(lua_State* L,int index)
    {
        if(!IsLuaTypeSTRING(L,index))
        {
            error(cout,"Target value not String");
        }
        return  lua_tostring(L,index);
    }
    
    DECLARE_READER_TYPE(Int)
    DECLARE_READER_TYPE(String)
public:
    inline LuaReader(const char* tableName)
        :L(luaL_newstate()),m_TableName(tableName)
    {
        luaL_openlibs(L);
        if(luaL_loadfile(L,m_RequirePath)||lua_pcall(L,0,0,0))
        {
            //error
        }
    }
    ~LuaReader()
    {
        lua_close(L);
    }
    

private:
    lua_State* L;
    const char* m_RequirePath="Require.lua";
    const char* m_TableName;
};

int main(int argc, char* argv[])
{
    {
        LuaReader reader("Test");
        int value =reader.GetIntFromTable("e","d");
        const char* str=reader.GetStringFromTable(2,"g","k");
        cout<<value<<endl;
        cout<<str<<endl;
    }
    
    return 0;
}

