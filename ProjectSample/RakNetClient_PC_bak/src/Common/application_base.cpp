#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include "application_base.h"
#include "system.h"

__COMMON_BEGIN_NAMESPACE

// 信号捕捉函数
extern "C" void app_sig_catch(int sig)
{
    Application_base::set_signal(sig);
}

// 信号注册函数
static void sig_reg()
{
    register_signal(SIGINT, app_sig_catch);
    register_signal(SIGTERM, app_sig_catch);
    register_signal(SIGHUP, NULL);
    register_signal(SIGPIPE, NULL);
}

// 应用程序全局信号
int Application_base::s_sig;

Application_base::Application_base()
{
    m_name[0]        = '\0';
    m_version[0]     = '\0';
    m_config_file[0] = '\0';
}

Application_base::~Application_base()
{
}

void Application_base::set_name(const char* p)
{
    strncpy(m_name, p, sizeof(m_name));
}

void Application_base::set_version(const char* p)
{
    strncpy(m_version, p, sizeof(m_version));
}

void Application_base::set_config_file(const char* p)
{
    strncpy(m_config_file, p, sizeof(m_config_file));
}

void Application_base::print_version() const
{
    printf("%s version: %s\n", m_name, m_version);
}

void Application_base::print_usage() const
{
    printf("Usage : %s [OPTION]...\n"
           "Options:    -c config               config file\n"
           "Options:    -d                      make daemon\n"
           "Options:    -h                      print Usage\n"
           "Options:    -v                      print version info\n", m_name);
}

int Application_base::start()
{
    // 1 检查参数

    // 2 加载配置文件
    if (load_config() != 0)
    {
        printf("load config %s failed.\n", m_config_file);
        return -1;
    }

    // 3 打开程序日志文件

    // 4 注册信号
    sig_reg();

    // 5 运行
    int ret = run();

    return ret;
}

__COMMON_END_NAMESPACE
