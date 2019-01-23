#ifndef __LEXEME_INCLUDED__
#define __LEXEME_INCLUDED__

typedef struct lexeme LEXEME;

extern LEXEME *newLEXEME(char *type);
extern LEXEME *newLEXEMEvalue(char *type, void *value);
extern LEXEME *newLEXEMEdouble(char *type, double value);
extern LEXEME *newLEXEMEint(char *type, int value);
extern LEXEME *newLEXEMEerror(char *type, int ch);
extern char *getTypeLEXEME(LEXEME *lexeme);
extern int isErrorLEXEME(LEXEME *lexeme);
extern void printLEXEME(FILE *fp, LEXEME *lexeme);

#endif
