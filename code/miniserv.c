#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <bits/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define HELLO "HELLO WORLD!\n"
#define ERR_N_PARAM "y a pas le bon nombres de parametre.\n"
#define ERR_INIT_SOCKET "Errueur initialisation de la socket.\n"
#define ERR_PARAM_SERV "Errueur Param SERV.\n"
#define ERR_OTHER "ERR IDK.\n"

typedef struct s_client
{
	struct sockaddr_in client_addr;
	socklen_t client_addr_len;
	int client_fd;
	int libre;
	size_t id;
} client;


u_int16_t get_port(char *str)
{
	return(atoi(str));
}

void my_print(char *str, int fd)
{
	int	len = 0;
	while (str[len])
	{
		len++;
	}
	write(fd, str, len);
}

void free_clean(int socket_fd, client *array_of_client)
{
	if (socket_fd >= 0)
		close(socket_fd);
	if (array_of_client)
	{
		size_t	i = 0;
		while (i < SOMAXCONN)
		{
			if ((*array_of_client).client_fd >= 0)
				close((*array_of_client).client_fd);
		}
		
	}
}

int	print_err(char *str, int socket_fd, client *array_of_client)
{
	my_print(str, STDERR_FILENO);
	free_clean(socket_fd, array_of_client);
	return (1);
}


int	main(int argc, char **argv)
{
	client array_of_client[SOMAXCONN];

	size_t	i_ac = 0;
	while (i_ac < SOMAXCONN)
	{
		array_of_client[i_ac].libre = 1;
		array_of_client[i_ac].id = -1;
		array_of_client[i_ac].client_fd = -1;
		array_of_client[i_ac].client_addr_len = sizeof(array_of_client[i_ac].client_addr);
	}
	i_ac = 0;

	if (argc <= 1 || !argv)
		return (print_err(ERR_N_PARAM, 2, array_of_client));

	int socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketfd < 0)
		return (print_err(ERR_INIT_SOCKET, 2, array_of_client) , 1);

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_LOOPBACK;
	address.sin_port = get_port(argv[1]);


	if (bind(socketfd, (const struct sockaddr *)(&address), sizeof(address) < 0))
		return (print_err(ERR_PARAM_SERV, 2, array_of_client) , 1);
	if (listen(socketfd, SOMAXCONN) < 0)
		return (print_err(ERR_PARAM_SERV, 2, array_of_client) , 1);

	while (1)
	{		
		// ajouter
		// msg
		// quite

		// accept client connection
		// if ((*client_fd = accept(socketfd,
		// 						 (struct sockaddr *)&client_addr,
		// 						 &client_addr_len)) < 0)
		// {
		// 	return (print_err(ERR_PARAM_SERV, 2, array_of_client) , 1);
		// }

	}

	free_clean(socketfd, array_of_client);
	return (0);
}