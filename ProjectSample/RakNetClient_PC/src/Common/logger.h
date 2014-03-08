//                       日志规范
// 1 日志文件命名格式: module.log
//   module: 模块名
// 2 日志文件命名格式: module_yyyymmddhhmiss.log
// 3 日志文件小于1GB,推荐10MB
// 4 日志格式: yyyy-mm-dd hh:mi:ss:usec level pid:tid file:line message
//   yyyy: 4位年
//   mm: 月
//   dd: 日
//   hh: 时,24
//   mi: 分
//   ss: 秒
//   usec: 微秒
//   level: 日志级别缩写, D I W E
//   pid: 进程ID
//   tid: 线程ID
//   file: 源文件名
//   line: 源文件行数
//   message: 日志内容

#ifndef _COMMON_LOGGER_H
#define _COMMON_LOGGER_H

#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "common.h"
#include "concurrent.h"

__COMMON_BEGIN_NAMESPACE

//启动模块打印机制之前的终端输出log,不使用这取消PRE_LOG定义
#define PRE_LOG
#ifdef  PRE_LOG
#define pre_log(...) printf(__VA_ARGS__)
#else
#define pre_log(...)
#endif

// logger为Logger对象
// ... 为printf参数格式
#define LOG_DEBUG(logger, ...)   LOG((logger), Logger::Debug,   "D", __VA_ARGS__)
#define LOG_INFO(logger, ...)    LOG((logger), Logger::Info,    "I", __VA_ARGS__)
#define LOG_WARNING(logger, ...) LOG((logger), Logger::Warning, "W", __VA_ARGS__)
#define LOG_ERROR(logger, ...)   LOG((logger), Logger::Error,   "E", __VA_ARGS__)

//是否启用终端输出log，不使用则注释该宏
//#define __PRINT_SCREEN

#define LOG(logger, level, level_string, ...) \
do {                                                                     \
    if ((logger).get_level() <= (level)) {                               \
        Synchronized::Autolock guard((logger));                          \
        struct timeval now;                                              \
        gettimeofday(&now, 0);                                           \
        struct tm now_tm;                                                \
        localtime_r(&(now.tv_sec), &now_tm);                             \
        (logger).append("%04d-%02d-%02d %02d:%02d:%02d.%06d "level_string" %d:%d "      \
            __FILE__":%d ",                                              \
            now_tm.tm_year+1900, now_tm.tm_mon+1, now_tm.tm_mday,        \
            now_tm.tm_hour, now_tm.tm_min, now_tm.tm_sec, now.tv_usec,   \
            getpid(), (unsigned int)pthread_self(), __LINE__);           \
        (logger).append( __VA_ARGS__ );                                  \
        (logger).append("\n");                                           \
        (logger).check_switch();                                         \
    }                                                                    \
} while (0)

class Logger : public Synchronized
{
    public:
        enum { Debug=1, Info=2, Warning=3, Error=4, Nolog=5 };
        enum { success = 0, error = -1 };

        Logger();
        ~Logger();

        // 日志设置与打开文件,简化日志初始化操作
        int init_log(const char* module, const char* log_dir, int log_level, int logfile_size=100*1024*1024);

        // 日志级别
        void set_level(int p)
        {
            m_level = p;
        }
        int  get_level() const
        {
            return m_level;
        }

        // 设置日志文件大小,缺省为100MB,单位:byte
        void set_max_size(long p)
        {
            m_max_size = p;
        }

        // 设置日志目录
        void set_dir(const char* p)
        {
            strncpy(m_dir, p, sizeof(m_dir));
        }

        // 设置模块名,用于生成文件名
        void set_module(const char* p)
        {
            strncpy(m_module, p, sizeof(m_module));
        }

        int open();
        int close();

        // 日志文件切换
        int switch_logfile();

        // 检查日志大小是否需要切换
        void check_switch()
        {
            if (ftell(m_fp) > m_max_size)
            {
                switch_logfile();
            }
        }

        void append(const char* format, ...);

        static int to_level(const char* p);

    private:
        Logger(const Logger&);
        Logger& operator=(const Logger&);

        int m_level;

        FILE* m_fp;
        long m_max_size;

        char m_dir[256];
        char m_module[256];
};

__COMMON_END_NAMESPACE

#endif
