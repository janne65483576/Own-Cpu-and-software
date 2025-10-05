#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int cmpstr(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int main()
{
    char *mnemonics[] = {"NOP", "ADD", "AND", "XOR", "SHR", "OR", "JMP", "BCC", "BCS", "BZC", "BZS", "BVS", "BVC", "MOV", "ST", "LD", "CALL", "RET", "POP", "PUSH"};
    qsort(mnemonics, sizeof(mnemonics) / sizeof(mnemonics[0]), sizeof(char *), cmpstr);

    printf("string:\n");
    printf("char *mnemonics[] = {");
    
    for(int i = 0; i < sizeof(mnemonics) / sizeof(mnemonics[0]); i++)
    {
        if (i != sizeof(mnemonics) / sizeof(mnemonics[0]) - 1)
        {
            printf("\"%s\", ", mnemonics[i]);
        }else 
        {
            printf("\"%s\"};\n", mnemonics[i]);
        }
    }

    typedef enum
    {
        ADD, SUB
    }mnemonic_enum;

    printf("enum:\n");
    printf("typedef enum\n{\n\t");
    
    for(int i = 0; i < sizeof(mnemonics) / sizeof(mnemonics[0]); i++)
    {
        if (i != sizeof(mnemonics) / sizeof(mnemonics[0]) - 1)
        {
            printf("%s_mn, ", mnemonics[i]);
        }else 
        {
            printf("%s_mn\n}mnemonic_enum;", mnemonics[i]);
        }
    }

    return 0;
}
