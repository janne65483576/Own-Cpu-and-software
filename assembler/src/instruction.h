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
} opcodes;

typedef enum
{
    REGISTER, MAR, ADDR_16, IMM_8, ADDR_8, LABEL, UNUSED, FLAG
} operand_type;

typedef struct{
    int16_t value;
    operand_type op_type;
    char *label; // only used if operand_type == LABEL
} operand;

typedef struct{
    // the first translation step writes the text the second use ist and construct the opcode
    union {char *mnemonic_text; opcodes opcode; };
    operand op_1;
    operand op_2;
} instruction;

typedef enum
{
    CARRY = 1, ZERO, OVERFLOW
} flags;

#endif // INSTRUCTION_H