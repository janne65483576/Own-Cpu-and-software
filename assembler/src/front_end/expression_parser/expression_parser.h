#ifndef EXPRESSION_PARSER_H
#define EXPRESSION_PARSER_H

#include "../lexer/lexer.h"

const TokenType expression_tokens[] = {    
    TOKEN_STRING, // identifier
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_SHL,
    TOKEN_SHR,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_XOR,
    TOKEN_NOT,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
};

typedef struct AstNode
{
    Token token;

    struct AstNode *left;
    struct AstNode *right;
} AstNode;

AstNode* parseExpression(TokenList *tokens, int rbp);
int calculate(AstNode *node);
void freeAst(AstNode *node);

#endif // EXPRESSION_PARSER_H