#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "instruction.h"
#include "mnemonic.h"
#include "label.h"

void print_label(label_t l)
{
    printf("\tText: %s\n", l.text);
    printf("\tInst_i: %d\n", l.instruction_index);
    //printf("Address: %d\n", l.address);
}

void print_label_list(label_list_t *label_list)
{
    for (int i = 0; i < label_list->count; i++)
    {
        printf("%d. label:\n", i);
        print_label(label_list->list[i]);
    }
}

void free_label_list(label_list_t *label_list)
{
    for (int i = 0; i < label_list->count; i++)
    {
        free(label_list->list[i].text);
    }

    free(label_list->list);
}

void add_label(label_list_t *label_list, label_t *label)
{
    // check if the label is already in the list
    for (int i = 0; i < label_list->count; i++)
    {
        //printf("label_text: %s --- label_list: %s\n", label_text, label_list.list[i].label);
        if (strcmp(label->text, label_list->list[i].text) == 0)
        {
            print_label_list(label_list);
            printf("The label \"%s\" is already defined.\n", label->text);
            exit(EXIT_FAILURE);
        }
    }
    
    if (label_list->count >= label_list->capacity)
    {
        label_list->capacity *= 2;
        label_list->list = realloc(label_list->list, sizeof(label_t) * label_list->capacity);
    }

    label_list->list[label_list->count++] = *label;

    //print_label(label_list.list[label_list.count - 1]);
}

// check if the text is a label and remove the colon
int is_label(char *text)
{
    int len = strlen(text);
    if (text[len - 1] == ':')
    {
        // remove the colon
        text[len - 1] = '\0';
        return 1;
    }
    return 0;
}

label_t *label_by_instruction_index(label_list_t *label_list, int instruction_index)
{
    for (int i = 0; i < label_list->count; i++)
    {
        if (label_list->list[i].instruction_index == instruction_index)
        {
            return &label_list->list[i];
        }
    }

    return NULL;
}

label_t *find_label(label_list_t *label_list, char *name)
{
    for (int i = 0; i < label_list->count; i++)
    {
        if (strcmp(label_list->list[i].text, name) == 0)
        {
            return &label_list->list[i];
        }
    }
    return NULL;
}

int get_inst_len_label(instruction_t *instruction, label_list_t *label_list, int pc)
{
    if (instruction->op_1.op_type != LABEL && instruction->op_2.op_type != LABEL)
    {
        // the instruction has 2 byte if it contains one 8 bit type
        if (instruction->op_1.op_type == ADDR_8 || instruction->op_2.op_type == ADDR_8 ||
            instruction->op_1.op_type == IMM_8 || instruction->op_2.op_type == IMM_8)
        {
            return 2;
        }
        
        if (instruction->op_1.op_type == ADDR_16 || instruction->op_2.op_type == ADDR_16)
        {
            return 3;
        }

        // otherwise there are always 1 byte
        return 1;
    }

    label_t *label;
    operand_t *operand;

    // its impossible that both operands have labels due to check made befor calling this function
    if (instruction->op_1.op_type == LABEL)
    {
        label = find_label(label_list, instruction->op_1.label);
        operand = &instruction->op_1;
    }else
    {
        label = find_label(label_list, instruction->op_2.label);
        operand = &instruction->op_2;
    }

    if (label == NULL)
    {
        printf("Instruction refers to a label which doesent exist.\n");
        exit(EXIT_FAILURE);
    }

    if (label->address == -1)
    {
        // instruction with the most biggest size
        operand->op_type = ADDR_16;
        return 3;
    }

    if (relative_addressing(instruction) == true)
    {
        int next_pc = pc + 2;
        int offset  = label->address - next_pc;

        if (offset >= -128 && offset <= 127)
        {
            // use 8 bit indirect addressing
            operand->op_type = ADDR_8;
            return 2;
        }else
        {
            // use 16 bit direct addressing
            operand->op_type = ADDR_16;
            return 3;
        }
    }else
    {
        int addr = label->address;
        if (addr >= 0 && addr <= 255)
        {
            // use 8 bit direct addressing
            operand->op_type = ADDR_8;
            return 2;
        }else
        {
            // use 16 bit direct addressing
            operand->op_type = ADDR_16;
            return 3;
        }
    }
}

int get_inst_len()
{
    
}

void insert_label_addresses(label_list_t *label_list, instruction_list_t *instruction_list)
{
    for (int i = 0; i < label_list->count; i++)
    {
        label_list->list[i].address = -1;
    }

    bool changed;

    do {
        changed = false;
        int pc = 0;

        for (int i = 0; i < instruction_list->count; i++)
        {
            // is a label at this position
            label_t *label = label_by_instruction_index(label_list, i);
            if (label && label->address != pc)
            {
                label->address = pc;
                changed = true;
            }

            int len = get_inst_len(&instruction_list->list[i], label_list, pc);

            pc += len; 
        }

    } while (changed);
}
