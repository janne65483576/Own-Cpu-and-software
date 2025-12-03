#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <strings.h>

#include "instruction.h"
#include "mnemonic.h"
#include "label.h"

#define MAX_MNEMONIC_LEN 10
#define MAX_OPPERAND_LEN 20 // + 1 because null termination
#define MAX_OPERAND 3

#define MAX_INSTRUCTIONS 100

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

    printf("opcode: %x\n", instr->opcode);
    putc('\n', stdout);
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
    //printf("address: %d\n", instr->address);
    puts("------------------");
}

void parse_operand(operand *operand_struct, char *operand_text)
{   
    if(strcasecmp(operand_text, "MAR") == 0)
    {
        operand_struct->op_type = MAR;
        return;
    }

    if(strcasecmp(operand_text, "c") == 0)
    {
        operand_struct->op_type = FLAG;
        operand_struct->value = 1;
        return;
    }

    if(strcasecmp(operand_text, "z") == 0)
    {
        operand_struct->op_type = FLAG;
        operand_struct->value = 2;
        return;
    }

    if(strcasecmp(operand_text, "v") == 0)
    {
        operand_struct->op_type = FLAG;
        operand_struct->value = 3;
        return;
    }

    char *endptr;

    // check if this could be a register
    if ((operand_text[0] == 'r') && (operand_text[1] >= '0') && (operand_text[1] <= '9'))
    {
        long reg = strtol(operand_text + 1, &endptr, 10);

        if (*endptr != '\0')
        {
            printf("Invalid operand: %s\n", operand_text);
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
    bool is_address = false;
    int start_off = 0;
    long val;

    if (operand_text[0] == '%')
    {
        start_off++;
        is_address = true;
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
            printf("invalid operand: %s\n", operand_text);
            exit(EXIT_FAILURE);
        }

        operand_struct->op_type = LABEL;
        operand_struct->label = strdup(operand_text);
        return;
    }

    if (val < INT16_MIN || val > UINT16_MAX)
    {
        printf("Too small or to big number: %lx\n", val);
        exit(EXIT_FAILURE);
    }

    if (val > INT8_MIN && val < INT8_MAX)
    {
        operand_struct->op_type = is_address ? ADDR_8 : IMM_8;
        operand_struct->value = val & 0xff;
        return;
    }
    
    if ((val < INT8_MIN || val > UINT8_MAX) && !is_address) {
        printf("A immediate value which isent an andress can not be greater or smaller than 8 bits. Value: %ld\n", val);
        exit(EXIT_FAILURE);
    }
    
    operand_struct->op_type = ADDR_16;
    operand_struct->value = val & 0xffff;
}

char **get_operands(char *instruction_text, char **operands)
{
    operands[0] = strtok(instruction_text, " ,");
    operands[1] = NULL;
    operands[2] = NULL;

    for (int i = 1; i < MAX_OPERAND; i++)
    {
        operands[i] = strtok(NULL, " ,");
        if(operands[i] == NULL)
        {
            break;
        }
    }

    return operands;
}

// converts the instructions structs to binary format by inserting the opcodes and the immediate/addresses
uint8_t *convert_to_bin(instruction *instructions, int inst_count, long *bin_size_final){
    int bin_i = 0;

    // use MAX_INSTRUCTIONS is not very good some instructions have 2 or 3 bytes
    uint8_t *bin = (uint8_t *)malloc(MAX_INSTRUCTIONS);
    int max_bin_size = MAX_INSTRUCTIONS;
    int bin_size = 0;

    for (int inst_i = 0; inst_i < inst_count; inst_i++)
    {
        print_instruction(&instructions[inst_i]);
        // dynamicly manage the array
        if (inst_i + 2 >= max_bin_size)
        {
            max_bin_size += MAX_INSTRUCTIONS;
            bin = realloc(bin, max_bin_size);
        }
        
        // write the opcode
        bin[bin_i++] = instructions[inst_i].opcode;

        if(instructions[inst_i].op_2.op_type == ADDR_8 || instructions[inst_i].op_2.op_type == IMM_8)
        {
            bin_size += 2;
            bin[bin_i++] = instructions[inst_i].op_2.value;
            continue;
        } else if (instructions[inst_i].op_1.op_type == ADDR_8 || instructions[inst_i].op_1.op_type == IMM_8)
        {
            bin_size += 2;
            bin[bin_i++] = instructions[inst_i].op_1.value;
            continue;
        }

        if(instructions[inst_i].op_2.op_type == ADDR_16)
        {
            bin_size += 3;
            printf("%d\n", instructions[inst_i].op_2.value);
            // store in little endian
            bin[bin_i++] = instructions[inst_i].op_2.value;
            bin[bin_i++] = instructions[inst_i].op_2.value >> 8;
            continue;
        } else if (instructions[inst_i].op_1.op_type == ADDR_16)
        {
            bin_size += 3;
            // store in little endian
            bin[bin_i++] = instructions[inst_i].op_1.value;
            bin[bin_i++] = instructions[inst_i].op_1.value >> 8;
            continue;
        }

        bin_size++;
    }

    *bin_size_final = bin_size;
    return bin;
}

uint8_t *get_bin(FILE *assembly_file, long *bin_size)
{
    instruction instructions[MAX_INSTRUCTIONS];
    int instruction_i = 0;
    size_t len = 0;
    char *line = NULL;
    int nread = 0;

    // the label list struct ist globally defined and is used around the whole assembler
    //init_label_list();

    // read in the labels and instructions but not translate them yet
    while ((nread = getline(&line, &len, assembly_file)) != -1)
    {
        // replace the end of line with a null terminator
        if (nread > 0 && line[nread - 1] == '\n') {
            line[nread - 1] = '\0';
        }
        
        if (line[0] == '\0')
        {
            goto skip;
        }

        // check if directive
        for(int i = 0; line[i] != '\0'; i++)
        {
            if(line[i] == '.')
            {
                // directive
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
            printf("The MAX_instructions limit is to less.");
            exit(EXIT_FAILURE);
        }

        //printf("line: %s\n", line);
        char *operands[MAX_OPERAND];
        get_operands(line, operands);

        instruction curr_instruction;

        curr_instruction.op_1.op_type = UNUSED;
        curr_instruction.op_2.op_type = UNUSED;
        // The instruction mnemonic is saved for later translation 
        curr_instruction.mnemonic_text = strdup(operands[0]);
 
        if (operands[1] != NULL)
        {
            parse_operand(&curr_instruction.op_1, operands[1]);

            if (operands[2] != NULL)
            {
                parse_operand(&curr_instruction.op_2, operands[2]);
            }
        }

        // if (is_label(operands[0]))
        // {
        //     if (curr_instruction.op_1.op_type == UNUSED && curr_instruction.op_2.op_type == UNUSED)
        //     {
        //         add_label(operands[0], instruction_i);
        //         goto skip;
        //     }
        //     else {
        //         printf("A label can not have operands.\n");
        //         exit(EXIT_FAILURE);
        //     }
        // }

        instructions[instruction_i++] = curr_instruction;

        skip:
            free(line);
            line = NULL;
    }

    // iterate over the instructions and translate them to opcodes
    // instruction_i is the count of instructions
    for (int i = 0; i < instruction_i; i++)
    {
        // save the ptr to free later
        char *mnemonic_text = instructions[i].mnemonic_text;
        // the function exist if an error occurs
        parse_mnemonic(mnemonic_text, &instructions[i]);
        free(mnemonic_text);
    }

    return convert_to_bin(instructions, instruction_i, bin_size);
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
    
    FILE *bin_file = fopen("prog.bin", "wb");
    fwrite(bin, sizeof(uint8_t), bin_size, bin_file);

    free(bin);
    fclose(assembly);

    return 0;
}