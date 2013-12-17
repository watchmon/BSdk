#include "module2.h"
#include <unistd.h>

MODULE2_BEGIN_NAMESPACE

Logger Module2::ms_logger;

Module2::Module2():m_int(0)
{
}

Module2::~Module2()
{
}

void Module2::run()
{
    for (int i = 0; i < 10; i++)
    {
        m_int = i;
        MOD2_INFO("Module2 %d run", i);
        sleep(1);
    }
}

void Module2::print_config() const
{
    MOD2_DEBUG("m_module_name is %s", get_name());
}

MODULE2_END_NAMESPACE
