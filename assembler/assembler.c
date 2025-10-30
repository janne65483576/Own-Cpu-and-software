#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <strings.h>

#define MAX_MNEMONIC_LEN 10
#define MAX_OPPERNAD_LEN 20 // + 1 because null termination
#define MAX_OPERAND 3

#define MAX_INSTRUCTIONS 100

// real opcodes 
// OOI = one operand instruction
typedef enum
{
    OOI, ADDm, ADDr, ANDm, ANDr, XORm, XORr, ORm, ORr, SH, LD, ST, JMP, JMPn, MOVr, MOVe, EF = 0b10001111
} opcodes;

// sorted alphabeticly -> use the sort.c
char *mnemonics[] = {"ADD", "ADDm", "ADDr", "AND", "ANDm", "ANDr", "BCC", "BCS", "BVC", "BVS", "BZC", "BZS", "CALL", "CALLn", "CCF", "CLF", "CVF", "CZF", "DEC", "INC", "JMP", "LD", "MOV", "MOVe", "MOVr", "NEG", "NOP", "NOT", "OR", "ORm", "ORr", "POP", "PUSH", "RET", "RETn", "ROL", "ROR", "SCF", "SEF", "SHL", "SHR", "ST", "SVF", "SZF", "XOR", "XORm", "XORr"};

typedef enum
{
	ADD_mn, ADDm_mn, ADDr_mn, AND_mn, ANDm_mn, ANDr_mn, BCC_mn, BCS_mn, BVC_mn, BVS_mn, BZC_mn, BZS_mn, CALL_mn, CALLn_mn, CCF_mn, CLF_mn, CVF_mn, CZF_mn, DEC_mn, INC_mn, JMP_mn, LD_mn, MOV_mn, MOVe_mn, MOVr_mn, NEG_mn, NOP_mn, NOT_mn, OR_mn, ORm_mn, ORr_mn, POP_mn, PUSH_mn, RET_mn, RETn_mn, ROL_mn, ROR_mn, SCF_mn, SEF_mn, SHL_mn, SHR_mn, ST_mn, SVF_mn, SZF_mn, XOR_mn, XORm_mn, XORr_mn
}mnemonic_enum;

typedef enum
{
    REGISTER, MAR, ADDR_16, IMM_8, ADDR_8, LABLE, UNUSED, FLAG
} operand_type;

typedef struct{
    int16_t value;
    operand_type op_type;
    char *lable; // only used if operand_type == LABLE
} operand;

typedef struct{
    opcodes opcode;
    operand op_1;
    operand op_2;

    uint16_t adress;
} instruction;

typedef enum
{
    CARRY = 1, ZERO, OVERFLOW
} flags;

typedef struct{
    // register or condition code for conditionl branch instructions
    uint8_t register_or_cond;
    uint8_t adress;
}translation;

typedef struct instruction_info instruction_info;

typedef void (*get_opcode_func)(instruction *, instruction_info *);

struct instruction_info {
    get_opcode_func func;
    union {
        uint8_t opcode; // for mnmnomics which only corresponds to one opcode
        struct {uint8_t reg; uint8_t mem;};
        struct {uint8_t normal; uint8_t extended;}; // for the MOV instruction
    };
};

void print_bits(uint8_t value) 
{
    for (int i = 7; i >= 0; i--) 
    {
        printf("%d", (value >> i) & 1);
    }
    printf("\n");
}

void print_instruction(const instruction *instr) {
    if (!instr) {
        printf("Error: instruction pointer is NULL!\n");
        return;
    }

    //printf("opcode: %x\n", instr->opcode);
    //putc('\n', stdout);
    if (instr->op_1.op_type != UNUSED)
    {
        printf("first operand:\n");
        printf("value: %d\n", instr->op_1.value);
        printf("operand_type: %d\n", instr->op_1.op_type);
    }
    putc('\n', stdout);
    if (instr->op_2.op_type != UNUSED)
    {
        printf("second operand:\n");
        printf("value: %d\n", instr->op_2.value);
        printf("operand_type: %d\n", instr->op_2.op_type);
    }
    //printf("adress: %d\n", instr->adress);
    puts("------------------");
}

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

    [JMP_mn] = {check_branch_inst, .opcode = JMP}, // this instruction is unconditionaly
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
        
        // or in the adress mode
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
        
        // or in the adress mode
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

    printf("Invalid operand types for singel operand instruction. (INC, DEC, NEG, NOT)\n");
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
            // use relative adressing
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

        // or in the adress mode
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

void check_MOVe_inst(instruction *inst, instruction_info *info){
    return;
}

void get_MOV_inst(instruction *inst, instruction_info *info){
    return;
}

void check_NOP_inst(instruction *inst, instruction_info *info)
{
    if (inst->op_1.op_type != UNUSED || inst->op_2.op_type != UNUSED) 
    {
        printf("A NOP instruction is does not have operands\n");
        exit(EXIT_FAILURE);
    }

    // emit a ORr r0, r0
    inst->opcode = info->opcode;

    printf("NOP\n");
}

int cmpstr(const void *a, const void *b) {
    return strcmp((const char *)a, *(const char **)b);
}

