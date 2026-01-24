#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "../lexer/token.h"
#include "instruction_definition.h"

Register16Bit getRegisterPair(Token *register_1, Token *register_2);
int parseInstruction(TokenList *tokens, Instruction *instruction);
int getMnemonicFromToken(TokenList *tokens, Instruction *instruction);

#endif // INSTRUCTION_H