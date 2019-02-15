#ifndef __LEXER_INCLUDED__
#define __LEXER_INCLUDED__

#include "lexeme.h"

typedef struct lexer LEXER;

extern LEXER *newLEXER(char *filename);
extern LEXEME *lex(LEXER *lexer);

#endif