uint8_t mnemonic_to_opcode(char *mnemonic, instruction *instruction)
{    
    if (instruction->op_1.op_type == LABLE && instruction->op_2.op_type == LABLE)
    {
        printf("A Instruction with double lable does not exist.\n");
        return 1;
    }
    
    char **result = bsearch(mnemonic, mnemonics, sizeof(mnemonics) / sizeof(mnemonics[0]), sizeof(char *), cmpstr);
    
    if(result == NULL)
    {
        printf("lable: %s (terminate -> not implemented yet)\n", mnemonic);
        exit(EXIT_FAILURE);
    }
    
    int mn_index = (int)(result - mnemonics);

    instruction_translation[mn_index].func(instruction, &instruction_translation[mn_index]);
    
    return 0;
}

void parse_operand(operand *operand_struct, char *operand_text)
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
            printf("Invlaid operand: %s\n", operand_text);
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
    bool is_adress = false;
    int start_off = 0;
    long val;

    if (operand_text[0] == '%')
    {
        start_off++;
        is_adress = true;
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
            printf("Invlaid operand: %s\n", operand_text);
            exit(EXIT_FAILURE);
        }

        operand_struct->op_type = LABLE;

        int len = strlen(operand_text);
        operand_struct->lable = (char *)malloc(len);
        memcpy(operand_struct->lable, operand_text + 1, len);
        return;
    }

    if (val < INT16_MIN || val > UINT16_MAX)
    {
        printf("Too small or to big number: %lx\n", val);
        exit(EXIT_FAILURE);
    }

    if (val > INT8_MIN && val < INT8_MAX)
    {
        operand_struct->op_type = is_adress ? ADDR_8 : IMM_8;
        operand_struct->value = val & 0xff;
        return;
    }
    
    if ((val < INT8_MIN || val > UINT8_MAX) && !is_adress) {
        printf("A Imidiate value which isent an andress can not be greater or smaller than 8 bits. Value: %ld\n", val);
        exit(EXIT_FAILURE);
    }
    
    operand_struct->op_type = ADDR_16;
    operand_struct->value = val & 0xffff;
}

void parse_instruction(instruction *instruction, char *instruction_text)
{
    char *operands[MAX_OPERAND];
    int operand_count = 0;

    operands[0] = strtok(instruction_text, " ,");

    for (int i = 1; i < MAX_OPERAND; i++)
    {
        operands[i] = strtok(NULL, " ,");
        if(operands[i] == NULL)
        {
            break;
        }
        operand_count++;
    }

    instruction->op_1.op_type = UNUSED;
    instruction->op_2.op_type = UNUSED;

    if (operand_count >= 1) 
    {
        parse_operand(&instruction->op_1, operands[1]);
        if (operand_count >= 2)
        {
            parse_operand(&instruction->op_2, operands[2]);
        }
    }

    mnemonic_to_opcode(operands[0], instruction);
}

uint8_t *linker(instruction *instructions, int inst_count, long *bin_size_final){
    int bin_i = 0;

    // use MAX_INSTRUCTIONS is not very good some instructions have 2 or 3 bytes
    uint8_t *bin = (uint8_t *)malloc(MAX_INSTRUCTIONS);
    int max_bin_size = MAX_INSTRUCTIONS;
    int bin_size = 0;

    for (int inst_i = 0; inst_i < inst_count; inst_i++)
    {
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
        }

        if(instructions[inst_i].op_2.op_type == ADDR_16)
        {
            bin_size += 3;
            printf("%d\n", instructions[inst_i].op_2.value);
            // store in little endian
            bin[bin_i++] = instructions[inst_i].op_2.value;
            bin[bin_i++] = instructions[inst_i].op_2.value >> 8;
            continue;
        }

        bin_size++;
    }

    *bin_size_final = bin_size;
    return bin;
}

uint8_t *get_bin(FILE *assembly_file, long *bin_size)
{   
    instruction instructions[MAX_INSTRUCTIONS];
    int instruction_i = 0;
    size_t len = 0;
    char *line = NULL;
    int nread = 0;

    while ((nread = getline(&line, &len, assembly_file)) != -1)
    {
        if (nread > 0 && line[nread - 1] == '\n') {
            line[nread - 1] = '\0';
        }
        
        if (line[0] == '\0')
        {
            goto skip;
        }

        // check if direktive
        for(int i = 0; line[i] != '\0'; i++)
        {
            if(line[i] == '.')
            {
                //directive
                break;
            }
            
            if(line[i] != ' ')
            {
                // no directive
                break;
            }
        }

        if (instruction_i >= MAX_INSTRUCTIONS)
        {
            printf("The MAX_INTRUCTIONS limit is to less.");
            free(line);
            exit(EXIT_FAILURE);
        }

        printf("line: %s\n", line);

        instruction curr_instruction;
        parse_instruction(&curr_instruction, line);

        instructions[instruction_i++] = curr_instruction;

        skip:
            free(line);
            line = NULL;
    }

    return linker(instructions, instruction_i, bin_size);
}

int check_if_asm_file(const char *filename)
{
    char *dot = strrchr(filename, '.');
    if (dot == NULL) return 0;
    return strcmp(dot + 1, "asm");
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("To many or to little arguments provided.");
        return 1;
    }

    if (check_if_asm_file(argv[1]))
    {
        printf("Either the file extension is not 'bin' or the file dont have an extension.");
        return 1;
    }

    FILE *assembly = fopen(argv[1], "rb");

    long bin_size;
    uint8_t *bin = get_bin(assembly, &bin_size);
    
    FILE *bin_file = fopen("prog.bin", "wb");
    fwrite(bin, sizeof(uint8_t), bin_size, bin_file);


    free(bin);
    fclose(assembly);

    return 0;
}