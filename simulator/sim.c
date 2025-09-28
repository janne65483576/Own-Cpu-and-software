#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef enum
{
    NOP, ADDr, SUBr, ADD, SUB, JMP, MOVr, MOVe, ST, LD // MOVe(xtended)
}instruction_names;

// typedef struct{
//     uint8_t gpr[4];
//     // bit0 -> zero flag, bit1 -> carry flag
//     uint8_t flags;
//     // mbr -> memory bank register
//     uint8_t mbr;
//     // mar -> memory adress register
//     uint16_t pc, sp, mar;
//     uint8_t *memory;
// }cpu_state;

int8_t gpr[4];
// bit0 -> zero flag, bit1 -> carry flag
uint8_t flags;
// mbr -> memory bank register
uint8_t mbr;
// mar -> memory adress register
uint16_t pc, sp, mar;
uint8_t *memory;

void print_bits(uint8_t value) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
    putchar('\n');
}

int8_t adress_mode_to_value_arithmetic(uint8_t opcode)
{
    // pc is pointing to the first byte of the instruction
    switch(opcode & 0x3) // select the tow bits containig the adress mode
    {
        case(0): // direct adressing
            pc += 2; // pointing to the last byte of the instruction
            return memory[(uint16_t)memory[pc] << 8 | memory[pc - 1]];
            break;
        case(1):
            return memory[mar++];
            break;
        case(2): // immidiate
            pc++;
            return memory[pc];
            break;
        case(3): // zero page
            pc++;
            return memory[memory[pc]];
            break;
    }
    return 0;
}

uint16_t adress_mode_to_value_JMP(uint8_t opcode)
{
    // pc is pointing to the first byte of the instruction
    switch(opcode & 0x3) // select the tow bits containig the adress mode
    {
        case(0):
            printf("adress: %d\n", (uint16_t)memory[pc + 2] << 8 | memory[pc + 1]);
            return (uint16_t)memory[pc + 2] << 8 | memory[pc + 1];
            break;
        case(1):
            return pc + memory[pc + 1];
            break;
        case(2):
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
    switch(opcode & 0x3) // select the tow bits containig the adress mode
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

void execute_instruction(uint8_t opcode)
{
    switch(opcode >> 4)
    {
        case(NOP):
            break;
        case(ADDr):
            gpr[opcode >> 2 & 0b11] = gpr[opcode >> 2 & 0b11] + gpr[opcode & 0b11];
            // set flags
            if(gpr[opcode >> 2 & 0b11] == 0)
            {
                flags |= 1;
            }
            if(gpr[opcode >> 2 & 0b11] == 0)
            {
                flags |= 1;
            }

            break;
        case(SUBr):
            gpr[opcode >> 2 & 0b11] = gpr[opcode >> 2 & 0b11] - gpr[opcode & 0b11];
            break;
        case(ADD):
            gpr[opcode >> 2 & 0b11] = gpr[opcode >> 2 & 0b11] + adress_mode_to_value_arithmetic(opcode);
            break;
        case(SUB):
            gpr[opcode >> 2 & 0b11] = gpr[opcode >> 2 & 0b11] - adress_mode_to_value_arithmetic(opcode);
            break;
        case(JMP):
            // conditional is not supported yet
            pc = adress_mode_to_value_JMP(opcode) - 1;
            break;
        // not supported yet
        case(MOVr):
            break;
        case(MOVe):
            break;
    }
    pc++;
}

void print_cpu_state()
{
    // TODO: add a memory printer and the other special purpose regs
    // Print general purpose registers
    for (int i = 0; i < 4; i++)
    {
        printf("r%d: %x\n", i, gpr[i]);
    }
}

void simulate_step(uint16_t start_adress, uint16_t end_adress)
{   
    pc = start_adress;
    while(pc <= end_adress)
    {   
        printf("Instruction ");
        print_bits(memory[pc]);
        printf("at Pc: %d\n", pc);

        execute_instruction(memory[pc]);

        print_cpu_state();
        printf("Press enter to continue\n");
        while (getchar() != '\n');
    }

    printf("Programm execution passed successfully!\n");
}

void simulate(uint16_t start_adress, uint16_t end_adress)
{   
    pc = start_adress;
    while(pc <= end_adress)
    {   
        printf("Instruction ");
        print_bits(memory[pc]);
        printf("at Pc: %d\n", pc);

        execute_instruction(memory[pc]);

        print_cpu_state();
    }

    printf("Programm execution passed successfully!\n");
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

    FILE *programm = fopen(argv[1], "rb");

    // get file size
    fseek(programm, 0L, SEEK_END);
    long file_size = ftell(programm);
    rewind(programm);

    memory = (uint8_t *)malloc(65536 * sizeof(uint8_t));

    // read entire file
    fread(memory, file_size, sizeof(uint8_t), programm);
    
    // this funktion uses the globaly defined state
    simulate(0, file_size - 1);

    return 0;
}