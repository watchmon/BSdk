#ifndef _THREAD_MODULE_BASE_H
#define _THREAD_MODULE_BASE_H

#include "concurrent.h"

__COMMON_BEGIN_NAMESPACE

class Thread_module_base : public Thread
{
    public:
        Thread_module_base() { }
        virtual ~Thread_module_base() { }

        // 打印模块的配置到日志文件
        virtual void print_config() const = 0;
};

__COMMON_END_NAMESPACE

#endif
