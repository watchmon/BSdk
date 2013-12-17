#ifndef __MODULE2__
#define __MODULE2__

#include "common/thread_module_base.h"
#include "common/logger.h"
#include "module2_namespace.h"

#define MOD2_DEBUG(...)   LOG_DEBUG(Module2::ms_logger, __VA_ARGS__)
#define MOD2_INFO(...)    LOG_INFO(Module2::ms_logger, __VA_ARGS__)
#define MOD2_WARNING(...) LOG_WARNING(Module2::ms_logger, __VA_ARGS__)
#define MOD2_ERROR(...)   LOG_ERROR(Module2::ms_logger, __VA_ARGS__)

USING_NAMESPACE_COMMON

MODULE2_BEGIN_NAMESPACE

class Module2 : public Thread_module_base
{
    public:
        Module2();
        virtual ~Module2();

    private:
        Module2(const Module2&);
        Module2& operator=(const Module2&);

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

MODULE2_END_NAMESPACE

#endif
