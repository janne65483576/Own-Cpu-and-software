#ifndef LEXER_H
#define LEXER_H

#include <stdint.h>

typedef enum
{
    TOKEN_REGISTER,
    TOKEN_LABEL,
    TOKEN_DIRECTIVE,

    // address
    TOKEN_PERCENT,

    // structure
    TOKEN_NEWLINE,
    TOKEN_COMMA,
    
    // control flow (in expressions)
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    
    // expressions
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_SHL, // "<<"
    TOKEN_SHR, // ">>"
    TOKEN_AND, // '&'
    TOKEN_OR,  // '|'
    TOKEN_XOR, // '^'
    TOKEN_NOT, // '!'

    // used for the instruction names and values (e.g. ADD )
    TOKEN_NUMBER,
    TOKEN_STRING,
}TokenType;

typedef enum
{
    R0, R1, R2, R3, SP, MAR, MBR, FLAGS
}TokenRegister;

typedef struct
{
    TokenType type;
    
    // based on the TokenType
    union
    {
        TokenRegister reg;
        char *str;
        uint64_t number;
    };
}Token;

typedef struct
{
    Token *list;
    int capacity;
    int count;
}TokenList;

int tokenize(TokenList *tokens, char *text_buffer, int buffer_len);

#endif // LEXER_H