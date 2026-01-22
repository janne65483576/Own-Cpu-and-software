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

    // TODO: implement it in the lexer
    // double register
    TOKEN_COLON,

    // expressions tokens
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
    Token_R0, Token_R1, Token_R2, Token_R3, Token_PC, Token_SP, Token_MAR, Token_MBR, Token_FLAGS
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
    int index;
}TokenList;

int tokenize(TokenList *tokens, char *text_buffer, int buffer_len);
Token *getToken(TokenList *tokens);
Token *peekToken(TokenList *tokens);

#endif // LEXER_H