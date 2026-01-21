#include <stdio.h>
#include "lexer.h"

const char *token_type_to_string(TokenType type)
{
    switch (type)
    {
        case TOKEN_LABEL:     return "LABEL";
        case TOKEN_DIRECTIVE: return "DIRECTIVE";
        case TOKEN_REGISTER:  return "REGISTER";
        case TOKEN_PERCENT:   return "PERCENT";
        case TOKEN_NEWLINE:   return "NEWLINE";
        case TOKEN_COMMA:     return "COMMA";
        case TOKEN_LPAREN:    return "LPAREN";
        case TOKEN_RPAREN:    return "RPAREN";
        case TOKEN_PLUS:      return "PLUS";
        case TOKEN_MINUS:     return "MINUS";
        case TOKEN_STAR:      return "STAR";
        case TOKEN_SLASH:     return "SLASH";
        case TOKEN_SHL:       return "SHL";
        case TOKEN_SHR:       return "SHR";
        case TOKEN_AND:       return "AND";
        case TOKEN_OR:        return "OR";
        case TOKEN_XOR:       return "XOR";
        case TOKEN_NOT:       return "NOT";
        case TOKEN_NUMBER:    return "NUMBER";
        case TOKEN_STRING:    return "STRING";
        default:        return "UNKNOWN";
    }
}

const char *register_to_string(TokenRegister reg)
{
    switch (reg)
    {
        case Token_R0:    return "R0";
        case Token_R1:    return "R1";
        case Token_R2:    return "R2";
        case Token_R3:    return "R3";
        case Token_SP:    return "SP";
        case Token_MAR:   return "MAR";
        case Token_MBR:   return "MBR";
        case Token_FLAGS: return "FLAGS";
        default:    return "UNKNOWN_REG";
    }
}

void print_token(const Token *token)
{
    if (!token)
    {
        printf("Token: (null)\n");
        return;
    }

    printf("Token { type=%s", token_type_to_string(token->type));

    switch (token->type)
    {
        case TOKEN_REGISTER:
            printf(", reg=%s", register_to_string(token->reg));
            break;

        case TOKEN_NUMBER:
            printf(", number=%llu", (unsigned long long)token->number);
            break;

        case TOKEN_STRING:
            printf(", string=\"");
            if (token->str)
            {
                printf("%s", token->str);
            }
            else
                printf("(null)");
            printf("\"");
            break;
        
        case TOKEN_LABEL:
            printf(", label=\"");
            if (token->str)
            {
                printf("%s", token->str);
            }
            else
                printf("(null)");
            printf("\"");
            break;
        
        case TOKEN_DIRECTIVE:
            printf(", directive=\"");
            if (token->str)
            {
                printf("%s", token->str);
            }
            else
                printf("(null)");
            printf("\"");
            break;

        default:
            break;
    }

    printf(" }\n");
}

void printTokenList(TokenList *tokens)
{
    for (int i = 0; i < tokens->count; i++)
    {
        print_token(&tokens->list[i]);
    }
}

void printTokenMinimal(Token *token)
{
    printf("type=%s", token_type_to_string(token->type));

    switch (token->type)
    {
        case TOKEN_REGISTER:
            printf("|| reg=%s", register_to_string(token->reg));
            break;

        case TOKEN_NUMBER:
            printf("|| number=%llu", (unsigned long long)token->number);
            break;

        case TOKEN_STRING:
            printf("|| string=%s", token->str ? token->str : "(null)");
            break;
        
        case TOKEN_LABEL:
            printf("|| label=%s", token->str ? token->str : "(null)");
            break;
        
        case TOKEN_DIRECTIVE:
            printf("directive=%s", token->str ? token->str : "(null)");
        break;

        default:
            break;
    }

    printf("\n");
}