#ifndef LABEL_H
#define LABEL_H

#include "instruction.h"

void init_label_list();
void add_label(char *label_text, int instruction_index);
void get_label_val(operand *op);
int is_label(char *text);

#endif // LABEL_H
