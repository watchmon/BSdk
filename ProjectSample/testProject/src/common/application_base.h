#ifndef _COMMON_APPLICATION_BASE_H
#define _COMMON_APPLICATION_BASE_H

#include "common.h"

__COMMON_BEGIN_NAMESPACE

// 应用程序基类
class Application_base
{
    public:
        Application_base();
        virtual ~Application_base();

        // 命令行输出版本, 子类可重写该方法修改版本打印格式
        virtual void print_version() const;

        // 打印帮助信息, 子类可重写添加帮忙信息
        virtual void print_usage() const;

        // 加载配置文件, 返回0为成功, 非0为失败
        virtual int load_config() = 0;

        void set_name(const char* p);
        void set_version(const char* p);
        void set_config_file(const char* p);

        const char* get_name() const
        {
            return m_name;
        }
        const char* get_version() const
        {
            return m_version;
        }
        const char* get_config_file() const
        {
            return m_config_file;
        }

        // 应用程序开始函数, 返回0成功
        int start();

        // 设置应用程序全局退出信号
        static void set_signal(int sig)
        {
            Application_base::s_sig = sig;
        }

        // 获取应用程序收接到的信号SIGINT,SIGTERM
        static int get_signal()
        {
            return Application_base::s_sig;
        }

    protected:
        // 主流程,由继承类实现
        virtual int run() = 0;

        // 待添加网管agent ....

    private:
        char m_name[256];
        char m_version[100];
        char m_config_file[256];

        // 应用程序全局退出信号，非0或SIGHUP则所有线程自行退出
        // 信号捕捉函数将修改此值
        static int s_sig;
};

__COMMON_END_NAMESPACE

#endif
