#pragma once
#include <cassert>
#include <fstream>

#include "LuaLog.h"
#include "LuaType.h"

extern "C"{
#include "src/lauxlib.h"
#include "src/lua.h"
#include "src/lualib.h"
}
class LuaContext
{
public:
    LuaContext()
        :L(luaL_newstate()),m_RequirePath("Require.lua")
    {
        luaL_openlibs(L);
        std::fstream f;
        
        CreateEnvPathTable();
        AddEnvPath("contentDir","C:/MainDic/Pan/demo/EM/Content/");
        
        f.open(m_RequirePath);
        LUA_ASSERT(f.is_open(),"File open failed");
        
        f.seekg(0,std::ios::end);
        std::string str;
        str.resize(f.tellg());
        f.seekg(0,std::ios::beg);   //不要用windows文本文件编辑lua文件
        f.read(&str[0],str.size());
        f.close();
        LUA_ASSERT(!luaL_dostring(L,str.c_str()),"Load file failed");
    }
    ~LuaContext()
    {
        lua_close(L);
    }
    
    inline void CreateEnvPathTable()
    {
        lua_newtable(L); //table envpath
        lua_setglobal(L,"EnvPath");
    }
    inline void AddEnvPath(const char* key,const char* value)
    {
        lua_getglobal(L,"EnvPath");
        lua_pushstring(L,key);
        lua_pushstring(L,value);
        lua_settable(L,-3);
        lua_setglobal(L,"EnvPath");
    }
    lua_State* GetLuaState()
    {
        return L;
    }
    static LuaContext& Get()
    {
        static LuaContext* instance=new LuaContext();
        return *instance;
    }
private:
    lua_State* L;
    const char* m_RequirePath;
    
};
