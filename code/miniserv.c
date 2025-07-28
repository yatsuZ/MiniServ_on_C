#include <unistd.h>
#include <stdlib.h>

#define HELLO "HELLO WORLD!\n"

void my_print(char *str, int fd)
{
	int	len = 0;
	while (str[len])
	{
		len++;
	}
	write(fd, str, len);
}


int	main(void)
{
	my_print(HELLO, STDOUT_FILENO);
	char *o = malloc(50);
	free(o);
	return (0);

}

// pour compiler
// cc -Wall -Wextrat -Werror ./miniserv.c
// pour executer
// ./a.out