#ifndef __PARSER__HPP__
#define __PARSER__HPP__

#include "Lexer.hpp"
#include "ServerSetup.hpp"

class Parser
{
    public:
    //------ public member --------- >>
    Lexer   lexer;
    Token   curr_token;
    Token   prev_token;

    // --------------------------------------------------------- //
    // --------------- Constructors and Operators -------------- //
    // --------------------------------------------------------- //
    Parser(Lexer lexer);
    Parser(const Parser& parser);
    Parser& operator=(const Parser& parser);

    // --------------------------------------------------------- //
    // ----------------- Member Methods ------------------------ //
    // --------------------------------------------------------- //
    void                                        eat(TypeToken token_type);
    std::vector<ServerSetup>                    parse();
    ServerSetup                                 parseServer();
    std::pair<short, u_int32_t>                 parseListen();
    std::vector<std::string>                    parseWords();
    std::string                                 parseWord();
    std::vector<std::pair<short, std::string> > parseErrorPages();
    t_location                                  parseLocation();

    // --------------------------------------------------------- //
    // -------------- Non Member Functions --------------------- //
    // --------------------------------------------------------- //
    static  void errorDisplay(std::string s);

}; // class Parser

#endif