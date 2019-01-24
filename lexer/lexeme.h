#ifndef __LEXEME_INCLUDED__
#define __LEXEME_INCLUDED__

#include <stdio.h>

typedef struct lexeme LEXEME;

extern LEXEME *newLEXEME(char *type);
extern LEXEME *newLEXEMEstring(char *type, char *value);
extern LEXEME *newLEXEMEdouble(double value);
extern LEXEME *newLEXEMEint(int value);
extern char *getTypeLEXEME(LEXEME *lexeme);
extern int isErrorLEXEME(LEXEME *lexeme);
extern void printLEXEME(FILE *fp, LEXEME *lexeme);

#endif
