#ifndef PARSER_H
#define PARSER_H

#include "../instruction/instruction_definition.h"
#include "../directive/directive_definition.h"
#include "../lexer/token.h"

typedef enum{
    SYMBOL_LABEL,
    SYMBOL_SET
}SymbolType;

typedef struct
{
    SymbolType type;
    char *identifier;
    union
    {
        int value;
        int address;
    };
}Symbol;

typedef struct
{
    Symbol *list;
    int capacity;
    int count;
}SymbolList;

typedef enum
{
    INTER_INSTRUCTION,
    INTER_DIRECTIVE,
}IntermediateType;

typedef struct
{
    IntermediateType type;
    int length;
    union
    {
        Directive directive;
        Instruction instruction;
    };
}IntermediateRepresentation;

typedef struct
{
    IntermediateRepresentation *list;
    int capacity;
    int count;
}IntermediateRepresentationList;

int parseTokens(IntermediateRepresentationList *intermediate_list, SymbolList *symbol_list, TokenList *tokens);
int resolveExpressions(IntermediateRepresentationList *intermediate_list, SymbolList *symbol_list);
int resolveLabels(IntermediateRepresentationList *intermediate_list, SymbolList *symbol_list);

#endif // PARSER_H