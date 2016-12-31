#include "test.h"

#include <sys/socket.h>
#include <poll.h>

void __send_function (char* data, int len)
{
	send (socket_fd, data, len, 0);
}

int __recv_function (char* data, int len, int timeout)
{
	struct pollfd event;
	
	event.fd = socket_fd;
	event.events = POLLIN;

	if (poll (&event, 1, timeout/1000))
		return recv (socket_fd, data, len, 0);

	return 0;
}
