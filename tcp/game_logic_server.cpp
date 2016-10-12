//
//  game_logic_server.cpp
//  server
//
//  Created by 冯文斌 on 16/10/10.
//  Copyright © 2016年 kelvin. All rights reserved.
//

#include "game_logic_server.hpp"

#define DEFAULT_BACKLOG 1000

GameLogicServer::GameLogicServer()
{
    lua_engine.InitState();
}

GameLogicServer::~GameLogicServer()
{
    uv_loop_close(m_loop);
    cout << "Server Terminated" << endl;
}

GameLogicServer* GameLogicServer::get_instance()
{
    static GameLogicServer server;
    return &server;
}

int GameLogicServer::init(uv_loop_t* loop, const char* ip, int port)
{
    m_ip = ip;
    m_port = port;
    m_loop = loop;
    
    uv_tcp_init(loop, &m_server);
    
    uv_ip4_addr(ip, m_port, &m_addr);
    
    uv_tcp_bind(&m_server, (const struct sockaddr*)&m_addr, 0);
    
    int r = uv_listen((uv_stream_t*) &m_server, DEFAULT_BACKLOG,
                      [](uv_stream_t* server, int status)
                      {
                          GameLogicServer::get_instance()->on_new_connection(server, status);
                      });
    
    if (r) {
        fprintf(stderr, "Listen error %s\n", uv_strerror(r));
        return 0;
    }
    
    cout << "logic server listen: " << m_ip << ":" << m_port << endl;
    return 1;
}

void GameLogicServer::on_msg_recv(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf)
{
    
    lua_engine.CallLuaAvg();
    
    totol_request++;
    
    cout << totol_request << endl;
    
    free(buf->base);
}

void GameLogicServer::on_new_connection(uv_stream_t *server, int status)
{
    if (status < 0) {
        fprintf(stderr, "New connection error %s\n", uv_strerror(status));
        return;
    }
    
    TCPSession new_session;
    new_session.connection = std::make_shared<uv_tcp_t>();
    
    uv_tcp_init(m_loop, new_session.connection.get());
    if (uv_accept(server, (uv_stream_t*)new_session.connection.get()) == 0) {
        
        uv_read_start((uv_stream_t*)new_session.connection.get(),
                      [](uv_handle_t* stream, size_t nread, uv_buf_t *buf)
                      {
                          GameLogicServer::get_instance()->alloc_buffer(stream, nread, buf);
                      },
                      [](uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
                      {
                          GameLogicServer::get_instance()->on_msg_recv(stream, nread, buf);
                      });
        
        uv_stream_t* key = (uv_stream_t*)new_session.connection.get();
        open_sessions.insert({key, new_session});
        
    }
    else {
        uv_close((uv_handle_t*)new_session.connection.get(), NULL);
    }
}
