#include <stdio.h>
#include <sys/socket.h>
#include <netinet/net.h>
#include <string.h>

#include "rdt.h"
#include "test.h"

int main (int argc, char** argv)
{
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char* tmp = NULL;

	socket_fd = socket (AF_INET, SOCK_STREAM, 0);

	server = gethostbyname (argv[1]);

	memset (&serv_addr, 0, sizeof (struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	memcpy (&(serv_addr.sin_addr.s_addr), &(server->h_addr), server->h_length);
	serv_addr.port = htons (PORT);

	connect (socket_fd, (struct sockaddr*)&serv_addr, sizeof (struct sockaddr_in));

	initialize (__recv_function, __send_function, 10000, 10000);

	while (!can_send)
		dispatch ();

	send_data ("Hello world!", strlen ("Hello world!") + 1);

	while (!tmp)
	{
		dispatch ();

		tmp = recv_data ();
	}

	printf ("%s", tmp);

	while (1)
		dispatch ();
}
