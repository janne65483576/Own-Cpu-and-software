#ifndef DIRECTIVE_H
#define DIRECTIVE_H

#include "../lexer/lexer.h"
#include "../expression_parser/expression_parser.h"

#define MAX_MACRO_LEN = 10

typedef enum{
    DIRECTIVE_ERROR,
    DIRECTIVE_SET,
    DIRECTIVE_INCLUDE,
    DIRECTIVE_MACRO,
    DIRECTIVE_END_MACRO,
    DIRECTIVE_ORG,
    DIRECTIVE_BYTE,
    DIRECTIVE_WORD,
    DIRECTIVE_FILL,
    DIRECTIVE_ALIGN,
}DirectiveType;

typedef struct
{
    DirectiveType type;
    AstNode operands[2];
}DirectiveIntermediate;

typedef struct
{
    char text[DIRECTIVE_ALIGN + 1];
    DirectiveType type;
}TextToDirective;

const TextToDirective text_to_directive[] = 
{
    {.text = "",         .type = DIRECTIVE_ERROR},
    {.text = ".set",     .type = DIRECTIVE_SET},
    {.text = ".include", .type = DIRECTIVE_INCLUDE},
    {.text = ".macro",   .type = DIRECTIVE_MACRO},
    {.text = ".emacro",  .type = DIRECTIVE_END_MACRO},
    {.text = ".org",     .type = DIRECTIVE_ORG},
    {.text = ".byte",    .type = DIRECTIVE_BYTE},
    {.text = ".word",    .type = DIRECTIVE_WORD},
    {.text = ".fill",    .type = DIRECTIVE_FILL},
    {.text = ".align",   .type = DIRECTIVE_ALIGN},
};

DirectiveType getDirectiveFromToken(TokenList *tokens);

#endif // DIRECTIVE_H