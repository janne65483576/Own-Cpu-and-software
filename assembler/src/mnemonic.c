#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "instruction.h"
#include "label.h"

// sorted alphabetical -> use the sort.c
char *mnemonics[] = {"ADD", "ADDm", "ADDr", "AND", "ANDm", "ANDr", "BCC", "BCS", "BVC", "BVS", "BZC", "BZS", "CALL", "CALLn", "CCF", "CLF", "CVF", "CZF", "DEC", "INC", "JMP", "LD", "MOV", "MOVe", "MOVr", "NEG", "NOP", "NOT", "OR", "ORm", "ORr", "POP", "PUSH", "RET", "RETn", "ROL", "ROR", "SCF", "SEF", "SHL", "SHR", "ST", "SVF", "SZF", "XOR", "XORm", "XORr"};

typedef enum
{
	ADD_mn, ADDm_mn, ADDr_mn, AND_mn, ANDm_mn, ANDr_mn, BCC_mn, BCS_mn, BVC_mn, BVS_mn, BZC_mn, BZS_mn, CALL_mn, CALLn_mn, CCF_mn, CLF_mn, CVF_mn, CZF_mn, DEC_mn, INC_mn, JMP_mn, LD_mn, MOV_mn, MOVe_mn, MOVr_mn, NEG_mn, NOP_mn, NOT_mn, OR_mn, ORm_mn, ORr_mn, POP_mn, PUSH_mn, RET_mn, RETn_mn, ROL_mn, ROR_mn, SCF_mn, SEF_mn, SHL_mn, SHR_mn, ST_mn, SVF_mn, SZF_mn, XOR_mn, XORm_mn, XORr_mn
} mnemonic_enum;

typedef struct instruction_info instruction_info;

typedef void (*get_opcode_func)(instruction *, instruction_info *);

struct instruction_info {
    get_opcode_func func;
    union {
        uint8_t opcode; // for mnemonic which only corresponds to one opcode
        struct {uint8_t reg; uint8_t mem;};
        struct {uint8_t normal; uint8_t extended;}; // for the MOV instruction
    };
};

// arithmetic instructions
void get_opcode_mem_or_reg(instruction *inst, instruction_info *info);
void check_reg_inst(instruction *inst, instruction_info *info);
void check_arr_mem_inst(instruction *inst, instruction_info *info);
void check_shift_inst(instruction *inst, instruction_info *info);
void check_OOI_inst(instruction *inst, instruction_info *info); // INC, DEC etc.

// control flow instructions
void check_branch_inst(instruction *inst, instruction_info *info);
void check_RET_inst(instruction *inst, instruction_info *info);
void check_CALL_inst(instruction *inst, instruction_info *info);

// flag instructions
void check_flag_inst(instruction *inst, instruction_info *info);

// memory instructions
void check_mem_inst(instruction *inst, instruction_info *info);
void check_stack_inst(instruction *inst, instruction_info *info);

// data transfer
void check_MOVr_inst(instruction *inst, instruction_info *info);
void check_MOVe_inst(instruction *inst, instruction_info *info);
void get_MOV_inst(instruction *inst, instruction_info *info);

void check_NOP_inst(instruction *inst, instruction_info *info);

