#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <strings.h>
#include <sys/types.h>

#include "instruction.h"
#include "mnemonic.h"
#include "label.h"

#define MAX_INSTRUCTIONS 100

void print_bits(uint8_t value) 
{
    for (int i = 7; i >= 0; i--) 
    {
        printf("%d", (value >> i) & 1);
    }
    printf("\n");
}

void print_instruction(const instruction_t *instr) {
    if (!instr) {
        printf("Error: instruction pointer is NULL!\n");
        return;
    }

    printf("opcode: %d || ", instr->opcode);
    print_bits(instr->opcode);
    printf("\n");
    if (instr->op_1.op_type != UNUSED)
    {
        printf("first operand:\n");
        printf("\tvalue: %d\n", instr->op_1.value);
        printf("\toperand_type: %d", instr->op_1.op_type);
    }
    printf("\n");
    if (instr->op_2.op_type != UNUSED)
    {
        printf("\nsecond operand:\n");
        printf("\tvalue: %d\n", instr->op_2.value);
        printf("\toperand_type: %d", instr->op_2.op_type);
    }
    //printf("address: %d\n", instr->address);
    printf("------------------\n");
}

void print_instruction_list(instruction_list_t *instruction_list)
{
    for (int i = 0; i < instruction_list->count; i++)
    {   
        printf("%d. instruction\n", i);
        print_instruction(&instruction_list->list[i]);
    }
}

void parse_operand(operand_t *operand_struct, char *operand_text)
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

// the current instruction struct is designed to only hold 2 operands
// because the isa does not support instructions with more opperands
#define MAX_OPERAND_COUNT 2

void get_operands(char *instruction_text, char **operands)
{
    operands[0] = strtok(instruction_text, " ,");
    operands[1] = NULL;
    operands[2] = NULL;

    for (int i = 1; i < MAX_OPERAND_COUNT + 1; i++)
    {
        operands[i] = strtok(NULL, " ,");
        if(operands[i] == NULL)
        {
            break;
        }
    }
}

// converts the instructions structs to binary format by inserting the opcodes and the immediate/addresses
uint8_t *convert_to_bin(instruction_t *instructions, int inst_count, long *bin_size_final){
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

void parse_assembly_file(char *filename, instruction_list_t *instruction_list, label_list_t *label_list)
{
    size_t len = 0;
    ssize_t nread = 0;
    char *line = NULL;

    // initialise all dynamic lists
    instruction_list->list     = malloc(50 * sizeof(instruction_t));
    instruction_list->capacity = 50;
    instruction_list->count    = 0;

    label_list->list     = malloc(sizeof(label_t) * 10);
    label_list->capacity = 10;
    label_list->count    = 0;

    FILE *asm_file = fopen(filename, "rb");

    while ((nread = getline(&line, &len, asm_file)) != -1)
    {
        // replace the end of line with a null terminator
        if (nread > 0 && line[nread - 1] == '\n') {
            line[nread - 1] = '\0';
        }
        
        char *line_saved = line;

        while (*line == ' ') {
            line++;
        }

        if (*line == '\0') {
            line = line_saved;
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

        char *operands[MAX_OPERAND_COUNT];
        get_operands(line, operands);
        
        instruction_t curr_instruction = 
        {
            .op_1.op_type = UNUSED,
            .op_2.op_type = UNUSED
        };

        if (operands[1] != NULL)
        {
            parse_operand(&curr_instruction.op_1, operands[1]);

            if (operands[2] != NULL)
            {
                parse_operand(&curr_instruction.op_2, operands[2]);
            }
        }

        // instruction containing a label
        if ((curr_instruction.op_1.op_type == LABEL && curr_instruction.op_2.op_type != LABEL) || (curr_instruction.op_1.op_type != LABEL && curr_instruction.op_2.op_type == LABEL))
        {
            // could save the index of instructions having a label as one of their opperands here to prevent an extra step later 
            // but decided to NOT do ist because of added complexity

            // use an intermediate representation which saves the complete pseudo instruction
            // we know that the instruction is not ready when one of the operands has the type label
            curr_instruction.opcode = get_intermediate_mnemonic(operands[0]);
            
            if (curr_instruction.opcode == -1)
            {
                printf("Unknown mnemonic: \"%s\".\n", operands[0]);
                exit(EXIT_FAILURE);
            }
        }else
        {
            if(parse_mnemonic(operands[0], &curr_instruction) == -1)
            {
                // mnemonic is not recognized
                if (is_label(operands[0]))
                {   
                    // check if there are more operands
                    if (curr_instruction.op_1.op_type == UNUSED && curr_instruction.op_2.op_type == UNUSED)
                    {
                        add_label(label_list, &(label_t){.text = strdup(operands[0]), .instruction_index = instruction_list->count - 1});
                        goto skip;
                    }
                    else {
                        printf("A label can not have operands.\n");
                        exit(EXIT_FAILURE);
                    }
                } else
                {
                    printf("Unknown mnemonic: \"%s\".\n", operands[0]);
                    exit(EXIT_FAILURE);
                }
            }
        }

        if (instruction_list->count >= instruction_list->capacity)
        {
            instruction_list->capacity *= 2;
            instruction_list->list = realloc(instruction_list->list, sizeof(instruction_t) * instruction_list->capacity);
        }

        instruction_list->list[instruction_list->count++] = curr_instruction;

        //printf("inst_i: %d\noperand: %s\n",instruction_list->count-1, operands[0]);

        skip:
            free(line);
            line = NULL;
    }
}

int check_correct_parameters(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("To many or to little arguments provided.\n");
        return 1;
    }

    char *dot = strrchr(argv[1], '.');
    if (dot == NULL)
    {
        printf("The filename is does not contain a '.' character.\n");
        return 1;
    }

    if (strcmp(dot + 1, "asm"))
    {
        printf("The file extension is not 'bin'.\n");
        return 1;
    }
    return 0;
}

int main(int argc, char **argv)
{
    check_correct_parameters(argc, argv);

    instruction_list_t instruction_list;
    label_list_t label_list;

    parse_assembly_file(argv[1], &instruction_list, &label_list);

    print_instruction_list(&instruction_list);
    print_label_list(&label_list);

    free_label_list(&label_list);

    return 0;
}