/// INCLUDE

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
// DEFINE

#define HELLO "HELLO WORLD!\n"
#define ERR_N_PARAM "y a pas le bon nombres de parametre.\n"
#define ERR_INIT_SOCKET "Errueur initialisation de la socket.\n"
#define ERR_PARAM_SERV "Errueur Param SERV.\n"
#define ERR_OTHER "ERR IDK.\n"

/// STRUCT

typedef struct s_client
{
	struct sockaddr_in client_addr;
	socklen_t client_addr_len;
	int client_fd;
	int libre;
	size_t id;
} client;

// DESTRUCTEUR

void free_clean(int socket_fd, client *array_of_client)
{
	if (socket_fd >= 0)
		close(socket_fd);
	if (array_of_client)
	{
		size_t	i = 0;
		while (i < SOMAXCONN)
		{
			if (array_of_client[i].client_fd >= 0)
				close(array_of_client[i].client_fd);
			i++;
		}
		
	}
}

// UTILS

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

int	print_err(char *str, int socket_fd, client *array_of_client)
{
	my_print(str, STDERR_FILENO);
	free_clean(socket_fd, array_of_client);
	return (1);
}

int	get_nbr_char(size_t	nbr)
{
	if (nbr < 10)
		return (1);
	return (get_nbr_char(nbr / 10) + 1);
}

int	main(int argc, char **argv)
{
	int	retval;
	client	array_of_client[SOMAXCONN];

	size_t	n_ac = 0;
	while (n_ac < SOMAXCONN)
	{
		array_of_client[n_ac].libre = 1;
		array_of_client[n_ac].id = -1;
		array_of_client[n_ac].client_fd = -1;
		array_of_client[n_ac].client_addr_len = sizeof(array_of_client[n_ac].client_addr);
		n_ac++;
	}
	n_ac = 0;

	if (argc <= 1 || !argv)
		return (print_err(ERR_N_PARAM, 2, array_of_client));

	int socketfd = socket(AF_INET, SOCK_STREAM, 0);

	if (socketfd < 0)
		return (print_err(ERR_INIT_SOCKET, 2, array_of_client) , 1);

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");// pas autorise
	address.sin_port = htons(get_port(argv[1]));

	if (bind(socketfd, (const struct sockaddr *)(&address), sizeof(address)) < 0)
		return (print_err(ERR_PARAM_SERV, 2, array_of_client) , 1);
	if (listen(socketfd, SOMAXCONN) < 0)
		return (print_err(ERR_PARAM_SERV, 2, array_of_client) , 1);

	int	taller_fd = socketfd;
	fd_set	rfds;
	char	*msg_new_c = "serveur : le client %ld vient d'arriver\n";

	while (1)
	{
		FD_ZERO(&rfds);
		FD_SET(socketfd, &rfds);
		retval = select(taller_fd + 1, &rfds, NULL, NULL, NULL);

		/* Don't rely on the value of tv now! */
		if (retval <= -1)
			return (print_err(ERR_OTHER, 2, array_of_client) , 1);
		else if (retval)
		{
			if (FD_ISSET(socketfd, &rfds))
			{
				int client_fd;
				struct sockaddr_in client_addr;
				socklen_t client_addr_len = sizeof(client_addr);

				client_fd = accept(socketfd, (struct sockaddr *)&client_addr, &client_addr_len);
				if (client_fd < 0)
					return (print_err(ERR_PARAM_SERV, 2, array_of_client) , 1);
				size_t	i = 0;
				while (i < SOMAXCONN && array_of_client[i].libre == 0)
				{
					i++;
				}
				if (array_of_client[i].libre == 0)
					return (print_err(ERR_OTHER, 2, array_of_client) , 1);

				array_of_client[i].libre = 0;
				array_of_client[i].id = n_ac;
				array_of_client[i].client_fd = client_fd;
				array_of_client[i].client_addr = client_addr;
				array_of_client[i].client_addr_len = sizeof(array_of_client[i].client_addr);
				n_ac++;

				size_t	new_id = array_of_client[i].id;
				char	msg[strlen(msg_new_c) + get_nbr_char(array_of_client[i].id) - 2];
				i = 0;
				sprintf(msg, msg_new_c, new_id);
				while (i < SOMAXCONN)
				{
					if (array_of_client[i].libre == 0)
						send(array_of_client[i].client_fd, msg, strlen(msg_new_c) + get_nbr_char(new_id) - 2, 0);
					i++;
				}
			}
			// else
			// {
			// 	size_t i = 0;
			// 	int	find = 1;
			// 	while (find && i < SOMAXCONN)
			// 	{
			// 		if (array_of_client[i].libre == 0 && (FD_ISSET(array_of_client[i].client_fd, &rfds)))
			// 			find = 0;
			// 		else
			// 			i++;
			// 	}
			// 	if (find)
			// 		return (print_err(ERR_OTHER, 2, array_of_client) , 1);
			// 	client	c = array_of_client[i];

			// }
			// Savoirs qui est 
		}
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