// indexed through the mn_index
instruction_info instruction_translation[] = 
{
    [ADD_mn] = {get_opcode_mem_or_reg, .reg = ADDr, .mem = ADDm},
    [AND_mn] = {get_opcode_mem_or_reg, .reg = ANDr, .mem = ANDm},
    [XOR_mn] = {get_opcode_mem_or_reg, .reg = XORr, .mem = XORm},
    [OR_mn]  = {get_opcode_mem_or_reg, .reg = ORr,  .mem = ORm},

    [ADDr_mn] = {check_reg_inst, .reg = ADDr},
    [ANDr_mn] = {check_reg_inst, .reg = ANDr},
    [XORr_mn] = {check_reg_inst, .reg = XORr},
    [ORr_mn]  = {check_reg_inst, .reg = ORr},

    [ADDm_mn] = {check_arr_mem_inst, .reg = ADDm},
    [ANDm_mn] = {check_arr_mem_inst, .reg = ANDm},
    [XORm_mn] = {check_arr_mem_inst, .reg = XORm},
    [ORm_mn]  = {check_arr_mem_inst, .reg = ORm},

    [SHL_mn] = {check_shift_inst, .opcode = SH | 0b00 << 6}, // rotate = 0, direction = 0
    [SHR_mn] = {check_shift_inst, .opcode = SH | 0b01 << 6}, // rotate = 0, direction = 1
    [ROL_mn] = {check_shift_inst, .opcode = SH | 0b10 << 6}, // rotate = 1, direction = 0
    [ROR_mn] = {check_shift_inst, .opcode = SH | 0b11 << 6}, // rotate = 1, direction = 1

    [NOT_mn] = {check_OOI_inst, .opcode = OOI | 0b00 << 6},
    [INC_mn] = {check_OOI_inst, .opcode = OOI | 0b01 << 6},
    [DEC_mn] = {check_OOI_inst, .opcode = OOI | 0b10 << 6},
    [NEG_mn] = {check_OOI_inst, .opcode = OOI | 0b11 << 6},

    [JMP_mn] = {check_branch_inst, .opcode = JMP}, // this instruction is unconditionally
    [BCS_mn] = {check_branch_inst, .opcode = JMP  | CARRY    << 4 }, // shift in the condition code
    [BZS_mn] = {check_branch_inst, .opcode = JMP  | ZERO     << 4 },
    [BVS_mn] = {check_branch_inst, .opcode = JMP  | OVERFLOW << 4 },
    [BCC_mn] = {check_branch_inst, .opcode = JMPn | CARRY    << 4 },
    [BZC_mn] = {check_branch_inst, .opcode = JMPn | ZERO     << 4 },
    [BVC_mn] = {check_branch_inst, .opcode = JMPn | OVERFLOW << 4 },

    [CALL_mn]  = {check_CALL_inst, .opcode = JMP  | 0b11 << 6},
    [CALLn_mn] = {check_CALL_inst, .opcode = JMPn | 0b11 << 6},
    [RET_mn]   = {check_RET_inst,  .opcode = JMP  | 0b10 << 6},
    [RETn_mn]  = {check_RET_inst,  .opcode = JMPn | 0b10 << 6},

    [CLF_mn] = {check_flag_inst, .opcode = EF | 0 << 6}, // set_or_clear = 0
    [SEF_mn] = {check_flag_inst, .opcode = EF | 1 << 6}, // set_or_clear = 1

    [CCF_mn] = {check_flag_inst, .opcode = EF | CARRY    << 4 | 0 << 6}, // set_or_clear = 0
    [CZF_mn] = {check_flag_inst, .opcode = EF | ZERO     << 4 | 0 << 6},
    [CVF_mn] = {check_flag_inst, .opcode = EF | OVERFLOW << 4 | 0 << 6},
    [SCF_mn] = {check_flag_inst, .opcode = EF | CARRY    << 4 | 1 << 6}, // set_or_clear = 1
    [SZF_mn] = {check_flag_inst, .opcode = EF | ZERO     << 4 | 1 << 6},
    [SVF_mn] = {check_flag_inst, .opcode = EF | OVERFLOW << 4 | 1 << 6},

    [ST_mn]   = {check_mem_inst,   .opcode = ST},
    [LD_mn]   = {check_mem_inst,   .opcode = LD},
    [POP_mn]  = {check_stack_inst, .opcode = LD | 0b11 << 6},
    [PUSH_mn] = {check_stack_inst, .opcode = ST | 0b11 << 6},

    [MOV_mn]  = {get_MOV_inst, .normal = MOVr, .extended = MOVe},
    [MOVr_mn] = {check_MOVr_inst, .opcode = MOVr},
    [MOVe_mn] = {check_MOVr_inst, .opcode = MOVe},

    // emit a or r0, r0
    [NOP_mn] = {check_NOP_inst, .opcode = ORr},
};

