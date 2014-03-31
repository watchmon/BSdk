#include "NetworkModule/network_interface.h"
#include "NetworkModule/user_message_identify.h"
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <iostream>

using namespace std;

pthread_t stop_pid;

void test_callback(const char* ip, int port, unsigned char* data, int size)
{
    remove_callback(ID_USER_MESSAGE_TEST);
    printf("ip is %s\n", ip);
    printf("port is %d\n", port);
    printf("data is %s\n", (char*)data);
    printf("size is %d\n", size);

    char test_msg[] = "helloworld too!";
    send_peer_message(ip, port, ID_USER_MESSAGE_TEST, test_msg, sizeof(test_msg));
}

void* send_message_thread(void* args)
{
    sleep(1);
    printf("%s\n", get_local_guid());
    regist_callback(ID_USER_MESSAGE_TEST, &test_callback);

}

int main(int argc, char* argv[])
{
    init_engine();

    pthread_create(&stop_pid, NULL, &send_message_thread, NULL);

    start_engine();

    pthread_join(stop_pid, NULL);


    return 0;
}
