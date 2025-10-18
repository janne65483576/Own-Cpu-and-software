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
    OOI, ADD, ADDr, AND, ANDr, XOR, XORr, OR, ORr, SH, LD, ST, JMP, JMPn, MOV, MOVe
} opcodes;

// sorted alphabeticly
char *mnemonics[] = {"ADD", "ADDr", "AND", "ANDr", "BCC", "BCS", "BVC", "BVS", "BZC", "BZS", "CALL", "CLF", "DEC", "INC", "JMP", "JMPn", "LD", "MOV", "MOVe", "NOP", "NOT", "OR", "ORr", "POP", "PUSH", "RET", "ROL", "ROR", "SHL", "SHR", "ST", "XOR", "XORr"};

typedef enum
{
    ADD_mn, ADDr_mn, AND_mn, ANDr_mn, BCC_mn, BCS_mn, BVC_mn, BVS_mn, BZC_mn, BZS_mn, CALL_mn, CLF_mn, DEC_mn, INC_mn, JMP_mn, JMPn_mn, LD_mn, MOV_mn, MOVe_mn, NOP_mn, NOT_mn, OR_mn, ORr_mn, POP_mn, PUSH_mn, RET_mn, ROL_mn, ROR_mn, SHL_mn, SHR_mn, ST_mn, XOR_mn, XORr_mn
}mnemonic_enum;

typedef enum
{
    REGISTER, MAR, ADDR_16, IMM_8, ADDR_8, LABLE, UNUSED, FLAG
} operand_type;

typedef struct{
    // register or condition code for conditionl branch instructions
    uint8_t register_or_cond;
    uint8_t adress;
}translation;

const translation translation_table_arr[] =
{
    [ADD_mn] = {ADDr, ADD},
    [AND_mn] = {ANDr, AND},
    [XOR_mn] = {XORr, XOR},
    [OR_mn] = {ORr, OR},

    [ADDr_mn] = {ADDr, 0},
    [ANDr_mn] = {ANDr, 0},
    [XORr_mn] = {XORr, 0},
    [ORr_mn] = {ORr, 0},

    [BCS_mn] = {1, 0},
    [BZS_mn] = {2, 0},
    [BVS_mn] = {3, 0},
    [BCC_mn] = {1, 0},
    [BZC_mn] = {2, 0},
    [BVC_mn] = {3, 0},

    [SHL_mn] = {SH | 0b00 << 6, 0}, // rotate = 0, direction = 0
    [SHR_mn] = {SH | 0b01 << 6, 0}, // rotate = 0, direction = 1
    [ROL_mn] = {SH | 0b10 << 6, 0}, // rotate = 1, direction = 0
    [ROR_mn] = {SH | 0b11 << 6, 0}, // rotate = 1, direction = 1

    [POP_mn]  = {LD | 3 << 6, 0},
    [PUSH_mn] = {ST | 3 << 6, 0},

    [JMP_mn]  = {JMP, 0},
    [JMPn_mn] = {JMPn, 0},
};

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

void shift_to_opcode(int mn_index, instruction *instruction)
{
    if (instruction->op_1.op_type == REGISTER && instruction->op_2.op_type == UNUSED)
    {
        instruction->opcode = translation_table_arr[mn_index].register_or_cond | instruction->op_1.value << 4;
        return;
    }

    printf("Invalid operand types for shift instruction.\n");
    exit(EXIT_FAILURE);
}

void jump_to_opcode(int mn_index, instruction *instruction)
{ 
    if (instruction->op_2.op_type == UNUSED || instruction->op_2.op_type == FLAG)
    {
        instruction->opcode = translation_table_arr[mn_index].register_or_cond;
        
        if (instruction->op_2.op_type == FLAG)
        {
            instruction->opcode |= instruction->op_2.value << 4;
        }

        if (instruction->op_1.op_type == ADDR_8)
        {
            instruction->opcode |= 1 << 6;
            return;
        }
        
        if (instruction->op_1.op_type == ADDR_16)
        {
            return;
        }
    }

    printf("Invalid operand types for JMP instruction.\n");
    exit(EXIT_FAILURE);
}

void branch_to_opcode(int mn_index, instruction *instruction)
{
    int conditinal_code = translation_table_arr[mn_index].register_or_cond;
    
    if (instruction->op_2.op_type == UNUSED)
    {
        if (instruction->op_1.op_type == ADDR_8)
        {
            instruction->opcode |= JMP | 1 << 6 | conditinal_code << 4;
            return;
        }
        
        if (instruction->op_1.op_type == ADDR_16)
        {
            instruction->opcode = JMP | conditinal_code << 4;
            return;
        }
    }

    printf("Invalid operand types for conditinal JMP instruction.\n");
    exit(EXIT_FAILURE);
}

void branch_not_to_opcode(int mn_index, instruction *instruction)
{
    int conditinal_code = translation_table_arr[mn_index].register_or_cond;

    if (instruction->op_2.op_type == UNUSED)
    {
        if (instruction->op_1.op_type == ADDR_8)
        {
            instruction->opcode |= JMPn | 1 << 6 | conditinal_code << 4;
            return;
        }
        
        if (instruction->op_1.op_type == ADDR_16)
        {
            instruction->opcode = JMPn | conditinal_code << 4;
            return;
        }
    }

    printf("Invalid operand types for conditinal JMP instruction.\n");
    exit(EXIT_FAILURE);
}

