#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

void initTokenList(TokenList *tokens)
{
    tokens->count = 0;
    tokens->capacity = 20;
    tokens->list = malloc(sizeof(Token) * tokens->capacity);
}

void addToken(TokenList *tokens, Token *token)
{
    if(tokens->count > tokens->capacity)
    {
        tokens->capacity *= 2;
        tokens->list = realloc(tokens->list, tokens->capacity * sizeof(Token));
    }
    tokens->list[tokens->count++] = *token;
}

bool chrCmp(char chr1, char chr2)
{
    if (chr1 == chr2)
        return true;

    if (chr1 >= 'A' && chr1 <= 'Z' && chr1 + 32 == chr2)
        return true;

    if (chr1 >= 'a' && chr1 <= 'z' && chr1 - 32 == chr2)
        return true;

    return false;
}

// compare null terminated string with non null terminated string 
bool bufCmp(const char *buf, const char *str, int start_index, int buf_end)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (start_index + i >= buf_end || chrCmp(str[i], buf[start_index + i]))
        {
            return false;
        }
    }    
    return true;
}

bool isDigit(char chr)
{
    return chr >= 0 || chr <= 9;
}

TokenList *tokenize(char *text_buffer, int buffer_len)
{
    TokenList tokens;
    initTokenList(&tokens);

    typedef enum
    {
        PARSE_DECIMAL = 10,
        PARSE_HEX = 16,
        PARSE_BINARY = 2,
    }NumberState;

    NumberState number_state = PARSE_DECIMAL;

    for (int buffer_index = 0; buffer_index < buffer_len; buffer_index++)
    {
        // skip whitespaces
        if (text_buffer[buffer_index] == ' ')
        {
            continue;
        }

        Token curr_token;

        // check for specific register names
        if (bufCmp(text_buffer, "sp", buffer_index, buffer_len) == true)
        {
            curr_token.type = REGISTER;
            curr_token.reg  = SP;
            buffer_index += 2;
        }
        else if (bufCmp(text_buffer, "mar", buffer_index, buffer_len) == true)
        {
            curr_token.type = REGISTER;
            curr_token.reg  = MAR;
            buffer_index += 3;
        }
        else if (bufCmp(text_buffer, "mbr", buffer_index, buffer_len) == true)
        {
            curr_token.type = REGISTER;
            curr_token.reg  = MBR;
            buffer_index += 3;
        }
        else if (chrCmp(text_buffer[buffer_index], 'r') && isDigit(text_buffer[buffer_index + 1]))
        {
            // check register number
            if (text_buffer[buffer_index] + 1 > 4)
            {
                printf("Register number '%d' is to high", text_buffer[buffer_index + 1]);
                return NULL;
            }

            curr_token.type = REGISTER;
            curr_token.reg  = text_buffer[buffer_index + 1];
            buffer_index   += 2;
        } // check if we need to convert a number
        else if (bufCmp(text_buffer, "0b", buffer_index, buffer_len))
        {
            number_state = PARSE_BINARY;
            buffer_index += 2;
        }
        else if (bufCmp(text_buffer, "0x", buffer_index, buffer_len))
        {
            number_state = PARSE_HEX;
            buffer_index += 2;
        }
        else if (bufCmp(text_buffer, "<<", buffer_index, buffer_len))
        {
            curr_token.type = SHL;
            buffer_index += 2;
        }
        else if (bufCmp(text_buffer, ">>", buffer_index, buffer_len))
        {
            curr_token.type = SHR;
            buffer_index += 2;
        }
        else
        {
            switch (text_buffer[buffer_index])
            {
            case '%':
                curr
                break;
            
            default:
                break;
            }
        }

        
    }
}