NAME = webserv

CC = clang++
FLAGS = -Werror -Wextra -Wall  -std=c++98
SRC =  	main.cpp \
		parser/Token.cpp \
		parser/Lexer.cpp \
		parser/ServerSetup.cpp \
		parser/Parser.cpp

all : $(NAME)

$(NAME) : $(SRC)
	@$(CC) $(SRC) $(FLAGS) -o $(NAME)

fclean : clean
	@rm -f $(NAME)

clean :

re : fclean all

make test :
	@make re && ./webserv

push : 
	git add .
	git commit -m "$(m)"
	git push origin