void ooi_to_opcode(int mn_index, instruction *instruction)
{
    if (instruction->op_2.op_type == UNUSED)
    {
        if (instruction->op_1.op_type == REGISTER)
        {
            instruction->opcode = instruction->op_1.value << 4;

            // set the mode
            switch (mn_index) 
            {
                case INC_mn:
                    instruction->opcode |= 1 << 6;
                break;
                case DEC_mn:
                    instruction->opcode |= 2 << 6;
                break;
                case NOT_mn:
                    instruction->opcode |= 3 << 6;
            }
            return;
        } else if (instruction->op_1.op_type == FLAG)
        {
            instruction->opcode = instruction->op_1.value << 4;
            return;
        }
    }

    printf("Invalid operand types for an OOI instruction.\n");
    exit(EXIT_FAILURE);
}

void arithmetic_to_opcode(int mn_index, instruction *instruction)
{
    if (instruction->op_1.op_type == REGISTER && instruction->op_2.op_type == REGISTER)
    {
        // construct corect maschine code
        instruction->opcode = translation_table_arr[mn_index].register_or_cond | instruction->op_1.value << 4 | instruction->op_2.value << 6;
        return;
    }
    
    // TODO replace the unequal
    if (instruction->op_1.op_type == REGISTER && instruction->op_2.op_type != REGISTER && instruction->op_2.op_type != UNUSED)
    {
        instruction->opcode = translation_table_arr[mn_index].adress | instruction->op_1.value << 4;
        
        switch (instruction->op_2.op_type) {
            // no ADDR_16 because it is the base case with 0x00
            case MAR:
                instruction->opcode |= 1 << 6;
            break;
            case IMM_8:
                instruction->opcode |= 1 << 7;
            break;
            case ADDR_8:
                instruction->opcode |= 0b11 << 6;
            break;
            default:
                // this is only here to prevent compiler warnings
            break;
        }
        return;
    }

    printf("Invalid operand types for an arithmetic instruction.\n");
    exit(EXIT_FAILURE);
}

void arithmetic_register_to_opcode(int mn_index, instruction *instruction)
{
    if (instruction->op_1.op_type == REGISTER && instruction->op_2.op_type == REGISTER)
    {
        // construct corect maschine code
        instruction->opcode = translation_table_arr[mn_index].register_or_cond | instruction->op_1.value << 4 | instruction->op_2.value << 6;
        return;
    }

    printf("Invalid operand types for an arithmetic register instruction.\n");
    exit(EXIT_FAILURE);
}

void stack_to_opcode(int mn_index, instruction *instruction)
{
    if (instruction->op_1.op_type == REGISTER && instruction->op_2.op_type == UNUSED)
    {   
        instruction->opcode = translation_table_arr[mn_index].register_or_cond;
        instruction->opcode |= instruction->op_1.value << 4;
        return;
    }

    printf("Invalid operand types for an arithmetic register instruction.\n");
    exit(EXIT_FAILURE);
}

void memory_to_opcode(int mn_index, instruction *instruction)
{
    if (instruction->op_1.op_type == REGISTER)
    {
        instruction->opcode = mn_index == LD_mn ? LD : ST;
        instruction->opcode |= instruction->op_1.value << 4;

        if (instruction->op_1.op_type == ADDR_8)
        {
            instruction->opcode |= 2 << 6;
        }

        if (instruction->op_1.op_type == MAR)
        {
            instruction->opcode |= 1 << 6;
        }
        return;
    }

    printf("Invalid operand types for an arithmetic register instruction.\n");
    exit(EXIT_FAILURE);
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
        // could be a lable
        exit(EXIT_FAILURE);
    }

    // get the index of the mnemonic
    switch((int)(result - mnemonics))
    {
        case NOP_mn:
            if (instruction->op_1.op_type != UNUSED || instruction->op_2.op_type != UNUSED) {
                printf("A NOP instruction is does not have operands\n");
                exit(EXIT_FAILURE);
            }
            // emit a ORr r0, r0
            instruction->opcode = ORr;

        case ADD_mn:
        case AND_mn:
        case XOR_mn:
        case OR_mn:
            arithmetic_to_opcode(result - mnemonics, instruction);
        break;

        case JMP_mn:
        case JMPn_mn:
            jump_to_opcode(result - mnemonics, instruction);
        break;

        case BCS_mn:
        case BZS_mn:
        case BVS_mn:
            branch_to_opcode(result - mnemonics, instruction);
        break;

        case BCC_mn:
        case BZC_mn:
        case BVC_mn:
            branch_not_to_opcode(result - mnemonics, instruction);
        break;
        
        case INC_mn:
        case DEC_mn:
        case CLF_mn:
        case NOT_mn:
            ooi_to_opcode(result - mnemonics, instruction);
        break;

        case ADDr_mn:
        case ANDr_mn:
        case ORr_mn:
        case XORr_mn:
            arithmetic_register_to_opcode(result - mnemonics, instruction);
        break;
        
        case SHL_mn:
        case SHR_mn:
        case ROL_mn:
        case ROR_mn:
            shift_to_opcode(result - mnemonics, instruction);
        break;

        case POP_mn:
        case PUSH_mn:
            stack_to_opcode(result - mnemonics, instruction);
        
    }
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

    // used MAX_INSTRUCTIONS is not very good some instructions have 2 or 3 bytes
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
            printf("The MAX_INTRUCTIONS limit is set to low.");
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