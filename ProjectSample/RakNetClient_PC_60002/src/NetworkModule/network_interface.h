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
//初始化网络引擎内部日志文件,需要第一步调用
INIT_ENGINE_RESULT init_engine();

typedef enum
{
    START_ENGINE_NOT_INIT,
    START_ENGINE_PROCESS
} START_ENGINE_RESULT;
//启动网络引擎，需要第二部调用
START_ENGINE_RESULT start_engine();

//停止网络引擎
//注意：再次启动网络引擎，需要重新调用init_engine()
void stop_engine();

typedef enum
{
    REGIST_CALLBACK_NOT_INIT,
    REGIST_CALLBACK_SUCC
} REGIST_CALLBACK_RESULT;
//注册消息回调，网络引擎接受到identify标志的消息之后，回调注册的fp
REGIST_CALLBACK_RESULT regist_callback(int identify, Fun_p fp);

typedef enum
{
    REMOVE_CALLBACK_NOT_INIT,
    REMOVE_CALLBACK_SUCC
} REMOVE_CALLBACK_RESULT;
//删除对应identify的回调函数，一般该函数在回调函数内部调用
REMOVE_CALLBACK_RESULT remove_callback(int identify);

//获取本地GUID，由于RakNet限制，NAT穿透需要GUID
const char* get_local_guid();

typedef enum
{
    PUNCH_PEER_NOT_INIT,
    PUNCH_PEER_SUCC
} PUNCH_PEER_RESULT;
//启动UDP打孔功能
//guid为另一端的GUID(可从另一端get_local_guid获得)
//succ_fp为打孔成功的回调，一般在回调函数内部需要调用remove_punch_callback,然后connect_peer_addr即可连接
//fail_fp为打孔失败的回调，一般在回调函数内部需要调用remove_punch_callback,然后开启代理功能(目前代理功能未实现)
PUNCH_PEER_RESULT punch_peer(const char* guid, Fun_p succ_fp, Fun_p fail_fp);

//辅助功能函数，由于NAT穿透的消息较多，通过还函数完成NAT相关回调的清除
void remove_punch_callback();

//connect_peer_addr成功之后，即可调用该函数发送消息
void send_peer_message(const char* ip,int port, int identify, const char* data, int size);

//网络引擎启动之后，可调用该函数向服务器发送消息
void send_server_message(int identify, const char* data, int size);

//连接P2P的接口
void connect_peer_addr(const char* ip, int port);
#ifdef __cplusplus
}
#endif

#endif
