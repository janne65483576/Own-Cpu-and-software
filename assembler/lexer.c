#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define MAX_MNEMONIC_LEN 3
#define MAX_OPPERNAD_LEN 3 // + 1 because null termination

typedef enum
{
    NOP, ADDr, SUBr, ADD, SUB, JMP, MOVr, MOVe, ST, LD // MOVe(xtended)
} opcodes;

typedef struct{
    uint16_t value;
    char *lable; // if lable == NULL, use value
} operand;

typedef struct{
    opcodes opcode;
    uint8_t adress_mode;
    operand op_1;
    operand op_2;

    uint16_t adress;
    uint16_t imm_adress;
} instruction;

typedef struct{
    char *mnemonic;
    char *op_1;
    char *op_2;
} tmp_instruction;

void print_instruction(const instruction *instr) {
    if (!instr) {
        printf("Error: instruction pointer is NULL!\n");
        return;
    }

    printf("Instruction:\n");
    printf("  Opcode       : 0x%02X (%u)\n", instr->opcode, instr->opcode);
    printf("  Address Mode : 0x%02X (%u)\n", instr->adress_mode, instr->adress_mode);
    printf("  Register 1   : 0x%02X (%u)\n", instr->op_1.value, instr->op_1.value);
    printf("  Register 2   : 0x%02X (%u)\n", instr->op_2.value, instr->op_2.value);
    printf("  Address      : 0x%04X (%u)\n", instr->adress, instr->adress);
    printf("  Imm Address  : 0x%04X (%u)\n", instr->imm_adress, instr->imm_adress);
}

uint8_t mnemonic_to_opcode(char *mnemonic, bool is_register)
{   
    // TODO: use a better algorithm 
    if (strcmp(mnemonic, "NOP"))
    {
        return NOP;
    }else if (strcmp(mnemonic, "ADD"))
    {
        return is_register ? ADDr : ADD;
    }else if (strcmp(mnemonic, "JMP") || strcmp(mnemonic, "JC") || strcmp(mnemonic, "JZ"))
    {
        return JMP;
    }else if (strcmp(mnemonic, "ST")) {
        return ST;
    }else if (strcmp(mnemonic, "LD")) {
        return LD;
    }else if (strcmp(mnemonic, "MOV")) {
        return MOVr;
    }
    printf("Invalid mnemonic %s", mnemonic);
    exit(EXIT_FAILURE);
}

void parse_operand(operand *operand_struct, char *operand_text)
{
    for (int i = 0; operand_text[i] != '\0'; i++)
    {
        printf("%c", operand_text[i]);
    }
}

void parse_instruction(instruction *instruction, char *instruction_text)
{
    char mnemonic[MAX_MNEMONIC_LEN];

    int operand_count = 0;
    int i = 0;
    int j = 0;

    //TODO: think about adding a loop which truncates the string at spaces

    for (; instruction_text[i] != ' ' && i < MAX_MNEMONIC_LEN; i++)
    {
        mnemonic[i] = instruction_text[i];
    }

    // skip zeros and commas
    while(instruction_text[i] == ' ' || instruction_text[i] == ',')
    {
        i++;
    }

    // add the first operand to op_1_str. Quit the loop if a comma or a space is found.
    // also if the MAX_OPPERNAD_LEN or the end of instrcution text is reached
    char op_1_str[MAX_OPPERNAD_LEN];

    while (instruction_text[i] != ' ' && instruction_text[i] != ',' && j < MAX_OPPERNAD_LEN && instruction_text[i] != '\0') {
        op_1_str[j++] = instruction_text[i++];
    }

    if (j >= MAX_OPPERNAD_LEN)
    {
        op_1_str[j - 1] = '\0';
        printf("Operand %s... is to long.", op_1_str);
        exit(EXIT_FAILURE);
    }
    
    // test if the the op_1_str has any data
    if (j == 0)
    {
        goto end;
    }

    operand_count++;
    op_1_str[j] = '\0';
    j = 0;

    // skip zeros and commas
    while(instruction_text[i] == ' ' || instruction_text[i] == ',')
    {
        i++;
    }

    // add the first operand to op_1_str. Quit the loop if a comma or a space is found.
    // also if the MAX_OPPERNAD_LEN or the end of instrcution text is reached
    char op_2_str[MAX_OPPERNAD_LEN];

    while (instruction_text[i] != ' ' && instruction_text[i] != ',' && j < MAX_OPPERNAD_LEN && instruction_text[i] != '\0') {
        op_2_str[j++] = instruction_text[i++];
    }
    
    if (j >= MAX_OPPERNAD_LEN)
    {
        op_2_str[j - 1] = '\0';
        printf("Operand %s... is to long.", op_2_str);
        exit(EXIT_FAILURE);
    }

    // test if the the op_2_str has any data
    if (j == 0)
    {
        goto end;
    }
    
    operand_count++;
    op_2_str[j] = '\0';
    j = 0;

    end:
    switch(operand_count)
    {
        case 0:
            printf("0 operands\n");
        break;

        case 1:
        {
            operand op_1;
            //parse_operand(&op_1, op_1_str);
            printf("1 operand: %s\n", op_1_str);
            break;
        }

        case 2:
        {
            operand op_1;
            operand op_2;
            //parse_operand(&op_1, op_1_str);
            //parse_operand(&op_2, op_2_str)
            printf("2 operands: %s and %s\n", op_1_str, op_2_str);
            break;
        }
    }

    return;
}

typedef enum {
    STATE_CODE,          // normal code
    STATE_ONE_COMMENT,   // in singel line comment
    STATE_MULTI_COMMENT, // int multi line comment
    STATE_MULTI_END      // the first char of end seqeuz "/*" is found
} ParserState;

char *lexer(char *assembly, long asm_size, long *tokens_size)
{   
    char *instruction_text = "INC r11, r11";

    instruction instruction_struct;
    parse_instruction(&instruction_struct, instruction_text);

    //print_instruction(&instruction_struct);


    // char *tokens = (char *)malloc(asm_size * sizeof(char));
    // long token_ptr = 0;
    // char c;

    // ParserState state = STATE_CODE;

    // for (long i = 0; i < asm_size; i++) {
    //     c = assembly[i];

    //     switch (state) {
    //         case STATE_ONE_COMMENT:
    //             if (c == '\n') {
    //                 state = STATE_CODE;
    //             }
    //             continue;

    //         case STATE_MULTI_COMMENT:
    //             if (c == '*') {
    //                 state = STATE_MULTI_END;
    //             }
    //             continue;

    //         case STATE_MULTI_END:
    //             if (c == '/') {
    //                 state = STATE_CODE;
    //             } else if (c != '*') {
    //                 state = STATE_MULTI_COMMENT;
    //             }
    //             continue;

    //         case STATE_CODE:
    //             // start singel line comment
    //             if (c == '/' && i + 1 < asm_size && assembly[i + 1] == '/') {
    //                 state = STATE_ONE_COMMENT;
    //                 i++;
    //                 continue;
    //             }

    //             // start multi line comment
    //             if (c == '/' && i + 1 < asm_size && assembly[i + 1] == '*') {
    //                 state = STATE_MULTI_COMMENT;
    //                 i++;
    //                 continue;
    //             }

    //             tokens[token_ptr++] = c;
    //             break;
    //     }
    // }
    // tokens[token_ptr++] = '\n';
    // tokens = realloc(tokens, token_ptr);
    // *tokens_size = token_ptr;
    // return tokens;
    return NULL;
    *tokens_size = 0;
}