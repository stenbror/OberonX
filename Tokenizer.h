
#include <map>
#include <string>

#pragma once

typedef enum {
    T_ARRAY, T_BEGIN, T_BY, T_CASE, T_CONST, T_DEFINITION, T_DIV, T_DO, T_ELSE, T_ELSIF, T_END, T_EXIT, T_FALSE, T_FOR,
    T_IF, T_IMPORT, T_IN, T_IS, T_LOOP, T_MOD, T_MODULE, T_NIL, T_OF, T_OR, T_POINTER, T_PROC, T_PROCEDURE, T_RECORD,
    T_REPEAT, T_RETURN, T_TO, T_TRUE, T_TYPE, T_THEN, T_UNTIL, T_VAR, T_WHILE, T_WITH, T_MINUS, T_COMMA, T_SEMICOLON,
    T_COLON, T_ASSIGN, T_DOT, T_UPTO, T_LEFTPAREN, T_RIGHTPAREN, T_LEFTBRACKET, T_RIGHTBRACKET, T_LEFTCURLY, T_RIGHTCURLY,
    T_MUL, T_SLASH, T_HASH, T_ARROW, T_PLUS, T_LESSEQUAL, T_EGUAL, T_GREATEREQUAL, T_BAR, T_TILDE, T_LESS, T_GREATER,
    T_EQUAL, T_IDENT, T_NUMBER, T_STRING, T_EOF
} TokenCode;



class Tokenizer
{
    public:
        Tokenizer();
        TokenCode GetSymbol();
        void Advance();
        unsigned int GetLine();
        unsigned int GetColumn();
        std::string GetText();

};