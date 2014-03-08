#ifndef __NETWORK_MODULE__
#define __NETWORK_MODULE__

#include <string>
#include "Common/thread_module_base.h"
#include "Common/logger.h"
#include "network_module_namespace.h"
#include "RakNetSource/RakPeerInterface.h"
#include "RakNetSource/NatPunchthroughClient.h"

#define NET_MOD_DEBUG(...)   LOG_DEBUG(Network_module::ms_logger, __VA_ARGS__)
#define NET_MOD_INFO(...)    LOG_INFO(Network_module::ms_logger, __VA_ARGS__)
#define NET_MOD_WARNING(...) LOG_WARNING(Network_module::ms_logger, __VA_ARGS__)
#define NET_MOD_ERROR(...)   LOG_ERROR(Network_module::ms_logger, __VA_ARGS__)

#define DEFAULT_MAX_CONNECTION        (100)
#define DEFAULT_MAX_INCOME_CONNECTION (50)
#define DEFAULT_LOCAL_PORT            (60000)
#define DEFAULT_SHUTDOWN_TIME         (100)

#ifndef DEFAULT_SERVER_IP
#define DEFAULT_SERVER_IP ("127.0.0.1")
#endif

#ifndef DEFAULT_SERVER_PORT
#define DEFAULT_SERVER_PORT (61111)
#endif

USING_NAMESPACE_COMMON

NETWORK_MODULE_BEGIN_NAMESPACE

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

    private:
        static Logger ms_logger;

        int m_max_connection;
        int m_max_income_connection;
        int m_local_port;

        RakNet::RakPeerInterface* m_rakpeer;
        NatPunchProxy* m_punch_proxy;

        bool m_stop_engine;
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

    private:
        RakNet::NatPunchthroughClient* m_punch_client;
};

NETWORK_MODULE_END_NAMESPACE

#endif
