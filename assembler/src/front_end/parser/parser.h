#ifndef PARSER_H
#define PARSER_H

#include "../instruction.h"
#include "../directive/directive.h"
#include "../lexer/lexer.h"

typedef struct
{
    Instruction *list;
    int count;
    int capacity;
}InstructionList;

typedef struct
{
    
}Symbol;

typedef struct
{
    
}SymbolList;

typedef enum
{
    INTER_INSTRUCTION,
    INTER_DIRECTIVE, // no .equ
}IntermediateType;

typedef struct
{
    IntermediateType type;
    union
    {
        DirectiveIntermediate directive;

    };
}IntermediateRepresentation;

typedef struct
{
    IntermediateRepresentation *list;
    int capacity;
    int count;
}IntermediateRepresentationList;


int parseTokens(InstructionList *instructions, TokenList *tokens);

#endif // PARSER_H