#include <sys/time.h>
#include <errno.h>
#include <sched.h>
#include <unistd.h>
#include "concurrent.h"

__COMMON_BEGIN_NAMESPACE

Synchronized::Synchronized()
{
    initial();
}

Synchronized::~Synchronized()
{
    destory();
}

int Synchronized::initial()
{
    int ret = 0;

    ret = pthread_mutex_init(&m_monitor, NULL);

    if (ret != 0)
    {
        printf(__FILE__":%d mutex_init failed %d %s\n", __LINE__, ret, strerror(errno));
        return error;
    }

    ret = pthread_cond_init(&m_cond, NULL);

    if (ret != 0)
    {
        printf(__FILE__":%d cond_init failed %d %s\n", __LINE__, ret, strerror(errno));
        return error;
    }

    return success;
}

int Synchronized::destory()
{
    int ret = 0;

    ret = pthread_cond_destroy(&m_cond);

    if (ret != 0)
    {
        printf(__FILE__":%d cond_destroy failed with %d %s\n", __LINE__, ret, strerror(errno));
        return error;
    }

    ret = pthread_mutex_destroy(&m_monitor);

    if (ret != 0)
    {
        printf(__FILE__":%d mutex_destroy failed with %d %s\n", __LINE__, ret, strerror(errno));
        return error;
    }

    return success;
}

int Synchronized::wait(long timeout)
{
    int ret = 0;

    if (timeout < 0)
    {
        // 阻塞等待
        ret = pthread_cond_wait(&m_cond, &m_monitor);
    }
    else
    {
        // 带超时机制等待
        struct timespec ts;
        struct timeval  tv;
        gettimeofday(&tv, NULL);
        ts.tv_sec  = tv.tv_sec  + (int)(timeout/1000);
        ts.tv_nsec = (tv.tv_usec + (timeout%1000)*1000) * 1000;

        ts.tv_sec += ts.tv_nsec/(1000*1000*1000);
        ts.tv_nsec %= 1000*1000*1000;

        ret = pthread_cond_timedwait(&m_cond, &m_monitor, &ts);
    }

    switch (ret)
    {
        case 0:
            return success;

        case ETIMEDOUT:
            return timeout;

        default:
            printf(__FILE__":%d pthread_cond* %d %s\n", __LINE__, ret, strerror(errno));
            return error;
    }
}

void Synchronized::notify()
{
    int ret = pthread_cond_signal(&m_cond);

    if (ret != 0)
    {
        printf(__FILE__":%d notify failed %d %s\n", __LINE__, ret, strerror(errno));
    }
}

void Synchronized::notify_all()
{
    int ret = pthread_cond_broadcast(&m_cond);

    if (ret != 0)
    {
        printf(__FILE__":%d notify_all failed %d %s\n", __LINE__, ret, strerror(errno));
    }
}

void Synchronized::lock()
{
    int ret = pthread_mutex_lock(&m_monitor);

    if (ret != 0)
    {
        printf(__FILE__":%d pthread_mutex_lock failed %s\n", __LINE__, strerror(errno));
    }
}

void Synchronized::unlock()
{
    int ret = pthread_mutex_unlock(&m_monitor);

    if (ret != 0)
    {
        printf(__FILE__":%d pthread_mutex_unlock failed %s\n", __LINE__, strerror(errno));
    }
}

int Synchronized::try_lock()
{
    int ret = pthread_mutex_trylock(&m_monitor);

    switch (ret)
    {
        case 0:
            return success;

        case EBUSY:
        case EAGAIN:
            return again;

        default:
            printf(__FILE__":%d pthread_mutex_trylock failed %s\n", __LINE__, strerror(errno));
            return error;
    }
}

