#include <stdio.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <string.h>

#include "rdt.h"
#include "test.h"


int main (int argc, char** argv)
{
	struct sockaddr_in serv_addr;
	struct sockaddr_in client_addr;
	socklen_t client_len;
	int welcome_fd;
	char* tmp = NULL;

	welcome_fd = socket (AF_INET, SOCK_STREAM, 0);

	memset (&serv_addr, 0, sizeof (struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons (PORT);
	bind (welcome_fd, (struct sockaddr*)&serv_addr, sizeof (struct sockaddr_in));
	listen (welcome_fd, 1);

	socket_fd = accept (welcome_fd, (struct sockaddr*)&client_addr, &client_len);

	initialize (__recv_function, __send_function, 10000, 10000);

	while (!tmp)
	{
		dispatch ();

		tmp = recv_data ();
	}

	printf ("%s", tmp);

	while (!can_send)
		dispatch ();

	send_data ("Goodbye world!", strlen ("Goodbye world!") + 1);

	while (1)
		dispatch ();
}
