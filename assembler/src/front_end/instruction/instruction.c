#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "instruction_definition.h"
#include "../lexer/lexer.h"
#include "../expression_parser/expression_parser.h"
#include "../expression_parser/expression_definition.h"


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

Flag getFlagFromToken(Token *token)
{
    switch (token->type)
    {
        case TOKEN_ZERO_FLAG:
            return FLAG_ZERO;
        case TOKEN_CARRY_FLAG:
            return FLAG_CARRY;
        case TOKEN_SIGN_FLAG:
            return FLAG_SIGN;
        default:
            printf("Provided Token is not a Flag.\n");
            return -1;
    }
}

static inline TokenType peekNextTokenType(TokenList *tokens)
{
    Token *token = peekToken(tokens);
    if (token == NULL) return TOKEN_EOF;
    return token->type;
}

int parseInstruction(TokenList *tokens, Instruction *instruction)
{
    int expected_operands = 0;

    for (int i = 0; i < MAX_OPERANDS; i++) {
        Token *curr_token = getToken(tokens);
        if (!curr_token) break;

        // parse operand
        switch (curr_token->type) {
            case TOKEN_REGISTER:
                // GPR oder Registerpair
                break;
            case TOKEN_NUMBER:
            case TOKEN_MINUS:
            case TOKEN_NOT:
                instruction->operand.list[i].kind = INSTRUCTION_OPERAND_EXPRESSION;
                instruction->operand.list[i].expression = parseExpression(tokens, 0);
                break;
            case TOKEN_ZERO_FLAG:
            case TOKEN_CARRY_FLAG:
            case TOKEN_SIGN_FLAG:
                instruction->operand.list[i].kind = INSTRUCTION_OPERAND_FLAG;
                instruction->operand.list[i].flag = getFlagFromToken(curr_token);
                break;
            default:
                printf("Invalid operand token\n");
                return -1;
        }

        Token *next = peekToken(tokens);
        if (next && next->type == TOKEN_COMMA) {
            getToken(tokens);
            expected_operands++;
        } else {
            break;
        }
    }
}