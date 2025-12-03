#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "instruction.h"

#define LABEL_LIST_INIT_SIZE 20
#define LABEL_LIST_INC 5 // When label_list is full increment with LABEL_LIST_INC

typedef struct
{
    int address;
    // after which instruction the label is defined
    int instruction_index;
    char *label;
    bool resolved;
} label;

struct label_list
{
    label *list;
    // count of the labels which are already in the list
    int count;
    // how many labels can fit in the allocated space
    int capacity;
};

struct label_list label_list;

void init_label_list()
{
    label_list.list = malloc(sizeof(label) * (LABEL_LIST_INIT_SIZE));
    label_list.capacity = LABEL_LIST_INIT_SIZE;
    label_list.count = 0;
    return;
}

void print_label(label l)
{
    printf("Text: %s\n", l.label);
    //printf("Offset from last label: %d\n", l.offset);
}

void print_label_list()
{
    for (int i = 0; i < label_list.count; i++)
    {
        printf("%d. label:\n", i + 1);
        print_label(label_list.list[i]);
    }
}

// only insert labels without the colon
void add_label(char *label_text, int instruction_index)
{
    // check if the label is already in the list
    for (int i = 0; i < label_list.count; i++)
    {
        //printf("label_text: %s --- label_list: %s\n", label_text, label_list.list[i].label);
        if (strcmp(label_text, label_list.list[i].label) == 0)
        {
            print_label_list();
            printf("The label \"%s\" is already defined.\n", label_text);
            exit(EXIT_FAILURE);
        }
    }
    
    if (label_list.count >= label_list.capacity)
    {
        label_list.capacity += LABEL_LIST_INC;
        label_list.list = realloc(label_list.list, sizeof(label) * label_list.capacity);
    }

    label label_new;

    label_new.label = strdup(label_text);
    label_new.instruction_index = instruction_index;

    label_list.list[label_list.count++] = label_new;

    //print_label(label_list.list[label_list.count - 1]);

    return;
}

int get_inst_len(instruction *inst)
{
    if (!inst) return 0;

    if (inst->op_1.op_type == IMM_8 || inst->op_2.op_type == IMM_8 ||
        inst->op_1.op_type == ADDR_8 || inst->op_2.op_type == ADDR_8) {
        return 2;
    }

    if (inst->op_1.op_type == ADDR_16 || inst->op_2.op_type == ADDR_16) {
        return 3;
    }

    // default/fallback: return 1 (or 0/error) depending on ISA design
    return 1;
}

// uses the instruction index to resolve the labels to addresses
void resolve_label(label *l, instruction *instructions, int instruction_count)
{
    if (!l || !instructions) return;

    if (l->resolved) return;
    l->resolved = 1;

    int address = 0;
    // sum lengths of instructions before this label's instruction index
    for (int i = 0; i < l->instruction_index && i < instruction_count; ++i)
    {
        address += get_inst_len(&instructions[i]);
    }

    l->address = address;
}

void get_label_val(operand *op)
{
    return;
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