#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int cmpstr(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int check_if_double(char *arr[], int size) 
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (i == j)
            {
                continue;
            }

            if (strcmp(arr[i], arr[j]) == 0) 
            {
                printf("Duplicate string found: %s\n", arr[i]);
                return 1;
            }
        }
    }
    return 0;
}

int main()
{
    char *mnemonics[] = {"NOP", "ADD", "AND", "XOR", "SHR", "OR", "JMP", "BCC", "BCS", "BZC", "BZS", "BVS", "BVC", "MOV", "ST", "LD", "CALL", "RET", "POP", "PUSH", "XORr", "ORr", "ANDr", "ADDr", "SH", "MOVe", "INC", "DEC", "CLF", "NOT"};
    
    if (check_if_double(mnemonics, sizeof(mnemonics) / sizeof(mnemonics[0])) == 1)
    {
        return 1;
    }

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
