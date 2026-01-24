#ifndef DIRECTIVE_H
#define DIRECTIVE_H

#include "directive_definition.h"
#include "../lexer/lexer.h"

DirectiveType getDirectiveFromToken(TokenList *tokens);

#endif // DIRECTIVE_H