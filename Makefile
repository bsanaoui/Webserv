NAME = webserv

CC = clang++
FLAGS = -Werror -Wextra -Wall  -std=c++98
SRC =  	main.cpp \
		parser_config/Token.cpp \
		parser_config/Lexer.cpp \
		parser_config/SetupServer.cpp \
		parser_config/Parser.cpp \
		utils/utils.cpp \
		parser_request/RequestInfo.cpp \
		parser_request/LexerRe.cpp \
		parser_request/ParserRe.cpp \
		response/Response.cpp

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
