#include "geoserver.h"

char	   *get_next_target(int client_fd);
char	   *form_geotrack_tuple(char *location);
void		send_tuple_to_client(int fd, char *tuple);
int			establish_connection();
int			load_city_hash(struct hash_cache * cache);
void		catch_broken_pipe(int sig);
void		remove_whitespace(char *str);
void		restore_whitespace(char *str);
void		backup_cache_file();

/* Persistant Data Structures */
struct hash_cache *city_to_latlong;
struct hash_cache *cache;
indicator	hci;

int			server_port = GEOSERVER_DEFAULT_PORT;

/* Global Networking Variables */
bool		exists_connection = FALSE;
int			listen_socket = -1;

int
main(int argc, char *argv[])
{
	/* Geotracking Variables */
	char	   *exec_string;
	char	   *current_target;
	char	   *geotrack_output;
	char	   *geotrack_location;
	char	   *return_tuple;
	FILE	   *fp;
	char	   *file_pos;

	/* Networking Variables */
	int			connection_socket;

	/* command line argument handling */
	int			args_left_to_handle = argc - 1;
	int			current_arg_position = 1;

	while (args_left_to_handle != 0)
	{
		switch (argv[current_arg_position][1])
		{
			case 'p':
				if (args_left_to_handle >= 2)
				{
					printdbg("Geoserver Port Change", argv[current_arg_position + 1]);
					server_port = atoi(argv[current_arg_position + 1]);
				}
				args_left_to_handle -= 2;
				current_arg_position += 2;
				break;
			default:
				fprintf(stderr, "Incorrect arguments\n");
				fprintf(stderr, "Usage: geoserver <-p PORT_NUM>\n");
				return -1;
		}
	}

	/* Signal Handler */
	signal(SIGPIPE, catch_broken_pipe);

	cache = (struct hash_cache *) malloc(sizeof(struct hash_cache));
	city_to_latlong = (struct hash_cache *) malloc(sizeof(struct hash_cache));

	/* Initialize hash table for city->(lat,long) mapping */
	load_city_hash(city_to_latlong);

	/* Initialize the cache */
	printdbg("Geoserver Init", "Creating cache...");
	hci = create_hashcache(cache, 300, 30, 1, CACHE_FILE);

	if (USE_PERSISTANT_CACHE)
	{
		printdbg("Geoserver Init", "Attempting to load persistant data from disk\n");
		hci = load_hashcache(cache, CACHE_FILE);
		backup_cache_file();
	}

	if (hci == SUCCESS)
		printdbg("Geoserver Init", "Cache created (loaded) and initialized...");
	else
		fprintf(stderr, "No persistant cache file yet, it will be created...\n");

	/* Wait for a network connection ... */
	connection_socket = establish_connection();

	/* main read-eval-send loop */
	while (1)
	{

		/* Check if the connection has been lost */
		if (exists_connection == FALSE)
		{
			/* If so, listen and establish a new one ... */
			connection_socket = establish_connection();
		}

		/* get the current target from the network */
		printdbg("Geoserver Main", "Getting next target from network");
		current_target = get_next_target(connection_socket);

		/* if the network connection was closed, restart this loop */
		if (current_target == NULL && exists_connection == FALSE)
			continue;

		/* look it up in the cache */
		printdbg("Geoserver Main", "Looking target up in cache");
		return_tuple = get_value(cache, current_target);

		/* big branch: either its in the cache or it isn't */
		if (return_tuple == NOT_IN_CACHE)
		{
			printdbg("Geoserver Main", "Target not in cache, geotracking...");

			/* form the command line and execute it */
			printdbg("Geoserver Main", "Forming command string");
			exec_string = (char *) malloc(sizeof(char) * 37);
			strcpy(exec_string, "./geotrack -l ");
			strcat(exec_string, current_target);
			strcat(exec_string, " > ");
			strcat(exec_string, TEMP_FILE);
			system(exec_string);
			printdbg("Geoserver Main", exec_string);

			/* now open the file and extract the city */
			printdbg("Geoserver Main ", "Opening file for input");
			fp = fopen(TEMP_FILE, "r");
			if (fp == NULL)
			{
				fprintf(stderr, "Error opening temp file for reading.  Exiting...\n");
				return -1;
			}
			geotrack_output = (char *) malloc(sizeof(char) * GT_OUTPUT_SIZE);
			geotrack_location = (char *) malloc(sizeof(char) * GT_OUTPUT_SIZE);
			printdbg("Geoserver Main ", "Getting a line from the file");

			fgets(geotrack_output, GT_OUTPUT_SIZE, fp);

			file_pos = strrchr(geotrack_output, '=');

			strcpy(geotrack_location, INVALID_LOCATION);

			if (file_pos[1] == '\n')
			{
				/* no location, leave variable alone */
			}
			else
			{
				/* otherwise, overwrite with location from file */
				sscanf(geotrack_output, "location=%s\n", geotrack_location);
			}

			/* get all the information into return_tuple */
			printdbg("Geoserver Main", "Forming tuple from location");
			return_tuple = form_geotrack_tuple(geotrack_location);

			/* throw the tuple into the cache */
			printdbg("Geoserver Main", "Adding tuple to cache");
			remove_whitespace(return_tuple);
			add_key(cache, current_target, return_tuple);

			/* remove the temp file */
			free(exec_string);
			exec_string = (char *) malloc(sizeof(char) * (strlen(TEMP_FILE) + 4));
			strcpy(exec_string, "rm ");
			strcat(exec_string, TEMP_FILE);
			printdbg("Geoserver", exec_string);
			system(exec_string);

			/* free malloc'd memory */
			free(exec_string);
			free(geotrack_location);
			free(geotrack_output);

		}
		else
			printdbg("Geoserver Main", "IP Address in cache, returning tuple");

		/* send the location to the client */
		printdbg("Geoserver Main", "Sending location to client");
		restore_whitespace(return_tuple);
		send_tuple_to_client(connection_socket, return_tuple);


		/* free up mallod'd memory (CHECK THIS, MAYBE MORE) */
		free(return_tuple);
	}

	return 0;
}

