#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef enum
{
    NOP, ADDr, SUBr, ADD, SUB, JMP, MOVr, MOVe, ST, LD // MOVe(xtended)
}instruction_names;

uint8_t gpr[4];
// bit0 -> zero flag, bit1 -> carry flag || mbr -> memory bank register
uint8_t flags, mbr;
// mar -> memory adress register
uint16_t pc, sp, mar;
uint8_t *memory; // is initialized later with malloc 


int8_t adress_mode_to_value_arithmetic(uint8_t opcode)
{
    // pc is pointing to the first byte of the instruction
    switch(opcode << 6) // select the tow bits containig the adress mode
    {
        case(0): // direct adressing
            return memory[(uint16_t)memory[pc + 2] << 8 | memory[pc + 1]];
            break;
        case(1):
            return memory[mar++];
            break;
        case(2): // immidiate
            return memory[pc + 1];
            break;
        case(3): // zero page
            return memory[memory[pc + 1]];
            break;
    }
    return 0;
}

uint16_t adress_mode_to_value_JMP(uint8_t opcode)
{
    // pc is pointing to the first byte of the instruction
    switch(opcode << 6) // select the tow bits containig the adress mode
    {
        case(0):
            return (uint16_t)memory[pc + 2] << 8 | memory[pc + 1];
            break;
        case(1):
            return pc + memory[pc + 1];
            break;
        case(2):
            sp += 2;
            return (uint16_t)memory[sp] << 8 | memory[sp - 1]; // return -> pop adress from stack // TODO: need to check for sp owerflow
            break;
        case(3):
            memory[sp--] = (uint8_t)pc; // push curr adress on the stack
            memory[sp--] = pc >> 8;
            return (uint16_t)memory[pc + 2] << 8 | memory[pc + 1];
            break;
    }
    return 0;
}

uint16_t adress_mode_to_value_MEM(uint8_t opcode)
{
    // pc is pointing to the first byte of the instruction
    switch(opcode << 6) // select the tow bits containig the adress mode
    {
        case(0):
            return (uint16_t)memory[pc + 2] << 8 | memory[pc + 1];
            break;
        case(1):
            return mar++;
            break;
        case(2):
            return memory[pc + 1];
            break;
        case(3):
            if(opcode == ST)
            {
                return sp--;
            }
            return sp++;
            break;
    }
    return 0;
}

void execute_instruction(uint8_t opcode, uint16_t immidiate)
{
    switch(opcode & 0xf)
    {
        case(NOP):
            break;
        case(ADDr):
            gpr[opcode << 4 & 0b11] = gpr[opcode << 4 & 0b11] + gpr[opcode << 6];
            pc += 1;
        case(SUBr):
            gpr[opcode << 4 & 0b11] = gpr[opcode << 4 & 0b11] - gpr[opcode << 6];
            pc += 1;
            break;
        case(ADD):
            gpr[opcode << 4 & 0b11] = gpr[opcode << 4 & 0b11] + adress_mode_to_value_arithmetic(opcode);
            break;
        case(SUB):
            gpr[opcode << 4 & 0b11] = gpr[opcode << 4 & 0b11] - adress_mode_to_value_arithmetic(opcode);
            break;
        case(JMP):
            // conditional is not supported yet
            pc = adress_mode_to_value_JMP(opcode);
            break;
        // not supported yet
        case(MOVr):
            break;
        case(MOVe):
            break;
    }
}

int check_if_bin_file(const char *filename)
{
    char *dot = strrchr(filename, '.');
    if (dot == NULL) return 0;
    return strcmp(dot + 1, "bin");
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("To many or to little arguments provided.");
        return 1;
    }

    if (check_if_bin_file(argv[1]))
    {
        printf("Either the file extension is not 'bin' or the file dont have an extension.");
        return 1;
    }

    FILE *programm = fopen(argv[1], "wb");

    // get file size
    fseek(programm, 0L, SEEK_END);
    long file_size = ftell(programm);
    rewind(programm);

    // alocate a buffer to work with the bytes more easily
    uint8_t *prog_buf = (uint8_t *)malloc(file_size);

    // read entire file
    fread(prog_buf, file_size, sizeof(uint8_t), programm);

    return 0;
}