#include "logger.h"

__COMMON_BEGIN_NAMESPACE

Logger::Logger() :
    m_level(Debug),
    m_fp(NULL),
    m_max_size(10*1024*1024), // default 10MB
    m_dir(),
    m_module()
{
    m_dir[0] = '\0';
    m_module[0] = '\0';
}

Logger::~Logger()
{
    close();
}

int Logger::open()
{
    close();

    char filename[256];
    snprintf(filename, sizeof(filename), "%s/%s.log", m_dir, m_module);

    m_fp = fopen(filename, "a");

    if (!m_fp)
    {
        printf("%s logger: open file (%s) failed %s\n", m_module, m_dir, strerror(errno));
        return error;
    }

    return success;
}

int Logger::close()
{
    if (m_fp)
    {
        fclose(m_fp);
        m_fp = NULL;
    }

    return success;
}

int Logger::switch_logfile()
{
    time_t now = time(NULL);
    struct tm now_tm;
    localtime_r(&now, &now_tm);

    char filename[256];
    snprintf(filename, sizeof(filename), "%s/%s.log", m_dir, m_module);

    char new_filename[256];
    snprintf(new_filename, sizeof(new_filename), "%s/%s_%04d%02d%02d%02d%02d%02d.log",
             m_dir, m_module,
             now_tm.tm_year+1900, now_tm.tm_mon+1, now_tm.tm_mday,
             now_tm.tm_hour, now_tm.tm_min, now_tm.tm_sec);

    close();

    if (rename(filename, new_filename) != 0)
    {
        printf("rename %s -> %s failed %s\n", filename, new_filename, strerror(errno));
    }

    return open();
}

void Logger::append(const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    vfprintf(m_fp, format, ap);
#if defined(__PRINT_SCREEN)
    vfprintf(stdout, format, ap);
#endif
    va_end(ap);
}

int Logger::to_level(const char* p)
{
    static const char debug[] = "debug";
    static const char info[] = "info";
    static const char warning[] = "warning";
    static const char error[] = "error";
    static const char nolog[] = "nolog";

    if (strncasecmp(p, debug, sizeof(debug)) == 0)
    {
        return Logger::Debug;
    }
    else if (strncasecmp(p, info, sizeof(info)) == 0)
    {
        return Logger::Info;
    }
    else if (strncasecmp(p, warning, sizeof(warning)) == 0)
    {
        return Logger::Warning;
    }
    else if (strncasecmp(p, error, sizeof(error)) == 0)
    {
        return Logger::Error;
    }
    else if (strncasecmp(p, nolog, sizeof(nolog)) == 0)
    {
        return Logger::Nolog;
    }
    else
    {
        // 缺省使用Debug
        return Logger::Debug;
    }
}

int Logger::init_log(const char* module, const char* log_dir, int log_level, int logfile_size)
{
    set_module(module);
    set_dir(log_dir);
    set_level(log_level);
    set_max_size(logfile_size);

    return open();
}

__COMMON_END_NAMESPACE
