#ifndef _COMMON_SYSTEM_H
#define _COMMON_SYSTEM_H

#include "common.h"
#include "base_type.h"

__COMMON_BEGIN_NAMESPACE

// 是进程进入后台运行
void make_daemon();

// sig_handler为NULL时则怱省该信号
int register_signal(int signum, void (*sig_handler)(int));

__COMMON_END_NAMESPACE

#endif
