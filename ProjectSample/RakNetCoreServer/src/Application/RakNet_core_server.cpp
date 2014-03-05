#include "Common/logger.h"
#include "Common/application_base.h"
#include "Common/inifile.h"
#include "Common/system.h"
#include "NetworkModule/network_module.h"

USING_NAMESPACE_COMMON
USING_NAMESPACE_NETWORK_MODULE

class RakNet_core_server : public Application_base
{
    public:
        RakNet_core_server();
        ~RakNet_core_server();

        int load_config();

    protected:
        int run();

    private:
        // 模块线程对象
        Network_module* m_network_module;
};

RakNet_core_server::RakNet_core_server() :
    m_network_module(NULL)
{
}

RakNet_core_server::~RakNet_core_server()
{
    if (m_network_module != NULL)
    {
        delete m_network_module;
        m_network_module = NULL;
    }
}

int RakNet_core_server::load_config()
{
    pre_log("start to load test server config file\n");

    // 打开配置文件
    Inifile inifile;

    if (inifile.open(get_config_file()) == false)
    {
        printf("load %s inifile failed\n", get_config_file());
        return 1;
    }

    m_network_module = new Network_module;

    //读取global配置信息
    char buffer[100];
    inifile.get_value("global", "log_dir", buffer, sizeof(buffer));
    pre_log("read global config, log_dir %s\n", buffer);

    m_network_module->init_log("network_module", buffer, "debug");

    inifile.get_value("network_module", "max_connection", buffer, sizeof(buffer));
    m_network_module->set_max_connection(buffer);

    inifile.get_value("network_module", "max_income_connection", buffer, sizeof(buffer));
    m_network_module->set_max_income_connection(buffer);

    inifile.get_value("network_module", "local_port", buffer, sizeof(buffer));
    m_network_module->set_local_port(buffer);

    return 0;
}

int RakNet_core_server::run()
{
    pre_log("begin to start thread child module.\n");

    // 启动各个模块线程
    m_network_module->start();

    for (int i=0; ; i++)
    {
        // 检查信号退出
        if ((SIGINT == get_signal()) || (SIGTERM == get_signal()))
        {
            pre_log("catch signal %d. exit\n", get_signal());
            m_network_module->set_signal(get_signal());
            break;
        }

        sleep(1);
    }

    m_network_module->join();
    delete m_network_module;
    m_network_module = NULL;

    return 0;
}

int main(int argc, char* argv[])
{
    pre_log("test server now start\n");

    bool is_daemon = false;
    const char* config_file = NULL;

    RakNet_core_server raknet_core_server;
    raknet_core_server.set_version("v1.0.0");
    raknet_core_server.set_name("raknet_core_server");

    //指定默认配置文件
    config_file = "../conf/config.ini";

    pre_log("read command line parameter\n");
    // 命令行解释
    int optch = 0;

    while ((optch=getopt(argc , argv , "c:dhv"))  != -1)
    {
        switch (optch)
        {
            case 'c':
                config_file = optarg;
                continue;

            case 'd':
                is_daemon = true;
                continue;

            case 'v':
                raknet_core_server.print_version();
                return 0;

            case 'h':
            default:
                raknet_core_server.print_usage();
                return 0;
        }
    }

    if (NULL == config_file)
    {
        raknet_core_server.print_usage();
        return 0;
    }

    // 允许没有配置文件,但指定了配置文件却没权限读取则退出失败返回
    if ((config_file[0] != '\0') && (access(config_file, R_OK) != 0))
    {
        pre_log("No access config file: (%s)\n", config_file);
        raknet_core_server.print_usage();
        return 1;
    }

    raknet_core_server.set_config_file(config_file);

    // 变成daemon进程
    if (is_daemon)
    {
        make_daemon();
    }

    raknet_core_server.start();

    return 0;
}
