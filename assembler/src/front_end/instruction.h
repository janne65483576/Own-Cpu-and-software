#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdint.h>
#include <stdbool.h>

// typedef enum
// {   
//     // arithmetic
// 	ADD_mn,
//     ADDm_mn,
//     ADDr_mn,
    
//     AND_mn,
//     ANDm_mn,
//     ANDr_mn,
    
//     OR_mn,
//     ORm_mn,
//     ORr_mn,

//     XOR_mn,
//     XORm_mn,
//     XORr_mn,

//     ROL_mn,
//     ROR_mn,
//     SHL_mn,
//     SHR_mn,
    
//     DEC_mn,
//     INC_mn,
//     NEG_mn,
//     NOT_mn,

    
//     // branch
//     BCC_mn,
//     BCS_mn,
    
//     BSC_mn,
//     BSS_mn,
    
//     BZC_mn,
//     BZS_mn,

//     JMP_mn,

//     // function calls
//     CALL_mn,
//     CALLn_mn,
//     RET_mn,
//     RETn_mn,
    
//     // memory
//     LD_mn,
//     ST_mn,
    
//     // stack
//     POP_mn,
//     PUSH_mn,

//     // flag
//     SEF_mn,
//     CEF_mn,
    
//     SCF_mn,
//     CCF_mn,

//     SZF_mn,
//     CZF_mn,
    
//     SSF_mn,
//     CSF_mn,    

//     // data transfer
//     MOV_mn,
//     MOVe_mn,
//     MOVr_mn,
    
//     NOP_mn,
// } PseudoInstructions;

// sorted alphabetical so binary sort can be used.
// sort with sort.c

typedef enum
{
	ADD_mn, ADDm_mn, ADDr_mn, AND_mn, ANDm_mn, ANDr_mn, BCC_mn, BCS_mn, BVC_mn, BVS_mn, BZC_mn, BZS_mn, CALL_mn, CALLn_mn, CCF_mn, CLF_mn, CVF_mn, CZF_mn, DEC_mn, INC_mn, JMP_mn, LD_mn, MOV_mn, MOVe_mn, MOVr_mn, NEG_mn, NOP_mn, NOT_mn, OR_mn, ORm_mn, ORr_mn, POP_mn, PUSH_mn, RET_mn, RETn_mn, ROL_mn, ROR_mn, SCF_mn, SEF_mn, SHL_mn, SHR_mn, ST_mn, SVF_mn, SZF_mn, XOR_mn, XORm_mn, XORr_mn
} Mnemonic;

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

    // one operand instructions
    INC = 0b00001111,
    DEC = 0b01001111,
    NEG = 0b10001111,
    NOT = 0b11001111,

    // shift
    SHL = 0b00001001,
    SHR = 0b01001001,
    ROL = 0b10001001,
    ROR = 0b11001001,

    // memory
    LD   = 0b00001010,
    POP  = 0b11001010,
    ST   = 0b00001011,
    PUSH = 0b11001011,

    // control flow
    JMP  = 0xc,

    BCS = 0b00011100,
    BZS = 0b00101100,
    BSS = 0b00111100,

    BCC = 0b00011101,
    BZC = 0b00101101,
    BSC = 0b00111101,

    CALL  = 0b10001101,
    CALLn = 0b10001110,
    RET   = 0b11001101,
    RETn  = 0b11001110,

    // data transfer
    MOVr = 0xe,
    MOVe = 0xf,

    // edit flags
    SEF = 0b10011111,
    CLF = 0b10001111,

    SCF = 0b10111111,
    SZF = 0b11011111,
    SSF = 0b11111111,

    CCF = 0b10101111,
    CZF = 0b11001111,
    CSF = 0b11101111,
} Opcode;

// these are only real address modes
typedef enum
{
    DIRECT_16 = 0b00,
    MAR_ADDR  = 0b01,
    IMM_ADDR  = 0b10,
    DIRECT_8  = 0b11,
}AddressMode;

typedef enum
{
    // register
    OPERAND_GPR,
    OPERAND_MAR,

    // addresses
    OPERAND_ADDR_8,
    OPERAND_ADDR_16,
    OPERAND_LABEL, // a label is a address where the size is unkown
    
    // flag is used for some branch and indirect modification of the flag register
    OPERAND_FLAG,

    OPERAND_IMM_8,
    OPERAND_IMM_16, // only for mov(e)

    OPERAND_REGISTER_16, 
    OPERAND_REGISTER_8
} OperandKind;

typedef enum 
{
    FLAG_ZERO, FLAG_CARRY, FLAG_SIGN
}Flag;

typedef enum 
{
    R0, R1, R2, R3
}GPRegister;

typedef enum
{
    R0_R1, R0_R2, R0_R3, R1_R0, R1_R2, R1_R3, R2_R0, R2_R1, R2_R3, R3_R0, R3_R1, R3_R2, PC, SP, MAR
}Register16Bit;

typedef enum
{
    R0_8BIT, R1_8BIT, R2_8BIT, R3_8BIT, PC_L, PC_H, SP_L, SP_H, MAR_L, MAR_H, FLAGS_REG, MBR
}Register8Bit;

typedef struct
{
    OperandKind kind;
    // some operand kinds dont have values
    union 
    {
        GPRegister    gpr;
        uint8_t       addr_8;
        uint16_t      addr_16;
        Flag          flag;
        int8_t        immediate_8;
        int16_t       immediate_16;
        Register16Bit register_16;
        Register8Bit  register_8;
    };
}Operand;

typedef struct
{
    Operand *list;
    int count;
}OperandList;

typedef struct
{
    Mnemonic mnemonic;
    OperandList operands;
}Instruction;

#endif // INSTRUCTION_H