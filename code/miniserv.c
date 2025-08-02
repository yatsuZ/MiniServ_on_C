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

// UTIL

void my_print(char *str, int fd)
{
	write(fd, str, strlen(str));
}

int	get_nbr_char(size_t	nbr)
{
	if (nbr < 10)
		return (1);
	return (get_nbr_char(nbr / 10) + 1);
}

int extract_message(char **buf, char **msg)
{
	char	*newbuf;
	int	i;

	*msg = 0;
	if (*buf == 0)
		return (0);
	i = 0;
	while ((*buf)[i])
	{
		if ((*buf)[i] == '\n')
		{
			newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
			if (newbuf == 0)
				return (-1);
			strcpy(newbuf, *buf + i + 1);
			*msg = *buf;
			(*msg)[i + 1] = 0;
			*buf = newbuf;
			return (1);
		}
		i++;
	}
	return (0);
}


// DESTRUCTEUR

void	clean_client(client	*c)
{
	c->client_addr_len = 0;
	if (c->client_fd >= 0)
		close(c->client_fd);
	c->client_fd = -1;
	c->libre = 1;
	c->id = -1;
}

void	end(client *lst_client, int socketfd, char *msg)
{
	if (socketfd >= 0)
		close(socketfd);
	if (lst_client)
	{
		size_t	i = 0;
		while (i < SOMAXCONN)
		{
			clean_client(&(lst_client[i++]));
		}
	}
	if (msg)
	{
		my_print(msg, 2);
		exit(1);
	}
	exit(0);
}

// INIT

void	init_array(client *array_of_client, size_t length)
{
	size_t	i = 0;
	while (i < length)
	{
		array_of_client[i].libre = 1;
		array_of_client[i].id = -1;
		array_of_client[i].client_fd = -1;
		array_of_client[i].client_addr_len = sizeof(array_of_client[i].client_addr);
		i++;
	}

}

struct sockaddr_in init_adress(char *av1)
{
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");// pas autorise
	address.sin_port = htons(atoi(av1));
	return (address);
}

// event

void	sendmessage(client	*aray_c, char *msg, size_t	index)
{
	size_t	i = 0;
	while (i < SOMAXCONN)
	{
		if (aray_c[i].libre == 0 && i != index)
			send(aray_c[i].client_fd, msg, strlen(msg), 0);
		i++;
	}
}

int	newC(client	*aray_c, int sfd, size_t *newid)
{
	size_t	i = 0;

	while (i < SOMAXCONN && aray_c[i].libre == 0)
	{
		i++;
	}

	if (aray_c[i].libre == 0)
		end(aray_c, sfd, ERR_OTHER);

	aray_c[i].client_fd = accept(sfd, (struct sockaddr *)&aray_c[i].client_addr, &aray_c[i].client_addr_len);
	if (aray_c[i].client_fd < 0)
		end(aray_c, sfd, ERR_OTHER);
	
	aray_c[i].libre = 0;
	aray_c[i].id = *newid;
	*newid = *newid + 1;

	char	*msg_new_c = "server: client %ld just arrived\n";
	char	msg[strlen(msg_new_c) + get_nbr_char(aray_c[i].id) - 2];

	sprintf(msg, msg_new_c, aray_c[i].id);
	sendmessage(aray_c, msg, i);

	return (aray_c[i].client_fd);
}


void	client_Event(client	*aray_c, int sfd, fd_set *rfds)
{
	size_t	i = 0;
	int find = 1;

	while (i < SOMAXCONN && find)
	{
		if (aray_c[i].libre == 0 && (FD_ISSET(aray_c[i].client_fd, rfds)))
			find = 0;
		else
			i++;
	}
	if (find)
		end(aray_c, sfd, ERR_OTHER);


	char	*buff = malloc(sizeof(char *) * 100001);
	bzero(buff, 100001);
	char	*new_msg = "client %d: %s";
	ssize_t	res = 0;
	res = recv(aray_c[i].client_fd, buff, 100000, 0);

	if (res <= 0)
	{
		free(buff);
		buff = 0;
		char	*msg_exit_c = "server: client %d just left\n";
		char	msg[strlen(msg_exit_c) + get_nbr_char(aray_c[i].id) - 2];

		sprintf(msg, msg_exit_c, aray_c[i].id);
		sendmessage(aray_c, msg, i);
		aray_c[i].libre = 1;
		aray_c[i].id = -1;
		if (aray_c[i].client_fd >= 0)
			close(aray_c[i].client_fd);
		aray_c[i].client_fd = -1;
		if (res < 0)
			end(aray_c, sfd, ERR_OTHER);
	}
	else
	{
		char *send;
		send = NULL;
		int c_fini = 0;
		if (buff[0] == 'S' && buff[1] == '\n' && buff[2] == '\0') // teste en securite valgrind leak etc
			c_fini = 1;
		while (extract_message(&buff, &send))
		{
			char	msgc[strlen(new_msg) + get_nbr_char(aray_c[i].id) + strlen(send) - 3];
			sprintf(msgc, new_msg, aray_c[i].id, send);
			sendmessage(aray_c, msgc, i);
			free(send);
			send = NULL;
		}
		free(buff);
		buff = NULL;
		if (c_fini)
			end(aray_c, sfd, NULL);
	}
}

int	exec(client	*aray_c, int sfd)
{
	size_t	newid = 0;
	int	ress;
	int	taller_fd = sfd;
	fd_set	rfds;

	// char	*msg_c = "client %ld :";

	while (1)
	{
		FD_ZERO(&rfds);
		FD_SET(sfd, &rfds);
		taller_fd = sfd;
		size_t j = 0;
		while (j < SOMAXCONN)
		{
			if (aray_c[j].libre == 0)
			{
				FD_SET(aray_c[j].client_fd, &rfds);
				if (aray_c[j].client_fd > taller_fd)
					taller_fd = aray_c[j].client_fd;
			}
			j++;
		}

		ress = select(taller_fd + 1, &rfds, NULL, NULL, NULL);

		if (ress <= -1)
			end(aray_c, sfd, ERR_OTHER);
		else if (ress)
		{
			if (FD_ISSET(sfd, &rfds))
			{
				int newfd = newC(aray_c, sfd, &newid);
				FD_SET(newfd, &rfds);
				if (taller_fd < newfd)
					taller_fd = newfd;
			}
			else
				client_Event(aray_c, sfd, &rfds);
		}
	}
}

int	main(int argc, char **argv)
{
	client	array_of_client[SOMAXCONN];

	if (argc <= 1 || !argv)
		end(NULL, -1, ERR_N_PARAM);

	init_array(array_of_client, SOMAXCONN);

	int socketfd = socket(AF_INET, SOCK_STREAM, 0);

	if (socketfd < 0)
		end(array_of_client, socketfd, ERR_INIT_SOCKET);

	struct sockaddr_in address = init_adress(argv[1]);

	if (bind(socketfd, (const struct sockaddr *)(&address), sizeof(address)) < 0)
		end(array_of_client, socketfd, ERR_PARAM_SERV);
	if (listen(socketfd, SOMAXCONN) < 0)
		end(array_of_client, socketfd, ERR_PARAM_SERV);

	exec(array_of_client, socketfd);
	return (end(array_of_client, socketfd, NULL), 0);
}

// faire un meilleur extracteur de message