#ifndef __NETWORK_MODULE__
#define __NETWORK_MODULE__

#include "Common/thread_module_base.h"
#include "Common/logger.h"
#include "network_module_namespace.h"
#include "RakNetSource/NatPunchthroughServer.h"
#include "RakNetSource/RakPeerInterface.h"

#define NET_MOD_DEBUG(...)   LOG_DEBUG(Network_module::ms_logger, __VA_ARGS__)
#define NET_MOD_INFO(...)    LOG_INFO(Network_module::ms_logger, __VA_ARGS__)
#define NET_MOD_WARNING(...) LOG_WARNING(Network_module::ms_logger, __VA_ARGS__)
#define NET_MOD_ERROR(...)   LOG_ERROR(Network_module::ms_logger, __VA_ARGS__)

USING_NAMESPACE_COMMON

NETWORK_MODULE_BEGIN_NAMESPACE

enum
{
    DEFAULT_MAX_CONNECTION = 100,
    DEFAULT_MAX_INCOME_CONNECTION = 50,
    DEFAULT_LOCAL_PORT = 61111,
    DEFAULT_SHUT_DOWN_TIME = 200
};

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

        void start_network_loop(RakNet::RakPeerInterface*);

    public:
        virtual void run();
        virtual void print_config() const;

        inline void init_log(const char* _module_name, const char* _log_dir, const char* _log_level)
        {
            ms_logger.init_log(_module_name, _log_dir, Logger::to_level(_log_level));
        }

        void set_max_connection(const char* _max_connection);

        void set_max_income_connection(const char* _max_income_connection);

        void set_local_port(const char* _local_port);

        void process_packet(RakNet::RakPeerInterface*, RakNet::Packet*);

    private:
        static Logger ms_logger;

        int m_max_connection;
        int m_max_income_connection;
        int m_local_port;

        NatPunchProxy* m_punch_proxy;
};

class NatPunchProxy: public RakNet::NatPunchthroughServerDebugInterface_Printf
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

    private:
        RakNet::NatPunchthroughServer* m_punch_server;
};

NETWORK_MODULE_END_NAMESPACE

#endif
