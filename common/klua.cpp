//
//  klua.cpp
//  lesson01
//
//  Created by 冯文斌 on 16/9/13.
//  Copyright © 2016年 kelvin. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <string>
#include "lua.hpp"
#include "klua.hpp"

using namespace std;
CLuaEngine::CLuaEngine()
{
    
}

CLuaEngine::~CLuaEngine()
{
    this->Close();
}

void CLuaEngine::Close()
{
    lua_close(m_pLuaState);
}

//调用lua的平均数函数
int CLuaEngine::CallLuaAvg(){
    //获取lua里面的add函数并把它放到lua的栈顶
    lua_getglobal(m_pLuaState, "lua_average");
    
    //调用lua函数,这里的0是参数的个数，0是返回值的个数
    lua_call(m_pLuaState, 0, 1);
    int nAvg = (int)lua_tointeger(m_pLuaState, -1);
    cout << "avg: " << nAvg << endl;
    
    //最后我们把返回值从栈顶拿掉
    lua_pop(m_pLuaState, 1);
    return 0;
}

//设置table到lua里
static void luaSetGlobalTable(lua_State* lua_state)
{
    lua_newtable(lua_state);
    lua_pushstring(lua_state, "key");
    lua_pushnumber(lua_state, 1);
    
    lua_rawset(lua_state, -3);
    
    lua_setglobal(lua_state, "G_tb");
    
    lua_pop(lua_state, 1);
    
    lua_getglobal(lua_state, "print_r");
    lua_call(lua_state, 0, 0);
}

//设置lua里的全局字符串
static void luaSetGlobalString(lua_State* lua_state, const char* key, const char* value)
{
    lua_pushstring(lua_state, value);
    lua_setglobal(lua_state, key);
    lua_pop(lua_state, 1);
}

//获取lua里的全局字符串
static string luaGetGlobalValue(lua_State* lua_state, const char* key)
{
    lua_getglobal(lua_state, key);
    string szGlobalValue = lua_tostring(lua_state, -1);
    lua_pop(lua_state, 1);
    return szGlobalValue;
}

//调用lua的add函数
static int luaAdd(lua_State *lua_state , int x, int y)
{
    int sum;
    //获取lua里面的add函数并把它放到lua的栈顶
    lua_getglobal(lua_state, "add");
    //往lua栈里面压入两个参数
    lua_pushnumber(lua_state, x);
    lua_pushnumber(lua_state, y);
    //调用lua函数,这里的2是参数的个数，1是返回值的个数
    lua_call(lua_state, 2, 1);
    //从栈顶读取返回值,注意这里的参数是-1
    sum = (int)lua_tointeger(lua_state, -1);
    //最后我们把返回值从栈顶拿掉
    lua_pop(lua_state, 1);
    return sum;
}

//Lua调用C++的函数
static int Average(lua_State* pLuaState){
    //获取参数个数
    int n = lua_gettop(pLuaState);
    double sum = 0;
    int temp = 0;
    
    for (int i = 1; i <= n; i++) {
        temp = lua_tonumber(pLuaState, i);
        sum += temp;
    }
    
    int avg = sum/n;
    lua_pushnumber(pLuaState, avg);
    
    return 1;
}

//Lua引擎初始化
int CLuaEngine::InitState()
{
    m_pLuaState = luaL_newstate();
    
    const luaL_Reg *lib = m_luaLibs;
    for(; lib->func != NULL; lib++)
    {
        luaL_requiref(m_pLuaState, lib->name, lib->func, 1);
        lua_settop(m_pLuaState, 0);
    }
    
    string szScriptPath = "/Users/Kelvin/workspace/server/script/main.lua";
    int nStatus = luaL_dofile(m_pLuaState, szScriptPath.c_str());
    
    int nResult = 0;
    if(nStatus == LUA_OK)
    {
        nResult = lua_pcall(m_pLuaState, 0, LUA_MULTRET, 0);
    }
    else
    {
        std::cout << " Could not load the script, error code:" << nStatus << std::endl;
    }
    
    return nResult;
}


static const struct luaL_Reg stuentlib_f [] = {
    {NULL, NULL}
};

int luaopen_student (lua_State *L) {
    luaL_newlib(L, stuentlib_f);
    return 1;
}

const luaL_Reg CLuaEngine::m_commonLibs[] =
{
    {"Average", Average},
    {NULL, NULL}
};

const luaL_Reg CLuaEngine::m_luaLibs[] =
{
    {"base", luaopen_base},
    {"io", luaopen_io},
    {"cc",luaopen_student},
    {"luaEngine", CLuaEngine::LuaOpenLib},
    {NULL, NULL}
};

int CLuaEngine::LuaOpenLib(lua_State* pLuaState)
{
    luaL_newlib(pLuaState, m_commonLibs);
    return 1;
}