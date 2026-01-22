#ifndef PARSER_H
#define PARSER_H

#include "../instruction.h"
#include "../instruction_definition.h"
#include "../lexer/lexer.h"

typedef struct
{
    Instruction *list;
    int count;
    int capacity;
}InstructionList;

int parseInstructions(InstructionList *instructions, TokenList *tokens);

#endif // PARSER_H