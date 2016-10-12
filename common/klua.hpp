//
//  klua.hpp
//  lesson01
//
//  Created by 冯文斌 on 16/9/13.
//  Copyright © 2016年 kelvin. All rights reserved.
//

#ifndef klua_hpp
#define klua_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include "lua.hpp"

#endif /* klua_hpp */

class CLuaEngine {
public:
    CLuaEngine();
    ~CLuaEngine();
    void Close();
    int InitState();
    int CallLuaAvg();
    static int LuaOpenLib(lua_State* pLuaState);
    static const luaL_Reg m_luaLibs[];
    static const luaL_Reg m_commonLibs[];
private:
    lua_State* m_pLuaState;
};
