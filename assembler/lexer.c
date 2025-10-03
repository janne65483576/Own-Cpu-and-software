#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <strings.h>

#define MAX_MNEMONIC_LEN 10
#define MAX_OPPERNAD_LEN 20 // + 1 because null termination
#define MAX_OPERAND 3

#define MAX_INSTRUCTIONS 100

typedef enum
{
    NOP, ADD, AND, XOR, SHR, OR, JMP, BCC, BCS, BZC, BZS, BVS, BVC, MOV, ST, LD, CALL, RET, POP, PUSH
} opcodes;

typedef enum
{
    REGISTER, MAR, ADDR_16, IMM_8, ADDR_8, LABLE, UNUSED
} operand_type;

typedef struct{
    int16_t value;
    operand_type op_type;
    char *lable; // only used if operand_type == LABLE
} operand;

typedef struct{
    opcodes opcode;
    operand op_1;
    operand op_2;

    uint16_t adress;
} instruction;

void print_instruction(const instruction *instr) {
    if (!instr) {
        printf("Error: instruction pointer is NULL!\n");
        return;
    }

    //printf("opcode: %x\n", instr->opcode);
    //putc('\n', stdout);
    if (instr->op_1.op_type != UNUSED)
    {
        printf("first operand:\n");
        printf("value: %d\n", instr->op_1.value);
        printf("operand_type: %d\n", instr->op_1.op_type);
    }
    putc('\n', stdout);
    if (instr->op_2.op_type != UNUSED)
    {
        printf("second operand:\n");
        printf("value: %d\n", instr->op_2.value);
        printf("operand_type: %d\n", instr->op_2.op_type);
    }
    //printf("adress: %d\n", instr->adress);
    puts("------------------");
}

uint8_t mnemonic_to_opcode(char *mnemonic)
{   
    // TODO Use an temporary encoding scheme
    return 0;
}

void parse_operand(operand *operand_struct, char *operand_text)
{   
    if(strcasecmp(operand_text, "MAR") == 0)
    {
        operand_struct->op_type = MAR;
        return;
    }

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
        return;
    }

    int base = 10;
    bool is_adress = false;
    int start_off = 0;
    long val;

    if (operand_text[0] == '%')
    {
        start_off++;
        is_adress = true;
    }
    
    if (operand_text[start_off] == '#')
    {
        base = 16;
        start_off++;
    }

    val = strtol(operand_text + start_off, &endptr, base);

    if (*endptr != '\0')
    {
        if (base == 16)
        {
            printf("Invlaid operand: %s\n", operand_text);
            exit(EXIT_FAILURE);
        }

        operand_struct->op_type = LABLE;

        int len = strlen(operand_text);
        operand_struct->lable = (char *)malloc(len);
        memcpy(operand_struct->lable, operand_text + 1, len);
        return;
    }

    if (val < INT16_MIN || val > UINT16_MAX)
    {
        printf("Too small or to big number: %lx\n", val);
        exit(EXIT_FAILURE);
    }

    if (val > INT8_MIN && val < INT8_MAX)
    {
        operand_struct->op_type = is_adress ? ADDR_8 : IMM_8;
        operand_struct->value = val & 0xff;
        return;
    }
    
    if ((val < INT8_MIN || val > UINT8_MAX) && !is_adress) {
        printf("A Imidiate value which isent an andress can not be greater or smaller than 8 bits. Value: %ld\n", val);
        exit(EXIT_FAILURE);
    }
    
    operand_struct->op_type = ADDR_16;
    operand_struct->value = val & 0xffff;
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

    instruction->opcode = mnemonic_to_opcode(operands[0]);
    instruction->op_1.op_type = UNUSED;
    instruction->op_2.op_type = UNUSED;

    if (operand_count >= 1) 
    {
        parse_operand(&instruction->op_1, operands[1]);
        if (operand_count >= 2)
        {
            parse_operand(&instruction->op_2, operands[2]);
        }
    }
}

void resolve_lables(instruction *instructions)
{

}

char *convert_instructions_binary(instruction *instructions)
{
    return NULL;
}

char *lexer(FILE *assembly_file, long *tokens_size)
{   
    instruction instructions[MAX_INSTRUCTIONS];
    int instruction_i = 0;
    size_t len = 0;
    char *line = NULL;
    int nread = 0;

    while ((nread = getline(&line, &len, assembly_file)) != -1)
    {
        if (nread > 0 && line[nread - 1] == '\n') {
            line[nread - 1] = '\0';
        }
        
        if (line[0] == '\0')
        {
            goto skip;
        }

        if (instruction_i >= MAX_INSTRUCTIONS)
        {
            printf("The MAX_INTRUCTIONS limit is set to low.");
            free(line);
            exit(EXIT_FAILURE);
        }

        printf("line: %s\n", line);

        instruction curr_instruction;
        parse_instruction(&curr_instruction, line);

        instructions[instruction_i++] = curr_instruction;

        skip:
            free(line);
            line = NULL;
    }

    void resolve_lables(instructions);

    char *convert_instructions_binary(instructions);

    *tokens_size = 0;
    return NULL;
}