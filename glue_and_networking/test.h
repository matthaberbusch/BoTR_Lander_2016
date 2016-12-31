#include <sys/socket.h>

#ifndef TEST_H
#define TEST_H

#define PORT 5050

int socket_fd;

void __send_function (char* data, int len);

int __recv_function (char* data, int len, int timeout);

#endif
