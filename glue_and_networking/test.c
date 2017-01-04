#include "test.h"

#include <sys/socket.h>
#include <poll.h>
#include <stdlib.h>

void __send_function (char* data, int len)
{
	if (!(rand () % 5)) //Simulate loss
		send (socket_fd, data, len, 0);
}

int __recv_function (char* data, int len, int timeout)
{
	struct pollfd event;
	int recv_len = 0;
	
	event.fd = socket_fd;
	event.events = POLLIN;

	if (poll (&event, 1, timeout/1000))
		recv_len = recv (socket_fd, data, len, 0);

	if (recv_len && (rand () % 5)) //Simulate corruption
		data [rand () % recv_len] = rand () & 0xFF;

	return recv_len;
}
