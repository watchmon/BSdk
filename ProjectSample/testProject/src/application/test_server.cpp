#include "common/logger.h"
#include "common/application_base.h"
#include "common/inifile.h"
#include "common/system.h"
#include "module1/module1.h"
#include "module2/module2.h"

USING_NAMESPACE_COMMON
USING_NAMESPACE_MODULE1
USING_NAMESPACE_MODULE2

class Test_server : public Application_base
{
    public:
        Test_server();
        ~Test_server();

        int load_config();

    protected:
        int run();

    private:
        // 模块线程对象
        Module1* m_module1;
        Module2* m_module2;
};

Test_server::Test_server() :
    m_module1(NULL),
    m_module2(NULL)
{
}

Test_server::~Test_server()
{
    if (m_module1 != NULL)
    {
        delete m_module1;
        m_module1 = NULL;
    }

    if (m_module2 != NULL)
    {
        delete m_module2;
        m_module2 = NULL;
    }
}

int Test_server::load_config()
{
    pre_log("start to load test server config file\n");

    // 打开配置文件
    Inifile inifile;

    if (inifile.open(get_config_file()) == false)
    {
        printf("load %s inifile failed\n", get_config_file());
        return 1;
    }

    m_module1 = new Module1;
    m_module2 = new Module2;

    //读取global配置信息
    char buffer[100];
    inifile.get_value("global", "log_dir", buffer, sizeof(buffer));
    pre_log("read global config, log_dir %s\n", buffer);

    m_module1->init_log("module1", buffer, "debug");
    m_module2->init_log("module2", buffer, "debug");

    return 0;
}

int Test_server::run()
{
    pre_log("begin to start thread child module.\n");

    // 启动各个模块线程
    m_module1->start();
    m_module2->start();

    for (int i=0; ; i++)
    {
        // 检查信号退出
        if ((SIGINT == get_signal()) || (SIGTERM == get_signal()))
        {
            pre_log("catch signal %d. exit\n", get_signal());
            m_module1->set_signal(get_signal());
            m_module2->set_signal(get_signal());
            break;
        }

        sleep(1);
    }

    m_module1->join();
    delete m_module1;
    m_module1 = NULL;

    m_module2->join();
    delete m_module2;
    m_module2 = NULL;

    return 0;
}

int main(int argc, char* argv[])
{
    pre_log("test server now start\n");

    bool is_daemon = false;
    const char* config_file = NULL;

    Test_server test_server;
    test_server.set_version("v1.0.0");
    test_server.set_name("test_server");

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
                test_server.print_version();
                return 0;

            case 'h':
            default:
                test_server.print_usage();
                return 0;
        }
    }

    if (NULL == config_file)
    {
        test_server.print_usage();
        return 0;
    }

    // 允许没有配置文件,但指定了配置文件却没权限读取则退出失败返回
    if ((config_file[0] != '\0') && (access(config_file, R_OK) != 0))
    {
        pre_log("No access config file: (%s)\n", config_file);
        test_server.print_usage();
        return 1;
    }

    test_server.set_config_file(config_file);

    // 变成daemon进程
    if (is_daemon)
    {
        make_daemon();
    }

    test_server.start();

    return 0;
}
