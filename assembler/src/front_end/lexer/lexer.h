#ifndef LEXER_H
#define LEXER_H

#include <stdint.h>
#include "token.h"

int tokenize(TokenList *tokens, char *text_buffer, int buffer_len);

#endif // LEXER_H