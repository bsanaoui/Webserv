#ifndef __LEXER_RE_HPP__
#define __LEXER_RE_HPP__

#include "../../parser_config/include/Token.hpp"

class LexerRe
{
    public:
        /* data */
        char			c;
        unsigned int	i;
        std::string		contents;

        // Constructors
        LexerRe(std::string contents);
        LexerRe();
        
        // Operations
        LexerRe(const LexerRe& lexer);
        LexerRe& operator=(const LexerRe& lexer);

        // Member Methods
        void    advance();
        void	skipWhiteSpaces();
        Token   getNextToken();
        Token   collectWord();
        Token   advanceWithToken(Token token);
        Token   collectBody();
        bool    isBody();

        // Non Member Methods
        static int	isWord(char c);
};

#endif