#ifndef __LEXEME_INCLUDED__
#define __LEXEME_INCLUDED__

#include <stdio.h>

typedef struct lexeme LEXEME;

extern LEXEME *newLEXEME(char *type, int lineNum);
extern LEXEME *newLEXEMEstring(char *type, char *value, int lineNum);
extern LEXEME *newLEXEMEdouble(double value, int lineNum);
extern LEXEME *newLEXEMEint(int value, int lineNum);
extern LEXEME *getLEXEMEleft(LEXEME *lexeme);
extern LEXEME *getLEXEMEright(LEXEME *lexeme);
extern void setLEXEMEleft(LEXEME *parent, LEXEME *child);
extern void setLEXEMEright(LEXEME *parent, LEXEME *child);
extern char *getTypeLEXEME(LEXEME *lexeme);
extern int getLineNumLEXEME(LEXEME *lexeme);
extern int isErrorLEXEME(LEXEME *lexeme);
extern void printLEXEME(FILE *fp, LEXEME *lexeme);

#endif
