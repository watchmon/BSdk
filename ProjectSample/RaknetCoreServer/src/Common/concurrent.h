#ifndef _COMMON_CONCURRENT_H
#define _COMMON_CONCURRENT_H

#include <set>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "runnable.h"
#include "common.h"

__COMMON_BEGIN_NAMESPACE

// 实现线程同步
class Synchronized
{
    public:
        enum { block = -1, locked, unlocked, again, timeout, success, error };

        // 构造时initial
        Synchronized();

        // 析构时destory
        virtual ~Synchronized();

        // 等待其他线程调用notify/notify_all方法通知
        // timeout<0阻塞等待，超时返回1，否则返回0 (单位:毫秒)
        int wait(long timeout = Synchronized::block);

        // 通知一个等待本对象的的线程
        void notify();
        void notify_all();

        // 加锁(阻塞)
        void lock();

        // 加锁(非阻塞)
        int try_lock();

        // 解锁(阻塞)
        void unlock();

        // 待添加信号量

        // Synchronized的自释放锁
        class Autolock
        {
            public:
                Autolock(Synchronized& s) : m_sync(s), m_release(0)
                {
                    m_sync.lock();
                }
                ~Autolock()
                {
                    if (m_release == 0)
                    {
                        m_sync.unlock();
                    }
                }

                void acquire()
                {
                    m_sync.lock();
                    m_release = 1;
                }
                void release()
                {
                    m_sync.unlock();
                    m_release = 0;
                }

            private:
                Synchronized& m_sync;
                int m_release;
        };

    protected:
        // 初始化
        int initial();

        // 释放资源
        int destory();

    private:
        pthread_cond_t  m_cond;
        pthread_mutex_t m_monitor;
};

// 线程类
class Thread : public Runnable
{
    public:
        enum { SUCCESS=0, ERROR=-1 };
        enum Mode { DETACHED=0, JOINED=-1 };
        enum Status { IDLE=0, RUNNING=1, FINISHED=2 };

        friend void* thread_starter(void*);

        Thread();
        Thread(Runnable* r);

        // 析构时将等待非DETACHED
        virtual ~Thread();

        // 被启动的线程将执行run()
        virtual void run();

        // 设置的runnable对象必须通过new生成
        void set_runnable(Runnable* r)
        {
            if (m_runnable != this)
            {
                delete m_runnable;
            }

            m_runnable = r;
        }
        void set_stack_size(long size)
        {
            m_stacksize = size;
        }
        void set_name(const char* name)
        {
            strncpy(m_name, name, sizeof(m_name));
        }
        void set_name(int id)
        {
            snprintf(m_name, sizeof(m_name), "%d", id);
        }

        Runnable&     get_runnable()
        {
            return *m_runnable;
        }
        long          get_stackSize() const
        {
            return m_stacksize;
        }
        const char*   get_name()      const
        {
            return m_name;
        }
        pthread_t     get_id()        const
        {
            return m_tid;
        }

        int is_alive() const
        {
            return (m_status == RUNNING);
        }

        // 通知线程退出,本信号为建议信号,线程通过轮询该信号与业务结合判断是否需要退出
        void set_signal(int sig)
        {
            m_sig = sig;
        }
        int get_signal() const
        {
            return m_sig;
        }

        // 线程绑定cpu核,小于0为随机.该函数在start()前设置
        void on_cpu(int i)
        {
            m_on_cpu = i;
        }

        // 启动一个线程
        int start();

        // 等待线程退出
        int join();

        // detach方式Thread必须通过new生成,由线程退出时delete释放线程对象
        int detach(int mode=DETACHED)
        {
            m_detach = mode;
            return 0;
        }

        // 向线程发信号
        int kill(int sig=SIGTERM);

        static int run_on_cpu(int i);

    private:
        Runnable* m_runnable;

        // 线程状态
        Status m_status;

        // 栈大小（单位：byte）
        long m_stacksize;

        // 线程tid（操作系统分配）
        pthread_t m_tid;

        // 线程名
        char m_name[256];

        // 分离标识
        int m_detach;

        int m_sig;

        int m_on_cpu;
};

__COMMON_END_NAMESPACE

#endif
