#include "module1.h"
#include <unistd.h>

MODULE1_BEGIN_NAMESPACE

Logger Module1::ms_logger;

Module1::Module1():m_int(0)
{
}

Module1::~Module1()
{
}

void Module1::run()
{
    for (int i = 0; i < 10; i++)
    {
        m_int = i;
        MOD1_INFO("Module1 %d run", i);
        sleep(1);
    }
}

void Module1::print_config() const
{
    MOD1_DEBUG("m_module_name is %s", get_name());
}

MODULE1_END_NAMESPACE
