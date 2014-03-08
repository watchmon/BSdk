﻿#include "NetworkModule/network_interface.h"
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

pthread_t stop_pid;

void* stop_engine_thread(void* args)
{
    sleep(5);
    printf("开始终止引擎\n");
    stop_engine();
}

int main(int argc, char* argv[])
{
    init_engine();

    pthread_create(&stop_pid, NULL, &stop_engine_thread, NULL);

    start_engine();

    pthread_join(stop_pid, NULL);


    return 0;
}