#ifndef __NETWORK_MODULE__
#define __NETWORK_MODULE__

#include "Common/thread_module_base.h"
#include "Common/logger.h"
#include "network_module_namespace.h"

#define NET_MOD_DEBUG(...)   LOG_DEBUG(Network_module::ms_logger, __VA_ARGS__)
#define NET_MOD_INFO(...)    LOG_INFO(Network_module::ms_logger, __VA_ARGS__)
#define NET_MOD_WARNING(...) LOG_WARNING(Network_module::ms_logger, __VA_ARGS__)
#define NET_MOD_ERROR(...)   LOG_ERROR(Network_module::ms_logger, __VA_ARGS__)

USING_NAMESPACE_COMMON

NETWORK_MODULE_BEGIN_NAMESPACE

class Network_module : public Thread_module_base
{
    public:
        Network_module();
        virtual ~Network_module();

    private:
        Network_module(const Network_module&);
        Network_module& operator=(const Network_module&);

    public:
        virtual void run();
        virtual void print_config() const;

        inline void init_log(const char* _module_name, const char* _log_dir, const char* _log_level)
        {
            ms_logger.init_log(_module_name, _log_dir, Logger::to_level(_log_level));
        }

    private:
        static Logger ms_logger;
};

NETWORK_MODULE_END_NAMESPACE

#endif
