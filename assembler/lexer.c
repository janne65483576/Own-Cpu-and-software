#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <strings.h>

#define MAX_MNEMONIC_LEN 10
#define MAX_OPPERNAD_LEN 20 // + 1 because null termination
#define MAX_OPERAND 3

typedef enum
{
    NOP, ADDr, SUBr, ADD, SUB, JMP, MOVr, MOVe, ST, LD // MOVe(xtended)
} opcodes;

typedef enum
{
    REGISTER, MAR, ADDR_16, IMM_8, ADDR_8, LABLE
} operand_type;

typedef struct{
    int16_t value;
    operand_type op_type;
    char *lable; // only used if operand_type == LABLE
} operand;

typedef struct{
    opcodes opcode;
    uint8_t adress_mode;
    operand op_1;
    operand op_2;

    uint16_t adress;
    uint16_t imm_adress;
} instruction;

typedef struct{
    char *mnemonic;
    char *op_1;
    char *op_2;
} tmp_instruction;

void print_instruction(const instruction *instr) {
    if (!instr) {
        printf("Error: instruction pointer is NULL!\n");
        return;
    }

    printf("Instruction:\n");
    printf("  Opcode       : 0x%02X (%u)\n", instr->opcode, instr->opcode);
    printf("  Address Mode : 0x%02X (%u)\n", instr->adress_mode, instr->adress_mode);
    printf("  Register 1   : 0x%02X (%u)\n", instr->op_1.value, instr->op_1.value);
    printf("  Register 2   : 0x%02X (%u)\n", instr->op_2.value, instr->op_2.value);
    printf("  Address      : 0x%04X (%u)\n", instr->adress, instr->adress);
    printf("  Imm Address  : 0x%04X (%u)\n", instr->imm_adress, instr->imm_adress);
}

uint8_t mnemonic_to_opcode(char *mnemonic, bool is_register)
{   
    // TODO: use a better algorithm 
    if (strcmp(mnemonic, "NOP"))
    {
        return NOP;
    }else if (strcmp(mnemonic, "ADD"))
    {
        return is_register ? ADDr : ADD;
    }else if (strcmp(mnemonic, "JMP") || strcmp(mnemonic, "JC") || strcmp(mnemonic, "JZ"))
    {
        return JMP;
    }else if (strcmp(mnemonic, "ST")) {
        return ST;
    }else if (strcmp(mnemonic, "LD")) {
        return LD;
    }else if (strcmp(mnemonic, "MOV")) {
        return MOVr;
    }
    printf("Invalid mnemonic %s", mnemonic);
    exit(EXIT_FAILURE);
}

void parse_operand(operand *operand_struct, char *operand_text)
{   
    char *endptr;

    // check if this could be a register
    if ((operand_text[0] == 'r') && (operand_text[1] >= '0') && (operand_text[1] <= '9'))
    {
        long reg = strtol(operand_text + 1, &endptr, 10);

        if (*endptr != '\0')
        {
            printf("Invlaid operand: %s\n", operand_text);
            exit(EXIT_FAILURE);
        }

        if (reg < 0 || reg > 4)
        {
            printf("Too small or to big register: r%ld\n", reg);
            exit(EXIT_FAILURE);
        }
        
        operand_struct->op_type = REGISTER;
        operand_struct->value = (int16_t)reg;
    }
}

void parse_instruction(instruction *instruction, char *instruction_text)
{
    char *operands[MAX_OPERAND];
    int operand_count = 0;

    operands[0] = strtok(instruction_text, " ,");

    for (int i = 1; i < MAX_OPERAND; i++)
    {
        operands[i] = strtok(NULL, " ,");
        if(operands[i] == NULL)
        {
            break;
        }
        operand_count++;
    }

    // printf("mnemonic : %s\n", operands[0]);

    // printf("first operand : %s\n", operands[1]);
    // printf("second operand : %s\n", operands[2]);

    // printf("operands : %d\n", operand_count);
    operand operand_struct;
    parse_operand(&operand_struct, operands[1]);
    printf("%d", operand_struct.value);
    if (operand_struct.op_type == REGISTER)
    {
        printf("valid register\n");
    }

}

typedef enum {
    STATE_CODE,          // normal code
    STATE_ONE_COMMENT,   // in singel line comment
    STATE_MULTI_COMMENT, // int multi line comment
    STATE_MULTI_END      // the first char of end seqeuz "/*" is found
} ParserState;

char *lexer(char *assembly, long asm_size, long *tokens_size)
{   
    char instruction_text[] = "INC r";

    instruction instruction_struct;
    parse_instruction(&instruction_struct, instruction_text);

    //print_instruction(&instruction_struct);


    // char *tokens = (char *)malloc(asm_size * sizeof(char));
    // long token_ptr = 0;
    // char c;

    // ParserState state = STATE_CODE;

    // for (long i = 0; i < asm_size; i++) {
    //     c = assembly[i];

    //     switch (state) {
    //         case STATE_ONE_COMMENT:
    //             if (c == '\n') {
    //                 state = STATE_CODE;
    //             }
    //             continue;

    //         case STATE_MULTI_COMMENT:
    //             if (c == '*') {
    //                 state = STATE_MULTI_END;
    //             }
    //             continue;

    //         case STATE_MULTI_END:
    //             if (c == '/') {
    //                 state = STATE_CODE;
    //             } else if (c != '*') {
    //                 state = STATE_MULTI_COMMENT;
    //             }
    //             continue;

    //         case STATE_CODE:
    //             // start singel line comment
    //             if (c == '/' && i + 1 < asm_size && assembly[i + 1] == '/') {
    //                 state = STATE_ONE_COMMENT;
    //                 i++;
    //                 continue;
    //             }

    //             // start multi line comment
    //             if (c == '/' && i + 1 < asm_size && assembly[i + 1] == '*') {
    //                 state = STATE_MULTI_COMMENT;
    //                 i++;
    //                 continue;
    //             }

    //             tokens[token_ptr++] = c;
    //             break;
    //     }
    // }
    // tokens[token_ptr++] = '\n';
    // tokens = realloc(tokens, token_ptr);
    // *tokens_size = token_ptr;
    // return tokens;
    return NULL;
    *tokens_size = 0;
}