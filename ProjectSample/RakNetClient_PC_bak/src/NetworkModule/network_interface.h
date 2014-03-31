#ifndef NETWORK_INTERFACE_H
#define NETWORK_INTERFACE_H

#include "user_common_define.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
    INIT_ENGINE_LOG_ERROR,
    INIT_ENGINE_SUCC
}
INIT_ENGINE_RESULT;
INIT_ENGINE_RESULT init_engine();

typedef enum
{
    START_ENGINE_NOT_INIT,
    START_ENGINE_PROCESS
} START_ENGINE_RESULT;
START_ENGINE_RESULT start_engine();

void stop_engine();

typedef enum
{
    REGIST_CALLBACK_NOT_INIT,
    REGIST_CALLBACK_SUCC
} REGIST_CALLBACK_RESULT;
REGIST_CALLBACK_RESULT regist_callback(int identify, Fun_p fp);

typedef enum
{
    REMOVE_CALLBACK_NOT_INIT,
    REMOVE_CALLBACK_SUCC
} REMOVE_CALLBACK_RESULT;
REMOVE_CALLBACK_RESULT remove_callback(int identify);

const char* get_local_guid();

typedef enum
{
    CONNECT_PEER_NOT_INIT,
    CONNECT_PEER_SUCC
} CONNECT_PEER_RESULT;
CONNECT_PEER_RESULT connect_peer(const char* guid);

void send_peer_message(const char* ip,int port, int identify, const char* data, int size);

void send_server_message(int identify, const char* data, int size);

#ifdef __cplusplus
}
#endif

#endif
