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
    MOD1_DEBUG("test");
    char a = 'b';
    MOD1_DEBUG("%d", 1);
    MOD1_DEBUG("%d", a);
    MOD1_INFO("%d", a);
    MOD1_WARNING("%d", a);
    MOD1_ERROR("%d", a);
    printf("%d\n", a);
    MOD1_INFO("%s", "a");
#if 0
    print_config();

    for (int i = 0; i < 10; i++)
    {
        m_int = i;
        MOD1_INFO("Module1 %d run", i);
        char test[10] = "abc";
        MOD1_INFO("Module1 %s run", test);
        sleep(1);
    }

#endif
}

void Module1::print_config() const
{
//   MOD1_DEBUG("m_module_name is %s", get_name());
}

MODULE1_END_NAMESPACE
