#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdint.h>

// real opcodes 
// OOI = one operand instruction
typedef enum
{
    // arithmetic register
    ADDr = 0b0000,
    ANDr = 0b0001,
    ORr  = 0b0010,
    XORr = 0b0011,

    // arithmetic memory
    ADDm = 0b0100,
    ANDm = 0b0101,
    XORm = 0b0110,
    ORm  = 0b0111,

    // one operand instruction
    OOI = 0x8,

    // shift
    SH = 0x9,

    // memory
    LD = 0xa,
    ST = 0xb,

    // control flow
    JMP  = 0xc,
    JMPn = 0xd,

    // data transfer
    MOVr = 0xe,
    MOVe = 0xf,

    // edit flags
    EF = 0b10001111
}opcodes;

typedef enum
{
    TOKEN_REGISTER, MAR, ADDR_16, IMM_8, ADDR_8, TOKEN_LABEL, UNUSED, FLAG
}operand_data_t;

typedef struct{
    int16_t value;
    operand_data_t op_type;
    char *label; // only used if operand_type == LABEL
}operand_t;

typedef struct{
    // the first translation step writes the text the second use its and construct the opcode
    opcodes opcode;
    operand_t op_1;
    operand_t op_2;
}instruction_t;

typedef struct
{
    instruction_t *list;
    int count;
    int capacity;
}instruction_list_t;

typedef enum
{
    CARRY = 1, ZERO, OVERFLOW
}flags_t;

#endif // INSTRUCTION_H