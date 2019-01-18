#ifndef __LEXEME_INCLUDED__
#define __LEXEME_INCLUDED__

typedef struct lexeme LEXEME;

extern LEXEME *newLEXEME(char *type);
extern char *getTypeLEXEME(LEXEME *lexeme);

#endif
