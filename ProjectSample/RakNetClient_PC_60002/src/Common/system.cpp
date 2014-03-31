#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include "system.h"

__COMMON_BEGIN_NAMESPACE

void make_daemon()
{
    if (fork() > 0)
    {
        _exit(0);
    }

    setsid();

    if (fork() > 0)
    {
        _exit(0);
    }

    for (int i = 0; i < 3; i++)
    {
        close(i);
    }
}

int register_signal(int signum, void (*sig_handler)(int))
{
    struct sigaction act;

    if (sig_handler == NULL)
    {
        act.sa_handler = SIG_IGN;
    }
    else
    {
        act.sa_handler = sig_handler;
    }

    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);

    if (sigaction(signum, &act, NULL) != 0)
    {
        return -1;
    }

    return 0;
}

__COMMON_END_NAMESPACE
