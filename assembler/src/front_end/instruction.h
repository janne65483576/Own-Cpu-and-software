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
} PseudoInstructions;

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
    CSF = 0b11101111

} Opcode;

typedef enum
{
    DIRECT_16 = 0b00,
    MAR_ADDR  = 0b01,
    IMM_ADDR  = 0b10,
    CALL_ADDR = 0b10,
    DIRECT_8  = 0b11,
    POP_ADDR  = 0b11,
    PUSH_ADDR = 0b11,
    RET_ADDR  = 0b11
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
    uint16_t value;
}Operand;

typedef struct 
{   
    Operand *list;
    int count;
}OperandList;

typedef struct  
{
    Opcode mnemonic;
    OperandList operands;
}Instruction;

#define MAX_OPERANDS 2
#define MAX_FORMS    7

typedef struct{
    OperandKind operands[MAX_OPERANDS];
    int count;
    
    uint8_t opcode;
    bool use_gpr_src;
    bool use_gpr_or_flag_dest;
} OperandForm;

typedef struct
{
    int form_count;
    OperandForm forms[MAX_FORMS];
}InstructionDefinition;

const InstructionDefinition instruction_definition[] =
{
    // arithmetic instructions
    [ADD_mn] = {
        .form_count = 5,
        .forms = { 
            {
                .operands = {OPERAND_GPR, OPERAND_GPR},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = true,
                .opcode               = ADDr
            },
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_16},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = ADDm | DIRECT_16 << 6
            },
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = ADDm | MAR_ADDR << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_8},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = ADDm | DIRECT_8 << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_IMM_8},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = ADDm | IMM_ADDR << 6
            }
        }
    },
    [AND_mn] = {
        .form_count = 5,
        .forms = { 
            {
                .operands = {OPERAND_GPR, OPERAND_GPR},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = true,
                .opcode               = ANDr
            },
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_16},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = ANDm | DIRECT_16 << 6
            },
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = ANDm | MAR_ADDR << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_8},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = ANDm | DIRECT_8 << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_IMM_8},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = ANDm | IMM_ADDR << 6
            }
        }
    },
    [XOR_mn] = {
        .form_count = 5,
        .forms = { 
            {
                .operands = {OPERAND_GPR, OPERAND_GPR},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = true,
                .opcode               = XORr
            },
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_16},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = XORm | DIRECT_16 << 6
            },
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = XORm | MAR_ADDR << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_8},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = XORm | DIRECT_8 << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_IMM_8},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = XORm | IMM_ADDR << 6
            }
        }
    },
    [OR_mn] = {
        .form_count = 5,
        .forms = { 
            {
                .operands = {OPERAND_GPR, OPERAND_GPR},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = true,
                .opcode               = ORr
            },
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_16},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = ORm | DIRECT_16 << 6
            },
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = ORm | MAR_ADDR << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_8},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = ORm | DIRECT_8 << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_IMM_8},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = ORm | IMM_ADDR << 6
            }
        }
    },

    // arithmetic register instructions
    [ADDr_mn] = {
        .form_count = 1,
        .forms = { 
            {
                .operands = {OPERAND_GPR, OPERAND_GPR},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = true,
                .opcode               = ADDr
            }
        }
    },
    [ANDr_mn] = {
        .form_count = 1,
        .forms = { 
            {
                .operands = {OPERAND_GPR, OPERAND_GPR},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = true,
                .opcode               = ANDr
            }
        }
    },
    [XORr_mn] = {
        .form_count = 1,
        .forms = { 
            {
                .operands = {OPERAND_GPR, OPERAND_GPR},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = true,
                .opcode               = XORr
            }
        }
    },
    [ORr_mn]  = {
        .form_count = 1,
        .forms = { 
            {
                .operands = {OPERAND_GPR, OPERAND_GPR},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = true,
                .opcode               = ORr
            }
        }
    },

    // arithmetic memory instructions
    [ADDm_mn] = {
        .form_count = 4,
        .forms = { 
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_16},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = ADDm | DIRECT_16 << 6
            },
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = ADDm | MAR_ADDR << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_8},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = ADDm | DIRECT_8 << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_IMM_8},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = ADDm | IMM_ADDR << 6
            }
        }
    },
    [ANDm_mn] = {
        .form_count = 4,
        .forms = { 
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_16},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = ANDm | DIRECT_16 << 6
            },
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = ANDm | MAR_ADDR << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_8},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = ANDm | DIRECT_8 << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_IMM_8},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = ANDm | IMM_ADDR << 6
            }
        }
    },
    [XORm_mn] = {
        .form_count = 4,
        .forms = { 
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_16},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = XORm | DIRECT_16 << 6
            },
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = XORm | MAR_ADDR << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_8},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = XORm | DIRECT_8 << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_IMM_8},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = XORm | IMM_ADDR << 6
            }
        }
    },
    [ORm_mn]  = {
        .form_count = 4,
        .forms = { 
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_16},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = ORm | DIRECT_16 << 6
            },
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = ORm | MAR_ADDR << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_8},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = ORm | DIRECT_8 << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_IMM_8},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = ORm | IMM_ADDR << 6
            }
        }
    },

    [SHL_mn] = {
        .form_count = 1,
        .forms = {
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = SHL
            }
        }
    },
    [SHR_mn] = {
        .form_count = 1,
        .forms = {
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = SHR
            }
        }
    },
    [ROL_mn] = {
        .form_count = 1,
        .forms = {
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = ROL
            }
        }
    },
    [ROR_mn] = {
        .form_count = 1,
        .forms = {
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = ROR
            }
        }
    },

    [NOT_mn] = {
        .form_count = 1,
        .forms = {
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = NOT
            }
        }
    },
    [INC_mn] = {
        .form_count = 1,
        .forms = {
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = INC
            }
        }
    },
    [DEC_mn] = {
        .form_count = 1,
        .forms = {
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = DEC
            }
        }
    },
    [NEG_mn] = {
        .form_count = 1,
        .forms = {
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = NEG
            }
        }
    },

    // branch instructions
    [JMP_mn] = {
        .form_count = 2,
        .forms = {
            {
                .operands = {OPERAND_ADDR_16},
                .count = 1,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = JMP << DIRECT_16
            },
            {
                .operands = {OPERAND_ADDR_8},
                .count = 1,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = JMP << DIRECT_8
            }
        }
    },
    [BCS_mn] = {
        .form_count = 2,
        .forms = {
            {
                .operands = {OPERAND_ADDR_16},
                .count = 1,
                .use_gpr_or_flag_dest = false,
                .use_gpr_src          = false,
                .opcode               = BCS << DIRECT_16
            },
            {
                .operands = {OPERAND_ADDR_8},
                .count = 1,
                .use_gpr_or_flag_dest = false,
                .use_gpr_src          = false,
                .opcode               = BCS << DIRECT_8
            }
        }
    },
    [BZS_mn] = {
        .form_count = 2,
        .forms = {
            {
                .operands = {OPERAND_ADDR_16},
                .count = 1,
                .use_gpr_or_flag_dest = false,
                .use_gpr_src          = false,
                .opcode               = BZS << DIRECT_16
            },
            {
                .operands = {OPERAND_ADDR_8},
                .count = 1,
                .use_gpr_or_flag_dest = false,
                .use_gpr_src          = false,
                .opcode               = BZS << DIRECT_8
            }
        }
    },
    [BVS_mn] = {
        .form_count = 2,
        .forms = {
            {
                .operands = {OPERAND_ADDR_16},
                .count = 1,
                .use_gpr_or_flag_dest = false,
                .use_gpr_src          = false,
                .opcode               = BSS << DIRECT_16
            },
            {
                .operands = {OPERAND_ADDR_8},
                .count = 1,
                .use_gpr_or_flag_dest = false,
                .use_gpr_src          = false,
                .opcode               = BSS << DIRECT_8
            }
        }
    },
    [BCC_mn] = {
        .form_count = 2,
        .forms = {
            {
                .operands = {OPERAND_ADDR_16},
                .count = 1,
                .use_gpr_or_flag_dest = false,
                .use_gpr_src          = false,
                .opcode               = BCC << DIRECT_16
            },
            {
                .operands = {OPERAND_ADDR_8},
                .count = 1,
                .use_gpr_or_flag_dest = false,
                .use_gpr_src          = false,
                .opcode               = BCC << DIRECT_8
            }
        }
    },
    [BZC_mn] = {
        .form_count = 2,
        .forms = {
            {
                .operands = {OPERAND_ADDR_16},
                .count = 1,
                .use_gpr_or_flag_dest = false,
                .use_gpr_src          = false,
                .opcode               = BZC << DIRECT_16
            },
            {
                .operands = {OPERAND_ADDR_8},
                .count = 1,
                .use_gpr_or_flag_dest = false,
                .use_gpr_src          = false,
                .opcode               = BZC << DIRECT_8
            }
        }
    },
    [BVC_mn] = {
        .form_count = 2,
        .forms = {
            {
                .operands = {OPERAND_ADDR_16},
                .count = 1,
                .use_gpr_or_flag_dest = false,
                .use_gpr_src          = false,
                .opcode               = BSC << DIRECT_16
            },
            {
                .operands = {OPERAND_ADDR_8},
                .count = 1,
                .use_gpr_or_flag_dest = false,
                .use_gpr_src          = false,
                .opcode               = BSC << DIRECT_8
            }
        }
    },

    // function flow instructions
    [CALL_mn]  = {
        .form_count = 2,
        .forms = {
            {
                .operands = {OPERAND_ADDR_16},
                .count = 1,
                .use_gpr_or_flag_dest = false,
                .use_gpr_src          = false,
                .opcode               = CALL
            },
            {
                .operands = {OPERAND_FLAG, OPERAND_ADDR_16},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = CALL
            }
        }
    },
    [CALLn_mn] = {
        .form_count = 2,
        .forms = {
            {
                .operands = {OPERAND_ADDR_16},
                .count = 1,
                .use_gpr_or_flag_dest = false,
                .use_gpr_src          = false,
                .opcode               = CALLn
            },
            {
                .operands = {OPERAND_FLAG, OPERAND_ADDR_16},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = CALLn
            }
        }
    },
    [RET_mn] = {
        .form_count = 2,
        .forms = {
            {
                .count = 0,
                .use_gpr_or_flag_dest = false,
                .use_gpr_src          = false,
                .opcode               = RET
            },
            {
                .operands = {OPERAND_FLAG},
                .count = 1,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = RET
            }
        }
    },
    [RETn_mn] = {
        .form_count = 2,
        .forms = {
            {
                .count = 0,
                .use_gpr_or_flag_dest = false,
                .use_gpr_src          = false,
                .opcode               = RETn
            },
            {
                .operands = {OPERAND_FLAG},
                .count = 1,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = RETn
            }
        }
    },

    // flag manipulation
    [CLF_mn] = {
        .form_count = 1,
        .forms = {
            {
                .operands = {OPERAND_FLAG},
                .count = 1, 
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = CLF
            }
        }
    },
    [SEF_mn] = {
        .form_count = 1,
        .forms = {
            {
                .operands = {OPERAND_FLAG},
                .count = 1, 
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = SEF
            }
        }
    },

    [CCF_mn] = {
        .form_count = 1,
        .forms = {
            .count = 0,
            .use_gpr_or_flag_dest = false,
            .use_gpr_src          = false,
            .opcode               = CCF
        }
    },
    [CZF_mn] = {
        .form_count = 1,
        .forms = {
            .count = 0,
            .use_gpr_or_flag_dest = false,
            .use_gpr_src          = false,
            .opcode               = CZF
        }
    },
    [CVF_mn] = {
        .form_count = 1,
        .forms = {
            .count = 0,
            .use_gpr_or_flag_dest = false,
            .use_gpr_src          = false,
            .opcode               = CSF
        }
    },
    [SCF_mn] = {
        .form_count = 1,
        .forms = {
            .count = 0,
            .use_gpr_or_flag_dest = false,
            .use_gpr_src          = false,
            .opcode               = SCF
        }
    },
    [SZF_mn] = {
        .form_count = 1,
        .forms = {
            .count = 0,
            .use_gpr_or_flag_dest = false,
            .use_gpr_src          = false,
            .opcode               = SZF
        }
    },
    [SVF_mn] = {
        .form_count = 1,
        .forms = {
            .count = 0,
            .use_gpr_or_flag_dest = false,
            .use_gpr_src          = false,
            .opcode               = SSF
        }
    },

    [ST_mn] = {
        .form_count = 2,
        .forms = {
            {
                .operands = {OPERAND_ADDR_16, OPERAND_GPR},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = ST | DIRECT_16 << 6
            },
            {
                .operands = {OPERAND_MAR, OPERAND_GPR},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = ST | MAR << 6
            }
        }
    },
    [LD_mn] = {
        .form_count = 2,
        .forms = {
            {
                .operands = {OPERAND_GPR, OPERAND_ADDR_16},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = LD | DIRECT_16 << 6
            },
            {
                .operands = {OPERAND_GPR, OPERAND_MAR},
                .count = 2,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = LD | DIRECT_16 << 6
            }
        }
    },
    [POP_mn] = {
        .form_count = 3,
        .forms = {
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = POP
            },
            // TODO:
            // {
            //     .operands = {OPERAND_REGISTER_8},
            //     .count = 1,
            //     
            // },
            // {
            //     .operands = {OPERAND_REGISTER_16},
            //     .addr_mode = NO_MODE,
            //     .count = 1
            // }
        }
    },
    [PUSH_mn] = {
        .form_count = 3,
        .forms = {
            {
                .operands = {OPERAND_GPR},
                .count = 1,
                .use_gpr_or_flag_dest = true,
                .use_gpr_src          = false,
                .opcode               = PUSH
            },
            // {
            //     .operands = {OPERAND_REGISTER_8},
            //     .addr_mode = NO_MODE,
            //     .count = 1
            // },
            // {
            //     .operands = {OPERAND_REGISTER_16},
            //     .addr_mode = NO_MODE,
            //     .count = 1
            // }
        }
    },

    [MOV_mn] = {
        .form_count = 7,
        .forms = {
            {
                .operands = {OPERAND_GPR, OPERAND_GPR},
                .addr_mode = NO_MODE,
                .count = 2
            },
            {
                .operands = {OPERAND_GPR, OPERAND_REGISTER_8},
                .addr_mode = NO_MODE,
                .count = 2
            },
            {
                .operands = {OPERAND_REGISTER_8, OPERAND_GPR},
                .addr_mode = NO_MODE,
                .count = 2
            },
            {
                .operands = {OPERAND_REGISTER_8, OPERAND_REGISTER_8},
                .addr_mode = NO_MODE,
                .count = 2
            },
            {
                .operands = {OPERAND_REGISTER_16, OPERAND_REGISTER_16},
                .addr_mode = NO_MODE,
                .count = 2
            },
            {
                .operands = {OPERAND_REGISTER_8, OPERAND_IMM_8},
                .addr_mode = NO_MODE,
                .count = 2
            },
            {
                .operands = {OPERAND_REGISTER_16, OPERAND_IMM_16},
                .addr_mode = NO_MODE,
                .count = 2
            }
        }
    },
    [MOVr_mn] = {
        .form_count = 1,
        .forms = {
            {
                .operands = {OPERAND_GPR, OPERAND_GPR},
                .addr_mode = NO_MODE,
                .count = 2
            }
        }
    },
    [MOVe_mn] = {
        .form_count = 7,
        .forms = {
            {
                .operands = {OPERAND_GPR, OPERAND_GPR},
                .addr_mode = NO_MODE,
                .count = 2
            },
            {
                .operands = {OPERAND_GPR, OPERAND_REGISTER_8},
                .addr_mode = NO_MODE,
                .count = 2
            },
            {
                .operands = {OPERAND_REGISTER_8, OPERAND_GPR},
                .addr_mode = NO_MODE,
                .count = 2
            },
            {
                .operands = {OPERAND_REGISTER_8, OPERAND_REGISTER_8},
                .addr_mode = NO_MODE,
                .count = 2
            },
            {
                .operands = {OPERAND_REGISTER_16, OPERAND_REGISTER_16},
                .addr_mode = NO_MODE,
                .count = 2
            },
            {
                .operands = {OPERAND_REGISTER_8, OPERAND_IMM_8},
                .addr_mode = NO_MODE,
                .count = 2
            },
            {
                .operands = {OPERAND_REGISTER_16, OPERAND_IMM_16},
                .addr_mode = NO_MODE,
                .count = 2
            }
        }
    },

    [NOP_mn] = {
        .form_count = 0
    }
};

#endif // INSTRUCTION_H