char *
get_next_target(int client_fd)
{
	int			bytes_so_far = 0;
	ssize_t		read_bytes;
	char	   *read_buffer = (char *) malloc(sizeof(char) * INC_MESSAGE_LENGTH);
	char	   *next_target_address = (char *) malloc(sizeof(char) * INC_MESSAGE_LENGTH);
	int			i;

	do
	{
		read_bytes = read(client_fd, read_buffer, INC_MESSAGE_LENGTH - bytes_so_far);
		if (read_bytes == -1 || read_bytes == 0)
		{
			exists_connection = FALSE;
			return NULL;
		}
		for (i = 0; i < read_bytes; i++)
			next_target_address[i + bytes_so_far] = read_buffer[i];
		bytes_so_far += read_bytes;
	} while (bytes_so_far != INC_MESSAGE_LENGTH);

	return next_target_address;
}

void
send_tuple_to_client(int fd, char *tuple)
{
	size_t		written_bytes;

	printdbg("Send Tuple", "Sending message to client");

	written_bytes = write(fd, tuple, (size_t) strlen(tuple));
	if (written_bytes == -1)
	{
		exists_connection = FALSE;
		return;
	}
	else if (written_bytes != strlen(tuple))
		fprintf(stderr, "Total Message not written: wrote %d of %d bytes\n", written_bytes, strlen(tuple));
	printdbg("Geoserver->Client", tuple);

	written_bytes = write(fd, DONE_MESSAGE, strlen(DONE_MESSAGE));
	if (written_bytes == -1)
	{
		exists_connection = FALSE;
		return;
	}
	else if (written_bytes != strlen(DONE_MESSAGE))
		fprintf(stderr, "Total DONE message not written: wrote %d of %d bytes\n", written_bytes, strlen(DONE_MESSAGE));
	printdbg("Geoserver->Client", DONE_MESSAGE);

}

char *
form_geotrack_tuple(char *location)
{
	char	   *final_delimeter;
	char	   *message;
	int			i,
				j;
	char	   *latlong;

	message = (char *) malloc(sizeof(char) * OUT_MESSAGE_LENGTH);

	if (strcmp(INVALID_LOCATION, location) == 0)
	{
		strcpy(message, location);
		strcat(message, "\n");
		return message;
	}


	printdbg("Forming Tuple", location);

	/* every location has a city */
	final_delimeter = strrchr(location, (int) '+');
	for (i = 0; i < (final_delimeter - location); i++)
	{
		if (location[i] == '+')
			message[i] = ' ';
		else
			message[i] = location[i];
	}

	/* move on to next 'field' */
	printdbg("Geoserver", "City Copied");
	message[i++] = ',';

	/* now, determine country by what follows final + */
	if (final_delimeter[3] == '\0')
	{
		/* there are only two chars after final +, USA */
		/* copy the state */
		message[i++] = final_delimeter[1];
		message[i++] = final_delimeter[2];
		message[i++] = ',';
		message[i++] = 'U';
		message[i++] = 'S';
		message[i++] = 'A';
	}
	else
	{
		/* must be Europe, put an empty state */
		message[i++] = 'Z';
		message[i++] = 'Z';
		message[i++] = ',';
		/* advance it past the + */
		final_delimeter += 1;
		/* copy in the country */
		for (j = 0; final_delimeter[j] != '\0'; j++)
			message[i++] = final_delimeter[j];
	}

	/* done with city,state,country...move on to lat/long */
	message[i++] = ',';

	/* now get the latitude and longitude */
	latlong = get_value(city_to_latlong, location);
	final_delimeter = strrchr(latlong, (int) ' ');
	final_delimeter[0] = ',';

	/* now copy latlong into message */
	for (j = 0; j < strlen(latlong); j++)
		message[i++] = latlong[j];

	/* null terminate the message */
	message[i++] = '\n';
	message[i++] = '\0';

	/* free any malloc'd memory */
	free(latlong);

	return message;
}



