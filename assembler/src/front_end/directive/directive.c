#include "directive.h"
#include "../lexer/lexer.h"
#include <string.h>

DirectiveType getDirectiveFromToken(TokenList *tokens)
{
    Token token = *getToken(tokens);

    for (int i = 0; i < sizeof(text_to_directive) / sizeof(TextToDirective); i++)
    {
        if (strcmp(text_to_directive[i].text, token.str) == 0)
        {
            return text_to_directive[i].type;
        }
    }

    return DIRECTIVE_ERROR;
}