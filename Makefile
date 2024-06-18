##
## EPITECH PROJECT, 2019
## Makefile
## File description:
## Makefile
##

SRC		=	src/entry.cpp \
			src/main.cpp

CFLAGS	=	-std=gnu11 -Wall -Wextra -I include

OBJ		=	$(SRC:.cpp=.o)

INCLUDE	=	-o

LIB		=	-g -lm

NAME	=	305construction

all:	$(NAME)

$(NAME):$(OBJ)
	g++ $(OBJ) $(CFLAGS) $(INCLUDE) $(NAME) $(LIB)

clean:
	rm -f $(OBJ)

fclean:	clean
	rm -f $(NAME)

re:	fclean all

.PHONY: all clean fclean re