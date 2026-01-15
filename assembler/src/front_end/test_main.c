#include <stdio.h>
#include "lexer.h"
#include <stdlib.h>

#include "lexer/lexer_debug.h"

long file_length(FILE *f)
{
    long size;

    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, 0, SEEK_SET);

    return size;
}

int main()
{
    // read in a file
    FILE *file = fopen("test_file.txt", "rb");
    long len = file_length(file);
    char *text_buffer = malloc(len);

    fread(text_buffer, 1, len, file);

    TokenList tokens;

    if (tokenize(&tokens, text_buffer, len) == -1)
    {
        printf("enncounterd an error\n");
        fclose(file);
        free(text_buffer);
        return -1;
    }

    printTokenList(&tokens);
    
    return 0;
}