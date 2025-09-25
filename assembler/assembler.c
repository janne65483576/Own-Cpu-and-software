#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "lexer.h"

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

    // get file size
    fseek(assembly, 0L, SEEK_END);
    long file_size = ftell(assembly);
    rewind(assembly);

    // alocate a buffer to work with the bytes more easily
    char *assembly_buf = (char *)malloc(file_size);

    // read entire file
    fread(assembly_buf, file_size, sizeof(uint8_t), assembly);

    long tokens_size;
    char *tokens = lexer(assembly_buf, file_size, &tokens_size);

    for(int i = 0; i < tokens_size; i++)
    {
        printf("%c", tokens[i]);
    }

    free(tokens);
    free(assembly_buf);
    return 0;
}