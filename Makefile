.SILENT:

DEF_COLOR = \033[0;39m
G = \033[0;92m
R = \033[0;91m
C = \033[0;96m
W = \033[1;97m

NAME = ircserv
CC	= c++ -std=c++98

CFLAGS = -Wall -Wextra -Werror -std=c++98

RM = rm -f

SRC_PATH = sources/
SRC =	main \
		$(SRC_PATH)/Server

EXTENSION = .cpp
SRCFC	= $(addsuffix $(EXTENSION), $(SRC))

OBJ = $(SRCFC:.cpp=.o)

$(NAME): $(OBJ)
	@echo "$(OBJ)"
	@echo "$(C)Compiling $(NAME) $(DEF_COLOR)"
	$(CC) $(OBJ) $(CFLAGS) -o $(NAME) -I includes/
	@echo "'${G}all${DEF_COLOR}' for mandatory ${NAME} executed successfully!"

all: $(NAME)

clean:
	$(RM) $(OBJ)
	@echo "'${G}clean${DEF_COLOR}' for ${NAME} executed successfully!"

fclean: clean
	$(RM) $(NAME)
	@echo "'${G}fclean${DEF_COLOR}' for ${NAME} executed successfully!"

re: fclean
	make all
	@echo "'${G}re${DEF_COLOR}' for ${NAME} executed successfully!"

.PHONY: clean fclean re

