#pragma once

#define LUA_ERROR(...)\
printf("ERROR FILE:%s,  LINE:%2d\n\%s\n",__FILE__,__LINE__,__FUNCSIG__);\
printf(__VA_ARGS__)\
    
#define LUA_ASSERT(expr,...)\
if(!(expr)){LUA_ERROR(__VA_ARGS__);__debugbreak();}
