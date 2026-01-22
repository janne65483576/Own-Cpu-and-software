#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expression_parser.h"
#include "../lexer/lexer.h"

bool illegalToken(Token *token)
{
    for (int i = 0; i < sizeof(expression_tokens) / sizeof(TokenType); i++)
    {
        if(expression_tokens[i] == token->type)
        {
            return false;
        }
    }
    return true;
}

Token *getExpressionToken(TokenList *tokens)
{
    Token *token = getToken(tokens);
    if (token == NULL || illegalToken(token)) return NULL;
    return token;
}

Token *peekExpressionToken(TokenList *tokens)
{
    Token *token = peekToken(tokens);
    if (token == NULL || illegalToken(token)) return NULL;
    return token;
}

void getBindingPower(TokenType type, int *left_binding_power, int *right_binding_power)
{
    switch(type)
    {
        case TOKEN_OR:
            *left_binding_power = 2;
            *right_binding_power = 3; 
            break;
        case TOKEN_XOR:
            *left_binding_power = 4;
            *right_binding_power = 5; 
            break;
        case TOKEN_AND:
            *left_binding_power = 6;
            *right_binding_power = 7;
            break;
        case TOKEN_SHL:
        case TOKEN_SHR:
            *left_binding_power = 8; 
            *right_binding_power = 9;
            break;
        case TOKEN_PLUS:
        case TOKEN_MINUS:
            *left_binding_power = 10;
            *right_binding_power = 11;
            break;
        case TOKEN_STAR:
        case TOKEN_SLASH:
            *left_binding_power = 20;
            *right_binding_power = 21;
            break;
        default:
            *left_binding_power = 0;
            *right_binding_power = 0;
            break;
    }
}

AstNode *newNode(TokenType type, double value, AstNode *left, AstNode *right)
{
    AstNode *node = malloc(sizeof(AstNode));
    node->token.type = type;
    node->token.number = value;

    node->left = left;
    node->right = right;
    return node;
}

AstNode* parseExpression(TokenList *tokens, int right_binding_power)
{
    // left hand side
    Token *curr_token = getToken(tokens);
    if (curr_token == NULL)
    {
        return NULL;
    }

    AstNode *lhs = NULL;

    switch (curr_token->type) 
    {
        case TOKEN_NUMBER:
            lhs = newNode(TOKEN_NUMBER, curr_token->number, NULL, NULL);
            break;
        
        case TOKEN_STRING:
            lhs = newNode(TOKEN_STRING, 0, NULL, NULL);
            lhs->token.str = curr_token->str;
            break;
        
        case TOKEN_MINUS:
        case TOKEN_NOT:
        {
            AstNode *rhs = parseExpression(tokens, 100);
            if (!rhs) return NULL;
            lhs = newNode(curr_token->type, 0, NULL, rhs);
            break;
        }

        case TOKEN_LPAREN:
            lhs = parseExpression(tokens, 0);
            if (lhs == NULL) return NULL;

            curr_token = getToken(tokens);
            if (curr_token == NULL || curr_token->type != TOKEN_RPAREN) return NULL;
            break;
        default:
            return NULL;
    }

    // look for the operator
    while(true)
    {
        // lexer should ensure that it is an operator
        Token *operator = peekToken(tokens);
        if (operator == NULL) break;

        // if we encounter an closed paren instead of an operator simply return
        if (operator->type == TOKEN_RPAREN) break;

        int left_binding_power, right_binding_power_next;
        getBindingPower(operator->type, &left_binding_power, &right_binding_power_next);

        if (left_binding_power <= right_binding_power) break;

        getToken(tokens); // increment the index

        // parse the right hand side
        AstNode *rhs = parseExpression(tokens, right_binding_power_next);
        // We have an operator but no other number -> error
        if (rhs == NULL) return NULL;

        lhs = newNode(operator->type, 0, lhs, rhs);
    }
    
    return lhs;
}

// not functional because of the identifier
int calculate(AstNode *node)
{
    if (!node) return 0;

    switch (node->token.type)
    {
        case TOKEN_NUMBER:
            return node->token.number;

        case TOKEN_PLUS:
            return calculate(node->left) + calculate(node->right);
        case TOKEN_MINUS:
            if (node->left)
            {
                return calculate(node->left) - calculate(node->right);
            }
            return -calculate(node->right);

        case TOKEN_STAR:
            return calculate(node->left) * calculate(node->right);
        case TOKEN_SLASH:
            return calculate(node->left) / calculate(node->right);

        case TOKEN_SHL:
            return calculate(node->left) << calculate(node->right);
        case TOKEN_SHR:
            return calculate(node->left) >> calculate(node->right);

        case TOKEN_AND:
            return calculate(node->left) & calculate(node->right);
        case TOKEN_OR:
            return calculate(node->left) | calculate(node->right);
        case TOKEN_XOR:
            return calculate(node->left) ^ calculate(node->right);

        case TOKEN_NOT:
            return ~calculate(node->right);

        default:
            return 0;
    }
}

void freeAst(AstNode *node)
{
    if (node == NULL) return;

    freeAst(node->left);
    freeAst(node->right);
    free(node);
}

int calculateExpression(int *result, TokenList *tokens)
{
    AstNode *ast = parseExpression(tokens, 0);
    if (ast == NULL) return -1;

    *result = calculate(ast);
    return 0;
}