void get_opcode_mem_or_reg(instruction *inst, instruction_info *info)
{
    if (inst->op_1.op_type == REGISTER && inst->op_2.op_type == REGISTER)
    {
        // register-register instruction
        inst->opcode = info->reg | inst->op_1.value << 4 | inst->op_2.value << 6;
        return;
    }
    
    if (inst->op_1.op_type == REGISTER && (inst->op_2.op_type == ADDR_8 || inst->op_2.op_type == ADDR_16 || inst->op_2.op_type == MAR || inst->op_2.op_type == IMM_8))
    {
        inst->opcode = info->mem | inst->op_1.value << 4;
        
        // or in the address mode
        switch (inst->op_2.op_type) {
            // no ADDR_16 because it is the base case with 0x00
            case MAR:
                inst->opcode |= 1 << 6;
            break;
            case IMM_8:
                inst->opcode |= 1 << 7;
            break;
            case ADDR_8:
                inst->opcode |= 0b11 << 6;
            break;
            default:
                // the "default" is only here to prevent compiler warnings
            break;
        }
        return;
    }

    printf("Invalid operand types for an arithmetic instruction.\n");
    exit(EXIT_FAILURE);
}
void check_reg_inst(instruction *inst, instruction_info *info)
{
    if (inst->op_1.op_type == REGISTER && inst->op_2.op_type == REGISTER)
    {
        inst->opcode = info->opcode | inst->op_1.value << 4 | inst->op_2.value << 6;
        return;
    }

    printf("Invalid operand types for an arithmetic register instruction.\n");
    exit(EXIT_FAILURE);
}
void check_arr_mem_inst(instruction *inst, instruction_info *info)
{
    if (inst->op_1.op_type == REGISTER && (inst->op_2.op_type == ADDR_8 || inst->op_2.op_type == ADDR_16 || inst->op_2.op_type == MAR || inst->op_2.op_type == IMM_8))
    {
        inst->opcode = info->mem | inst->op_1.value << 4;
        
        // or in the address mode
        switch (inst->op_2.op_type) {
            // no ADDR_16 because it is the base case with 0x00
            case MAR:
                inst->opcode |= 1 << 6;
            break;
            case IMM_8:
                inst->opcode |= 1 << 7;
            break;
            case ADDR_8:
                inst->opcode |= 0b11 << 6;
            break;
            default:
                // the "default" is only here to prevent compiler warnings
            break;
        }
        return;
    }

    printf("Invalid operand types for an arithmetic memory instruction.\n");
    exit(EXIT_FAILURE);
}
void check_shift_inst(instruction *inst, instruction_info *info)
{
    if (inst->op_1.op_type == REGISTER && inst->op_2.op_type == UNUSED)
    {
        inst->opcode = info->opcode | inst->op_1.value << 4;
        return;
    }

    printf("Invalid operand types for shift instruction.\n");
    exit(EXIT_FAILURE);
}
void check_OOI_inst(instruction *inst, instruction_info *info)
{
    if (inst->op_1.op_type == REGISTER && inst->op_2.op_type == UNUSED)
    {
        inst->opcode = info->opcode | inst->op_1.value << 4;
        return;
    }

    printf("Invalid operand types for single operand instruction. (INC, DEC, NEG, NOT)\n");
    exit(EXIT_FAILURE); 
    return;
}
void check_branch_inst(instruction *inst, instruction_info *info)
{
    // TODO: a BZS can be executed with an extra flag argument "BZS c" but it will not have an impact

    if ((inst->op_1.op_type == ADDR_16 || inst->op_1.op_type == ADDR_8) && (inst->op_2.op_type == UNUSED || inst->op_2.op_type == FLAG))
    {
        inst->opcode = info->opcode;

        if (inst->op_1.op_type == ADDR_8)
        {
            // use relative addressing
            inst->opcode |= 0b01 << 6;
        }
        return;
    }
    
    printf("Invalid operand types a branch instruction.\n");
    exit(EXIT_FAILURE); 
    return;
}
void check_CALL_inst(instruction *inst, instruction_info *info)
{
    if ((inst->op_1.op_type == ADDR_16 || inst->op_1.op_type == ADDR_8) && (inst->op_2.op_type == UNUSED || inst->op_2.op_type == FLAG))
    {
        inst->opcode = info->opcode;

        if (inst->op_2.op_type == FLAG)
        {
            // or in the condition code
            switch(inst->op_2.value){
                case(CARRY):
                    inst->opcode |= CARRY << 4;
                    break;

                case(ZERO):
                    inst->opcode |= ZERO << 4;
                    break;

                case(OVERFLOW):
                    inst->opcode |= OVERFLOW << 4;
                    break;
            }
        }
        return;
    }

    printf("Invalid operand types a CALL instruction.\n");
    exit(EXIT_FAILURE); 
    return;
}
void check_RET_inst(instruction *inst, instruction_info *info)
{
    if ((inst->op_1.op_type == UNUSED || inst->op_1.op_type == FLAG) && inst->op_2.op_type == UNUSED)
    {
        inst->opcode = info->opcode;

        if (inst->op_1.op_type == FLAG)
        {
            // or in the condition code
            switch(inst->op_1.value){
                case(CARRY):
                    inst->opcode |= CARRY << 4;
                    break;

                case(ZERO):
                    inst->opcode |= ZERO << 4;
                    break;

                case(OVERFLOW):
                    inst->opcode |= OVERFLOW << 4;
                    break;
            }
        }
        return;
    }

    printf("Invalid operand types a RET instruction.\n");
    exit(EXIT_FAILURE); 
    return;
}
void check_flag_inst(instruction *inst, instruction_info *info)
{
    if ((inst->op_1.op_type == UNUSED || inst->op_1.op_type == FLAG) && inst->op_2.op_type == UNUSED) 
    {
        inst->opcode = info->opcode;

        if(inst->op_1.op_type == FLAG){
            switch(inst->op_2.value) 
            {
                case(CARRY):
                    inst->opcode |= CARRY << 4;
                    break;

                case(ZERO):
                    inst->opcode |= ZERO << 4;
                    break;

                case(OVERFLOW):
                    inst->opcode |= OVERFLOW << 4;
                    break;
            }
        }
        return;
    }
    
    printf("Invalid operand types a edit flag instruction (SCF, CZF ...).\n");
    exit(EXIT_FAILURE); 
    return;
}
void check_mem_inst(instruction *inst, instruction_info *info)
{
    if (inst->op_1.op_type == REGISTER && (inst->op_2.op_type == ADDR_8 || inst->op_2.op_type == ADDR_16 || inst->op_2.op_type == MAR))
    {
        inst->opcode = info->opcode | inst->op_1.value << 4;

        // or in the address mode
        switch (inst->op_2.op_type) {
            // no ADDR_16 because it is the base case with 0x00
            case MAR:
                inst->opcode |= 0b01 << 6;
            break;

            case ADDR_8:
                inst->opcode |= 0b10 << 6;
            break;

            default:
            // This default is only here to prevent compiler warnings
            break;
        }
        return;
    }

    printf("Invalid operand types a memory instruction.\n");
    exit(EXIT_FAILURE); 
    return;
}
void check_stack_inst(instruction *inst, instruction_info *info)
{
    if (inst->op_1.op_type == REGISTER && inst->op_2.op_type == UNUSED)
    {
        inst->opcode = info->opcode;
    }

    printf("Invalid operand types a stack instruction.\n");
    exit(EXIT_FAILURE); 
    return;
}
void check_MOVr_inst(instruction *inst, instruction_info *info)
{
    if (inst->op_1.op_type == REGISTER && inst->op_2.op_type == REGISTER)
    {
        inst->opcode = info->opcode | inst->op_1.value << 4 | inst->op_2.value;
    }

    printf("Invalid operand types MOVr instruction.\n");
    exit(EXIT_FAILURE);
    return;
}
void check_MOVe_inst(instruction *inst, instruction_info *info)
{
    return;
}
void get_MOV_inst(instruction *inst, instruction_info *info)
{
    return;
}
void check_NOP_inst(instruction *inst, instruction_info *info)
{
    // emit a or r0, r0
    inst->opcode = info->opcode | 0 << 4 | 0 << 6;
    return;
}

int cmpstr(const void *a, const void *b) 
{
    return strcmp((const char *)a, *(const char **)b);
}

void parse_mnemonic(char *mnemonic, instruction *instruction)
{
    if (instruction->op_1.op_type == LABEL && instruction->op_2.op_type == LABEL)
    {
        printf("A Instruction with double label does not exist.\n");
        exit(EXIT_FAILURE);
    }
    
    char **result = bsearch(mnemonic, mnemonics, sizeof(mnemonics) / sizeof(mnemonics[0]), sizeof(char *), cmpstr);
    
    if (result == NULL)
    {
        printf("Undefined mnemonic: \"%s\".\n", mnemonic);
        exit(EXIT_FAILURE);
    }

    // replace the label with values
    if (instruction->op_1.op_type == LABEL) {
        //get_label_val(&instruction->op_1);
    }
    if (instruction->op_2.op_type == LABEL) {
        //get_label_val(&instruction->op_2);
    }
    
    int mn_index = (int)(result - mnemonics);

    instruction_translation[mn_index].func(instruction, &instruction_translation[mn_index]);
    return;
}