#ifndef DIRECTIVE_DEFINITION_H
#define DIRECTIVE_DEFINITION_H

#include "../expression_parser/expression_parser.h"

#define MAX_DIRECTIVE_OPERANDS 2

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

typedef enum{
    DIRECTIVE_OPERAND_NUMBER,
    DIRECTIVE_OPERAND_NUMBER_ARRAY,
    DIRECTIVE_OPERAND_IDENTIFIER,
    DIRECTIVE_OPERAND_STRING,
    DIRECTIVE_OPERAND_EXPRESSION,
    DIRECTIVE_OPERAND_EXPRESSION_ARRAY
}DirectiveOperandType;

typedef struct
{
    DirectiveOperandType type;
    union
    {
        int number;
        int *number_array;
        char *string;
        char *identifier;
        AstNode *expression;
        AstNode **expression_array;
    };
}DirectiveOperand;

typedef struct
{
    DirectiveType type;
    DirectiveOperand operands[MAX_DIRECTIVE_OPERANDS];
}Directive;

typedef struct
{
    int count;
    DirectiveOperandType operand_types[MAX_DIRECTIVE_OPERANDS];
}DirectiveForm;

const DirectiveForm directive_definition[DIRECTIVE_ALIGN + 1] = 
{
    [DIRECTIVE_SET] = {
        .operand_types = {DIRECTIVE_OPERAND_IDENTIFIER, DIRECTIVE_OPERAND_NUMBER},
        .count = 2
    },

    [DIRECTIVE_INCLUDE] = {
        .operand_types = {DIRECTIVE_OPERAND_STRING},
        .count = 1
    },

    // [DIRECTIVE_MACRO] = {
    //     .operand_types = {DIRECTIVE_IDENTIFIER, DIRECTIVE_NUMBER},
    //     .count = 2
    // },

    // [DIRECTIVE_END_MACRO] = {
    //     .operand_types = {},
    //     .count = 0
    // },

    [DIRECTIVE_ORG] = {
        .operand_types = {DIRECTIVE_OPERAND_NUMBER},
        .count = 1
    },

    [DIRECTIVE_BYTE] = {
        .operand_types = {DIRECTIVE_OPERAND_NUMBER_ARRAY},
        .count = 1
    },

    [DIRECTIVE_WORD] = {
        .operand_types = {DIRECTIVE_OPERAND_NUMBER_ARRAY},
        .count = 1
    },

    [DIRECTIVE_FILL] = {
        .operand_types = {DIRECTIVE_OPERAND_NUMBER, DIRECTIVE_OPERAND_NUMBER},
        .count = 2
    },

    [DIRECTIVE_ALIGN] = {
        .operand_types = {DIRECTIVE_OPERAND_NUMBER},
        .count = 1
    },
};

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

#endif // DIRECTIVE_DEFINITION_H