/*
 * Waits for a connection, then returns the new network stream
 */
int
establish_connection()
{

	/* Socket variables are global */
	int			connection_socket;
	struct sockaddr_in server_addr,
				client_addr;
	int			client_length;
	int			ret_status;

	int			opt_true = 1;

	if (listen_socket == -1)
	{
		/* Open a socket */
		listen_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (listen_socket <= 0)
		{
			fprintf(stderr, "Could not create a listening socket: returned %d\n", listen_socket);
			exit(-1);
		}
		else
			printdbg("Geoserver Networking", "Socket Created");

		/* what does this line do? */
		bzero(&server_addr, sizeof(server_addr));

		/* set socket options */
		ret_status = setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, (void *) &opt_true, sizeof(int));
		if (ret_status < 0)
			printdbg("Geoserver Networking", "Could not set re-usable socket option");
		else
			printdbg("Geoserver Networking", "Socket options changed: Socket now reusable");

		/* set up the server's address info */
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		server_addr.sin_port = htons(server_port);

		/* Bind the socket */
		ret_status = bind(listen_socket, (struct sockaddr *) & server_addr, sizeof(server_addr));

		switch (ret_status)
		{
			case -1:
				fprintf(stderr, "Could not bind socket to port %d\n", server_port);
				exit(-1);
			case 0:
				printdbg("Geoserver Networking", "Socket Bound");
				break;
			default:
				fprintf(stderr, "Unknown return value from bind: %d\n", ret_status);
				exit(-1);
		}

		/* Change the socket to a listening socket */
		ret_status = listen(listen_socket, LISTEN_QUEUE_SIZE);

		switch (ret_status)
		{
			case -1:
				fprintf(stderr, "Could not listen on the socket...\n");
				exit(-1);
			case 0:
				printdbg("Geoserver Networking", "Listening on socket");
				break;
			default:
				fprintf(stderr, "Unknown return value from listen: %d\n", ret_status);
				exit(-1);
		}
	}
	else
		printdbg("Geoserver Main", "Lost connection.  Waiting for a new client...");

	/* Wait for a connection */
	connection_socket = accept(listen_socket, (struct sockaddr *) & client_addr, &client_length);

	if (connection_socket <= 0)
		fprintf(stderr, "Error accepting a new client.  Accept returned: %d\n", connection_socket);
	else
		printdbg("Geoserver Networking", "New client accepted!");

	/* let the world know a connection exists */
	exists_connection = TRUE;

	/* return the new connection */
	return connection_socket;
}

int
load_city_hash(struct hash_cache * cache)
{

	indicator	hci;
	FILE	   *fp;
	int			ret_value;
	float		latitude,
				longitude;
	char	   *city;
	char	   *latlongstring;
	int			num_cities;
	int			i;

	hci = create_hashcache(cache, 50, NO_MAX_LENGTH, NEVER_FLUSH, LATLONG_FILE);
	if (hci == SUCCESS)
		printdbg("Load_Lat_Long", "HashCache created successfully...");
	else
	{
		fprintf(stderr, "Could not create hashcache for city location hashtable...\n");
		return -1;
	}

	fp = fopen(LATLONG_BASE, "r");
	if (fp == NULL)
	{
		fprintf(stderr, "Could not open latitude/longitude file...\n");
		return -1;
	}

	fscanf(fp, "%d\n", &num_cities);

	for (i = 0; i < num_cities; i++)
	{
		latlongstring = (char *) malloc(100);
		city = (char *) malloc(100);
		ret_value = fscanf(fp, "%s %f %f\n", city, &latitude, &longitude);
		if (ret_value != 0)
		{
			sprintf(latlongstring, "%f %f", latitude, longitude);
			add_key(cache, city, latlongstring);
		}
		free(latlongstring);
		free(city);
	}

	fclose(fp);

	printdbg("Load_Lat_Long", "Finished loading hashtable");

	return 0;
}

void
remove_whitespace(char *str)
{
	char	   *ptr;

	ptr = strchr(str, (int) ' ');
	while (ptr != NULL)
	{
		ptr[0] = WS_DELIMETER;
		ptr = strchr(str, (int) ' ');
	}
}

void
restore_whitespace(char *str)
{
	char	   *ptr;

	ptr = strchr(str, (int) WS_DELIMETER);
	while (ptr != NULL)
	{
		ptr[0] = ' ';
		ptr = strchr(str, (int) WS_DELIMETER);
	}
}

void
catch_broken_pipe(int sig)
{
	exists_connection = FALSE;
	return;
}

void
backup_cache_file()
{
	char	   *command = (char *) malloc(sizeof(char) * 50);

	strcpy(command, "cat ");
	strcat(command, CACHE_FILE);
	strcat(command, " >> cache.log");
	system(command);
	free(command);
	system("echo ------------------------ >> cache.log");
}
