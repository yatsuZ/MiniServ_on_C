#include <unistd.h>
#include <stdlib.h>

#define HELLO "HELLO WORLD!\n"
#define WRONG_N_PARAM "y a pas le bon nombres de parametre.\n"


void my_print(char *str, int fd)
{
	int	len = 0;
	while (str[len])
	{
		len++;
	}
	write(fd, str, len);
}


int	main(int argc, char **argv)
{
	char *msg = "VOCI LE PARAM 1 = ";
	if (argc <= 1 || !argv)
		return (my_print(WRONG_N_PARAM, STDERR_FILENO) , 1);
	my_print(msg, STDOUT_FILENO);
	my_print(argv[1], 1);
	msg = "\n";
	my_print(msg, STDOUT_FILENO);
	my_print(HELLO, STDOUT_FILENO);
	char *o = malloc(50);
	free(o);
	return (0);

}