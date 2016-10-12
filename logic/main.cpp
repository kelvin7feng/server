//
//  main.cpp
//  server
//
//  Created by 冯文斌 on 16/10/11.
//  Copyright © 2016年 kelvin. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>

#include "game_logic_server.hpp"

#define LOCAL_SERVER_PORT 7000

int main() {
    uv_loop_t *loop = uv_default_loop();
    GameLogicServer *server = GameLogicServer::get_instance();
    server->init(loop, "0.0.0.0", LOCAL_SERVER_PORT);
    
    return uv_run(loop, UV_RUN_DEFAULT);
}