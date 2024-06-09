.SILENT:

DEF_COLOR = \033[0;39m
G = \033[0;92m
R = \033[0;91m
C = \033[0;96m
W = \033[1;97m

NAME = ircserv
CC	= c++

CPPFLAGS = -g -Wall -Wextra -Werror -std=c++98

RM = rm -f

SRC_PATH = sources
SRC =	main \
		$(SRC_PATH)/Channel \
		$(SRC_PATH)/Client \
		$(SRC_PATH)/Command \
		$(SRC_PATH)/Server	\
		$(SRC_PATH)/Utils

EXTENSION = .cpp
SRCFC	= $(addsuffix $(EXTENSION), $(SRC))

$(NAME):  $(SRCFC)
	@echo "$(SRCFC)"
	@echo "$(C)Compiling $(NAME) $(DEF_COLOR)"
	$(CC) $(SRCFC) $(CPPFLAGS) -o $(NAME) -I includes/
	@echo "'${G}all${DEF_COLOR}' for mandatory ${NAME} executed successfully!"

all: $(NAME)

clean:
	@echo "'${G}clean${DEF_COLOR}' for ${NAME} executed successfully!"

fclean: clean
	$(RM) $(NAME)
	@echo "'${G}fclean${DEF_COLOR}' for ${NAME} executed successfully!"

re: fclean
	make all
	@echo "'${G}re${DEF_COLOR}' for ${NAME} executed successfully!"

run: re
		clear
		./$(NAME) 6667	password

valgrind: re
		clear
		valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME) 6667	password

.PHONY: clean fclean re

