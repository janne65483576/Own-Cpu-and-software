#ifndef LABEL_H
#define LABEL_H

#include <stdbool.h>

#include "instruction.h"

typedef struct
{
    int address;
    // after which instruction the label is defined
    int instruction_index;
    char *text;
}label_t;

typedef struct 
{
    label_t *list;
    int count;
    int capacity;
}label_list_t;

void add_label(label_list_t *label_list, label_t *label);
int is_label(char *text);
void free_label_list(label_list_t *label_list);
void print_label_list(label_list_t *label_list);

#endif // LABEL_H
