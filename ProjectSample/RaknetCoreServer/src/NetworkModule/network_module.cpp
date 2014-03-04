#include "network_module.h"
#include <unistd.h>

NETWORK_MODULE_BEGIN_NAMESPACE

Logger Network_module::ms_logger;

Network_module::Network_module():m_int(0)
{
}

Network_module::~Network_module()
{
}

void Network_module::run()
{

}

void Network_module::print_config() const
{
    NET_MOD_DEBUG("m_module_name is %s", get_name());
}

NETWORK_MODULE_END_NAMESPACE
