#include <cstdlib>
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

// real opcodes 
// OOI = one operand instruction
typedef enum
{
    OOI, ADD, ADDr, AND, ANDr, XOR, XORr, OR, ORr, SH, LD, ST, JMP, JMPn, MOV, MOVe
} opcodes;

// sorted alphabeticly
char *mnemonics[] = {"ADD", "ADDr", "AND", "ANDr", "BCC", "BCS", "BVC", "BVS", "BZC", "BZS", "CALL", "CLF", "DEC", "INC", "JMP", "LD", "MOV", "MOVe", "NOP", "NOT", "OR", "ORr", "POP", "PUSH", "RET", "SH", "ST", "XOR", "XORr"};

typedef enum
{
    ADD_mn, ADDr_mn, AND_mn, ANDr_mn, BCC_mn, BCS_mn, BVC_mn, BVS_mn, BZC_mn, BZS_mn, CALL_mn, CLF_mn, DEC_mn, INC_mn, JMP_mn, LD_mn, MOV_mn, MOVe_mn, NOP_mn, NOT_mn, OR_mn, ORr_mn, POP_mn, PUSH_mn, RET_mn, SH_mn, ST_mn, XOR_mn, XORr_mn
}mnemonic_enum;

typedef enum
{
    REGISTER, MAR, ADDR_16, IMM_8, ADDR_8, LABLE, UNUSED
} operand_type;

typedef struct{
    uint8_t register_;
    uint8_t adress;
}translation;

const translation translation_table_arr[] =
{
    [0] = {ADD, ADDr}, // ADD_mn
    [2] = {AND, ANDr}, // AND_mn
    [27] = {XOR, XORr}, // XOR_mn
    [20] = {OR, ORr}, // OR_mn
};

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

void print_bits(uint8_t value) 
{
    for (int i = 7; i >= 0; i--) 
    {
        printf("%d", (value >> i) & 1);
    }
    printf("\n");
}

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

void arithmetic_to_opcode(int mn_index, instruction *instruction)
{
    if(instruction->op_1.op_type == REGISTER && instruction->op_2.op_type == REGISTER)
    {
        // construct corect maschine code
        instruction->opcode = translation_table_arr[mn_index].register_ | instruction->op_1.value << 4 | instruction->op_2.value << 6;
    }else if (instruction->op_1.op_type == REGISTER && instruction->op_2.op_type != REGISTER && instruction->op_2.op_type != UNUSED)
    {
        instruction->opcode = translation_table_arr[mn_index].adress | instruction->op_1.value << 4;
        
        switch (instruction->op_2.op_type) {
            // no ADDR_16 it is the base case with 0x00
            case MAR:
                instruction->opcode |= 1 << 6;
            break;
            case IMM_8:
                instruction->opcode |= 1 << 7;
            break;
            case ADDR_8:
                instruction->opcode |= 0b11 << 6;
            break;
            default:
                // this is only here to prevent compiler warnings
            break;
        }
    }else
    {
        printf("Invalid operand types for an instruction.\n");
        exit(EXIT_FAILURE);
    }
}

int cmpstr(const void *a, const void *b) {
    const char *key = (const char *)a;
    const char *elem = *(const char **)b;
    return strcmp(key, elem);
}

uint8_t mnemonic_to_opcode(char *mnemonic, instruction *instruction)
{
    if (instruction->op_1.op_type == LABLE || instruction->op_2.op_type == LABLE)
    {
        printf("A Instruction with double lable does not exist.\n");
        return 1;
    }
    
    char **result = bsearch(mnemonic, mnemonics, sizeof(mnemonics) / sizeof(mnemonics[0]), sizeof(char *), cmpstr);
    
    if(result == NULL)
    {
        // could be a lable
        exit(EXIT_FAILURE);
    }

    // get the index of the mnemonic
    switch((int)(result - mnemonics))
    {   
        case ADD_mn:
        case AND_mn:
        case XOR_mn:
        case OR_mn:
            arithmetic_to_opcode(result - mnemonics, instruction);
            break;
    }
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

        if (reg < 0 || reg > 3)
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

    mnemonic_to_opcode(operands[0], instruction);
}

uint8_t *linker(instruction *instructions, int inst_count){
    int bin_i = 0;

    // used MAX_INSTRUCTIONS is not very good some instructions hav 2 or 3 bytes
    uint8_t *bin = (uint8_t *)malloc(MAX_INSTRUCTIONS);
    int bin_size = MAX_INSTRUCTIONS;

    for (int inst_i = 0; inst_i < inst_count; inst_i++)
    {
        // dynamicly manage the array
        if (inst_i + 2 >= bin_size)
        {
            bin_size += MAX_INSTRUCTIONS;
            bin = realloc(bin, bin_size);
        }
        
        // write the opcode
        bin[bin_i++] = instructions[inst_i].opcode;
    }
    
    return NULL;
}

uint8_t *get_bin(FILE *assembly_file, long *bin_size)
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

        // check if direktive
        for(int i = 0; line[i] != '\0'; i++)
        {
            if(line[i] == '.')
            {
                //directive
                break;
            }
            
            if(line[i] != ' ')
            {
                // no directive
                break;
            }
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

    print_bits(instructions[0].opcode);

    return linker(instructions, instruction_i);
}

int check_if_asm_file(const char *filename)
{
    char *dot = strrchr(filename, '.');
    if (dot == NULL) return 0;
    return strcmp(dot + 1, "asm");
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("To many or to little arguments provided.");
        return 1;
    }

    if (check_if_asm_file(argv[1]))
    {
        printf("Either the file extension is not 'bin' or the file dont have an extension.");
        return 1;
    }

    FILE *assembly = fopen(argv[1], "rb");

    long bin_size;
    uint8_t *bin = get_bin(assembly, &bin_size);
    
    fclose(assembly);

    return 0;
}