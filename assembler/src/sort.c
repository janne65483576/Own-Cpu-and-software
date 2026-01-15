#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int cmpchr(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int check_if_double(char *arr[], int size) 
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 1; j < size; j++)
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
    char *mnemonics[] = {
        // arithmetic instructions
        "ADD",
        "AND",
        "XOR",
        "OR",

        // arithmetic register instructions
        "ADDr",
        "ANDr",
        "XORr",
        "ORr",

        // arithmetic memory instructions
        "ADDm",
        "ANDm",
        "XORm",
        "ORm",

        // bit shift
        "SHL",
        "SHR",
        "ROR",
        "ROL",

        "NOP",
        "JMP",

        // conditional branches
        "BCC",
        "BCS",
        "BZC",
        "BZS",
        "BVS",
        "BVC",

        // data transfer
        "MOV",
        "MOVr",
        "MOVe",

        // memory instructions
        "ST",
        "LD",

        // stack instructions
        "CALL",
        "CALLn",
        "RET",
        "RETn",
        "POP",
        "PUSH",

        // flag instructions
        "CLF", // clear flag -> e.g. CLF z
        "SEF",
        
        "SCF",
        "SZF",
        "SVF",
        "CCF",
        "CZF",
        "CVF",
        
        // one operant instructions
        "NOT",
        "INC",
        "DEC",
        "NEG",

        };
    
    if (check_if_double(mnemonics, sizeof(mnemonics) / sizeof(mnemonics[0])) == 1)
    {
        return 1;
    }

    qsort(mnemonics, sizeof(mnemonics) / sizeof(mnemonics[0]), sizeof(char *), cmpchr);

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
