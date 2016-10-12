//
//  game_logic_server.hpp
//  server
//
//  Created by 冯文斌 on 16/10/10.
//  Copyright © 2016年 kelvin. All rights reserved.
//

#ifndef game_logic_server_hpp
#define game_logic_server_hpp

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

#include <map>
#include <uv.h>

#include "google.pb.h"

#include "klua.hpp"
#include "tcp_client.hpp"
#include "tcp_base_server.hpp"

#endif /* game_logic_server_hpp */

using namespace std;

class GameLogicServer : public TCPBaseServer {
    
public:
    
    GameLogicServer();
    
    ~GameLogicServer();
    
    GameLogicServer(const GameLogicServer& GameLogicServer);
    
    static GameLogicServer* get_instance();
    
    virtual int init(uv_loop_t* loop, const char* ip, int port);
    
    virtual void on_new_connection(uv_stream_t *server, int status);
    
    virtual void on_msg_recv(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf);
    
protected:
    
    session_map_t open_sessions;
    
    CLuaEngine lua_engine;
    
    int totol_request;
    
};