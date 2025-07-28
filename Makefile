# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yzaoui <yzaoui@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/28 15:48:52 by yzaoui            #+#    #+#              #
#    Updated: 2025/07/28 16:26:15 by yzaoui           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY = all clean exec debug_leak

CC = cc

FALGS = -g3 -Wall -Wextra -Werror

SRC = ./code/miniserv.c

NAME = exec_miniserv.out


all :
	$(CC) $(FALGS) $(SRC) -o $(NAME)

clean:
	rm -rf $(NAME)

exec : clean all
	./$(NAME)

debug_leak: clean all
	valgrind --leak-check=full --track-fds=yes ./$(NAME)