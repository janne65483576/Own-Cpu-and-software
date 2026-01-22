#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include "lexer.h"

void initTokenList(TokenList *tokens)
{
    tokens->count = 0;
    tokens->index = 0;
    tokens->capacity = 20;
    tokens->list = malloc(sizeof(Token) * tokens->capacity);
}

void freeTokenList(TokenList *tokens)
{
    if (tokens == NULL) return;
    
    for (int i = 0; i < tokens->count; i++)
    {
        if ((tokens->list[i].type == TOKEN_STRING ||tokens->list[i].type == TOKEN_DIRECTIVE || tokens->list[i].type == TOKEN_LABEL) && (tokens->list[i].str != NULL))
        {
            free(tokens->list[i].str);
        }
    }

    free(tokens->list);
}

void addToken(TokenList *tokens, Token *token)
{
    if (tokens->count >= tokens->capacity)
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
    int i = 0;

    // compare characters
    while (str[i] != '\0')
    {
        if (start_index + i >= buf_end)
            return false;

        if (!chrCmp(str[i], buf[start_index + i]))
            return false;

        i++;
    }

    return true;
}

bool isDigit(char chr)
{
    return chr >= '0' && chr <= '9';
}

int tokenize(TokenList* tokens, char *text_buffer, int buffer_len)
{
    initTokenList(tokens);

    typedef enum
    {
        PARSE_DECIMAL = 10,
        PARSE_HEX = 16,
        PARSE_BINARY = 2,
    }NumberState;

    NumberState number_state = PARSE_DECIMAL;
    bool is_directive = false;
    int buffer_index = 0;

    while (buffer_index < buffer_len)
    {
        // skip whitespaces
        while (buffer_index < buffer_len && (text_buffer[buffer_index] == ' ' || text_buffer[buffer_index] == '\t' || text_buffer[buffer_index] == '\r')) 
        {
            buffer_index++;
        }

        if (buffer_index >= buffer_len) break;

        // skip comments
        if (text_buffer[buffer_index] == '#')
        {
            while (buffer_index < buffer_len && text_buffer[buffer_index] != '\n')
            {
                buffer_index++;
            }
            continue;
        }

        Token curr_token;

        // check for specific register names
        if (bufCmp(text_buffer, "sp", buffer_index, buffer_len) == true)
        {
            curr_token.type = TOKEN_REGISTER;
            curr_token.reg  = Token_SP;
            buffer_index += 2;
        }
        else if (bufCmp(text_buffer, "mar", buffer_index, buffer_len) == true)
        {
            curr_token.type = TOKEN_REGISTER;
            curr_token.reg  = Token_MAR;
            buffer_index += 3;
        }
        else if (bufCmp(text_buffer, "mbr", buffer_index, buffer_len) == true)
        {
            curr_token.type = TOKEN_REGISTER;
            curr_token.reg  = Token_MBR;
            buffer_index += 3;
        }
        else if (chrCmp(text_buffer[buffer_index], 'r') && isDigit(text_buffer[buffer_index + 1]))
        {
            // check register number
            if (text_buffer[buffer_index + 1] - '0' > 3)
            {
                printf("Register number '%d' at %d is to high\n", text_buffer[buffer_index + 1] - '0', buffer_index + 1);
                goto free;
            }

            curr_token.type = TOKEN_REGISTER;
            curr_token.reg  = (TokenRegister)text_buffer[buffer_index + 1] - '0';
            buffer_index   += 2;
        } // check if we need to convert a number
        else if (bufCmp(text_buffer, "0b", buffer_index, buffer_len))
        {
            number_state = PARSE_BINARY;
            buffer_index += 2;
            // dont create a token
            continue;
        }
        else if (bufCmp(text_buffer, "0x", buffer_index, buffer_len))
        {
            number_state = PARSE_HEX;
            buffer_index += 2;
            continue;
        }
        else if (bufCmp(text_buffer, "<<", buffer_index, buffer_len))
        {
            curr_token.type = TOKEN_SHL;
            buffer_index += 2;
        }
        else if (bufCmp(text_buffer, ">>", buffer_index, buffer_len))
        {
            curr_token.type = TOKEN_SHR;
            buffer_index += 2;
        }
        else if (isDigit(text_buffer[buffer_index]))
        {
            // parse number with correct state
            errno = 0;
            char *endptr;
            curr_token.number = strtoull(&text_buffer[buffer_index], &endptr, number_state);

            if ((curr_token.number == ULLONG_MAX || curr_token.number == 0) && errno == ERANGE)
            {
                printf("Number out of range at position %d\n", buffer_index);
                goto free;
            }

            int endptr_pos = endptr - text_buffer;
            if (endptr_pos < buffer_len && *endptr != ' ' && *endptr != '\n' && *endptr != '\r' && *endptr != ',' && *endptr != ')')
            {
                printf("Invalid character '%c' after number at position %d\n", *endptr, buffer_index);
                goto free;
            }

            curr_token.type = TOKEN_NUMBER;
            buffer_index = endptr_pos;
            number_state = PARSE_DECIMAL;
        }
        else if (number_state != PARSE_DECIMAL)
        {
            // the number_state must be decimal because if we encounter an 0b or 0x then this section will be skipped
            // If we had an 0x or 0b in the last iteration and the current digit is a number the number has been parsed 
            // and number state is back hat decimal

            if (number_state == PARSE_BINARY)
            {
                printf("After an 0b at %d a number is required.\n", buffer_index);
            }else
            {
                printf("After an 0x at %d a number is required.\n", buffer_index);
            }

            goto free;
        }
        else
        {
            // single character Tokens
            switch (text_buffer[buffer_index])
            {
            case '%':
                curr_token.type = TOKEN_PERCENT;
                buffer_index++;
                break;
            case ':':
                // change the last string token to an label
                if (tokens->count <= 0 || tokens->list[tokens->count - 1].type != TOKEN_STRING)
                {
                    printf("A label consists out of a string and a text. Text before %d is missing", buffer_index);
                    goto free;
                }
                
                tokens->list[tokens->count - 1] = (Token){
                    .type = TOKEN_LABEL,
                    .str = tokens->list[tokens->count - 1].str,
                };
                
                buffer_index++;
                continue;
                break;
            case '\n':
                curr_token.type = TOKEN_NEWLINE;
                buffer_index++;
                break;
            case ',':
                curr_token.type = TOKEN_COMMA;
                buffer_index++;
                break;
            case '(':
                curr_token.type = TOKEN_LPAREN;
                buffer_index++;
                break;
            case ')':
                curr_token.type = TOKEN_RPAREN;
                buffer_index++;
                break;
            case '+':
                curr_token.type = TOKEN_PLUS;
                buffer_index++;
                break;
            case '-':
                curr_token.type = TOKEN_MINUS;
                buffer_index++;
                break;
            case '*':
                curr_token.type = TOKEN_STAR;
                buffer_index++;
                break;
            case '/':
                curr_token.type = TOKEN_SLASH;
                buffer_index++;
                break;
            case '&':
                curr_token.type = TOKEN_AND;
                buffer_index++;
                break;
            case '|':
                curr_token.type = TOKEN_OR;
                buffer_index++;
                break;
            case '^':
                curr_token.type = TOKEN_XOR;
                buffer_index++;
                break;
            case '!':
                curr_token.type = TOKEN_NOT;
                buffer_index++;
                break;
            case '.':
                curr_token.type = TOKEN_DIRECTIVE;
                buffer_index++;
                is_directive = true;
                
            default:
                // interpret as a string
                int end_index = buffer_index;
                
                // points to the last character
                while (end_index < buffer_len  &&
                text_buffer[end_index] != ' '  &&
                text_buffer[end_index] != '\n' &&
                text_buffer[end_index] != '\r' &&
                text_buffer[end_index] != ','  &&
                text_buffer[end_index] != ':'  )
                {
                    end_index++;
                }

                // plus +1 -> null terminator
                curr_token.str = malloc(sizeof(char) * (end_index - buffer_index + 1));
                
                memcpy(curr_token.str, text_buffer + buffer_index, end_index - buffer_index);
                curr_token.str[end_index - buffer_index] = '\0';
                
                buffer_index = end_index;
                if (is_directive)
                {
                    is_directive = false;
                    curr_token.type = TOKEN_DIRECTIVE;
                }
                else
                {
                    curr_token.type = TOKEN_STRING;
                }
            }
        }
        addToken(tokens, &curr_token);
    }

    return 0;

    free:
        freeTokenList(tokens);
        return -1;
}

Token *getToken(TokenList *tokens)
{
    if (tokens->index >= tokens->count)
    {
        return NULL;
    }
    
    return &tokens->list[tokens->index++];
};

Token *peekToken(TokenList *tokens)
{
    if (tokens->index >= tokens->count)
    {
        return NULL;
    }
    
    return &tokens->list[tokens->index];
};