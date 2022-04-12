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
    if(luaL_loadfile(L,fileName)||lua_pcall(L,0,0,0,0))
    {
        //error    
    }
}
class LuaReader
{
public:
    LuaReader(const char* fileName):L(luaL_newstate())
    {
        if(luaL_loadfile(L,fileName)||lua_pcall(L,0,0,0,0))
        {
            //error
            
        }
    }
    // int Get
    
private:
    lua_State* L;
};


int main(int argc, char* argv[])
{
    lua_State* L=luaL_newstate();
    luaL_openlibs(L);
    
    Load(L,"Test.lua");
    lua_getglobal(L,"t");
    if(!lua_istable(L,-1))
    {
        cout<<"Test not table"<<endl;
    }
    cout<<GetTableField(L,1)<<endl;
    return 0;
}

