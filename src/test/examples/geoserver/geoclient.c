#include "geoserver.h"

void		rescan(int server_socket, char *ip_address);
void		send_to_server(int server_fd, char *msg);

int
main(int argc, char *argv[])
{
	int			server_socket;
	struct sockaddr_in server_address;
	int			ret_status;
	int			i;

	server_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (server_socket <= 0)
	{
		fprintf(stderr, "Could not create a socket: returned %d\n", server_socket);
		exit(-1);
	}
	else
		printdbg("Geoclient.c", "Socket Created");
	bzero(&server_address, sizeof(server_address));

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(GEOSERVER_DEFAULT_PORT);
	ret_status = inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);

	if (ret_status == 0)
		fprintf(stderr, "Error converting localhost to an address..");
	else
		printdbg("Geoclient.c", "Created server address correctly.");

	ret_status = connect(server_socket, (struct sockaddr *) & server_address, sizeof(server_address));

	if (ret_status == -1)
		fprintf(stderr, "Could not connect to server...\n");
	else
		printdbg("Geoclient.c", "Connection Made");

	for (i = 1; i < argc; i++)
		rescan(server_socket, argv[i]);

	while (1)
	{
	}

	return 0;

}

void
rescan(int server_socket, char *ip_address)
{

	send_to_server(server_socket, ip_address);

}

void
send_to_server(int server_fd, char *msg)
{
	size_t		bytes_written;
	char	   *message = (char *) malloc(sizeof(char) * INC_MESSAGE_LENGTH);
	int			i;

	printdbg("Geoclient", "send_to_server called");

	for (i = 0; i < strlen(msg); i++)
		message[i] = msg[i];
	for (; i < INC_MESSAGE_LENGTH; i++)
		message[i] = '\0';

	bytes_written = write(server_fd, message, INC_MESSAGE_LENGTH);

	if (bytes_written != INC_MESSAGE_LENGTH)
		fprintf(stderr, "Message not sent to server: %d of %d bytes written", bytes_written, INC_MESSAGE_LENGTH);
}
