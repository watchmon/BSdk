#ifndef __MODULE1__
#define __MODULE1__

#include "common/thread_module_base.h"
#include "common/logger.h"
#include "module1_namespace.h"

#define MOD1_DEBUG(...)   LOG_DEBUG(Module1::ms_logger, __VA_ARGS__)
#define MOD1_INFO(...)    LOG_INFO(Module1::ms_logger, __VA_ARGS__)
#define MOD1_WARNING(...) LOG_WARNING(Module1::ms_logger, __VA_ARGS__)
#define MOD1_ERROR(...)   LOG_ERROR(Module1::ms_logger, __VA_ARGS__)

USING_NAMESPACE_COMMON

MODULE1_BEGIN_NAMESPACE

class Module1 : public Thread_module_base
{
    public:
        Module1();
        virtual ~Module1();

    private:
        Module1(const Module1&);
        Module1& operator=(const Module1&);

    public:
        virtual void run();
        virtual void print_config() const;

        inline void init_log(const char* _module_name, const char* _log_dir, const char* _log_level)
        {
            ms_logger.init_log(_module_name, _log_dir, Logger::to_level(_log_level));
        }

    private:
        static Logger ms_logger;
        int m_int;
};

MODULE1_END_NAMESPACE

#endif
