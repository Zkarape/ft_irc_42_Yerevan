NAME      = ircserv

CC        = c++ 

FLAGS     = -Wextra -Werror -Wall -g3 -ggdb3 -std=c++98 -pedantic

SRCDIR = srcs

HEADERS = includes

SRCS_FILES = $(wildcard $(SRCDIR)/*.cpp)

HEADER = $(wildcard $(HEADERS)/*.hpp)

OBJ_FILES = $(SRC_FILES:.cpp=.o)

%.o: %.cpp $(HEADER) Makefile
		$(CC) $(FLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ_FILES)
		$(CC) $(FLAGS) -o $(NAME) $^

clean:
	rm -rf $(OBJ_FILES)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all fclean clean re
