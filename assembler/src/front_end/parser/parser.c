#include "../instruction.h"
#include "../lexer/lexer.h"
#include "parser.h"

int parseInstructions(InstructionList *instructions, TokenList *tokens)
{
    Token *curr_token;
    
    while(curr_token)
    {
        switch(curr_token->type)
        {
            case TOKEN_LABEL:
                // simply save the label
                break;
            
            case TOKEN_DIRECTIVE:
                    
            break;
            
            case TOKEN_STRING:
                break;
            
        }
        curr_token = getToken(tokens);
    }
}