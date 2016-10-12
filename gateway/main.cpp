//
//  main.cpp
//  server
//
//  Created by 冯文斌 on 16/10/10.
//  Copyright © 2016年 kelvin. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>
#include "gateway_server.hpp"

#define LOCAL_SERVER_PORT 7001
#define LOGIC_SERVER_PORT 7000

int main() {
    uv_loop_t *loop = uv_default_loop();
    GatewayServer *server = GatewayServer::get_instance();
    server->init(loop, "0.0.0.0", LOCAL_SERVER_PORT);
    
    TCPClient* client = TCPClient::get_instance();
    client->init(loop, "127.0.0.1", LOGIC_SERVER_PORT);
    
    return uv_run(loop, UV_RUN_DEFAULT);
}