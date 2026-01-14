#ifndef LEXER_H
#define LEXER_H

#include <stdint.h>

typedef enum
{
    REGISTER,

    // address
    PERCENT,

    // only for labels
    COLON,

    // structure
    NEWLINE,
    COMMA,
    
    // control flow (in expressions)
    LPAREN,
    RPAREN,
    
    // expressions
    PLUS,
    MINUS,
    STAR,
    SLASH,
    SHL, // "<<"
    SHR, // ">>"
    AND, // '&'
    OR,  // '|'
    XOR, // '^'
    NOT, // '!'

    // used for identification of directives (decimal numbers are not suported in the isa)
    DOT,

    // used for the instruction names and values (e.g. ADD )
    NUMBER,
    STRING,
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

#endif // LEXER_H