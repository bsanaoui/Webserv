#ifndef __TOKEN_HPP__
#define __TOKEN_HPP__

#include <iostream>

enum  Type
{
    WORD,
    OPEN_BRACKET,
    CLOSE_BRACKET,
    SEMICOLON,
    TOKEN_EOF
};

class Token
{
    public:
        /* data */
        Type type;
        std::string value;

        Token(Type type, std::string value);
        Token(const Token& token);
        Token& operator=(const Token& token);
};


#endif