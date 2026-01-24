#ifndef TOKEN_H
#define TOKEN_H

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
    // for double register
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

    // not implemented in the lexer
    TOKEN_ZERO_FLAG,
    TOKEN_CARRY_FLAG,
    TOKEN_SIGN_FLAG,

    TOKEN_EOF,
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
        int number;
    };
}Token;

typedef struct
{
    Token *list;
    int capacity;
    int count;
    int index;
}TokenList;

Token *getToken(TokenList *tokens);
Token *peekToken(TokenList *tokens);

typedef struct
{
    
}TokenTree;

#endif // TOKEN_H