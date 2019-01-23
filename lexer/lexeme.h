#ifndef __LEXEME_INCLUDED__
#define __LEXEME_INCLUDED__

typedef struct lexeme LEXEME;

extern LEXEME *newLEXEME(char *type);
extern LEXEME *newLEXEMEerror(char *type, int ch);
extern char *getTypeLEXEME(LEXEME *lexeme);
extern void printLEXEME(FILE *fp, LEXEME *lexeme);

#endif
