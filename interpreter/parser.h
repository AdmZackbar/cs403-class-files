#ifndef __PARSER_INCLUDED__
#define __PARSER_INCLUDED__

#include "lexeme.h"

extern LEXEME *parse(char *filename);
extern LEXEME *advance();
extern int check(char *type);
extern LEXEME *match(char *type);

#endif
