#ifndef USER_COMMON_DEFINE_H
#define USER_COMMON_DEFINE_H

//日志文件名
#define MODULE_NAME ("network_engine")

//日志路径
#define LOG_PATH ("../log")

//日志生成等级,可选项为debug,info,warning,error
#define LOG_LEVEL ("debug")

//缺省的服务器IP地址
//#define DEFAULT_SERVER_IP ("127.0.0.1")
#define DEFAULT_SERVER_IP ("115.28.151.38")
//#define DEFAULT_SERVER_IP ("115.29.234.233")

//缺省的服务器端口
#define DEFAULT_SERVER_PORT (61111)

//缺省的本地端口
#define DEFAULT_LOCAL_PORT (60001)

//可以注册回调的函数签名
typedef void (*Fun_p)(const char* ip, int port, unsigned char* data, int size);

#endif
