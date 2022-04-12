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






#define DECLARE_READER_TYPE(type) \
using Int=int;\
using String=const char*;\
private:\
template<typename T,typename ...Args>\
type Get##type##FromTableHelper(T key,Args... args)\
{\
LuaPushElement(L,key);\
lua_gettable(L,-2);\
type value=Get##type##FromTableHelper(args...);\
lua_pop(L,1);\
return value;\
}\
template<typename T>\
type Get##type##FromTableHelper(T key)\
{\
LuaPushElement(L,key);\
lua_gettable(L,-2);\
type value=(type)LuaTo##type(L,-1);\
lua_pop(L,1);\
return value;\
}\
public:\
template<typename T,typename ...Args>\
type Get##type##FromTable(const char* tableName,T key,Args... args)\
{\
lua_getglobal(L,tableName);\
return Get##type##FromTableHelper(key,args...);\
}\

class LuaReader
{
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
        return lua_tointeger(L,index);
    }
    const char* LuaToString(lua_State* L,int index)
    {
        return  lua_tostring(L,index);
    }
public:
    inline LuaReader(const char* fileName)
        :L(luaL_newstate())
    {
        luaL_openlibs(L);
        if(luaL_loadfile(L,fileName)||lua_pcall(L,0,0,0))
        {
            //error
        }
    }
    DECLARE_READER_TYPE(Int);
    DECLARE_READER_TYPE(String);

private:
    lua_State* L;
};

int main(int argc, char* argv[])
{
    LuaReader reader("Require.lua");
    int value =reader.GetIntFromTable("t","c","d");
    const char* str=reader.GetStringFromTable("t",2,"g","k");
    cout<<value<<endl;
    cout<<str<<endl;
    return 0;
}

