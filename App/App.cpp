#include <assert.h>
#include <fstream>
#include <functional>
#include <unordered_map>

#include "LuaTable.h"


extern "C"{
#include "src/lobject.h"
#include "src/lua.h"
#include "src/lualib.h"
#include "src/lauxlib.h"
}

#include <iostream>
using namespace std;

#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS  /* avoid warnings about ISO C functions */
#endif

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
    // 压入键类型
    void LuaPushElement(lua_State* L,int e)
    {
        lua_pushinteger(L,e);
    }
    void LuaPushElement(lua_State* L,const char* e)
    {
        lua_pushstring(L,e);
    }
    // 返回类型
    lua_Integer LuaToInt(lua_State* L,int index)
    {
        assert(IsLuaTypeNUMBER(L,index),"Target value not Int");
        return lua_tointeger(L,index);
    }
    const char* LuaToString(lua_State* L,int index)
    {
        assert(IsLuaTypeSTRING(L,index),"Target value not String");
        return  lua_tostring(L,index);
    }
    int LuaToBool(lua_State* L,int index)
    {
        assert(IsLuaTypeBOOLEAN(L,index),"Target value not Bool");
        return lua_toboolean(L,index);
    }
    
public:
    using Int=int;
    using String=const char*;
    using Bool=bool;
    DECLARE_READER_TYPE(Int)
    DECLARE_READER_TYPE(String)
    DECLARE_READER_TYPE(Bool)
    
    // template<typename T,typename ...Arg>
    // void TraverseTable(const std::function<bool(int,int)>& func,T key,Arg ...arg)
    // {
    //     lua_getglobal(L,m_TableName);
    //     if(!IsLuaTypeTABLE(L,-1)){
    //         error(cout,"Error table name");
    //     }
    //     TraverseTableHelper(func,key,arg...);
    //     
    // }
    // template<typename T,typename ...Arg>
    // void TraverseTableHelper(const std::function<bool(int,int)>& func,T key,Arg ...arg)
    // {
    //     LuaPushElement(L,key);
    //     if(!IsLuaTypeTABLE(L,-2))\
    //         error(cout,"lua_gettable error:not a table");\
    //     lua_gettable(L,-2);\
    //     TraverseTableHelper(args...);
    //     lua_pop(L,1);
    // }
    // template<typename T>
    // void TraverseTableHelper(const std::function<bool(int,int)>& func,T key)
    // {
    //     LuaPushElement(L,key);
    //     if(!IsLuaTypeTABLE(L,-2))
    //         error(cout,"lua_gettable error:not a table");
    //     lua_gettable(L,-2);
    //     type value=(type)LuaTo##type(L,-1);
    //     lua_pop(L,1);
    // }
    //template<typename T>
    //void TraverTableHelper(const std::function<bool(int,int)>& func)




    
public:
    inline LuaReader(const char* tableName)
        :L(luaL_newstate()),m_TableName(tableName)
    {
        luaL_openlibs(L);
        std::fstream f;
        f.open(m_RequirePath);

        assert(f.is_open(),"File open failed");

        f.seekg(0,std::ios::end);
        std::string str;
        str.resize(f.tellg());
        f.seekg(3,std::ios::beg);
        f.read(&str[0],str.size());
        f.close();

        assert(!luaL_dostring(L,str.c_str()),"Load file failed");

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


#define Init()\
lua_State* L=luaL_newstate();\
luaL_openlibs(L);\
std::fstream f;\
f.open("Require.lua");\
\
assert(f.is_open(),"File open failed");\
\
f.seekg(0,std::ios::end);\
std::string str;\
str.resize(f.tellg());\
f.seekg(3,std::ios::beg);\
f.read(&str[0],str.size());\
f.close();\
\
assert(!luaL_dostring(L,str.c_str()),"Load file failed");\
\
lua_getglobal(L,"T");\
int i=lua_gettop(L);\
lua_pushnil(L);\
// while(lua_next(L,i))\
// {\
// lua_pushvalue(L,-2);\
// printf("%s-%s\n",lua_tostring(L,-1),lua_tostring(L,-2));\
// lua_pop(L,2);\
// }\





//LUA_READER_DECLARE_COMMON_TYPE;







int main(int argc, char* argv[])
{
    Init()

    lua_getglobal(L,"T");
    LuaTable table(L,-1);
    lua_pop(L,1);
    // LuaData data(L,-1);
    // cout<<GetDataCheckString(data)<<endl;
    
    cout<<table("a",L_Table())("b",L_Double())<<endl; 

    
    return 0;
}



// #define BITS_PER_WORD (CHAR_BIT*sizeof(unsigned int))
// #define I_WORD(i) ((unsigned int)i/BITS_PER_WORD)
// #define I_Bits(i) (1<<(unsigned int)i%BITS_PER_WORD)
//
// typedef struct 
// {
//     int size;
//     unsigned int value[1];
// }BitArray;
//
// static int newarray(lua_State* L)
// {
//     int n=(int) luaL_checkinteger(L,1);
//     luaL_argcheck(L,n>=1,1,"invalid size");
//     size_t nbytes=sizeof(BitArray)+I_WORD(n-1)*sizeof(unsigned int);
//
//     BitArray* a=(BitArray*)lua_newuserdata(L,nbytes);
//     return 1;
// }
// BitArray* checkarray(lua_State* L);
// static int getarray(lua_State* L)
// {
//     BitArray* array=(BitArray*)lua_touserdata(L,1);
//     luaL_argcheck(L,array!=nullptr,1,"array expected");
//     int index =lua_tointeger(L,2)-1;
//     luaL_argcheck(L,0<=index&&index<array->size,2,"invalid index");
//     lua_pushboolean(L,array->value[I_WORD(index)]&I_Bits(index));
//     return 1;
// }
// static int setarray(lua_State* L)
// {
//     BitArray* array=(BitArray*)lua_touserdata(L,1);
//     luaL_argcheck(L,array!=nullptr,1,"array expected");
//     int index=lua_tointeger(L,2)-1;
//     luaL_argcheck(L,index>=0&&index<array->size,2,"invalid index");
//     luaL_checkany(L,3);
//     int value=lua_toboolean(L,3);
//     if(value)
//     {
//         array->value[I_WORD(index)]|=I_Bits(index);
//     }
//     else
//     {
//         array->value[I_WORD(index)]&=-I_Bits(index);
//     }
//     return 1;
// }
//
// static const struct luaL_Reg arraylib[]={
//     {"new",newarray},
//     {"set",setarray},
//     {"get",getarray},
//     {NULL,NULL}
// };
// int luaopen_array(lua_State* L)
// {
//     luaL_newlib(L,arraylib);
//     return 1;
// }