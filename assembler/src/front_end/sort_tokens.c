#include <stdio.h>
#include <stdlib.h>

int cmpchr(const void *a, const void *b) {
    char c1 = *(const char*)a;
    char c2 = *(const char*)b;

    if (c1 < c2) return -1;
    else if (c1 > c2) return 1;
    else return 0;
}

int main()
{
    char chars[] = {'%', ':', '\n', ',', '(', ')', '+', '-', '*', '/', '&', '|', '^', '!'};
    size_t n = sizeof(chars) / sizeof(chars[0]);

    qsort(chars, n, sizeof(char), cmpchr);

    printf("{");
    for (size_t i = 0; i < n; i++)
    {
        if (i != n - 1)
            printf("'%c', ", chars[i]);
        else
            printf("'%c'};\n", chars[i]);
    }

    return 0;
}