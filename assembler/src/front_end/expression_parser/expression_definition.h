#ifndef EXPRESSION_DEFINITION_H
#define EXPRESSION_DEFINITION_H

#include "../lexer/token.h"

typedef struct AstNode
{
    Token token;

    struct AstNode *left;
    struct AstNode *right;
} AstNode;

#endif // EXPRESSION_DEFINITION_H