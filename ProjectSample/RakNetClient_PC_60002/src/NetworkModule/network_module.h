#ifndef __NETWORK_MODULE__
#define __NETWORK_MODULE__

#include <string>
#include <map>
#include "Common/thread_module_base.h"
#include "Common/logger.h"
#include "network_module_namespace.h"
#include "RakNetSource/RakPeerInterface.h"
#include "RakNetSource/NatPunchthroughClient.h"
#include "user_common_define.h"

#define NET_MOD_DEBUG(...)   LOG_DEBUG(Network_module::ms_logger, __VA_ARGS__)
#define NET_MOD_INFO(...)    LOG_INFO(Network_module::ms_logger, __VA_ARGS__)
#define NET_MOD_WARNING(...) LOG_WARNING(Network_module::ms_logger, __VA_ARGS__)
#define NET_MOD_ERROR(...)   LOG_ERROR(Network_module::ms_logger, __VA_ARGS__)

#define DEFAULT_MAX_CONNECTION        (100)
#define DEFAULT_MAX_INCOME_CONNECTION (50)
#define DEFAULT_SHUTDOWN_TIME         (100)

#ifndef DEFAULT_LOCAL_PORT
#define DEFAULT_LOCAL_PORT            (60000)
#endif

#ifndef DEFAULT_SERVER_IP
#define DEFAULT_SERVER_IP ("127.0.0.1")
#endif

#ifndef DEFAULT_SERVER_PORT
#define DEFAULT_SERVER_PORT (61111)
#endif


USING_NAMESPACE_COMMON

NETWORK_MODULE_BEGIN_NAMESPACE

typedef enum
{
    CONNECT_CLIENT_GUID_ERROR,
    CONNECT_CLIENT_NOT_INIT_PROXY,
    CONNECT_CLIENT_NOT_CONN_SERVER,
    CONNECT_CLIENT_SUCC
} CONNECT_CLIENT_RESULT;

class NatPunchProxy;
class Network_module : public Thread_module_base
{
    public:
        Network_module();
        virtual ~Network_module();

        friend class NatPunchProxy;

    private:
        Network_module(const Network_module&);
        Network_module& operator=(const Network_module&);

        void init_network_module();

        void start_network_loop();

        void process_packet(RakNet::Packet*);


    public:
        virtual void run();
        virtual void print_config() const;

        void punch_client(const char* p_guid);
        void connect_client(const char* ip, int port);

        inline int init_log(const char* _module_name, const char* _log_dir, const char* _log_level)
        {
            return ms_logger.init_log(_module_name, _log_dir, Logger::to_level(_log_level));
        }

        void set_max_connection(const char* _max_connection);
        void set_max_income_connection(const char* _max_income_connection);
        void set_local_port(const char* _local_port);

        std::string get_local_guid();

        inline void stop_engine()
        {
            m_stop_engine = true;
        }

        void regist_message_callback(int, Fun_p);

        void remove_message_callback(int);

        void send_server_message(int identify, const char* data, int size);

        void send_client_message(const char* ip, int port, int identify, const char* data, int size);

        void regist_punch_callback(Fun_p punch_succ, Fun_p punch_fail);

        void remove_punch_callback();

    private:
        static Logger ms_logger;

        bool m_stop_engine;

        int m_max_connection;
        int m_max_income_connection;
        int m_local_port;

        RakNet::RakPeerInterface* m_rakpeer;
        RakNet::SystemAddress* m_server_addr;
        NatPunchProxy* m_punch_proxy;

        pthread_mutex_t m_mutex;
        std::map<int, Fun_p> m_callback_map;
};

class NatPunchProxy: public RakNet::NatPunchthroughDebugInterface_Printf
{
    public:
        NatPunchProxy();
        ~NatPunchProxy();

    private:
        NatPunchProxy(const NatPunchProxy&);
        NatPunchProxy& operator=(const NatPunchProxy&);

    public:
        void init_proxy(RakNet::RakPeerInterface*);
        void stop_proxy(RakNet::RakPeerInterface*);

        RakNet::NatPunchthroughClient* get_punch_client()
        {
            return m_punch_client;
        }

    private:
        RakNet::NatPunchthroughClient* m_punch_client;
};

NETWORK_MODULE_END_NAMESPACE

#endif
