#include <stdlib.h>
#include <string.h>

#include "../instruction.h"
#include "../lexer/lexer.h"
#include "parser.h"
#include "../instruction_definition.h"

void addInstruction(InstructionList *instructions, Instruction *instruction)
{
    if(instructions->capacity < instructions->count)
    {
        instructions->capacity *= 2;
        instructions->list = realloc(instructions->list, instructions->capacity * sizeof(Instruction));
    }

    instructions->list[instructions->count] = *instruction;
}

int strCmp(const void *a, const void *b) 
{
    return strcmp((const char *)a, *(const char **)b);
}

int getMnemonicFromToken(TokenList *tokens, Instruction *instruction)
{    
    char **result = bsearch(getToken(tokens)->str, MnemonicStr, sizeof(MnemonicStr) / sizeof(MnemonicStr[0]), sizeof(MnemonicStr), strCmp);
    
    if (result == NULL)
    {
        return -1;
    }
    
    instruction->mnemonic = (Mnemonic)(result - (char **)MnemonicStr);

    return 0;
}

Register16Bit getRegisterPair(Token *register_1, Token *register_2)
{
    if (register_1->reg == 0 && register_2->reg == 1)
    {
        return R0_R1;
    }
    if (register_1->reg == 0 && register_2->reg == 2)
    {
        return R0_R2;
    }
    if (register_1->reg == 0 && register_2->reg == 3)
    {
        return R0_R3;
    }
    if (register_1->reg == 1 && register_2->reg == 0)
    {
        return R1_R0;
    }
    if (register_1->reg == 1 && register_2->reg == 2)
    {
        return R1_R2;
    }
    if (register_1->reg == 1 && register_2->reg == 3)
    {
        return R1_R3;
    }
    if (register_1->reg == 2 && register_2->reg == 0)
    {
        return R2_R0;
    }
    if (register_1->reg == 2 && register_2->reg == 1)
    {
        return R2_R1;
    }
    if (register_1->reg == 2 && register_2->reg == 3)
    {
        return R2_R3;
    }
    if (register_1->reg == 3 && register_2->reg == 0)
    {
        return R3_R0;
    }
    if (register_1->reg == 3 && register_2->reg == 1)
    {
        return R3_R1;
    }
    if (register_1->reg == 3 && register_2->reg == 2)
    {
        return R3_R2;
    }
    if (register_1->reg == register_2->reg)
    {
        return -1;
    }
}

int parseInstruction(TokenList *tokens, Instruction *instruction)
{
    Token *curr_token = getToken(tokens);
    if (curr_token == NULL) return -1;

    switch (curr_token->type) {
        
        case TOKEN_REGISTER:
            // check if this could be a register pair (r0:r1)
            if(peekToken(tokens) != NULL && peekToken(tokens)->type == TOKEN_COLON)
            {
                getToken(tokens);
                if(peekToken(tokens) != NULL && peekToken(tokens)->type == TOKEN_REGISTER)
                {
                    instruction->operands.list[0].register_16 = getRegisterPair(curr_token, getToken(tokens));
                    instruction->operands.list[0].kind = OPERAND_REGISTER_16;
                    break;
                }else 
                {
                    return -1;
                }
            }
            instruction->operands.list[0].kind = OPERAND_GPR;
            instruction->operands.list[0].gpr  = (GPRegister)curr_token->reg;
            break;
        
        case TOKEN_NUMBER:
            instruction->
            
            
    }
}

int parseTokens(InstructionList *instructions, TokenList *tokens)
{
    Token *curr_token;
    
    while(curr_token)
    {
        switch(curr_token->type)
        {
            case TOKEN_LABEL:                
            break;
            
            case TOKEN_DIRECTIVE:
            break;
            
            case TOKEN_STRING:
            // check if the string is an instruction
            break;
            
        }
        curr_token = getToken(tokens);
    }
}