void* thread_starter(void* t)
{
    Thread* thread = (Thread*)t;

    // 分离线程
    if (thread->m_detach == Thread::DETACHED)
    {
        if (pthread_detach(pthread_self()) != 0)
        {
            printf("thread_starter:%s detach %u\n", thread->get_name(), (unsigned int)thread->m_tid);
        }
    }

    Thread::run_on_cpu(thread->m_on_cpu);

    printf("thread_starter:%s started %u\n", thread->get_name(), (unsigned int)thread->m_tid);
    thread->get_runnable().run();
    printf("thread_starter:%s ended %u\n", thread->get_name(), (unsigned int)thread->m_tid);

    thread->m_status = Thread::FINISHED;

    // 分离线程结束后释放Thread对象
    if (thread->m_detach == Thread::DETACHED)
    {
        delete thread;
    }

    return t;
}

Thread::Thread() :
    m_runnable(this),
    m_status(IDLE),
    m_stacksize(-1),
    m_tid(0),
    m_name(),
    m_detach(JOINED),
    m_sig(0),
    m_on_cpu(-1)
{
    m_name[0]='\0';
}

Thread::Thread(Runnable* r) :
    m_runnable(r),
    m_status(IDLE),
    m_stacksize(-1),
    m_tid(0),
    m_name(),
    m_detach(JOINED),
    m_sig(0),
    m_on_cpu(-1)
{
    m_name[0]='\0';
}

Thread::~Thread()
{
    if ((m_status != IDLE) && (m_detach != DETACHED))
    {
        join();
    }

    // 释放被分配的Runnable接口对象
    if (m_runnable != this)
    {
        delete m_runnable;
    }

    printf("Thread:%s ~Thread()\n", get_name());
}

void Thread::run()
{
    printf("Thread:%s empty run method!\n", get_name());
}

int Thread::join()
{
    if (m_status == RUNNING ||
        m_status == FINISHED)
    {
        void* retstat;
        int err = pthread_join(m_tid, &retstat);

        if (err)
        {
            printf("Thread:%s Join failed %d\n", get_name(), err);
            return ERROR;
        }

        m_status = IDLE;
        printf("Thread:%s Joined thread successfully %u\n", get_name(), (unsigned int)m_tid);
        return SUCCESS;
    }
    else
    {
        printf("Thread:%s Thread not running %u\n", get_name(), (unsigned int)m_tid);
        return ERROR;
    }
}

int Thread::kill(int sig)
{
    if (m_status == RUNNING)
    {
        int err = pthread_kill(m_tid, sig);

        if (err)
        {
            printf("Thread:%s pthread_kill %d\n", get_name(), err);
            return ERROR;
        }

        return SUCCESS;
    }

    return ERROR;
}

int Thread::start()
{
    if (m_status == IDLE)
    {
        pthread_attr_t attr;
        pthread_attr_init(&attr);
//        if (pthread_attr_setstacksize(&attr, m_stacksize) != 0) {
//            printf("Thread: pthread_attr_setstacksize %d failed\n", m_stacksize);
//        }
//        if (pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM) != 0) {
//            printf("Thread: pthread_attr_setscope PTHREAD_SCOPE_SYSTEM failed\n");
//        }
//        if (pthread_attr_setschedpolicy(&attr, SCHED_RR) != 0) {
//            printf("Thread: pthread_attr_setschedpolicy SCHED_RR failed\n");
//        }
        int err = pthread_create(&m_tid, NULL, thread_starter, (void*)this);
        pthread_attr_destroy(&attr);

        if (err)
        {
            printf("Thread:%s cannot start thread %d\n", get_name(), err);
            m_status = IDLE;
            return ERROR;
        }
        else
        {
            m_status = RUNNING;
            return SUCCESS;
        }
    }
    else
    {
        printf("Thread%s: thread already running!\n", get_name());
        return ERROR;
    }
}

int Thread::run_on_cpu(int i)
{
    int num_procs = sysconf(_SC_NPROCESSORS_CONF);

    if ((i<0) || (i >= num_procs))
    {
        return -1;
    }

    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(i, &mask);

    if (sched_setaffinity(0, sizeof(mask), &mask) == -1)
    {
        return -1;
    }

    return 0;
}

__COMMON_END_NAMESPACE
