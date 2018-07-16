/* teststreamsource.c
 *
 * this is a sample source which sends data in the format
 * expected by the teststream wrapper.
 *
 *	1. connect to the wrapper clearinghouse at the
 *	   hostname and port specified on the command line.
 *	2. send the stream name as passed on the command line.
 *		the stream name is sent as the first 1024 byte block
 *		on the newly established connection.
 *	3. send data tuples.  each tuple is two longs.
 */


#include <signal.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#include <sys/time.h>

#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <netdb.h>
#include "netinet/in.h"
#include <string.h>				/* TEMP */


typedef int int32;

#define CONTROL_MSG_SIZE 1024

int			sendSourceName(int socket, char *name);
int			establishConnection(char *host, int port);
int			sendData(int sock, int numelts, int brange, int erange);
int			writen(int fd, char *ptr, int nbytes);




int
main(int argc, char **argv)
{
	int			socket;
	int			status;

	if (argc != 7)
	{
		printf("Usage: %s <hostip> <port> <sourcename> <numvalues> <brange> <erange>\n",
			   argv[0]);
		exit(1);
	}

	socket = establishConnection(argv[1], atoi(argv[2]));
	if (socket == -1)
		exit(1);
	if (sendSourceName(socket, argv[3]) < 0)
	{
		printf("could not send source name\n");
		close(socket);
		exit(1);
	}
	if ((status = sendData(socket, atoi(argv[4]), atoi(argv[5]), atoi(argv[6]))) < 0)
		printf("error while sending data\n");
	close(socket);
	exit(status);


}

int
sendSourceName(int socket, char *source)
{
	int			length = strlen(source) + 1;
	int			msglength = sizeof(int32) + length;
	char		buf[msglength];

	printf("sizeof(int32) is %d\n", sizeof(int32));
	/* memset(buf, 0, msglength); */
	sprintf(buf, "%4d%s", length, source);

	/*
	 * memcpy((void *)buf, (void *) (&length), sizeof(int32));
	 * strcpy(buf+sizeof(int32), source);
	 */
	if (writen(socket, buf, msglength) < msglength)
		return -1;
	else
		return 0;
}

int
establishConnection(char *hostip, int port)
{
	int			sockfd;
	struct sockaddr_in serv_addr;
	struct hostent *entry;

	entry = gethostbyname(hostip);
	if (entry == NULL)
	{
		herror(hostip);
		return -1;
	}

	fprintf(stderr, "HostIP: %s %s %d\n", hostip,
			inet_ntoa(*(struct in_addr *) (entry->h_addr)),
			entry->h_length);

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	/* serv_addr.sin_addr.s_addr = inet_addr(entry->h_addr); */
	serv_addr.sin_addr = *(struct in_addr *) (entry->h_addr);
	serv_addr.sin_port = htons(port);

	/* open a TCP socket */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stderr, "source: cannot open socket\n");
		perror(NULL);
		return -1;
	}


	/* connect to well known TCQ wrapper */
	if (connect(sockfd, (struct sockaddr *) & serv_addr, sizeof(serv_addr)) < 0)
	{
		fprintf(stderr, "source: cannot connect to TCQ wrapper\n");
		perror(NULL);
		return -1;
	}


	return sockfd;

}


int
sendData(int sockfd, int nelems, int brange, int erange)
{
	static int	firsttime = 1;
	int			sendsize = 2 * sizeof(long);
	char	   *sendbuf = malloc(sendsize);
	int			count = 0;
	int			c = 0;
	int			d = 0;
	int			irand = 0;
	int			i;


	if (firsttime)
	{
		firsttime = 0;
		srand48(1);
		srand(1);
	}

	fprintf(stderr, "%d,%d\n", sizeof(int), sizeof(long));
	fprintf(stderr, "brange:%d, erange:%d\n",
			brange, erange);

	bzero(sendbuf, sendsize);

	irand = 0;

	while ((count++ < nelems) || (nelems < 0))
	{

		irand = rand();
		c = brange + (irand % (erange - brange));
		irand = rand();
		d = brange + (irand % (erange - brange));
		memcpy(sendbuf, &c, sizeof(long));
		memcpy((sendbuf + sizeof(long)), &d, sizeof(long));

		fprintf(stderr, "[%d,%d]\n", c, d);
		for (i = 0; i < sendsize; i++)
			fprintf(stderr, "%x", sendbuf[i]);
		fprintf(stderr, "\n");

		if ((writen(sockfd, sendbuf, sendsize) == 0) && (errno == SIGPIPE))
		{
			close(sockfd);
			return -1;

		}
		if ((count % 100) == 0)
			sleep(1);
	}
	return 0;
}




int
writen(int fd, char *ptr, int nbytes)
{
	int			nleft,
				nwritten;

	nleft = nbytes;

	while (nleft > 0)
	{
		nwritten = write(fd, ptr, nleft);
		if (nwritten <= 0)
		{
			fprintf(stderr, "writen: write failed\n");
			return nwritten;
		}

		nleft -= nwritten;
		ptr += nwritten;
	}
	return (nbytes - nleft);

}
